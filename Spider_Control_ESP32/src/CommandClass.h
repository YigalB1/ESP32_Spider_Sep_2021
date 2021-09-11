//#include <Arduino.h>
#include "SpiderClass.h"

class Command {
    #define MAX_NUM_OF_PARAMS 5 // assuming no more than 5 parameters
    #define MAX_LENGTH_OF_PARAM 10 // the size of longest param
    #define DUMB_VALUE 9999  // not a valid value
    #define COMMENT 0
    #define COMMAND 1
    #define ILEGAL  2
    #define SPACE   ' '
    #define NO_SPACE -1
    #define CMD_LENGTH 30
    #define NUM_OF_COMMANDS 5

    public:
    //String inputString;         // a string to hold incoming data
    String str_in = "init value just for debug12345678901234567890";         // a string to hold incoming data
    char input_line[10]="no no";
    //char in_str[20]; // debugging
    char cmd_word[CMD_LENGTH]=""; // the command name 
    int valid_cmd_num; // the number of the valid command (each command has a number associated)

    typedef struct
    {
        char cmd[CMD_LENGTH];
        int num_of_params;
        
    }  command_type;
    command_type commands_set[NUM_OF_COMMANDS];

    
   
    //boolean stringComplete = false;  // whether the string is complete
    int line_type; // holds the type of the line: command/comment/ilegal
    String cmd_str = "";
    int num_of_params = 0;  
    bool valid_parameters = false;
    bool valid_command_found = false;

    char cmd_params[MAX_NUM_OF_PARAMS][MAX_LENGTH_OF_PARAM];
    String params[MAX_NUM_OF_PARAMS];
    //int delimiters[MAX_NUM_OF_PARAMS]; 
    
    String InString;

    Arduino_Servo_cmd_struct servo_cmd = Arduino_Servo_cmd_struct();

    void init() {
        strcpy(commands_set[0].cmd, "LED");
        commands_set[0].num_of_params = 2;
        strcpy(commands_set[1].cmd, "MOTOR");
        commands_set[1].num_of_params = 3;
        strcpy(commands_set[2].cmd, "TBD1");
        commands_set[2].num_of_params = 0;
        strcpy(commands_set[3].cmd, "TBD2");
        commands_set[3].num_of_params = 0;
        strcpy(commands_set[4].cmd, "TBD3");
        commands_set[4].num_of_params = 0;
         

    }

    // ****** find_next_space *******
    int find_next_space(char _in_str[]) {
        // returns the first space
        Serial.print("entering find_next_space.  ");
        Serial.print("_in_str parameter is: ");
        Serial.print(_in_str);
        Serial.print("_in_str length is: ");
        Serial.print(strlen(_in_str));
        //Serial.print("_in_str size is: ");
        //Serial.print(sizeof(_in_str[]));
        
        for (unsigned int loc=0;loc<strlen(_in_str);loc++) {
            if(_in_str[loc]==SPACE)
                return loc;
        } // of for loop
        return NO_SPACE;
    } // of find_next_space

    // ************* New_Parser
    void new_parser(char _input_str[]) {
        
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

    } // of NEW_PARSER

