#include <Arduino.h>

// Define the pins for Motor 1
//m1 = Y
// m2 = z
// m3 = X
#define EN_PIN_M1  PC3 //enable 
#define STEP_PIN_M1 PB8 //step
#define DIR_PIN_M1  PB7 //direction

// Define the pins for Motor 2
#define EN_PIN_M2  PC3 //enable 
#define STEP_PIN_M2 PB6 //step
#define DIR_PIN_M2  PB5 //direction 

// Define the pins for Motor 3
#define EN_PIN_M3  PC3 //enable 
#define STEP_PIN_M3 PC2 //step
#define DIR_PIN_M3  PB9 //direction

#define limit_x PA5
#define limit_y PA6
#define limit_z PA7

int pos_x = 0;
int pos_y = 0;
int pos_z = 0;

int x_cur = 0;
int y_cur = 0;
int z_cur = 0;

signed int dist_x = 0;
signed int dist_y = 0;
signed int dist_z = 0;

//Max Distance
int max_pos_x = 0;
int max_pos_y = 0;
int max_pos_z = 0;

//Direction of rotation
int dir_x = 0;
int dir_y = 0;
int dir_z = 1; 

void setup() {
  Serial1.begin(9600); // Initialize serial communication
  
  //Limit Switch
  pinMode(limit_x, INPUT);
  pinMode(limit_y, INPUT);
  pinMode(limit_z, INPUT);

  // Setup Motor 1
  pinMode(EN_PIN_M1, OUTPUT);
  digitalWrite(EN_PIN_M1, LOW); // Activate driver (LOW active)
  pinMode(DIR_PIN_M1, OUTPUT);
  digitalWrite(DIR_PIN_M1, LOW); // Set the direction pin to low
  pinMode(STEP_PIN_M1, OUTPUT);
  digitalWrite(STEP_PIN_M1, LOW); // Set the step pin to low

  // Setup Motor 2
  pinMode(EN_PIN_M2, OUTPUT);
  digitalWrite(EN_PIN_M2, LOW); // Activate driver (LOW active)
  pinMode(DIR_PIN_M2, OUTPUT);
  digitalWrite(DIR_PIN_M2, LOW); // Set the direction pin to low
  pinMode(STEP_PIN_M2, OUTPUT);
  digitalWrite(STEP_PIN_M2, LOW); // Set the step pin to low

  // Setup Motor 3
  pinMode(EN_PIN_M3, OUTPUT);
  digitalWrite(EN_PIN_M3, LOW); // Activate driver (LOW active)
  pinMode(DIR_PIN_M3, OUTPUT);
  digitalWrite(DIR_PIN_M3, LOW); // Set the direction pin to low
  pinMode(STEP_PIN_M3, OUTPUT);
  digitalWrite(STEP_PIN_M3, LOW); // Set the step pin to low

  ghar_wapsi();
  
}

// This function sets the number of steps, the direction, and the speed for a motor
void rotateMotor(long steps, bool direction, int speed, int stepPin, int dirPin) {
  digitalWrite(dirPin, direction); // Set the motor direction

  // Step the motor
  for (long i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }
}

void ghar_wapsi(){
  if (digitalRead(limit_x) == LOW){
    for (int i=0; i<=95000; i+=100){
      if (digitalRead(limit_x) == HIGH){
        break;
      }
      rotateMotor(100, 0, 50, PC2, PC9);
    }
  }

  if (digitalRead(limit_y) == LOW){
    for (int i=0; i<=95000; i+=100){
      if (digitalRead(limit_y) == HIGH){
        break;
      }
      rotateMotor(100, 0, 50, PB8, PB7);
    }
  }

  if (digitalRead(limit_z) == LOW){
    for (int i=0; i<=95000; i+=100){
      if (digitalRead(limit_z) == HIGH){
        break;
      }
      rotateMotor(100, 1, 40, PB6, PB5);
    }
  }
  moveMotors(0, 0, 28788);
  
  pos_x = 0;
  pos_y = 0;
  pos_z = 28788;
}

void loop() {
  
  if (Serial1.available() > 0) {

    int distance_m1 = Serial1.parseInt();
    int speed_m1 = 100;
    long steps_m1 = distance_m1 * 17000 / 32;

    // Read the distance, direction, and speed for Motor 2
    int distance_m2 = Serial1.parseInt();
    int speed_m2 = 40;
    
    // if (distance_m2>=10){
    //   distance_m2 = 10;
    // } 

    // if (distance_m2 < 10 && distance_m2>=0) {
    //   distance_m2 = 5;
    // }
  
    long steps_m2 = distance_m2 * 95000 / 33;
    // Read the distance, direction, and speed for Motor 3
    int distance_m3 = Serial1.parseInt();
    int speed_m3 = 100;
    long steps_m3 = distance_m3 * 19300 / 30;
 
    if(steps_m3 == 0 && steps_m1 == 0 && steps_m2 == 0){ 
    }
    else{
      moveMotors(steps_m3,steps_m1,steps_m2);
      pos_x = steps_m3;
      pos_y = steps_m1;
      pos_z = steps_m2;
      Serial1.println("done");
    }
  }
}

void moveMotors(int x_steps, int y_steps, int z_steps) {
  // Move all three motors simultaneously until one of them reaches its maximum steps
  dist_y = pos_y - y_steps;
  dist_z = pos_z - z_steps;
  dist_x = pos_x - x_steps;
  pos_x = 0;
  pos_y = 0;
  pos_z = 0;
  for (int i = 0; i <= 95000; i++) {
    // Track Motor 3 steps and direction
    if (dist_z<0){
      if (pos_z < abs(dist_z)) {
        rotateMotor(1, 0, 30, STEP_PIN_M2, DIR_PIN_M2); // Move 1 step in Z direction
        pos_z += 1;
      }
    }
    else if(dist_z>0){
      if (pos_z < abs(dist_z)) {
        rotateMotor(1, 1, 30, STEP_PIN_M2, DIR_PIN_M2); // Move 1 step in Z direction
        pos_z += 1;
      }
    }
    if(dist_x<0){
      if (pos_x < abs(dist_x)) {
        rotateMotor(1, 1, 40, STEP_PIN_M3, DIR_PIN_M3); // Move 1 step in X direction
        pos_x += 1;
      }
    }
    else if(dist_x>0){
      //pos_x = 0;
      if (pos_x < abs(dist_x)) {
        rotateMotor(1, 0, 40, STEP_PIN_M3, DIR_PIN_M3); // Move 1 step in X direction
        pos_x += 1;
      }
    }
    if (dist_y<0){
      if (pos_y < abs(dist_y)) {
        rotateMotor(1, 1, 40, STEP_PIN_M1, DIR_PIN_M1); // Move 1 step in Y direction
        pos_y += 1;
      }
    }
    else if (dist_y>0){
      if (pos_y < abs(dist_y)) {
        rotateMotor(1, 0, 40, STEP_PIN_M1, DIR_PIN_M1); // Move 1 step in Y direction
        pos_y += 1;
      }
    }

    // Break the loop if all motors have reached their maximum steps
    if (pos_x == abs(dist_x) && pos_y == abs(dist_y) && pos_z == abs(dist_z)) {
    
      break;
    }
  }
}
