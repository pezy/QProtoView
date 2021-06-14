#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

namespace google {
namespace protobuf {
class Message;
class TreeItem;

class TreeModel : public QAbstractItemModel {
  Q_OBJECT

 public:
  explicit TreeModel(const Message &data, QObject *parent = 0);
  ~TreeModel();

  QVariant data(const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

 private:
  void setupModelData(const Message &data, TreeItem *parent);

  TreeItem *rootItem;
};
}  // namespace protobuf
}  // namespace google
