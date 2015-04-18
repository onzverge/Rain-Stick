#include <Servo.h>

/*
PROJECT: Remote Pan/Titlt Platform using SensoDuino
BASED ON: SensoDuino 0.18
PROGRAMMER: Hazim Bitar (techbitar at gmail dot com)
DATE: Oct 22, 2013
FILE: sensoduino_pan_tilt.ino
LICENSE: Public domain
*/

#define START_CMD_CHAR '>'
#define END_CMD_CHAR '\n'
#define DIV_CMD_CHAR ','

#define DEBUG 1 // Set to 0 if you don't want Serial1 output of sensor data

// Create servo objects
Servo panServo;
Servo tiltServo;

// Center servos
int tiltVal = 0; 
int panVal = 180; 

// sensoduino stuff
String inText;
float value0, value1, value2;

void setup() {
   Serial.begin(9600);
   Serial1.begin(9600);
tiltServo.attach(10);
tiltServo.write(0);

  // Attach servo objects to Arduino pins
  panServo.attach(11); 
  tiltServo.attach(10);
  // The default speed of the HC-05  Bluetooth Serial1 modules
  Serial.println("\nSensoDuino 0.18 by TechBitar.com (2013).\n");
  Serial.println("Android Sensor Type No: ");
  Serial.println("1- ACCELEROMETER  (m/s^2 - X,Y,Z)");
  Serial.println("2- MAGNETIC_FIELD (uT - X,Y,Z)");
  Serial.println("3- ORIENTATION (yaw, pitch, roll)");
  Serial.println("4- GYROSCOPE (rad/s - X,Y,Z)");
  Serial.println("5- LIGHT (SI lux)");
  Serial.println("6- PRESSURE (hPa millibar)");
  Serial.println("7- DEVICE TEMPERATURE (C)");
  Serial.println("8- PROXIMITY (Centimeters or 1,0)");
  Serial.println("9- GRAVITY (m/s^2 - X,Y,Z)");
  Serial.println("10- LINEAR_ACCELERATION (m/s^2 - X,Y,Z)");
  Serial.println("11- ROTATION_VECTOR (X,Y,Z)" );
  Serial.println("12- RELATIVE_HUMIDITY (%)");
  Serial.println("13- AMBIENT_TEMPERATURE (C)");
  Serial.println("14- MAGNETIC_FIELD_UNCALIBRATED (uT - X,Y,Z)");
  Serial.println("15- GAME_ROTATION_VECTOR (X,Y,Z)");
  Serial.println("16- GYROSCOPE_UNCALIBRATED (rad/s - X,Y,Z)");
  Serial.println("17- SIGNIFICANT_MOTION (1,0)");
  Serial.println("97 - AUDIO (Vol.)");
  Serial.println("99 - GPS2 (bearing, speed, date/time)");
  Serial.println("\n\nNOTE: IGNORE VALUES OF 99.99\n\n");
  //Serial.flush();
}


void loop()
{
  Serial1.flush();
  int inCommand =0;
  int sensorType = 0;
  unsigned long logCount = 0L;

  char getChar = ' ';  //read Serial1

  // wait for incoming data
  if (Serial1.available() < 1) return; // if Serial1 empty, return to loop().

  // parse incoming command start flag
  getChar = Serial1.read();
  if (getChar != START_CMD_CHAR) return; // if no command start flag, return to loop().

  // parse incoming pin# and value 
  sensorType = Serial1.parseInt(); // read sensor typr
  logCount = Serial1.parseInt();  // read total logged sensor readings
  value0 = Serial1.parseFloat();  // 1st sensor value
  value1 = Serial1.parseFloat();  // 2rd sensor value if exists
  value2 = Serial1.parseFloat();  // 3rd sensor value if exists

  // send sensoduino readings to Serial1 monitor/terminal
  if (DEBUG) {
    Serial.print("Sensor type: ");
    Serial.println(sensorType);
    Serial.print("Sensor log#: ");
    Serial.println(logCount);
    Serial.print("Val[0]: ");
    Serial.println(value0);
    Serial.print("Val[1]: ");
    Serial.println(value1);
    Serial.print("Val[2]: ");
    Serial.println(value2);
    Serial.println("-----------------------");
    delay(10);
  }

// Check sensor type. If not for  Accelerometer (#1) then ignore readings
// sensorType 1 is the Accelerometer sensor

  if (sensorType !=1) return;   

  panVal = value0; // value0 = X sensor reading
  tiltVal = value1;  // value1 = Y sensor reading

  tiltVal = map(tiltVal, -9, 9, 0, 90);   // Map Accelerometer Y value to tilt servo angle. 
  tiltServo.write(tiltVal);
  delay(10);
  Serial.print("tiltVal:");
  Serial.println(tiltVal);
  panVal = map(panVal, 9.6, -10.25, 0, 90);  // Map Accelerometer X value to pan servo angle.
  panServo.write(panVal);     
  delay(10); 
  Serial.print("panVal:");
  Serial.println(panVal);
}
