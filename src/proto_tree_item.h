#pragma once

#include <QList>
#include <QVariant>

namespace google {
namespace protobuf {

class TreeItem {
 public:
  explicit TreeItem(const QList<QVariant> &data, TreeItem *parentItem = 0);
  ~TreeItem();

  void appendChild(TreeItem *child);

  TreeItem *child(int row);
  int childCount() const;
  int columnCount() const;
  QVariant data(int column) const;
  int row() const;
  TreeItem *parentItem();

 private:
  QList<TreeItem *> m_childItems;
  QList<QVariant> m_itemData;
  TreeItem *m_parentItem;
};
}  // namespace protobuf
}  // namespace google
