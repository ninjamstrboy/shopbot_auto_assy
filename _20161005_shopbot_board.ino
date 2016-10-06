// bugs: when you call bolt, sometimes it throws in a jaws open immediately before

#include <Servo.h>
#ifndef ARDPRINTF
#define ARDPRINTF
#define ARDBUFFER 16
#include <stdarg.h>
#include <Arduino.h>

// mount the servos
Servo stinger_servo;
Servo gripper_servo;


 ////////////////////////////////////////////////////////
 ///////////Set all the variables and flags /////////////
 ////////////////////////////////////////////////////////

// Output channels from the Shopbot
int shopbot_OP2 = A0;
int shopbot_OP3 = A1;
int shopbot_OP5 = A2;
int shopbot_OP6 = A3;
int shopbot_OP7 = A7;
float shopbot_OP2_value = 0;
float shopbot_OP3_value = 0; 
float shopbot_OP5_value = 0; 
float shopbot_OP6_value = 0;
float shopbot_OP7_value = 0;

// flags to run the programs
bool program_1 = false;
bool program_2 = false;
bool program_3 = false;
bool program_4 = false;
bool program_5 = false;
bool program_6 = false;
bool program_7 = false;
bool program_8 = false;
bool program_9 = false;

// set flags for end effector opposite to what they should be at the start
// not using these for now, setting the program_1 flags above seems to be sufficient
bool jaws_open_flag = true;
bool stinger_engaged_flag = false;
bool gripper_engaged_flag = false;

//Motor B - linkage/jaws of life motor
int PWMB = 9; //Speed control
int BIN1 = 7; //Direction
int BIN2 = 8; //Direction
int close_time = 750;
int open_time = 500;
int small_time = 200;
int close_time_half = 425;
int open_time_half = 345; //previous 375


//Motor A - Phillips screwdriver3rtfgh3rtfgh3rtfgh3rtfgh3rtfgh3rtfgh
int PWMA = 10; //Speed control
int AIN1 = 12; //Direction
int AIN2 = 11; //Direction
int STBY = 13;
int driving_time = 6000;

// servos pinout location
int stinger_arduino_pin = 3;
int stinger_engaged = 1850; // in microseconds
int stinger_disengaged = 1050; // in microseconds

int gripper_arduino_pin = 5;
int gripper_engaged = 40; // in degrees 65
int gripper_disengaged = 110; // in degrees 125


 //////////////////////////////
 ///////////Setup /////////////
 //////////////////////////////

void setup() {
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  Serial.begin(9600);
  
  stinger_servo.attach(stinger_arduino_pin);
  gripper_servo.attach(gripper_arduino_pin); 
  stinger_servo.writeMicroseconds(1050);
  gripper_servo.write(gripper_disengaged);
  
  Serial.println("starting program...");
}


 //////////////////////////////
 ///////////Main loop//////////
 //////////////////////////////

bool isLastStateReading=false;

