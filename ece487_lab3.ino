/*    
 *     
 *     
        Samuel Courtney 
        CWID: 11682010  

          Lab Assignment 3

          Assignment Description:
            This lab assignment prompts the user that he/she has 4 seconds to give input. With each input, the timer should reset to zero.
            If an input is not provided by the 4 second time interval, the system will reset. All inputs 0-15 are accepted and reported
            on the serial monitor and seven segment display. When the reset is activated, the DP will blink for 5 seconds.          
 *          
 *
 */
#include <avr/wdt.h>

//Setup loop that initializes the program, serial monitor and some pinouts
void setup() {
  MCUSR = 0;              // init MCUSR
  int startupInt = 16;    // start segement on display
  Serial.setTimeout(50);  // Makes the LEDs turn off faster
  Serial.begin(115200);   // Sets Baud Rate (bps) to 115200

  delay(100);             // Protects against garbage characters upon upload
  pinMode(2,OUTPUT);      //Seven segment 'a' diode for common cathode (part #5161AG-1)
  pinMode(3,OUTPUT);      //Seven segment 'b' ..
  pinMode(4,OUTPUT);      //Seven segment 'c' ..
  pinMode(5,OUTPUT);      //Seven segment 'd' .. 
  pinMode(6,OUTPUT);      //Seven segment 'e' ..
  pinMode(7,OUTPUT);      //Seven segment 'f' .. 
  pinMode(9,OUTPUT);      //Seven segment 'g' ..
  pinMode(10,OUTPUT);     //Seven segment '.' ..
  pinMode(8,OUTPUT);

  while (! Serial);                // The progam will wait until the serial is ready
    for(int i = 0; i <= 35; i++) { // Printing new lines to get rid of trash charaters in the serial monitor
        Serial.println(" ");
    }
    
  displayNumber(startupInt); //displays initial segment
  
  // Once clean & ready, prompt user with instructions & initial count:
  Serial.println("Enter an number 0-15 to illuminate on the seven segment display.");
  Serial.println("NOTE: You have four seconds to enter input. \n\n\tYour time begins now: 0.000s");
}
//The main functional loop of the code that contains all of the information needed to run
void loop() {
unsigned long userInt = 0;        // userInt from string
String userString;                // A string that handles the user input from the serial monitor
unsigned long seconds = 0;        //seconds elapsed
unsigned long milliseconds = 0;   //milliseconds elapsed
unsigned long previousMillis = 0; //millis previous handler

while(1) {
unsigned long currentMillis = millis();                   //setting up the timer function
seconds = (((currentMillis- previousMillis)/1000)%4);     //calculation of the seconds for print
milliseconds = (((currentMillis - previousMillis)%4000)); //calculation of the millis for print
 // How much time has passed, accounting for rollover with subtraction && timeout at 4 seconds
  if ((currentMillis - previousMillis) <= 4000) {

    while (Serial.available() > 0) { //init serial monitor
        
        userString = Serial.readStringUntil('\n');  // Reads user input until newline (i.e. enter is pressed)
        userInt = userString.toInt();               //converts user string to useable value
    
        if(userString == "0") {                     //string conditional to get around return 0
        printTime(seconds, milliseconds, userInt);  //print funtion
        displayNumber(userInt);                     // sevensegment display number
        previousMillis = currentMillis;             //sets the previous time to the current time
        seconds = 0;                                //reset seconds
        milliseconds = 0;                           //reset milliseconds
        }
        if(userInt > 0 && userInt <= 15) {
        printTime(seconds, milliseconds, userInt);  //print funtion
        displayNumber(userInt);                     // sevensegment display number
        previousMillis = currentMillis;             //sets the previous time to the current time
        seconds = 0;                                //reset seconds
        milliseconds = 0;                           //reset milliseconds
        } 
        else if (userString != "0") {
        Serial.print("\n\t'");                      //lines to beautify text
        Serial.print(userString);
        Serial.print("' ");
        Serial.print("is an invaild input.\n");
        userInt = -1;                               // for error segments
        displayNumber(userInt);
        previousMillis = currentMillis;
        seconds = 0;                                //reset seconds
        milliseconds = 0;                           //reset milliseconds
        }
      Serial.print("\t\n Enter an input 0-15.\n");  //constant prompt
    }
  }
  else{
    Serial.println("\n------------------------------------------\nSYSTEM TIMEOUT --- RESET CYCLE ACTIVATED.\n------------------------------------------");
    for(int i=0; i<5; i++) {
      digitalWrite(2, LOW); // DP LED indicatior of reset
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10,HIGH);
      delay(500); //0.5s on
      digitalWrite(10,LOW);
      delay(500); //0.5s off
    }
    wdt_enable(WDTO_15MS); // turn on the WDT
      for(;;) { 
      // do nothing and wait for reset
      } 
    }
  }
}  
//a printing function  that prints out the entered int & reaction time
void printTime(int s, int ms, int userInt1) {

      Serial.print("\tYou entered: ");//lines to beautify text
      Serial.print(userInt1, HEX);
      Serial.print("\treaction time: ");
      Serial.print(s);
      Serial.print(".");
      
        if(ms%1000<100){              //conditional for correct sigfigs
           Serial.print("0");
           Serial.print(ms%1000);     //conditional for correct sigfigs
        }
      else { Serial.print(ms%1000); } //conditional for correct sigfigs
      Serial.print("\n");
      return;
}
// displays the userNumber, init conditional, and invalid input
void displayNumber(int userNumber) {

   if(userNumber == 0){ //displays zero
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
    }
    if(userNumber == 1){ //displays one
       digitalWrite(2, LOW);
       digitalWrite(3, HIGH);
       digitalWrite(4, HIGH);
       digitalWrite(5, LOW);
       digitalWrite(6, LOW);
       digitalWrite(7, LOW);
       digitalWrite(9, LOW);
       digitalWrite(10, LOW);
    }
    if(userNumber == 2) { //displays two
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
    }
    if(userNumber == 3) { //displays three
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
    }
    if(userNumber == 4) { //displays four
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
    }
    if(userNumber == 5) { //displays five
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
    }
    if(userNumber == 6) { //displays six
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
    }
    if(userNumber == 7) { //displays seven
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
    }
    if(userNumber == 8) { //displays eight
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
    }
    if(userNumber == 9) { //displays nine
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
    }
    if(userNumber == 10) { //displays A.
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
    }
    if(userNumber == 11) { //displays b.
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
    }
    if(userNumber == 12) { //displays C.
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);
    }
    if(userNumber == 13) { //displays d.
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
    }
    if(userNumber == 14) { //displays E.
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
    }
    if(userNumber == 15) { //displays F.
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
    }
    if(userNumber == 16){ // "-" init (16)
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(9, HIGH); //
      digitalWrite(10, LOW);
    }
    if(userNumber == -1) {
      digitalWrite(2, HIGH); // "---"
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH); //
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(9, HIGH); //
      digitalWrite(10, LOW);
    }
   return;
 }
