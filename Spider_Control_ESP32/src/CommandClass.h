//#include <Arduino.h>
#include "SpiderClass.h"

class Command {
    #define MAX_NUM_OF_PARAMS 5 // assuming no more than 5 parameters
    #define MAX_LENGTH_OF_PARAM 10 // the size of longest param
    #define DUMB_VALUE 9999  // not a valid value
    #define IS_COMMENT 0
    #define IS_COMMAND 1
    #define IS_ILLEGAL  2
    #define SPACE   ' '
    #define NO_SPACE -1
    #define CMD_LENGTH 30
    #define NUM_OF_COMMANDS 5
    #define NUM_OF_LED_PARAMS 3     // including the command
    #define NUM_OF_MOTOR_PARAMS 5   // including the command

    public:
    char input_line[10]="no no";
    char cmd_word[CMD_LENGTH]=""; // the command name 
    int valid_cmd_num; // the number of the valid command (each command has a number associated)
    int num_of_params = 0; 
    bool valid_cmd;
     int line_type; // holds the type of the line: command/comment/ilegal
    String cmd_str = "";
    bool valid_parameters = false;
    bool valid_command_found = false;
    char cmd_params[MAX_NUM_OF_PARAMS][MAX_LENGTH_OF_PARAM];
    //String InString;
    Arduino_Servo_cmd_struct servo_cmd = Arduino_Servo_cmd_struct();
    

    //typedef struct
    struct  command_type {
        char cmd[CMD_LENGTH];
        int cmd_num;
        
    } ;
    //}  command_type;
    command_type commands_set[NUM_OF_COMMANDS];

   





    
    

    void init() {
        strcpy(commands_set[0].cmd, "LED");
        commands_set[0].cmd_num = 2;
        strcpy(commands_set[1].cmd, "MOTOR");
        commands_set[1].cmd_num = 3;
        strcpy(commands_set[2].cmd, "TBD1");
        commands_set[2].cmd_num = 0;
        strcpy(commands_set[3].cmd, "TBD2");
        commands_set[3].cmd_num = 0;
        strcpy(commands_set[4].cmd, "TBD3");
        commands_set[4].cmd_num = 0;         
    }


    // ************* New_Parser
    int new_parser(char _input_str[]) {
        
        Serial.print("-------- entering new_parser.  ");
        
        char tmp3_word[CMD_LENGTH];
        memset(tmp3_word, '\0', sizeof(tmp3_word)); // clean for next usage
        strncpy(tmp3_word,_input_str,strlen(_input_str));
        Serial.print("tmp3_word:  ");
        Serial.println(tmp3_word);
        
        const char s[2] = " ";
        char *token;
        
        /* get the first token */
        token = strtok(tmp3_word, s);
        num_of_params = 0;       

        /* walk through other tokens */
        while( token != NULL ) {
            Serial.print("token: ");
            Serial.println(token);          

            memset(cmd_params[num_of_params], '\0', sizeof(cmd_params[num_of_params])); // clean for next usage
            strncpy(cmd_params[num_of_params],token,strlen(token));
            Serial.print("current param: ");
            Serial.println(cmd_params[num_of_params]);
            num_of_params++;
            
            token = strtok(NULL, s);
        } // of WHILE
        Serial.println("");
        for (int i=0;i<num_of_params;i++) {
            Serial.print("Param mum: ");
            Serial.println(i);
            Serial.print("Param value: ");
            Serial.println(cmd_params[i]);            
        }
        bool ret_val;
        if (num_of_params>0) 
            ret_val = true;
        else
            ret_val = false;
        
        return ret_val;

    } // of NEW_PARSER

    
      
    //int space_cnt = 0;
    

    bool analyze() {
        Serial.print("In analyze.    ");

        
        //bool found_command = false;
        valid_command_found = false;

        // analyzing the command               

        if (cmd_params[0][0] == '#' ) { // # is a comment
            line_type = IS_COMMENT;
            Serial.println(">> in analyze: COMMENT");
            valid_command_found = false;
            return IS_COMMENT;
        } // of IF

        // Should be a command
        Serial.println(">>  in analyze: Valid command.");
        Serial.print(">> input command string: ");
        Serial.println(cmd_params[0]);

        Serial.print("commands available: ");
        for (int i=0;i<NUM_OF_COMMANDS;i++) {
            Serial.print(commands_set[i].cmd);
        }

        Serial.println("searching for matching command: ");
        for (int i=0;i<NUM_OF_COMMANDS;i++) {
            if(strcmp(cmd_params[0], commands_set[i].cmd) == 0) {
                valid_cmd_num = i; // the number of the command in the commands set
                valid_command_found = true;
                Serial.print(commands_set[i].cmd);
            } // of IF          
        } // of FOR loop

        if (valid_command_found == false) {
            line_type = IS_ILLEGAL;
            return IS_ILLEGAL;
        }

        line_type = IS_COMMAND;

        // check command parfameters
        return IS_COMMAND;
    
    }; // of analyze

