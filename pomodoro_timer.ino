/*
  This is a simple pomodoro timer that can be used to practice the pomodoro technique. For
  a schematic, take a look on my site http://jpreardon.com/ 
  
  For more on the pomodoro technique, check out this site http://www.pomodorotechnique.com
  
  Created 2012-12-27
  By: John P. Reardon <jp@jpreardon.com>
  
*/

// Define debug state (1 for debug, 0 for not)
#define DEBUG 0

// Define available modes
#define IDLE 1
#define SHORT_BREAK 2
#define LONG_BREAK 3
#define POMODORO 4
#define IN_PROCESS 5

// Constants for input/output pins
const int greenLed1 = 7;  // Green LEDs are for the pomodoro count
const int greenLed2 = 8;
const int greenLed3 = 9;
const int greenLed4 = 10;  
const int redLed1 = 2;    // Red LEDs count the time left
const int redLed2 = 3;    // in a pomodoro, or a break
const int redLed3 = 4;
const int redLed4 = 5;
const int redLed5 = 6;
const int amberLed1 = 0;  // Short break indicator
const int amberLed2 = 1;  // Long break indicator
const int button1 = 11;   // Start/Interrupt
const int button2 = 12;   // Reset

// Constants for time values (in minutes)
const int pomodoroTime = 25;
const int shortBreakTime = 5;
const int longBreakTime = 15;

// Fields to hold state
int currentMode = IDLE;    // Values are IDLE, SHORT_BREAK, LONG_BREAK, POMODORO, IN_PROCESS
int nextMode = POMODORO;   // Holds the next mode
int currentPomodoroCount;  // Number of completed pomodoros
long stateStartTime;       // In milliseconds
int ledState = LOW;        // This will be used if a LED needs to blink
long previousMillis = 0;   // Last time the LED was updated
long blinkInterval = 1000; // In milliseconds
int modeFinished = 0;  // To let us know if the last pomodoro or break was completed

