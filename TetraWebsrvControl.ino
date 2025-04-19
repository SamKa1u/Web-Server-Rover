//Motor pins
#define M1A 8
#define M1B 9
#define M2A 10
#define M2B 11
#define Serv0 15
//Control pins 
#define Pin0 28
#define Pin1 7
#define Pin2 26
#define Pin3 6
//Infrared pins
#define IRL 4
#define IRR 16

//Define variables
int msg, IRL_sig, IRR_sig;
char obs;

void setup() {
  Serial.begin(9600);
//initialize Motors as output
  pinMode(M1A,OUTPUT);
  pinMode(M2A,OUTPUT);
  pinMode(M1B,OUTPUT);
  pinMode(M2B,OUTPUT);
//initialize Control pins
  pinMode(Pin0,INPUT);
  pinMode(Pin1,INPUT);
  pinMode(Pin2,INPUT);
  pinMode(Pin3,INPUT);
//initialize Infrared pins as inputs
  pinMode(IRL,INPUT);
  pinMode(IRR,INPUT);

}
/*
  Interpret 4 bit number from ESP as command 
    args: 
      pin0,pin1,pin2,pin3 (bool) : the 4 bit number 
    returns:
      (int) : denotes instruction from webserver
*/
int command() {
  bool pin0 = digitalRead(Pin0);
  bool pin1 = digitalRead(Pin1);
  bool pin2 = digitalRead(Pin2);
  bool pin3 = digitalRead(Pin3);

  if (pin0 == 0 && pin1 == 0 && pin2 == 0 && pin3 == 1 ){
    return 2;
  }
  else if (pin0 == 0 && pin1 == 0 && pin2 == 1 && pin3 == 0 ){
    return 3;
  }
  else if (pin0 == 0 && pin1 == 0 && pin2 == 1 && pin3 == 1 ){
    return 4;
  }
  else if (pin0 == 0 && pin1 == 1 && pin2 == 0 && pin3 == 0 ){
    return 5;
  }
  else if (pin0 == 0 && pin1 == 1 && pin2 == 0 && pin3 == 1 ){
    return 6;
  }
  else if (pin0 == 0 && pin1 == 1 && pin2 == 1 && pin3 == 0 ){
    return 7;
  }
  else if (pin0 == 0 && pin1 == 1 && pin2 == 1 && pin3 == 1 ){
    return 8;
  }
  else if (pin0 == 1 && pin1 == 0 && pin2 == 0 && pin3 == 0 ){
    return 9;
  }
  else if (pin0 == 1 && pin1 == 0 && pin2 == 0 && pin3 == 1 ){
    return 10;
  }
  else if (pin0 == 1 && pin1 == 0 && pin2 == 1 && pin3 == 0 ){
    return 11;
  }
  else if (pin0 == 1 && pin1 == 0 && pin2 == 1 && pin3 == 1 ){
    return 12;
  }
  else if (pin0 == 1 && pin1 == 1 && pin2 == 0 && pin3 == 0 ){
    return 13;
  }
  else if (pin0 == 1 && pin1 == 1 && pin2 == 0 && pin3 == 1 ){
    return 14;
  }
  else if (pin0 == 1 && pin1 == 1 && pin2 == 1 && pin3 == 0 ){
    return 15;
  }
  else if (pin0 == 1 && pin1 == 1 && pin2 == 1 && pin3 == 1 ){
    return 16;
  }
  else {
    return 1;
  }
}
/*
  detect and maneuver around obstacles 
    args: 
      none
    returns:
      void
*/
void sentry() {
  IRR_sig = !digitalRead(IRL);
  IRL_sig = !digitalRead(IRR);

  //Process sensor data
  if (IRL_sig && !IRR_sig ) {
    obs = 'l';
  }
  else if (IRR_sig && !IRL_sig){
    obs = 'r';
  }
  else if (IRL_sig && IRR_sig){
    obs = 'f';
  }
  else {
    obs = 'n';
  }
  //Engage motors according to obstacle location
  switch (obs) {
      case 'n':
        //forwards
      forwards();
      break;
      case 'l':
        //turn right
      left();
      break;
      case 'r':
        //turn left
      right();
      break;
      case 'f':
        //reverse
      reverse();
      break;
    }    
}
//Movement options
void reverse(){
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);
}
void left(){
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);
}
void right(){
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);
}
void forwards(){
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);
}
void stop(){
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, LOW);
}
void loop() {
  // Read the control pins:
  msg = command();
  Serial.println(msg);
  //actuator commands
    switch (msg) {
      case 2:
        //reverse
        reverse();
        break;
      case 3:
        //turn left
        left();
        break;
      case 4:
        //turn right
        right();
        break;
      case 5:
        //forwards
        forwards();
        break;
      case 6:
        //auto
        sentry();
        break;
      default:
        //stop
        stop();
        break;
    }
}