    // ****** Parse_it *******
    int parse_it(char _msg_in[]) {
        // return 0 if OK, error code if error

        Serial.print("parse_it parameter is: ");
        Serial.print(_msg_in);
        Serial.print(".. length is: ");
        Serial.println(strlen(_msg_in));

        // check first char before pasing
        if (_msg_in[0] == '#') {
            line_type = COMMENT;
            Serial.println("COMMENT - ignoring line");
            return 1;
        }
        if (_msg_in[0] == ' ' ) {
            line_type = ILEGAL;
            Serial.println("ILEGAL - first char is space");
            return 2;
        }
        if (strlen(_msg_in) == 0 ) { 
            line_type = ILEGAL;
            Serial.println("ILEGAL - empty line");
            return 3;
        }

        int nxt_space = find_next_space(_msg_in);
        int length = strlen(_msg_in);
        Serial.print("next space detected: ");
        Serial.print(nxt_space);
        Serial.print("  length of the word: : ");
        Serial.println(length);

        if (nxt_space==NO_SPACE) {
            // no space, so one word command
            strncpy(cmd_word,_msg_in,strlen(_msg_in)); // copy the message 
            Serial.print("in parse_it, one word command. ");
            return 0;
        }
        
        // a cmd with parameters to parse
        strncpy(cmd_word,_msg_in,nxt_space); // copy the command part from the message
        Serial.print("in parse_it, spaces detected. ");
            
        
        Serial.print("cmd_word is: ");
        Serial.println(cmd_word);
        // remove the cmd word from the input message for farther processing (parameters)
        char tmp1_word[CMD_LENGTH];
        memset(tmp1_word, '\0', sizeof(tmp1_word));
        strncpy(tmp1_word,_msg_in+nxt_space+1,strlen(_msg_in)-nxt_space-1);
        Serial.print("_msg_in: ");
        Serial.print(_msg_in);
        Serial.print("   nxt_space: ");
        Serial.print(nxt_space);
        Serial.print("  tmp1_word: ");
        Serial.println(tmp1_word);
        Serial.print("  tmp1_word length: ");
        Serial.println(strlen(tmp1_word));




        Serial.println(" ^^^^ entering newer WHILE loop ");
        num_of_params =0;
        char tmp2_word[CMD_LENGTH];
        Serial.print("  tmp1_word length: ");
        Serial.println(strlen(tmp1_word));
        Serial.print("  tmp2_word length: ");
        Serial.println(strlen(tmp2_word));
        memset(tmp2_word, '\0', sizeof(tmp2_word));
        while (strlen(tmp1_word) > 0) {
            Serial.print("  tmp1_word: ");
            Serial.print(tmp1_word);
            Serial.print("  tmp1_word length: ");
            Serial.println(strlen(tmp1_word));
            // removing leading spaces, just in case
            while (tmp1_word[0]==SPACE) {
                Serial.print(" in inner while.  tmp1_word: ");
                Serial.println(tmp1_word);
                strncpy(tmp2_word,tmp1_word,strlen(tmp1_word)-1); // remove leading spaces using tmp2 variable
                memset(tmp1_word, '\0', sizeof(tmp1_word));
            } // of inner WHILE loop
            // now next parameter is ready to be parsed
            Serial.println("DEBUG: going to find_next_space ");
            int nxt_space = find_next_space(tmp1_word);
            if (nxt_space==NO_SPACE) {
                // no more spaces, this is the last parameter
                // copy it into the parameters array
                memset(cmd_params[num_of_params], '\0', sizeof(cmd_params[num_of_params]));
                strncpy(cmd_params[num_of_params], tmp1_word,strlen(cmd_params[num_of_params]));
                num_of_params++;
            } // of IF
            else {
                // we have more spaces, so more than one parameter to go
                memset(cmd_params[num_of_params], '\0', sizeof(cmd_params[num_of_params])); 
                strncpy(cmd_params[num_of_params],tmp1_word,nxt_space-1); 
                int len_of_param = strlen(cmd_params[num_of_params]);
                for (int i=0;i<strlen(tmp1_word)-len_of_param;i++) {
                    // remove the param from the string
                    tmp2_word[i] = tmp1_word[i+len_of_param];
                    memset(tmp1_word, '\0', CMD_LENGTH);
                    strncpy(tmp1_word,tmp2_word,strlen(tmp2_word));


                }
            }
        } // of while 
        Serial.print("finished WHILE.  ");
        Serial.print("num_of_params: ");
        Serial.print(num_of_params);
        for (int i=0;i<num_of_params;i++) {
            Serial.print(". param: ");
            Serial.print(cmd_params[i]);
        }
        Serial.println("");

        

        // should be a valid command here
        //strncpy(cmd_word,_msg_in,strlen(_msg_in));
        
        valid_cmd_num = 999; // dumb value 
        bool cmd_found = false;
        for (int i=0;i<NUM_OF_COMMANDS;i++) {
            Serial.print("i: ");
            Serial.print(i);
            Serial.print("  command : ");
            Serial.println(commands_set[i].cmd);
            if ( strcmp(cmd_word, commands_set[i].cmd) == 0) {
                Serial.print(">>>>> Command was found: ");
                Serial.print(cmd_word);
                Serial.print(" command num: ");
                Serial.println(i);
                cmd_found = true;
                valid_cmd_num = i;
            } // of if
        } // of for loop
        if (!cmd_found) {
            Serial.print("ERROR: not a valid command ");
            return 1;
        }
        
        return 0;  

    } // of parse_it

        
  
  
  int space_cnt = 0;

    

