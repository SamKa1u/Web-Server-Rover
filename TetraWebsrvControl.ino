#include <hardware/uart.h>

//uart instance and pins
#define TX_PIN 4
#define RX_PIN 5
#define BAUD_RATE 9600
#define UART_ID uart1

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
#define IRL 2
#define IRR 16

//Define variables
int msg, IRL_sig, IRR_sig;
char obs;

// Buffer for incoming data
char buffer[100];
int bufferIndex = 0;

void setup() {
  Serial.begin(115200);

//initialize UART1
// Initialize UART1
  uart_init(UART_ID, BAUD_RATE);
  gpio_set_function(TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(RX_PIN, GPIO_FUNC_UART);
  Serial.println("RP2040 UART Comms initialized");

//initialize Motors as output
  pinMode(M1A,OUTPUT);
  pinMode(M2A,OUTPUT);
  pinMode(M1B,OUTPUT);
  pinMode(M2B,OUTPUT);

//initialize Infrared pins as inputs
  pinMode(IRL,INPUT);
  pinMode(IRR,INPUT);
}

void loop() {

  // Check for incoming data
  if (uart_is_readable(UART_ID)) {    
    while (uart_is_readable(UART_ID) && bufferIndex < 99) {
      buffer[bufferIndex] = uart_getc(UART_ID);
      bufferIndex++;
    }
    // End of command
    buffer[bufferIndex] = '\0';
    processCommand(buffer);
    bufferIndex = 0;
    
  }
  delay(50);
}

void processCommand(const char* cmd) {
  // Check if it's a joystick command (J:X,Y)
  if (cmd[0] == 'J' && cmd[1] == ':') {
    // Parse X,Y values
    int x = 0, y = 0;
    sscanf(&cmd[2], "%d,%d", &x, &y);
    
    Serial.print("Received joystick command: X=");
    Serial.print(x);
    Serial.print(", Y=");
    Serial.println(y);
    
    // Convert joystick values to motor control
    setMotors(y, x);

  }
}
int tolerance(int value) {
  if (value > -10 && value < 10) {
    return 0;
  }
  else {
    return value;
  }
}
void setMotors(int X, int Y) {
  // Y - forward/back
  // X - turning
  X = tolerance(X);
  Y = tolerance(Y);

  int leftSpeed = Y + X;
  int rightSpeed = Y - X;
  //echo command
  char leftspd = leftSpeed;
  char rightspd = rightSpeed; 
  Serial.print("Echoed Left speed:");
  Serial.print(leftspd+);
  Serial.print(" Echoed right speed:");
  Serial.println(rightspd));
  uart_puts(UART_ID, leftspd+rightspd+"\n");
  leftSpeed = constrain(leftSpeed, -100, 100);
  rightSpeed = constrain(rightSpeed, -100, 100);

  //Drive motors
  setLeft(leftSpeed);
  setRight(rightSpeed);
}

// M1
void setLeft(int speed){
  if (speed > 0) {
    //Forwards
    digitalWrite(M1A, LOW);
    digitalWrite(M1B, map(speed, 0, 100, 0, 255));
  }
  else if (speed < 0) {
    //Backwards
    digitalWrite(M1A, map(abs(speed), 0, 100, 0, 255));
    digitalWrite(M1B, LOW);
  }
  else{
    digitalWrite(M1A, LOW);
    digitalWrite(M1B, LOW);
  }
}
// M2
void setRight(int speed){
  if (speed > 0) {
    //Forwards
    digitalWrite(M2A, LOW);
    digitalWrite(M2B, map(speed, 0, 100, 0, 255));
  }
  else if (speed < 0) {
    //Backwards
    digitalWrite(M2A, map(abs(speed), 0, 100, 0, 255));
    digitalWrite(M2B, LOW);
  }
  else{
    digitalWrite(M2A, LOW);
    digitalWrite(M2B, LOW);
  }
}

// Autonomous Motor Control
void sentry() {
  Serial.println("sen");
  IRR_sig = !digitalRead(IRL);
  IRL_sig = !digitalRead(IRR);

  // Process sensor data here.
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

void reverse(){
  Serial.println("rv");
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);
}
void left(){
  Serial.println("l");
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);
}
void right(){
  
  Serial.println("rg");
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);
}
void forwards(){
  
  Serial.println("f");
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);
}
void stop(){
  Serial.println("sp");
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, LOW);
}


