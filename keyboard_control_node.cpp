#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <mavros_msgs/OverrideRCIn.h>
#include<mavros_msgs/RCIn.h>

geometry_msgs::Twist keyboard_state;
void keyboard_cb(const geometry_msgs::Twist::ConstPtr& msg){
    keyboard_state = *msg;
}

int main(int argc, char **argv)
{   

    ros::init(argc, argv, "keyboard_control_node");
    ros::NodeHandle nh;
    			
    ros::Subscriber keyboard_sub = nh.subscribe<geometry_msgs::Twist>
            ("/turtle1/cmd_vel", 10, keyboard_cb);
    ros::Publisher rc_pub = nh.advertise<mavros_msgs::OverrideRCIn>
            ("/mavros/rc/override", 10);

    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(30.0);
	
	mavros_msgs::OverrideRCIn forward,backward,left,right,neutral;
	forward.channels = {1500,1600,1500,1500,1000,1000,1000,1000};
	backward.channels ={1500,1400,1500,1500,1000,1000,1000,1000};
	left.channels =    {1400,1500,1500,1500,1000,1000,1000,1000};
	right.channels =   {1600,1500,1500,1500,1000,1000,1000,1000};
	neutral.channels = {1500,1500,1500,1500,1000,1000,1000,1000}; 

	
    //send a few setpoints before starting

    while(ros::ok()){

		if(keyboard_state.linear.x == 2){
			rc_pub.publish(forward);
		}
		else if(keyboard_state.linear.x == -2){
			rc_pub.publish(backward);
		}
		else if(keyboard_state.angular.z == 2){
			rc_pub.publish(left);
		}
		else if(keyboard_state.angular.z == -2){
			rc_pub.publish(right);
		}
		else{
			rc_pub.publish(neutral);
		}
		keyboard_state.linear.x = 0 ;
		keyboard_state.angular.z = 0;
		ros::spinOnce();
		rate.sleep();
    }

    return 0;
}