    void init_me() {
        //str_in.reserve(100);
        str_in = "";
        Serial.print("Starting init_me in Command class");
        Serial.print("    str_in length: ");
        Serial.print(str_in.length());
        Serial.print("    str_in value:  ");
        Serial.println(str_in);

        /*
        for ( int i = 0; i < MAX_NUM_OF_PARAMS; i++)  {
            delimiters[i]=DUMB_VALUE; // aka no value
        } // of for loop
        */
    }; // of init()

    void analyze() {
        Serial.print("In analyze.    ");
        Serial.print(" input_string: ");
        Serial.println(str_in);
        
        //bool found_command = false;
        valid_command_found = false;
        num_of_params = 0;  

        
        if (str_in.indexOf(" ") == 0 ) {
            line_type = ILEGAL;
            Serial.println("ILEGAL");
            return;
        }

        if (str_in.indexOf("#") == 0 ) {
            line_type = COMMENT;
            Serial.println("COMMENT");
            return;
        }

        // Should be a command
        Serial.println(">>  Valid command");
        Serial.print(">> input_string: ");
        Serial.println(str_in);


        int delim_space = str_in.indexOf(" ");
        Serial.print(">> delim_space: ");
        Serial.println(delim_space);
        //int delim_eol   = str_in.indexOf("\n");

        //cmd_str = "XXXXYY"; // for debug 
        
        if ( delim_space == -1 ) { // if no space, then ther is one word - must be a command
            cmd_str = str_in;
            str_in = "";
            line_type = COMMAND;
        }
        else { // extract the command from the input string
            Serial.println("..... in ELSE");
            cmd_str = str_in.substring(0,delim_space); 
            str_in.remove(0,delim_space);
        };

        Serial.print(">>1 cmd_str is: ");
        Serial.println(cmd_str);
        Serial.print(">>2 str_in is: ");
        Serial.println(str_in);


        
        

        // collect all parameters 
        Serial.println(" ^^^^ entering WHILE loop ");

        while (str_in.length() > 0) {
            Serial.print("--- inputString:");
            Serial.println(str_in);
            delim_space = str_in.indexOf(" ");
            //delim_eol   = str_in.indexOf("\n");
            Serial.print("--- delimiter: ");
            Serial.println(delim_space);

            // remove leading spaces
            if (delim_space == 0 ) // first member is space, then delete it and skip 
            {
                str_in.remove(0,1);
                continue;
            } // of if

            Serial.print("--- After clean leading spaces. inputString:");
            Serial.println(str_in);


            // input_string has a value (not empty, no space)
            delim_space = str_in.indexOf(" ");

            Serial.print(">> delimiter:");
            Serial.println(delim_space);

            //delim_eol   = str_in.indexOf("\n");

            

            if (delim_space == -1) { // last parameter
                params[num_of_params++] = str_in;
                str_in = "";
                
                Serial.print(">-----       params[0]:");
                Serial.print(params[0]);
                Serial.print("       params[1]:");
                Serial.println(params[1]);
                continue;
            }
            else {
                // there is a space in string, so more than 1 parameter left
                String tmp_str = str_in.substring(0,delim_space);
                Serial.print(">> tmp_str:");
                Serial.print(tmp_str);
                Serial.print("       num_of_params:");
                Serial.println(num_of_params);
                Serial.print("       params[0]:");
                Serial.print(params[0]);
                Serial.print("       params[1]:");
                Serial.println(params[1]);
                
                params[num_of_params++] = tmp_str;
            }

            Serial.print("<<>> ..inloop delimiter:  ");
            Serial.print(delim_space);
            Serial.print("    inputString: ");
            Serial.println(str_in);
            Serial.print("    num_of_params: ");
            Serial.println(num_of_params);


            // we have at least one more space
            params[num_of_params++] = str_in.substring(0,delim_space);
            str_in.remove(0,delim_space);
            Serial.print(" num_of_params: ");
            Serial.println(num_of_params);
       

        } // of while loop



//                if (CMD_STR == "MOTOR") {
//            Serial.println("motor");
//        }

    };