void loop() {

  shopbot_OP6_value = analogRead(shopbot_OP6);
  delay(10); //maybe there is a better way to clear the multiplexer?
  shopbot_OP6_value = analogRead(shopbot_OP6);

  //Serial.print("(OP2, OP3, OP5, OP6) ");
  //ardprintf("%f %f %f %f", shopbot_OP2_value, shopbot_OP3_value, shopbot_OP5_value, shopbot_OP6_value);
  //Serial.println();

  //delay(1000);
  
// pause Arduino in loop while the Shopbot sets the outputs (as they don't all get set simultaneously
// which can cause the Arduino to interpret them incorrectly

  if (shopbot_OP6_value < 950 && isLastStateReading==false) {
     isLastStateReading=true;    
    
      shopbot_OP2_value = analogRead(shopbot_OP2);
      delay(10);
      shopbot_OP3_value = analogRead(shopbot_OP3);
      delay(10);
      shopbot_OP5_value = analogRead(shopbot_OP5);
      delay(10);
      shopbot_OP7_value = analogRead(shopbot_OP7);
      
  /////////////////////////////
 ///program 1 = open jaws ////
 /////////////////////////////
 
  if (shopbot_OP2_value > 950 && shopbot_OP3_value < 950 && shopbot_OP5_value < 950 && shopbot_OP7_value < 950) {
    program_1 = true;}
  else {
    program_1 = false;}
  
  if (program_1 == true){
      Serial.println("Opening jaws");
      move(2, 255, 0); //motor 2, full speed, left
      delay(open_time); //go for 1 second
      stop();
      program_1 = false; 
      shopbot_OP2_value = 0;
      shopbot_OP3_value = 0;
      shopbot_OP5_value = 0;
      shopbot_OP7_value = 0;
      }
       
    
 //////////////////////////////
 ///program 2 = jaws close ////
 /////////////////////////////
 
  if (shopbot_OP2_value < 950 && shopbot_OP3_value > 950 && shopbot_OP5_value < 950 && shopbot_OP7_value < 950) {
    program_2 = true;}
  else {
    program_2 = false;}
  
  if (program_2 == true) {
     Serial.println("Closing jaws");
     move(2, 255, 1); //motor 2, full speed, left
     delay(close_time); //go for 1 second
     stop();
     program_2 = false; 
     shopbot_OP2_value = 0;
     shopbot_OP3_value = 0;
     shopbot_OP5_value = 0;
     shopbot_OP7_value = 0;}

  
//////////////////////////////
 ///program 3 = bolt ////
 /////////////////////////////
  if (shopbot_OP2_value > 950 && shopbot_OP3_value > 950 && shopbot_OP5_value < 950 && shopbot_OP7_value < 950) {
    program_3 = true;}
  else {
    program_3 = false;}
     
  if (program_3 == true) {
     Serial.print("Bolting");
     Serial.println();
     stinger_servo.writeMicroseconds(stinger_engaged);; //motor 2, full speed, left
     delay(1000);
     move(1, 255, 0); //motor 2, full speed, left
     delay(driving_time); //go for 1 second
     stop(); //stop       
     stinger_servo.writeMicroseconds(stinger_disengaged); //motor 2, full speed, left
     delay(1000); //go for 1 second
     program_3 = false;
     shopbot_OP2_value = 0;
     shopbot_OP3_value = 0;
     shopbot_OP5_value = 0;
     shopbot_OP7_value = 0;}

////////////////////////////////////
 ///program 4 = engage gripper ////
 /////////////////////////////////

  if (shopbot_OP2_value < 950 && shopbot_OP3_value < 950 && shopbot_OP5_value > 950 && shopbot_OP7_value < 950) {
     program_4 = true;}
  else {
    program_4 = false;}

  if (program_4 == true) {
      Serial.print("Engage gripper");
      Serial.println();
      gripper_servo.write(gripper_engaged);
      delay(2000);
      program_4 = false;
      shopbot_OP2_value = 0;
      shopbot_OP3_value = 0;
      shopbot_OP5_value = 0;
      shopbot_OP7_value = 0;}


//////////////////////////////////////
 ///program 5 = disengage gripper ////
 //////////////////////////////////////
 
  if (shopbot_OP2_value > 950 && shopbot_OP3_value < 950 && shopbot_OP5_value > 950 && shopbot_OP7_value < 950) {
    program_5 = true;}
  else {
    program_5 = false;}

  if (program_5 == true) {
      Serial.print("Disengage gripper");
      Serial.println();
      gripper_servo.write(gripper_disengaged);
      delay(2000);
      program_5 = false;
      shopbot_OP2_value = 0;
      shopbot_OP3_value = 0;
      shopbot_OP5_value = 0;
      shopbot_OP7_value = 0;}


 /////////////////////////////
 ///program 6 = open jaws half ////
 /////////////////////////////
 
  if (shopbot_OP2_value < 950 && shopbot_OP3_value > 950 && shopbot_OP5_value > 950 && shopbot_OP7_value < 950) {
    //Serial.println("checking");
    program_6 = true;}
  else {
    program_6 = false;}
  
  if (program_6 == true){
      Serial.println("Opening jaws half");
      move(2, 255, 0); //motor 2, full speed, left
      delay(open_time_half); //go for 1 second
      stop();
      program_6 = false;
      shopbot_OP2_value = 0;
      shopbot_OP3_value = 0;
      shopbot_OP5_value = 0;
      shopbot_OP7_value = 0;}

    
 //////////////////////////////
 ///program 7 = jaws close half ////
 /////////////////////////////
 
  if (shopbot_OP2_value > 950 && shopbot_OP3_value > 950 && shopbot_OP5_value > 950 && shopbot_OP7_value < 950) {
    program_7 = true;}
  else {
    program_7 = false;}
  
  if (program_7 == true) {
     Serial.println("Closing jaws half");
     move(2, 255, 1); //motor 2, full speed, left
     delay(close_time_half); //go for 1 second
     stop();
     program_7 = false;
     shopbot_OP2_value = 0;
     shopbot_OP3_value = 0;
     shopbot_OP5_value = 0;
     shopbot_OP7_value = 0;}


//////////////////////////////
 ///program 8 = unbolt ////
 /////////////////////////////
  
  if (shopbot_OP2_value < 950 && shopbot_OP3_value < 950 && shopbot_OP5_value < 950 && shopbot_OP7_value > 950) {
    program_8 = true;}
  else {
    program_8 = false;}
     
  if (program_8 == true) {
     Serial.print("Bolting");
     Serial.println();
     stinger_servo.writeMicroseconds(stinger_engaged);; //motor 2, full speed, left
     delay(1000);
     move(1, 255, 1); //motor 2, full speed, left
     delay(driving_time); //go for 1 second
     stop(); //stop       
     stinger_servo.writeMicroseconds(stinger_disengaged); //motor 2, full speed, left
     delay(1000); //go for 1 second
     program_8 = false;
     shopbot_OP2_value = 0;
     shopbot_OP3_value = 0;
     shopbot_OP5_value = 0;
     shopbot_OP7_value = 0;}
  }
  else{
    if(shopbot_OP6_value > 950 && isLastStateReading==true){
      isLastStateReading=false;}
  }
  delay(10);  

  

///////////////////////////////////////
/////Serial monitor values /////////////
////////////////////////////////////////

  if(Serial.available() > 0){
    char c = Serial.read();
    
    if (c == 'c'){
      Serial.println("close");
      move(2, 255, 1); //motor 2, full speed, left
      delay(close_time); //go for 1 second
      stop(); } 

    if (c == 'q'){
      Serial.println("close");
      move(2, 255, 1); //motor 2, full speed, left
      delay(close_time_half); //go for 1 second
      stop(); } 
    
    if (c == 'o'){
      Serial.println("open");
      move(2, 255, 0); //motor 2, full speed, left
      delay(open_time); //go for 1 second
      stop(); }

    if (c == 'h'){
      move(2, 255, 0); //motor 2, full speed, left
      delay(open_time_half); //go for 1 second
      stop(); }

    if (c == 'l'){
      Serial.println("close");
      move(2, 255, 1); //motor 2, full speed, left
      delay(close_time_half); //go for 1 second
      stop(); } 
    
    if (c == 'k'){
      move(2, 255, 0); //motor 2, full speed, left
      delay(open_time_half); //go for 1 second
      stop(); }
    
    if (c == 'b'){
      Serial.println("Bolt");
      stinger_servo.writeMicroseconds(1850);; //motor 2, full speed, left
      delay(1000);
      move(1, 255, 0); //motor 2, full speed, left
      delay(driving_time); //go for 1 second
      stop(); //stop     
      stinger_servo.writeMicroseconds(1150); //motor 2, full speed, left
      delay(1000); //go for 1 second 
      stinger_servo.write(0); } //motor 2, full speed, left
   
    if (c == 'd'){
      Serial.println("Unbolt");
      stinger_servo.writeMicroseconds(1850);; //motor 2, full speed, left
      delay(1000);
      move(1, 255, 1); //motor 2, full speed, left
      delay(6000); //go for 1 second
      stop(); //stop     
      stinger_servo.writeMicroseconds(1150); //motor 2, full speed, left
      delay(1000); //go for 1 second 
      stinger_servo.write(0);} //motor 2, full speed, left
    
    if(c == 'r'){
      move(1, 255, 1); //motor 2, full speed, left
      delay(1000); //go for 1 second
      stop(); }    
    
    if(c == 't'){
      move(1, 255, 0); //motor 2, full speed, left
      delay(1000); //go for 1 second
      stop(); }

    if(c == 'g'){
      Serial.println("gripping");
      gripper_servo.write(gripper_engaged);
      delay(2000);}
      
    if(c == 'u'){
      gripper_servo.write(gripper_disengaged);
      delay(2000);}}    
}



