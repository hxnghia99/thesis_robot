/** Test odom topic by publishing dummy odom
 *  Author: H.X.Nghia
 *  Date:   April 5th 2021
 **/
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <nav_msgs/Odometry.h>

int main(int argc, char ** argv) {
    ros::init(argc, argv, "publish_dummy_odom");
    ros::NodeHandle nh;
    ros::Publisher dummy_odom_pub = nh.advertise<nav_msgs::Odometry>("odom",20);
    nav_msgs::Odometry dummy_odom_msg;
    ros::Rate rate(10);
    float temp = 0;
    while(ros::ok()) {
        temp+=0.005;
        dummy_odom_msg.pose.pose.position.x = temp;
        dummy_odom_pub.publish(dummy_odom_msg);
        ros::spinOnce();
        rate.sleep();
    }
}