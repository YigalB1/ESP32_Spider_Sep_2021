#include <Arduino.h>
//#include <servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define NUM_OF_SERVOS_IN_LEG 3
#define NUM_OF_LEGS_IN_SPIDE 4


struct Arduino_Servo_cmd_struct
    {
        bool valid; // if starting with # it is a comment
        int servo_num;
        int servo_angle;
        int wait_time;
    };

class Spider_Servo
    {
        //Adafruit_PWMServoDriver servos_control = Adafruit_PWMServoDriver();
        int servoMIN = 200;
        int servoMAX = 420;
               
        
        Arduino_Servo_cmd_struct Servo_cmd;


        //Arduino_Serial_Port Servo_port;

        public:       
        void set_motor_to_angle (Adafruit_PWMServoDriver _servo_cntrl, Arduino_Servo_cmd_struct _servo_cmd) {
              Serial.print("in set_motor_to_angle. servo num: ");
              Serial.print(_servo_cmd.servo_num);

              int pulselength = map(_servo_cmd.servo_angle, 0, 180, servoMIN, servoMAX);
            // TBD: write values to servo
            _servo_cntrl.setPWM(_servo_cmd.servo_num, 0, pulselength);
            _servo_cntrl.setPWM(0, 0, pulselength); // fpr debug, to remove
            Serial.print("  Angle: ");
            Serial.print(_servo_cmd.servo_angle);
            Serial.print("   Pulse width: ");
            Serial.println(pulselength);
            delay(2000);


        } // of set_motor_to_angle

    }; // of Spider_Servo class
// **************************************************************

    class Spider_Leg
    {
        public:
        Spider_Servo motor[NUM_OF_SERVOS_IN_LEG];        
    }; // of Spider_Leg class
// **************************************************************
    class Spider_Leds
    {
        public:
        int led_red;
        int led_yellow;
        int led_green;
        
        public: void set_led_on(int _led_num) {
            //Serial.print("Led ON, leg: ");
            //Serial.println(_led_num);
            digitalWrite(_led_num, HIGH);
        } // of set_led_on

        public: void set_led_off(int _led_num) {
            //Serial.print("Led OFF, leg: ");
            //Serial.println(_led_num);
            digitalWrite(_led_num, LOW);
        } // of set_led_off
        
    }; // of Spider_Leds class
// **************************************************************
    class spider_anatomy {
        public:
        Adafruit_PWMServoDriver servos_control = Adafruit_PWMServoDriver();    
        Spider_Leds leds = Spider_Leds();
        Spider_Leg left_front_leg = Spider_Leg();
        Spider_Leg left_back_leg = Spider_Leg();
        Spider_Leg right_front_leg = Spider_Leg();
        Spider_Leg right_back_leg = Spider_Leg();
    
    }; // of spider_anatomy class
    