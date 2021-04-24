cmake_minimum_required(VERSION 3.0.2)
project(thesis_robot_rosserial)

find_package(catkin REQUIRED COMPONENTS
  rospy
)

catkin_python_setup()

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

catkin_install_python(PROGRAMS
  src/serial_node.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
