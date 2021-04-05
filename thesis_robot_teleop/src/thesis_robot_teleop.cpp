/** Velocity controller for robot
 	Author: H.X.Nghia
 	Date: April 5th 2021
**/

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"

#include <stdio.h>
#include <unistd.h>
#include <termios.h>

//function prototype
int getch(void);
void checkAngularLimitVel(float& angular_vel);
void checkLinearLimitVel(float& linear_vel);
void ros_info_vel(float goal_linear, float goal_angular);

//global variables
const float ROBOT_MAX_LIN_VEL(0.1), ROBOT_MAX_ANG_VEL(0.15);
const float LIN_VEL_STEP_SIZE(0.025), ANG_VEL_STEP_SIZE(0.05);

int main(int argc, char ** argv) {
	ros::init(argc, argv, "cmd_vel_publisher");
	ros::NodeHandle nh;
	ros::Publisher cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 10);
	geometry_msgs::Twist cmd_vel_msg;

	ros::Rate loop_rate(10);

	//Variables Declaration
	float goal_linear_vel(0), goal_angular_vel(0);
	char key(' ');

	const char* msg_c = R"(
Control Thesis Robot
---------------------------
Moving around:
	w
a	s	d
	x
w/x : increase/decrease linear velocity ( 0.05/time)
a/d : increase/decrease angular velocity  (0.05/time)

space key, s : force stop
---------------------------
CTRL-C to quit
)";
	std_msgs::String msg;
	msg.data = msg_c;
	ROS_INFO("%s", msg.data.c_str());

	//Initialize the Twist message
	cmd_vel_msg.linear.x = 0;
	cmd_vel_msg.linear.y = 0;
	cmd_vel_msg.linear.z = 0;

	cmd_vel_msg.angular.x = 0;
	cmd_vel_msg.angular.y = 0;
	cmd_vel_msg.angular.z = 0;

	while(ros::ok()) {
		key = getch();

		//check for exit ctrl+C
		if(key=='\x03') {
			goal_linear_vel = 0;
			goal_angular_vel = 0;
			ros_info_vel(goal_linear_vel, goal_angular_vel);
			break;
		}

		switch(key) {
			case 'w': 	goal_linear_vel += LIN_VEL_STEP_SIZE;
						break;
			case 'x': 	goal_linear_vel -= LIN_VEL_STEP_SIZE;
						break;
			case 'a': 	goal_angular_vel += ANG_VEL_STEP_SIZE;
						break;
			case 'd': 	goal_angular_vel -= ANG_VEL_STEP_SIZE;
						break;
			case 's':	goal_angular_vel = 0.0;
						goal_linear_vel = 0.0;
						break;
			case ' ':	goal_angular_vel = 0.0;
						goal_linear_vel = 0.0;
						break;
			default:;
		}
		//check Limit for velocity
		checkLinearLimitVel(goal_linear_vel);
		checkAngularLimitVel(goal_angular_vel);
		ros_info_vel(goal_linear_vel, goal_angular_vel);

		//update the Twist message
		cmd_vel_msg.linear.x = goal_linear_vel;
		cmd_vel_msg.angular.z = goal_angular_vel;

		cmd_vel_pub.publish(cmd_vel_msg);

		ros::spinOnce();
		loop_rate.sleep();
	}
}

// For non-blocking keyboard inputs
int getch(void)
{
  int ch;
  struct termios oldt;
  struct termios newt;

  // Store old settings, and copy to new settings
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  // Make required changes and apply the settings
  newt.c_lflag &= ~(ICANON | ECHO);
  newt.c_iflag |= IGNBRK;
  newt.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF);
  newt.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
  newt.c_cc[VMIN] = 1;
  newt.c_cc[VTIME] = 0;
  tcsetattr(fileno(stdin), TCSANOW, &newt);

  // Get the current character
  ch = getchar();

  // Reapply old settings
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return ch;
}

void ros_info_vel(float goal_linear, float goal_angular) {
	//ROS_INFO("Current:	Linear_vel = %1.2f (m/s) \t Angular_vel = %1.2f (rad/s)", goal_linear*2, goal_angular*2);
	printf("Current:	Linear_vel = %1.2f (m/s) \t Angular_vel = %1.2f (rad/s) \n", goal_linear*2, goal_angular*2);
}

void checkLinearLimitVel(float& linear_vel) {
	if(linear_vel > ROBOT_MAX_LIN_VEL)
		linear_vel = ROBOT_MAX_LIN_VEL;
	else if(linear_vel < -ROBOT_MAX_LIN_VEL)
		linear_vel = -ROBOT_MAX_LIN_VEL;
}

void checkAngularLimitVel(float& angular_vel) {
	if(angular_vel > ROBOT_MAX_ANG_VEL)
		angular_vel = ROBOT_MAX_ANG_VEL;
	else if(angular_vel < -ROBOT_MAX_ANG_VEL)
		angular_vel = -ROBOT_MAX_ANG_VEL;
}

