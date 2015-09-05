/************************************************************************
* File Name          : RemoteControl
* Author             : Simon
* Updated            : -
* Version            : V0.0.1
* Date               : 3 september, 2015
* Description        : -(tba)
* License            : -
* Copyright(C) 2014 UFactory Team. All right reserved.
*************************************************************************/
#include <EEPROM.h>
#include <UF_uArm.h>
#include <ros.h>

#include <sensor_msgs/JointState.h>
UF_uArm* uarm; // initialize the uArm library

float positions[5] = {0.0};
char *joint_names[5] = {"SERVO_ROT", "SERVO_L", "SERVO_R", "SERVO_HAND_ROT", "SERVO_HAND"};

ros::NodeHandle*  nh;

sensor_msgs::JointState joint_state_msg;
ros::Publisher joint_state_pub("/joint_states", &joint_state_msg);

void setup()
{
  uarm = new UF_uArm();
  uarm->init();          // initialize the uArm position
    
  uarm->setServoSpeed(SERVO_R,    0);  // 0=full speed, 1-255 slower to faster
  uarm->setServoSpeed(SERVO_L,    0);  // 0=full speed, 1-255 slower to faster
  uarm->setServoSpeed(SERVO_ROT, 50);  // 0=full speed, 1-255 slower to faster

  nh = new ros::NodeHandle;
  nh->initNode();
  nh->advertise(joint_state_pub);

  joint_state_msg.position_length = 5;
  joint_state_msg.position = positions;
  joint_state_msg.name_length = 5;
  joint_state_msg.name = joint_names;
}

void loop()
{
  uarm->calibration();   // if corrected, you could remove it, no harm though

  joint_state_msg.header.stamp = nh->now();
  joint_state_msg.position[0] = (float)uarm->readAngle(SERVO_ROT);
  joint_state_msg.position[1] = (float)uarm->readAngle(SERVO_L);
  joint_state_msg.position[2] = (float)uarm->readAngle(SERVO_R);
  joint_state_msg.position[3] = (float)uarm->readAngle(SERVO_HAND_ROT);
  joint_state_msg.position[4] = (float)uarm->readAngle(SERVO_HAND);

  //button_d4_msg.data = (digitalRead(BTN_D4) == 0);
  //button_d7_msg.data = (digitalRead(BTN_D7) == 0);
  //button_d4_pub.publish(&button_d4_msg);
  //button_d7_pub.publish(&button_d7_msg);

  joint_state_pub.publish( &joint_state_msg );
  nh->spinOnce();
  delay(50);

  uarm->gripperDetach();
}