void setup() {
  if (DEBUG) Serial.begin(9600);
  
  // Set pins to the proper mode
  pinMode(greenLed1, OUTPUT);
  pinMode(greenLed2, OUTPUT);
  pinMode(greenLed3, OUTPUT);
  pinMode(greenLed4, OUTPUT);
  pinMode(redLed1, OUTPUT);
  pinMode(redLed2, OUTPUT);
  pinMode(redLed3, OUTPUT);
  pinMode(redLed4, OUTPUT);
  pinMode(redLed5, OUTPUT);
  pinMode(amberLed1, OUTPUT);
  pinMode(amberLed2, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
}

void loop() {
  // Check to see if buttons are pressed (code to come)
  checkButtons();
  
  // Increment Everything
  incrementTime();
  
  // Display current state
  displayState();
}

void checkButtons() {
  // See if any button are pressed, if they are, do something
  
  if(digitalRead(button2) == HIGH) {
    // debounce, ignore any button presses one second after a state change
    if ((millis() - stateStartTime) < 1000) {
      return;
    }
    // full reset
    setCurrentMode(IDLE);
    if (DEBUG) Serial.println("RESET!");
  }
  
  if(digitalRead(button1) == HIGH) {
    // debounce, ignore any button presses one second after a state change
    if ((millis() - stateStartTime) < 1000) {
      return;
    }
    
    switch (currentMode) {
      case IDLE:
        // do nothing
        resetPomodoroCount();
        setCurrentMode(POMODORO);
        nextMode = SHORT_BREAK;
        if (DEBUG) Serial.println("changing to pomodoro mode");
        break;
      case SHORT_BREAK:
        setCurrentMode(IN_PROCESS);
        nextMode = POMODORO;
        if (DEBUG) Serial.println("changing to in process mode from short break");
        break;
      case LONG_BREAK:
        resetPomodoroCount();
        setCurrentMode(IDLE);
        if (DEBUG) Serial.println("changing to idle mode from long break");
        break;
      case POMODORO:
        setCurrentMode(IN_PROCESS);
        nextMode = POMODORO;  
        if (DEBUG) Serial.println("changing to in process mode from pomodoro");
        break;
      case IN_PROCESS:
        setCurrentMode(nextMode);
        modeFinished = 0;
        if (DEBUG) Serial.print("changing from in process to next mode: ");
        if (DEBUG) Serial.println(nextMode);
        break;
    }
  }
}

void incrementTime() {
  
  unsigned long currentMillis = millis();
  
  // Increment the timer, see if the state needs to change
  switch (currentMode) {
    case IDLE:
      // Idle mode, do nothing
      break;
    case SHORT_BREAK:
      // short break code here
      // If break is over, change modes
      if(currentMillis - stateStartTime > convertMinuteToMillis(shortBreakTime)){
        setCurrentMode(IN_PROCESS);
        nextMode = POMODORO;
        modeFinished = 1;
      }
      break;
    case LONG_BREAK:
      // long break code here
      if(currentMillis - stateStartTime > convertMinuteToMillis(longBreakTime)){
        setCurrentMode(IDLE);
        nextMode = POMODORO;
        modeFinished = 1;
      }
      break;
    case POMODORO:
      // pomodoro code here
      if(currentMillis - stateStartTime > convertMinuteToMillis(pomodoroTime)){
        currentPomodoroCount++;
        modeFinished = 1;
        if (currentPomodoroCount == 4) {
          setCurrentMode(IN_PROCESS);
          nextMode = LONG_BREAK;
        } else {
          setCurrentMode(IN_PROCESS);
          nextMode = SHORT_BREAK;
        }
      }
      break;
    case IN_PROCESS:
      // do nothing
    default:
      // do nothing
      break;
  }
 
}

void displayState() {
  
  unsigned long currentMillis = millis();
  unsigned long millisPassed;
  float numLeds;
  
  // We always light the proper number of pomodoro number lights
  lightGreenLeds(currentPomodoroCount);
  
  // Light up leds based on the current state
  switch (currentMode) {
    case IDLE:
      // Idle mode, turn off the lights, this is going to need to be fixed
      // turnAllOff();
      break;
    case SHORT_BREAK:
      // We're on a break, light a break light
      digitalWrite(amberLed1, HIGH);
      // Figure out how much time has passed
      millisPassed = currentMillis - stateStartTime;
      // Figure out how many LEDs to light up, cast a variable as a float so we get a float back
      numLeds = ((float)shortBreakTime - convertMillisToMinute(millisPassed)) / (shortBreakTime / 5); 
      // Light em up
      lightRedLeds(numLeds);
      break;
    case LONG_BREAK:
      // We're on a break, light a break light
      digitalWrite(amberLed2, HIGH);
      // Figure out how much time has passed
      millisPassed = currentMillis - stateStartTime;
      // Figure out how many LEDs to light up, cast a variable as a float so we get a float back
      numLeds = ((float)longBreakTime - convertMillisToMinute(millisPassed)) / (longBreakTime / 5);
      // Light em up
      lightRedLeds(numLeds);
      break;
    case POMODORO:
      // Turn off the break lights
      digitalWrite(amberLed1, LOW);
      digitalWrite(amberLed2, LOW);
      // Figure out how much time has passed
      millisPassed = currentMillis - stateStartTime;
      // Figure out how many LEDs to light up, cast a variable as a float so we get a float back
      numLeds = ((float)pomodoroTime - convertMillisToMinute(millisPassed)) / (pomodoroTime / 5);
      // Light em up
      lightRedLeds(numLeds);
      break;
    case IN_PROCESS:
      if (modeFinished) {
        blinkRedLeds();
      }
      else {
        lightRedLeds(0);
      }
      break;
    default:
      // Do nothing
      break;
  }
  
}

// Blink the red LEDs
void blinkRedLeds() {
  digitalWrite(redLed1, blinkLed());
  digitalWrite(redLed2, blinkLed());
  digitalWrite(redLed3, blinkLed());
  digitalWrite(redLed4, blinkLed());
  digitalWrite(redLed5, blinkLed());
}

// Light the number of red LEDs specified
void lightRedLeds(float numLeds) {
  if (numLeds > 4) {
    digitalWrite(redLed1, HIGH);
    digitalWrite(redLed2, HIGH);
    digitalWrite(redLed3, HIGH);
    digitalWrite(redLed4, HIGH);
    digitalWrite(redLed5, blinkLed());
  }
  else if (numLeds > 3) {
    digitalWrite(redLed1, HIGH);
    digitalWrite(redLed2, HIGH);
    digitalWrite(redLed3, HIGH);
    digitalWrite(redLed4, blinkLed());
    digitalWrite(redLed5, LOW);
  }
  else if (numLeds > 2) {
    digitalWrite(redLed1, HIGH);
    digitalWrite(redLed2, HIGH);
    digitalWrite(redLed3, blinkLed());
    digitalWrite(redLed4, LOW);
    digitalWrite(redLed5, LOW);
  }
  else if (numLeds > 1) {
    digitalWrite(redLed1, HIGH);
    digitalWrite(redLed2, blinkLed());
    digitalWrite(redLed3, LOW);
    digitalWrite(redLed4, LOW);
    digitalWrite(redLed5, LOW);  
  }
 else if (numLeds > 0) {
     digitalWrite(redLed1, blinkLed());
     digitalWrite(redLed2, LOW);
     digitalWrite(redLed3, LOW);
     digitalWrite(redLed4, LOW);
     digitalWrite(redLed5, LOW);
 }
 else {
     digitalWrite(redLed1, LOW);
     digitalWrite(redLed2, LOW);
     digitalWrite(redLed3, LOW);
     digitalWrite(redLed4, LOW);
     digitalWrite(redLed5, LOW);
 }
}

// Light the number of green LEDs specified
void lightGreenLeds(int numLeds) {
    switch (numLeds) {
    case 1:
      digitalWrite(greenLed1, HIGH);
      digitalWrite(greenLed2, LOW);
      digitalWrite(greenLed3, LOW);
      digitalWrite(greenLed4, LOW);
      break;
    case 2:
      digitalWrite(greenLed1, HIGH);
      digitalWrite(greenLed2, HIGH);
      digitalWrite(greenLed3, LOW);
      digitalWrite(greenLed4, LOW);
      break;
    case 3:
      digitalWrite(greenLed1, HIGH);
      digitalWrite(greenLed2, HIGH);
      digitalWrite(greenLed3, HIGH);
      digitalWrite(greenLed4, LOW);
      break;
    case 4:
      digitalWrite(greenLed1, HIGH);
      digitalWrite(greenLed2, HIGH);
      digitalWrite(greenLed3, HIGH);
      digitalWrite(greenLed4, HIGH);
      break;
    default:
      digitalWrite(greenLed1, LOW);
      digitalWrite(greenLed2, LOW);
      digitalWrite(greenLed3, LOW);
      digitalWrite(greenLed4, LOW);
  }
}
 
int blinkLed() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis > blinkInterval) {
    
    previousMillis = currentMillis;
    
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
  }
  return ledState;
}
 
