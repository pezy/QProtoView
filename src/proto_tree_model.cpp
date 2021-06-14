
#include "proto_tree_model.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

#include <QStringList>

#include "proto_tree_item.h"

namespace google {
namespace protobuf {
//! [0]
TreeModel::TreeModel(const Message &data, QObject *parent)
    : QAbstractItemModel(parent) {
  QList<QVariant> rootData;
  rootData << "Title"
           << "Summary";
  rootItem = new TreeItem(rootData);
  setupModelData(data, rootItem);
}
//! [0]

//! [1]
TreeModel::~TreeModel() { delete rootItem; }
//! [1]

//! [2]
int TreeModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return static_cast<TreeItem *>(parent.internalPointer())->columnCount();
  else
    return rootItem->columnCount();
}
//! [2]

//! [3]
QVariant TreeModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return QVariant();

  if (role != Qt::DisplayRole) return QVariant();

  TreeItem *item = static_cast<TreeItem *>(index.internalPointer());

  return item->data(index.column());
}
//! [3]

//! [4]
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const {
  if (!index.isValid()) return 0;

  return QAbstractItemModel::flags(index);
}
//! [4]

//! [5]
QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const {
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return rootItem->data(section);

  return QVariant();
}
//! [5]

//! [6]
QModelIndex TreeModel::index(int row, int column,
                             const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent)) return QModelIndex();

  TreeItem *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<TreeItem *>(parent.internalPointer());

  TreeItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}
//! [6]

//! [7]
QModelIndex TreeModel::parent(const QModelIndex &index) const {
  if (!index.isValid()) return QModelIndex();

  TreeItem *childItem = static_cast<TreeItem *>(index.internalPointer());
  TreeItem *parentItem = childItem->parentItem();

  if (parentItem == rootItem) return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}
//! [7]

//! [8]
int TreeModel::rowCount(const QModelIndex &parent) const {
  TreeItem *parentItem;
  if (parent.column() > 0) return 0;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<TreeItem *>(parent.internalPointer());

  return parentItem->childCount();
}
//! [8]

QVariant GetDataFromFieldDescriptor(const Message *message,
                                    const FieldDescriptor *fd) {
  const Reflection *reflection = message->GetReflection();
  switch (fd->cpp_type()) {
    case FieldDescriptor::CPPTYPE_BOOL:
      return QVariant(reflection->GetBool(*message, fd));
    case FieldDescriptor::CPPTYPE_DOUBLE:
      return QVariant(reflection->GetDouble(*message, fd));
    case FieldDescriptor::CPPTYPE_FLOAT:
      return QVariant(reflection->GetFloat(*message, fd));
    case FieldDescriptor::CPPTYPE_INT32:
      return QVariant(reflection->GetInt32(*message, fd));
    case FieldDescriptor::CPPTYPE_INT64:
      return QVariant(reflection->GetInt64(*message, fd));
    case FieldDescriptor::CPPTYPE_MESSAGE:
      return QVariant();
    case FieldDescriptor::CPPTYPE_STRING:
      return QVariant(reflection->GetString(*message, fd).data());
    case FieldDescriptor::CPPTYPE_UINT32:
      return QVariant(reflection->GetUInt32(*message, fd));
    case FieldDescriptor::CPPTYPE_UINT64:
      return QVariant(reflection->GetUInt64(*message, fd));
    default:
      return QVariant();
  }
}

struct MessageLevel {
  const Message *msg;
  int level;
  TreeItem *parent;
};

void TreeModel::setupModelData(const Message &data, TreeItem *parent) {
  QList<TreeItem *> parents;
  QList<int> indentations;
  parents << parent;
  indentations << 0;

  QList<MessageLevel> msg_list;
  msg_list << MessageLevel{&data, 0, nullptr};

  while (!msg_list.empty()) {
    const auto *parent_msg = msg_list.front().msg;
    const auto level = msg_list.front().level;
    auto *parent_item = msg_list.front().parent;
    msg_list.pop_front();

    if (!parent_item) {
      parent_item = parents.last();
    }

    if (level > indentations.last()) {
      if (parent_item->childCount() > 0) {
        parents << parent_item->child(parent_item->childCount() - 1);
        indentations << level;
      }
    } else {
      while (level < indentations.last() && parents.count() > 0) {
        parents.pop_back();
        indentations.pop_back();
      }
    }

    const Descriptor *d = parent_msg->GetDescriptor();
    for (int i = 0; i < d->field_count(); ++i) {
      const FieldDescriptor *child_fd = d->field(i);
      QList<QVariant> column_data;
      column_data << child_fd->name().data();
      if (child_fd->is_repeated()) {
        column_data << "repeated";
      } else if (child_fd->type() == FieldDescriptor::TYPE_MESSAGE) {
        column_data << "";
        msg_list << MessageLevel{
            &(parent_msg->GetReflection()->GetMessage(*parent_msg, child_fd)),
            level + 1};
      } else {
        column_data << GetDataFromFieldDescriptor(parent_msg, child_fd);
      }

      parent_item->appendChild(new TreeItem(column_data, parent_item));
    }
  }
}
}  // namespace protobuf
}  // namespace google