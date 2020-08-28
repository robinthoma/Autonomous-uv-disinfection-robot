#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <ros.h>
#include <geometry_msgs/Twist.h>
// Pin variables for motors.
const int right_back = 5;
const int right_front = 6;
const int left_back = 9;
const int left_front = 10;



ros::NodeHandle  nh;

void MoveFwd() {
  digitalWrite(right_back, HIGH);
  digitalWrite(left_back, HIGH);
  analogWrite(right_front,LOW);
  analogWrite(left_front, LOW);
}

void MoveStop() {
  digitalWrite(right_back, LOW);
  digitalWrite(left_back, LOW);
  analogWrite(right_front,LOW);
  analogWrite(left_front, LOW);
}
void MoveLeft() {
  digitalWrite(right_back, HIGH);
  digitalWrite(left_back, LOW);
  analogWrite(right_front,LOW);
  analogWrite(left_front, HIGH);
}
void MoveRight() {
  digitalWrite(right_back, LOW);
  digitalWrite(left_back, HIGH);
  analogWrite(right_front,HIGH);
  analogWrite(left_front, LOW);
}
void MoveBack() {
  digitalWrite(right_back, LOW);
  digitalWrite(left_back, LOW);
  analogWrite(right_front,HIGH);
  analogWrite(left_front, HIGH);
}

void cmd_vel_cb(const geometry_msgs::Twist & msg) {
  // Read the message. Act accordingly.
  // We only care about the linear x, and the rotational z.
  const float x = msg.linear.x;
  const float z_rotation = msg.angular.z;

  // Decide on the morot state we need, according to command.
  if (x > 0 && z_rotation == 0) {
    MoveFwd();
  }
  else if (x == 0 && z_rotation == 1) {
    MoveRight();
  }
else if (x == 0 && z_rotation < 0) {
    MoveLeft();
  }
else if (x < 0 && z_rotation == 0) {
    MoveBack();
  }
else{
    MoveStop();
  }
}
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", cmd_vel_cb);
void setup() {
  pinMode(right_back, OUTPUT);    
  pinMode(left_back, OUTPUT);
  pinMode(right_front, OUTPUT);
  pinMode(left_front, OUTPUT);
  
  
  nh.initNode();
  nh.subscribe(sub);
}

void loop() {
  nh.spinOnce();
  delay(1);
}