//////////////////////////////////
///DC motor control function//////
//////////////////////////////////

void move(int motor, int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if (motor == 1) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
    //Serial.println("motor a going");
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
    //Serial.println("motor b going");
  }
}

void stop(){
//enable standby  
  digitalWrite(STBY, LOW); 
}

//////////////////////////////////////////////
// function to print out values more neatly///
//////////////////////////////////////////////

int ardprintf(char *str, ...)
{
  int i, count=0, j=0, flag=0;
  char temp[ARDBUFFER+1];
  for(i=0; str[i]!='\0';i++)  if(str[i]=='%')  count++;

  va_list argv;
  va_start(argv, count);
  for(i=0,j=0; str[i]!='\0';i++)
  {
    if(str[i]=='%')
    {
      temp[j] = '\0';
      Serial.print(temp);
      j=0;
      temp[0] = '\0';

      switch(str[++i])
      {
        case 'd': Serial.print(va_arg(argv, int));
                  break;
        case 'l': Serial.print(va_arg(argv, long));
                  break;
        case 'f': Serial.print(va_arg(argv, double));
                  break;
        case 'c': Serial.print((char)va_arg(argv, int));
                  break;
        case 's': Serial.print(va_arg(argv, char *));
                  break;
        default:  ;
      };
    }
    else 
    {
      temp[j] = str[i];
      j = (j+1)%ARDBUFFER;
      if(j==0) 
      {
        temp[ARDBUFFER] = '\0';
        Serial.print(temp);
        temp[0]='\0';
      }
    }
  };
  Serial.println();
  return count + 1;
}
#undef ARDBUFFER
#endif