    void execute_command(spider_anatomy _spider) {
        Serial.print(" *** in execute_command. cmd_str = ");
        Serial.println(cmd_str);
        Serial.print(" params[0]: ");
        Serial.print(params[0]);
        Serial.print(" params[1]: ");
        Serial.println(params[1]);

        switch (valid_cmd_num) {
            case 0:
                Serial.print(" *** in case,  LED command detected");
                break;
            case 1:
                Serial.print(" *** in case,  MOTOR command detected");
                break;
            default:
                Serial.print(" *** in case,  defaullt -> ERROR: unknown command");
                break;
        } // of CASE

        return; // the below should be deleted


        if (cmd_str == "MOTOR") {
            valid_command_found = true;
            execute_motor_command(_spider);
            return;
        } // of MOTOR command detect

        if (cmd_str == "LED") {
            valid_command_found = true;
            execute_led_command(_spider);
            Serial.println(" *** in execute_command. LED identified");
            return;
        } // of MOTOR command detect




        Serial.println("Command not recognized");

    } // of execute_command()

    // ************************************************

    void execute_motor_command(spider_anatomy __spider) {
        valid_parameters = true;
        Serial.print("In execute_motor_command. params[0]: "); 
        Serial.print(params[0]);
        Serial.print(" params[1]: ");
        Serial.println(params[1]);
        
        if (num_of_params!=4) {
            Serial.println("ERROR: 4 pamaters are required for MOTOR command");                
            return;
        } // of if
            
        // check params are numeric
        for (int i=0;i<4;i++) {
            if (!is_str_numeric(params[0])) {
                Serial.println("ERROR in MOTOR command: not a numeric parameter");
                return;     
            } // of if
        } // of for loop
       
            
        int leg_num = params[0].toInt();
        int motor_num = params[1].toInt();
        int angle_num = params[2].toInt();
        int time_num = params[3].toInt();

        servo_cmd.servo_num = leg_num*3 + motor_num;
        servo_cmd.servo_angle = angle_num;
        servo_cmd.wait_time = time_num;
        servo_cmd.valid = true;

        __spider.left_back_leg.motor[motor_num].set_motor_to_angle(__spider.servos_control,servo_cmd);

        //__spider.left_back.motor.

        // TBD: activate servo motor accordingly
             
    } // of execute_motor_command()


    void execute_led_command(spider_anatomy __spider) {

        Serial.println("+++Command is: LED");
        Serial.print("In execute_led_command. params[0]: "); 
        Serial.print(params[0]);
        Serial.print(" params[1]: ");
        Serial.println(params[1]);





            if (num_of_params!=2) {
                valid_parameters = false;
                Serial.println("ERROR: 2 pamaters are required for LED command");                
                return;
            } // of if
            // check params and execute the command
             if (params[0] != "GREEN" && params[0] != "YELLOW" && params[0] != "RED") {
                 valid_parameters = false;
                 Serial.println("ERROR in LED command: color is wrong");
                 return;
             } // of if

             if (params[1] != "ON" && params[1] != "OFF" ) {
                 valid_parameters = false;
                 Serial.println("ERROR in LED command: should be ON or OFF");
                 return;
             } // of if



             if (params[0] == "GREEN" && params[1] == "ON") {
                __spider.leds.set_led_on(__spider.leds.led_green);
                return;
             }
             if (params[0] == "GREEN" && params[1] == "OFF") {
                __spider.leds.set_led_off(__spider.leds.led_green);
                return;
             }

            if (params[0] == "YELLOW" && params[1] == "ON") {
                __spider.leds.set_led_on(__spider.leds.led_yellow);
                return;
             }
             if (params[0] == "YELLOW" && params[1] == "OFF") {
                __spider.leds.set_led_off(__spider.leds.led_yellow);
                return;
             }

             if (params[0] == "RED" && params[1] == "ON") {
                __spider.leds.set_led_on(__spider.leds.led_red);
                return;
             }
             if (params[0] == "RED" && params[1] == "OFF") {
                __spider.leds.set_led_off(__spider.leds.led_red);
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