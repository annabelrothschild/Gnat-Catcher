/* Annabel Rothschild, HCC2 Fall 2020, Tangible User Interface Project
 *  This is code for a 'gnat catcher' that has two settings:
 *  1. User can turn the catcher on at any time by pressing a push-button, which controls one set of lights (LEDs)
 *  2. The catcher will turn on automatically in low light settings (ie at nighttime) and off in high light settings (ie during the day) -- this feature controls a second, independent set of lights (LEDs)
 
 NOTE:
 This code uses the concept of 'debounce' to handle what looks like 
 multiple input from the push button but is just a delayed reaction. 
 That code builds off of this (https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce)
 Arduino documentation example.
 */

//constants for LED pin assignments
//w_i are the white LEDs
const int w1 = 13;
const int w2 = 12;
const int w3 = 11;
const int w4 = 10;

//y_i are the yellow LEDs
const int y1 = 6;
const int y2 = 5;
const int y3 = 4;
const int y4 = 3;

//pin assignments for the light sensor and push button
const int sensor = 0;
const int button = 2;

//store button state
int buttonState = 0;

//store whether or not the white lights are already on
boolean whitesOn = false;

//store current light sensor reading
int lightVal;

//initialize arrays of white and yellow lights for iteration
int whites[4] = {w1, w2, w3, w4};
int yellows[4] = {y1, y2, y3, y4};

//ensure that when push button is read, mutliple pulses that belong together are grouped
//ie, debounce is the timing of what counts as one push vs a separate one
long debounce = 1000;

//function to change on/off setting for all LEDs of one color
void lightsOn(int *colors, int val) {
  //for each LED in the given color group
  for (int i = 0; i < 4; i++) {
    //rewrite the light's on/off value to be consistent with the value given in the argument
    digitalWrite(colors[i], val); 
  }
}

void setup() {
  //set all LEDs as output
  pinMode(w1, OUTPUT);
  pinMode(w2, OUTPUT);
  pinMode(w3, OUTPUT);
  pinMode(w4, OUTPUT);

  pinMode(y1, OUTPUT);
  pinMode(y2, OUTPUT);
  pinMode(y3, OUTPUT);
  pinMode(y4, OUTPUT);

  //set light sensor and push button as input
  pinMode(sensor, INPUT);
  pinMode(button, INPUT);

  //set up serial monitor for debugging
  Serial.begin(9600);
}

void loop() {
   // if button is pressed change the state of the white LEDs

   //first read state of button (ie HIGH or LOW)
   buttonState = digitalRead(button);
   //for debugging
   Serial.println(buttonState);
   //if the button is LOW (this corresponds to the button being actively pressed by the user because of my circuit design)
   if (buttonState == LOW) {
      //if the white lights are already on, then they should be turned off
       if (whitesOn == true) {
          //debugging statement
           Serial.println("Turning white LED off");
           //assign new off value to all LEDs in the white group
           lightsOn(whites, LOW);
           //indicate that whites are now off
           whitesOn = false;
       } else {
        //in this case we want to turn the white lights on because they are currently off but the user is trying to turn them on
           Serial.println("Turning white LED on");
           //turn on white LEDs and mark that you've done so
           lightsOn(whites, HIGH);
           whitesOn = true;
       }
       //delay for the 'debounce' period, or the residual button press reads so that you've not reading a slow press as lots of different press-es
       delay(debounce);
   }
    
   // turn yellow LEDs on or off based on photo resistor
   lightVal = analogRead(sensor);
   //debugging
   Serial.println(lightVal);

   // light is hard-coded for my room, where 1000 is when the sun has set 
   //purpose of this section is for #2 setting listed in top of file notes

   //if room is dark and light sensor detects that
   if (lightVal > 1000) {
    //turn on the yellow group
      lightsOn(yellows, HIGH);
      //short delay to avoid rapidly blinking lights
      delay(5000);
    } else if (lightVal <= 1000) {
      //if the room is still light
      //turn the yellow group off
      lightsOn(yellows, LOW);
      //short delay to avoid rapidly blinking lights
      delay(5000);
    }
}
