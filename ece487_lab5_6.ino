/*    
 *     
 *     
        Samuel Courtney 
        CWID: 11682010  

          Lab Assignment 5 & 6

          Assignment Description:
            This lab assignment prompts the user that he/she has 4 seconds to give input. With each input, the timer should reset to zero.
            If an input is not provided by the 4 second time interval, the system will reset. Input of 'a, b, and c' will be accepted. All other inputs
            will reset the WDT, but display that they are invalid. If a 'a' is pressed, the program will start a set of ADC conversions. 
            The system will then convert the analog values to digital 30 times and report the avg conversion time. If 'b' is pressed, this will 
            be done in a polling loop. If 'c' is pressed, the conversions will be done in a interrupt driven style.      
 *          
 *
 */
#include <avr/wdt.h>

volatile int ADCvalue;
unsigned long currentTime = micros();

//Setup loop that initializes the program, serial monitor and some pinouts - prompts the user initially
void setup() {
  MCUSR = 0;              // init MCUSR
  Serial.setTimeout(50);  // Makes the LEDs turn off faster
  Serial.begin(115200);   // Sets Baud Rate (bps) to 115200
  delay(100);             // Protects against garbage characters upon upload
  pinMode(A0,INPUT);      // Analog Read in pin
  
  
  while (! Serial);                // The progam will wait until the serial is ready
    for(int i = 0; i <= 35; i++) { // Printing new lines to get rid of trash charaters in the serial monitor
        Serial.println(" ");
    }
  // Once clean & ready, prompt user with instructions & initial count:
  Serial.println("\n-------------------------------------------\n\t\tBOARD RESET\n-------------------------------------------"); 
  Serial.println("Select a type of conversion to perform ('a' for AnalogRead; 'b' for polling; 'c' for interrupts).");
  Serial.println("NOTE: You have four seconds to enter input before the system times out. \n\n\tYour time begins now.");
}
//The main loop of code that handles user input and calls on the specified functions
void loop() {
String userString; // A string that handles the user input from the serial monitor 
 
  wdt_enable(WDTO_4S);                            //enables the watchdog timer countdown
  while(1) {
    while (Serial.available() > 0) {             //init serial monitor
        userString = Serial.readStringUntil('\n'); // Reads user input until newline (i.e. enter is pressed)
       
       if(userString == "a") {                    //string conditional 'a'
          disableWDT();                           //disables wdt for conversions
          Serial.println("\n\nA series of 30 [AnalogRead] potentiometer measurements is about to begin. Ready?");
          delay(1500);                            //user 'padding' to allow for hand placement
          analogReader();                         //calls on the 'analogReader' function shown below
          enableWDT();                            //once finished, enables the wdt again
        }
       if(userString == "b") {                    //string conditional 'b' polling
          disableWDT();                           //disables wdt for conversions
          Serial.println("\n\nA series of 30 [Polled I/O] potentiometer measurements is about to begin. Ready?");
          delay(1500);                            //user 'padding' to allow for hand placement
          PolledIOReader();                       //calls for Polled I/O conversion function
          enableWDT();                            //once finished, enables the wdt again
        }
       if(userString == "c") {                    //string conditional 'c' ISR driven
          disableWDT();//disables wdt for conversions
          Serial.println("\n\nA series of 30 [ISR Driven] potentiometer measurements is about to begin. Ready?");
          delay(1500);                            //user 'padding' to allow for hand placement
          ISRReader();                            //calls for Polled I/O conversion function
          enableWDT();                            //once finished, enables the wdt again
        }
        if (userString != "a") {         
          if (userString != "b") {    //<< some weird arduino IDE workaround >>
            if (userString != "c") {   //Moreover: since if(userString != "a" || userString != "b" ... ) was not working
              disableWDT();
              Serial.print("\n\t'");                      //lines to beautify text
              Serial.print(userString);
              Serial.print("' ");
              Serial.print("is an invaild input.\n");
              enableWDT();
            }  
          }
        }
       Serial.print("\t\nSelect a type of conversion to perform ('a' for AnalogRead; 'b' for polling; 'c' for interrupts).\n");  //constant prompt
      }
   }  
}
//function to enable the watchdog timer
void enableWDT (){
  wdt_enable(WDTO_4S);
}
//function to disable the watchdog timer
void disableWDT(){
  wdt_reset();
  wdt_disable();
}
//analogRead function that enables the ADC through the standard method of analog to digital converting
void analogReader() {

//some basic ADC enable code for consistency
ADCSRA |= (1 << ADEN);    // Enable the ADC
ADMUX   = 0;              // use ADC0
ADMUX  |= (1 << REFS0);   // use AVcc as the reference
ADMUX  |= (0 << ADLAR);   // Right adjust for 10 bit resolution
ADCSRA |= (1 << ADIE);    // Enable Interrupts 
    
unsigned long startTime = 0;      //time that the conversion will start timing
int timeDiff = 0;                 //holds the difference between the start-time and end-time
float avg = 0;                    //average of all the time conversion values recorded
unsigned long sum = 0;            //sum of the values from conversions
int sensorValue = analogRead(A0); // read the input on analog pin 0:

  for(int i=1; i<31; i++) {
  startTime = micros();           //start the time
  sensorValue = analogRead(A0);   //read the input on analog pin 0:
  currentTime = micros();         //record the next time
  printVal(startTime, avg, sum, sensorValue, i); //print it real good!
  timeDiff = currentTime - startTime; //calculate differnce between two times
  sum = sum + timeDiff;               //add upp all the time differnce
  avg = ((float) sum / 30);           //gets the average of all the times
  
 }
  //prints the average
   Serial.print("\n\t avg conversion time = ");
   Serial.print(avg);
   Serial.print(" usecs\n");
   while (Serial.available() > 0) { //init serial monitor to do nothing when converting
       Serial.readString();
   }
  ADCSRA &= ~(1 << ADEN);    // disable the ADC
  return;
}
//Polled I/O function that enables the ADC through polling
void PolledIOReader() {
//some basic ADC enable code for bitlevel manipulation
ADCSRA |= (1 << ADEN);    // Enable the ADC
ADMUX = 0;                // use ADC0
ADMUX |= (1 << REFS0);    // use AVcc as the reference
ADMUX |= (0 << ADLAR);    // Right adjust for 10 bit resolution
ADCSRA |= (1 << ADPS0);   // prescale 
ADCSRA |= (1 << ADEN);    // Enable the ADC
ADCSRA |= (1 << ADIE);    // Enable Interrupts 

unsigned long startTime = 0;      //time that the conversion will start timing
int timeDiff = 0;                 //holds the difference between the start-time and end-time
float avg = 0;                    //average of all the time conversion values recorded
unsigned long sum = 0;            //sum of the values from conversions
int sensorValue;                  // read the input on analog pin 0 - HOLDER:
    
  for(int i=1; i<31; i++) {
    startTime= micros();              //start the timer
    ADCSRA |= (1 << ADSC);            // Start the ADC conversion
    while(bit_is_set(ADCSRA, ADSC)) { //while there is an input to the ADCSRA => ADSC 
    sensorValue = ADCvalue;           // read the input on analog pin 0:
    currentTime = micros();           //get the time it took to convert
    }
  //again, explained previously (Refer to analogReader function for more details)
  printVal(startTime, avg, sum, sensorValue, i);
  timeDiff = currentTime - startTime;
  sum = sum + timeDiff;
  avg = ((float) sum / 30);
  }
  //prints the average time of 30 conversions
  Serial.print("\n\t avg conversion time = ");
  Serial.print(avg);
  Serial.print(" usecs\n");
    while (Serial.available() > 0) { //init serial monitor
           Serial.readString();
    }
  ADCSRA &= ~(1 << ADEN);    //disables the ADC
  return;
}

