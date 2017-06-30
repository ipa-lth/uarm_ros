#!/usr/bin/env python
import rospy
import tf
import math
import sys
import datetime
import numpy as np

from trajectory_msgs.msg import JointTrajectory
from sensor_msgs.msg import JointState
from SerialDataGateway import SerialDataGateway

class UArm(object):

    def __init__(self, port="/dev/ttyUSB0", baudrate=9600):
        '''
        Initializes the receiver class.
        port: The serial port to listen to.
        baudrate: Baud rate for the serial communication
        '''
        
        rospy.init_node('uarm')

        port = rospy.get_param("~port", "/dev/ttyUSB0")
        baudRate = int(rospy.get_param("~baudRate", 9600))

        rospy.loginfo("Starting with serial port: " + port + ", baud rate: " + str(baudRate))

        # subscriptions
        rospy.Subscriber("cmd_vel", JointTrajectory, self._HandleVelocityCommand)

        #Publisher
        self._jsPub = rospy.Publisher('joint_states', JointState, queue_size = 128)

        self._SerialDataGateway = SerialDataGateway(port, baudRate,  self._HandleReceivedLine)


    def _HandleReceivedLine(self, line):
        js = JointState()
        count = 0
        substring = ""
        ##  parsing received line into Joinstate
        #print line
        if line.startswith('%'):
            #print line[1:].split(',')
            data = np.array(line[1:].split(','), dtype=float)
            
            js.position = data            
            js.name = ['joint_0', 'joint_1', 'joint_2', 'joint_3']
            js.header.stamp = rospy.Time().now()
            self._jsPub.publish(js)

    def _HandleVelocityCommand(self, data):
        """ Handle movement requests. """
        
        if len(data.points) > 0:
            qd_d = list(data.points[0].velocities)
        else:
            return

        rospy.logdebug("Handling command: {}".format(qd_d) )

        message = '{},{},{},{}\n'.format(qd_d[0], qd_d[1], qd_d[2], qd_d[3])
        rospy.logdebug("Sending speed command message: " + message)
        self._SerialDataGateway.Write(message)

    def Start(self):
        rospy.logdebug("Starting")
        self._SerialDataGateway.Start()

    def Stop(self):
        rospy.logdebug("Stopping")
        self._SerialDataGateway.Stop()
        

if __name__ == '__main__':
    uarm = UArm()
    try:
        uarm.Start()
        rospy.spin()
    except rospy.ROSInterruptException:
        rospy.logerr("ROSInterruptException")
    finally:
        uarm.Stop()
