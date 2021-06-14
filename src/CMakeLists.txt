cmake_minimum_required(VERSION 3.1.0)
# ########### ProtoViewer

project(
  ProtoViewer
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH /Users/pezy/Qt5.12.8/5.12.8/clang_64)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(
  Qt5
  COMPONENTS Core Widgets Gui
  REQUIRED)

include_directories(
  /Users/pezy/.gradle/caches/modules-2/files-2.1/com.hobot.native.hobot-adas.stable.3rd-party/protobuf/2.6.1_mac/files/protobuf/include
  /Users/pezy/.gradle/caches/modules-2/files-2.1/com.hobot.native.hobot.auto/hobot-auto-protocol/0.0.11_mac/files/hobot-auto-protocol/include
)

link_directories(
  /Users/pezy/.gradle/caches/modules-2/files-2.1/com.hobot.native.hobot-adas.stable.3rd-party/protobuf/2.6.1_mac/files/protobuf/lib
  /Users/pezy/.gradle/caches/modules-2/files-2.1/com.hobot.native.hobot.auto/hobot-auto-protocol/0.0.11_mac/files/hobot-auto-protocol/lib
)

add_executable(
  ProtoViewer
  main.cpp
  proto_tree_item.h
  proto_tree_item.cpp
  proto_tree_model.h
  proto_tree_model.cpp)

target_link_libraries(ProtoViewer hobot-auto-protocol protobuf Qt5::Core
                      Qt5::Widgets Qt5::Gui)
