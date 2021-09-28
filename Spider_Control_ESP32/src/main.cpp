
#include "CommandClass.h"

const int red_led_pin = 23;
const int yellow_led_pin = 19;
const int green_led_pin = 18;

Command current_command = Command();
spider_anatomy my_spider = spider_anatomy();

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // string completly recieved from USB

void do_it(char _cmd_in[]);

void setup() {
  pinMode(red_led_pin, OUTPUT);
  pinMode(yellow_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);

  Serial.begin(9600);
  Serial.println("I am Arduino. I am alive.Starting setup()");
  Serial.println("=========================================");

  my_spider.servos_control.begin();
  // servo T = 20ms, so freq is 50
  my_spider.servos_control.setPWMFreq(60); 

  my_spider.leds.led_red    = red_led_pin;
  my_spider.leds.led_yellow = yellow_led_pin;
  my_spider.leds.led_green  = green_led_pin;

  my_spider.leds.set_led_on(red_led_pin);
  my_spider.leds.set_led_on(yellow_led_pin);
  my_spider.leds.set_led_on(green_led_pin);
  delay(500);
  my_spider.leds.set_led_off(red_led_pin);
  my_spider.leds.set_led_off(yellow_led_pin);
  my_spider.leds.set_led_off(green_led_pin);
  delay(500);
  current_command.init();
} // of SETUP()


// ********************** LOOP() ************************
void loop() {
  Serial.print("***** startinbg LOOP, stringComplete: ");
  Serial.println(stringComplete);

  //int pulse_l = 170; was 200
  int pulse_l = 125;
  int pulse_h = 575;
  int pulse_m = int((pulse_l+pulse_h)/2);
  int pulse_w = pulse_l;
  while(true) {
    Serial.print(" pulse_w: ");
    Serial.print(pulse_w);
    Serial.print(" ");

    for (int motor_num=0;motor_num<16;motor_num++) {
      Serial.print(motor_num);
      Serial.print(".");
      my_spider.servos_control.setPWM(motor_num,0,pulse_w);
    } // of for loop
    
    delay(3000);
    if (pulse_w==pulse_l) {
      pulse_w = pulse_m;
    }
    else if (pulse_w==pulse_m)
     {
      pulse_w = pulse_h;
    }
    else if (pulse_w==pulse_h) {
      pulse_w = pulse_l;
    }
    Serial.println(" change angle ");
  } // of while

  if(stringComplete) {
    //Serial.println("in loop, new command from USB recieved");
    //current_command.str_in = inputString;
    //current_command.analyze();
    //current_command.execute_command(my_spider);
    //stringComplete = false;
    return;
  } // of IF

  char cmd_line[CMD_LENGTH];
  strcpy(cmd_line,"LED RED ON");
  do_it(cmd_line);
  delay(500);

  strcpy(cmd_line,"LED RED OFF");
  do_it(cmd_line);
  
  strcpy(cmd_line,"LED YELLOW ON");
  do_it(cmd_line);
  delay(500);

  strcpy(cmd_line,"LED YELLOW OFF");
  do_it(cmd_line);
  
  strcpy(cmd_line,"LED GREEN ON");
  do_it(cmd_line);
  delay(500);
  
  strcpy(cmd_line,"LED GREEN OFF");
  do_it(cmd_line);

  Serial.println("-------  starting motors");
  
  String str;
  char tmp_char[5];
  int start,end,count;

  start = 10;
  end = 175;
  count=80;
  while(true) {  

    memset(cmd_line, '\0', sizeof(cmd_line)); // clean for next usage
    strcpy(cmd_line,"MOTOR 0 0 0 1");
    do_it(cmd_line); // leg 0 motor 0 angle 0 time 1

    memset(cmd_line, '\0', sizeof(cmd_line)); // clean for next usage
    strcpy(cmd_line,"MOTOR 1 0 0 1");
    do_it(cmd_line); // leg 1 motor 0 angle 0 time 1

    memset(cmd_line, '\0', sizeof(cmd_line)); // clean for next usage
    strcpy(cmd_line,"MOTOR 1 1 0 1");
    do_it(cmd_line); // leg 1 motor 1 angle 0 time 1

    Serial.print("******* Angle: 0 ");
    delay(5000);

    memset(cmd_line, '\0', sizeof(cmd_line)); // clean for next usage
    strcpy(cmd_line,"MOTOR 0 0 180 1");
    do_it(cmd_line); // leg 0 motor 0 angle 180 time 1

    memset(cmd_line, '\0', sizeof(cmd_line)); // clean for next usage
    strcpy(cmd_line,"MOTOR 1 0 180 1");
    do_it(cmd_line); // leg 1 motor 0 angle 180 time 1

    memset(cmd_line, '\0', sizeof(cmd_line)); // clean for next usage
    strcpy(cmd_line,"MOTOR 1 1 180 1");
    do_it(cmd_line); // leg 1 motor 1 angle 180 time 1

    Serial.println(" .....  Angle: 180");
    delay(5000);
    continue;


    for (int i=start; i<end ; i+=count) {
      memset(tmp_char, '\0', sizeof(tmp_char)); // clean for next usage
      memset(cmd_line, '\0', sizeof(cmd_line)); // clean for next usage
      
      str=String(i);
      str.toCharArray(tmp_char,5);
      Serial.print(" str is: ");
      Serial.print(str);
      Serial.print("  angle is: ");
      Serial.println(tmp_char);

      //strcpy(cmd_line,"MOTOR 0 0 20 1");
      strcpy(cmd_line,"MOTOR 1 0 ");
      strcat(cmd_line,tmp_char);
      strcat(cmd_line," 1");
      Serial.print("motor command line: ");
      Serial.print(cmd_line);
      do_it(cmd_line);
      delay(1000);

    } // of FOR loop
    delay(5000);
  } // of WHILE
  

  Serial.println("Motor 0 ");  
// leg/motor/angle/delay
  strcpy(cmd_line,"MOTOR 0 0 20 1");
  do_it(cmd_line);
  delay(1000);
  
  strcpy(cmd_line,"MOTOR 0 0 100 1");
  do_it(cmd_line);
  delay(1000);

  Serial.println("Motor 1 ");  
  strcpy(cmd_line,"MOTOR 0 1 45 1");
  do_it(cmd_line);
  delay(1000);

  strcpy(cmd_line,"MOTOR 0 1 135 1");
  do_it(cmd_line);
  delay(1000);

  Serial.println("Motor 2 ");  
  strcpy(cmd_line,"MOTOR 0 2 170 1");
  do_it(cmd_line);
  delay(1000);

  strcpy(cmd_line,"MOTOR 0 2 30 1");
  do_it(cmd_line);
  delay(1000);

  delay(200000);

  
} // of LOOP()


void serialEvent() {
  Serial.print("In serialEvent handler. stringComplete: ");
  Serial.println(stringComplete);
  return; // Just for debug, to remove thsi return to allow the USB communication to work
  
  if (stringComplete) 
    return; // at first, stop reading commands until prev command was parsed

  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
} // of serial event


void do_it(char _cmd_in[]) {
  char loc_cmd[CMD_LENGTH]; 

  memset(loc_cmd, '\0', sizeof(loc_cmd)); // clean for next usage
  strcpy(loc_cmd, _cmd_in);  
  current_command.new_parser(loc_cmd);
  int cmd_ret = current_command.analyze();
  if (cmd_ret==IS_COMMAND) {
    // there is a valid command
    current_command.execute_command(my_spider);
  }
}