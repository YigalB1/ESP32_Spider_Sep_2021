
#include "CommandClass.h"

const int red_led_pin = 23;
const int Yellow_led_pin = 19;
const int green_led_pin = 18;

Command current_command = Command();
spider_anatomy my_spider = spider_anatomy();

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // string completly recieved from USB


void setup() {
  pinMode(red_led_pin, OUTPUT);
  pinMode(Yellow_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);

  Serial.begin(9600);
  Serial.println("I am Arduino. I am alive.Starting setup()");
  Serial.println("=========================================");

  my_spider.servos_control.begin();
  my_spider.servos_control.setPWMFreq(60);
  my_spider.leds.led_green = red_led_pin;
  my_spider.leds.led_yellow = Yellow_led_pin;
  my_spider.leds.led_red = green_led_pin;

  my_spider.leds.set_led_on(red_led_pin);
  my_spider.leds.set_led_on(Yellow_led_pin);
  my_spider.leds.set_led_on(green_led_pin);
  delay(500);
  my_spider.leds.set_led_off(red_led_pin);
  my_spider.leds.set_led_off(Yellow_led_pin);
  my_spider.leds.set_led_off(green_led_pin);
  delay(500);
  current_command.init();
} // of SETUP()

void loop() {
  Serial.println("***** startinbg LOOP");
  digitalWrite(red_led_pin, HIGH);  
  digitalWrite(Yellow_led_pin, LOW);  
  digitalWrite(green_led_pin, HIGH);  
  delay(500);                  // wait for a second
   digitalWrite(red_led_pin, LOW);  
  digitalWrite(Yellow_led_pin, HIGH);  
  digitalWrite(green_led_pin, LOW);  
  delay(500);
  Serial.print("** MAIN loop. stringComplete: ");
  Serial.println(stringComplete);

  if(stringComplete) {
    //Serial.println("in loop, new command from USB recieved");
    //current_command.str_in = inputString;
    //current_command.analyze();
    //current_command.execute_command(my_spider);
    //stringComplete = false;
    return;
  } // of IF



  Serial.println("In main LOOP: new_parser: LED RED ON sequence");
  char second_cmd[CMD_LENGTH];
  memset(second_cmd, '\0', sizeof(second_cmd)); // clean for next usage
  strcpy(second_cmd, "LED RED ON");
    
  current_command.new_parser(second_cmd);
  Serial.println("Back from new parser");
  for (int i=0;i<current_command.num_of_params;i++) {
    Serial.print(current_command.params[i]);
    Serial.print(" ");
  } // of FOR loop

  
  Serial.println("");

  delay(200000);
  return;


  Serial.println("In main LOOP: strating LED sequence");
  char first_cmd[] = "LED";
  int ret_value = current_command.parse_it(first_cmd);
  Serial.print("ret value: ");
  Serial.println(ret_value);
  if (ret_value==0) {
    Serial.println("In main LOOP: parse_it OK, starting execute");
    current_command.execute_command(my_spider);
  }
  else {
    Serial.println("In main LOOP: error after parse_it");
  }

  Serial.println("In main LOOP: strating LED RED ON sequence");
  //second_cmd[] = "LED RED ON";
  ret_value = current_command.parse_it(second_cmd);
  Serial.print("ret value: ");
  Serial.println(ret_value);

  delay(200000);
  return;
  
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