void turnAllOn() {
 // You guessed it, turn all the lights on at once
 digitalWrite(greenLed1, HIGH);
 digitalWrite(greenLed2, HIGH);
 digitalWrite(greenLed3, HIGH);
 digitalWrite(greenLed4, HIGH);
 digitalWrite(redLed1, HIGH);
 digitalWrite(redLed2, HIGH);
 digitalWrite(redLed3, HIGH);
 digitalWrite(redLed4, HIGH);
 digitalWrite(redLed5, HIGH);
 digitalWrite(amberLed1, HIGH);
 digitalWrite(amberLed2, HIGH);
}

void turnAllOff() {
 // You guessed it, turn all the lights off at once
 digitalWrite(greenLed1, LOW);
 digitalWrite(greenLed2, LOW);
 digitalWrite(greenLed3, LOW);
 digitalWrite(greenLed4, LOW);
 digitalWrite(redLed1, LOW);
 digitalWrite(redLed2, LOW);
 digitalWrite(redLed3, LOW);
 digitalWrite(redLed4, LOW);
 digitalWrite(redLed5, LOW);
 digitalWrite(amberLed1, LOW);
 digitalWrite(amberLed2, LOW);
}

long convertMinuteToMillis(int minutes) {
  return minutes * 60000;
}

int convertMillisToMinute(long millis) {
  return millis / 60000;
}

// This sets the current mode
void setCurrentMode(int newMode) {
  currentMode = newMode;
  stateStartTime = millis();
}

void resetPomodoroCount() {
  currentPomodoroCount = 0;
}