    // ******** execute_command ************
    void execute_command(spider_anatomy _spider) {
        Serial.print(" *** in execute_command. the command is: ");
        Serial.println(cmd_params[0]);

        switch (valid_cmd_num) {
            case 0:
                //Serial.println(" *** in case,  LED command detected ");
                execute_led_command(_spider);
                break;
            case 1:
                Serial.println(" *** in case,  MOTOR command detected");
                execute_motor_command(_spider);
                break;
            default:
                Serial.println(" *** in case,  ERROR: default, unknown command");
                break;
        } // of CASE


    } // of execute_command()

    // ************************************************

    void execute_motor_command(spider_anatomy __spider) {
        valid_parameters = true;
        Serial.print("In execute_motor_command. params: "); 
        for (int i=0;i<NUM_OF_MOTOR_PARAMS;i++) {
            Serial.print(cmd_params[i]);
            Serial.print(" ");    
        }
        Serial.println();
        
        if (num_of_params!=NUM_OF_MOTOR_PARAMS) {
            Serial.println("ERROR: 5 pamaters are required for MOTOR command (including the command)");                
            return;
        } // of if
            
        // check params are numeric
        for (int i=1;i<NUM_OF_MOTOR_PARAMS;i++) { // first parameter is the command
            if (!is_str_numeric(cmd_params[i])) {
                Serial.print("ERROR in MOTOR command: not a numeric parameter. i: ");                
                Serial.println(cmd_params[i]);
                return;     
            } // of if
        } // of for loop
       
            
        int leg_num   = atoi(cmd_params[1]);
        int motor_num = atoi(cmd_params[2]);
        int angle_num = atoi(cmd_params[3]);
        int time_num  = atoi(cmd_params[4]);

        Serial.print("leg/motor/angle/time: ");
        Serial.print(leg_num);
        Serial.print(" / ");
        Serial.print(motor_num);
        Serial.print(" / ");
        Serial.print(angle_num);
        Serial.print(" / ");
        Serial.println(time_num);

        servo_cmd.servo_num = leg_num*3 + motor_num;
        servo_cmd.servo_angle = angle_num;
        servo_cmd.wait_time = time_num;
        servo_cmd.valid = true;

        Serial.print("servo num: ");
        Serial.println(servo_cmd.servo_num);
        
        __spider.left_back_leg.motor[motor_num].set_motor_to_angle(__spider.servos_control,servo_cmd);

        //__spider.left_back.motor.

        // TBD: activate servo motor accordingly
             
    } // of execute_motor_command()


    void execute_led_command(spider_anatomy __spider) {

        Serial.print("starting execute_led_command     ");
        Serial.print("params[0]: "); 
        Serial.print(cmd_params[0]);
        Serial.print(" params[1]: ");
        Serial.print(cmd_params[1]);
        Serial.print(" params[2]: ");
        Serial.print(cmd_params[2]);
        Serial.print(" num_of_params: ");
        Serial.println(num_of_params);

            if (num_of_params!=3) { // the command itself is the first param
                valid_parameters = false;
                Serial.println("ERROR: 2 pamaters are required for LED command");                
                return;
            } // of if
            // check params and execute the command

            valid_parameters = false;

            bool red_led    = !strcmp(cmd_params[1],"RED");  // returns 0 if equal
            bool yellow_led = !strcmp(cmd_params[1],"YELLOW");
            bool green_led  = !strcmp(cmd_params[1],"GREEN");

            bool cmd_on  = !strcmp(cmd_params[2],"ON");
            bool cmd_off = !strcmp(cmd_params[2],"OFF");

            // check for errors
            if (!red_led && !yellow_led && !green_led) {
                Serial.println("***ERROR: no led was selected");
                return;
            }

            if (!cmd_on && !cmd_off) {
                Serial.println("ERROR: LED command must be ON or OFF");
                return;
            }

            // start executing
            if (green_led && cmd_off) {
                Serial.print("GREEN OFF. leg: ");
                Serial.println(__spider.leds.led_green);
                __spider.leds.set_led_off(__spider.leds.led_green);
                return;
            }

            if (green_led && cmd_on) {
                Serial.print("GREEN ON. leg: ");
                Serial.println(__spider.leds.led_green);
                __spider.leds.set_led_on(__spider.leds.led_green);
                return;
            }

            if (yellow_led && cmd_off) {
                Serial.print("YELLOW OFF. leg: ");
                Serial.println(__spider.leds.led_yellow);
                __spider.leds.set_led_off(__spider.leds.led_yellow);
                return;
            }

            if (yellow_led && cmd_on) {
                Serial.print("YELLOW ON. leg: ");
                Serial.println(__spider.leds.led_yellow);
                __spider.leds.set_led_on(__spider.leds.led_yellow);
                return;
            }
            if (red_led && cmd_off) {
                Serial.print("RED OFF. leg: ");
                Serial.println(__spider.leds.led_red);
                __spider.leds.set_led_off(__spider.leds.led_red);
                return;
            }

            if (red_led && cmd_on) {
                Serial.print("RED ON. leg: ");
                Serial.println(__spider.leds.led_red);
                __spider.leds.set_led_on(__spider.leds.led_red);
                return;
            }

            // shouldnt arrive this point
            Serial.println("What am I doing here???");
    } // of execute_led_command()


    bool is_str_numeric(String _instr) {
        
        for (unsigned int i=0;i<_instr.length();i++) {
            if (!isdigit(_instr.charAt(i))) {
                return false;
            }
        }

        return true;
    }

   

}; // of class command