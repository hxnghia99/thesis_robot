#!/usr/bin/env python

######################################
# file serial_node.py
# Author:   HXNghia
# Date:     April 23th, 2021
######################################

import rospy
from rosserial_client import SerialClient
from serial import SerialException
from time import sleep
import multiprocessing
import sys
sys.dont_write_bytecode = True

if __name__=="__main__":
    rospy.init_node("serial_node")
    rospy.loginfo("ROS Serial Python Node")

    port_name = rospy.get_param('~port','/dev/ttyUSB0')
    baud = int(rospy.get_param('~baud','115200'))

    fix_pyserial_for_test = rospy.get_param('~fix_pyserial_for_test', False)

            # Use serial port
    while not rospy.is_shutdown():
        rospy.loginfo("Connecting to %s at %d baud" % (port_name,baud) )
        try:
            print("Trying .............")
            client = SerialClient(port_name, baud, fix_pyserial_for_test=fix_pyserial_for_test)
            client.run()
        except KeyboardInterrupt:
            break
        except SerialException:
            sleep(1.0)
            continue
        except OSError:
            sleep(1.0)
            continue
        except:
            rospy.logwarn("Unexpected Error: %s", sys.exc_info()[0])
            client.port.close()
            sleep(1.0)
            continue