//This is a function for the Interrupt Service Routine in the following function 'ISRReader'
ISR(ADC_vect)
{  
  ADCvalue = ADCL | (ADCH << 8); //Get the value recorded by the ADC
  currentTime = micros();        //Update the time
}

//ISRfunction that enables the ADC through the ISR functionality
void ISRReader() {
//seting some of the bitwise regs in the ADC
ADCSRA |= (1 << ADEN);    // Enable the ADC
ADMUX = 0;                // use ADC0
ADMUX |= (1 << REFS0);    // use AVcc as the reference
ADMUX |= (0 << ADLAR);    // Right adjust for 10 bit resolution
ADCSRA |= (1 << ADPS0);   // prescale 
ADCSRA |= (1 << ADEN);    // Enable the ADC
ADCSRA |= (1 << ADIE);    // Enable Interrupts 
    

unsigned long startTime = 0;      //time that the conversion will start timing
int timeDiff = 0;                 //holds the difference between the start-time and end-time
float avg = 0;                    //average of all the time conversion values recorded
unsigned long sum = 0;            //sum of the values from conversions
int sensorValue;                  // read the input on analog pin 0:

  for(int i=1; i<31; i++) {
  startTime= micros();
  ADCSRA |= (1 << ADSC);    // Start the ADC conversion
  delay(1);                 // Need for ISR to capture first conversion on board reset
  sensorValue = ADCvalue;   // read the input on analog pin 0:
  ADCSRA &= ~(1 << ADSC);   // Start the ADC conversion
  printVal(startTime, avg, sum, sensorValue, i);
  timeDiff = currentTime - startTime;
  sum = sum + timeDiff;
  }
  //get the avg and print
   avg = ((float) sum / 30);
   Serial.print("\n\t avg conversion time = ");
   Serial.print(avg);
   Serial.print(" usecs\n");
  
  ADCSRA &= ~(1 << ADEN);    // disable the ADC
  return;
}
//A function that will print each conversion based on the selected method
/* Fun fact: I had this piece of code three times, so I figured you would enjoy a function to print in each case */
void printVal(unsigned long startTime, float avg, unsigned long sum, int sensorValue, int i) {
//Basic printing commands because Arduino IDE doesn't know what a printf(..., &%d is)
  if(sensorValue > 255) {
    Serial.print("#");
    Serial.print(i);
    Serial.print(":\tdigital value = ");
    Serial.print(sensorValue, HEX); // print out the value you read:
    Serial.print("\tTime = ");
   Serial.print(currentTime - startTime);
   Serial.print(" usecs\n");
     delay(300);
   }
   else if(sensorValue <= 255 && sensorValue > 15) {
   Serial.print("#");
   Serial.print(i);
   Serial.print(":\tdigital value = ");
   Serial.print("0");
   Serial.print(sensorValue, HEX); // print out the value you read:
   Serial.print("\tTime = ");
   Serial.print(currentTime - startTime);
   Serial.print(" usecs\n");
   delay(300); 
   }
   else {
   Serial.print("#");
   Serial.print(i);
   Serial.print(":\tdigital value = ");
   Serial.print("00");
   Serial.print(sensorValue, HEX); // print out the value you read:
   Serial.print("\tTime = ");
   Serial.print(currentTime - startTime);
   Serial.print(" usecs\n");
   delay(300); 
   }
   while (Serial.available() > 0) { //init serial monitor
       Serial.readString();
   }
return;
}
