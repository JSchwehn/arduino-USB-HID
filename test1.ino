/**

   Sending on Keyboard stroke on a button press.
   Handles Button de-bouncing

*/


// character to send
#define KEY_U 0x18 

const int buttonPin =  4;   // the number of the pushbutton pin
const int ledPin    = 12;   // the number of the LED pin

int ledState        = LOW;  // the current state of the output pin
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin
uint8_t buf[8] = { 0 };	    // Keyboard report buffer 

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay    = 90; // the debounce time; increase if the output flickers

void setup() {
  // set the pin modes
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // set initial LED state
  digitalWrite(ledPin, ledState);
  
  // enable USB communication
  Serial.begin(9600);
  delay(200); // give the system a wait periode
}

void loop() {
  
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      
      // send char and enable the LED to have a visble feedback
      if (buttonState == HIGH) {
        ledState = 1;
        buf[0] = 0;
        buf[2] = KEY_U;
        Serial.write(buf,8);
      } else {
        // send the keyup and disable the LED
        ledState = 0;
        releaseKey();
      }
    }
  }
  
  // set the LED:
  digitalWrite(ledPin, ledState);
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}

void releaseKey() 
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8);	// Release key 
}
