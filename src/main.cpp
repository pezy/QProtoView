#include <protocol/real3d.pb.h>

#include <QApplication>
#include <QFile>
#include <QTreeView>

#include "proto_tree_model.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  Real3DProto::Real3DProto real3d;
  real3d.mutable_header()->set_frame_id(42);
  real3d.mutable_header()->set_cam_idx(0);
  real3d.mutable_header()->set_time_stamp(42);

  google::protobuf::TreeModel model(real3d);

  QTreeView view;
  view.setModel(&model);
  view.setWindowTitle(QObject::tr("Proto Viewer"));
  view.show();
  return app.exec();
}
