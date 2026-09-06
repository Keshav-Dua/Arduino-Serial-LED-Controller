#include <Arduino.h>
// -------------------- variables we use everywhere--------------------
const int NUM_LEDS = 4;
int ledPins[4];
const int MODE_OFF = 0;
const int MODE_ON = 1;
const int MODE_BLINK = 2;

int ledMode[4];
boolean ledIsOn[4];
unsigned long ledLastToggleTime[4];
unsigned long ledBlinkInterval[4];

const int PATTERN_NONE = 0;
const int PATTERN_CHASE = 1;
const int PATTERN_ALT = 2;

int currentPattern;
unsigned long patternInterval;
unsigned long lastPatternTime;
int patternPosition;
boolean alternateState;

// -------------------- Function declarations --------------------
void parseCommand(String commandText);
void handleOnOff(String wordList[], int wordCount, boolean turnOn);
void handleBlink(String wordList[], int wordCount);
void handlePattern(String wordList[], int wordCount);
void updateBlinkingLeds();
void updatePattern();
void setLed(int ledIndex, int mode);
void printStatus();
void printHelp();
// -------------------- setup() ----------------------
void setup() {
  Serial.begin(9600);

  ledPins[0] = 4;
  ledPins[1] = 5;
  ledPins[2] = 6;
  ledPins[3] = 7;

  currentPattern = PATTERN_NONE;
  patternInterval = 300;
  lastPatternTime = 0;
  patternPosition = 0;
  alternateState = false;

  int i;
  for (i = 0; i < NUM_LEDS; i = i + 1) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);

    ledMode[i] = MODE_OFF;
    ledIsOn[i] = false;
    ledLastToggleTime[i] = 0;
    ledBlinkInterval[i] = 0;
  }

  Serial.println("Serial LED Interface ready. Type HELP for commands.");
}

// -------------------- loop() --------------------

void loop() {
  if (Serial.available() > 0) {
    String receivedText = Serial.readStringUntil('\n');
    parseCommand(receivedText);
  }

  updateBlinkingLeds();
  updatePattern();
}

// -------------------- input --------------------

void parseCommand(String commandText) {

  commandText.trim();

  if (commandText.length() == 0) {
    return;
  }

  commandText.toUpperCase();

  String wordList[4];
  int wordCount = 0;
  int wordStartPosition = 0;
  int textLength = commandText.length();

  int i;
  for (i = 0; i <= textLength; i = i + 1) {

    boolean weAreAtTheEnd = false;
    if (i == textLength) {
      weAreAtTheEnd = true;
    }

    boolean thisCharacterIsASpace = false;
    if (weAreAtTheEnd == false) {
      if (commandText[i] == ' ') {
        thisCharacterIsASpace = true;
      }
    }

    if (weAreAtTheEnd == true || thisCharacterIsASpace == true) {
      if (i > wordStartPosition) {
        if (wordCount < 4) {
          wordList[wordCount] = commandText.substring(wordStartPosition, i);
          wordCount = wordCount + 1;
        }
      }
      wordStartPosition = i + 1;
    }
  }

  if (wordCount == 0) {
    return;
  }

  String firstWord = wordList[0];

  if (firstWord == "ON") {
    handleOnOff(wordList, wordCount, true);
  }
  else if (firstWord == "OFF") {
    handleOnOff(wordList, wordCount, false);
  }
  else if (firstWord == "BLINK") {
    handleBlink(wordList, wordCount);
  }
  else if (firstWord == "PATTERN") {
    handlePattern(wordList, wordCount);
  }
  else if (firstWord == "STATUS") {
    printStatus();
  }
  else if (firstWord == "HELP") {
    printHelp();
  }
  else {
    Serial.println("Unknown command. Type HELP for a list.");
  }
}

// -------------------- ON and OFF commands --------------------

void handleOnOff(String wordList[], int wordCount, boolean turnOn) {

  if (wordCount < 2) {
    Serial.println("Usage: ON/OFF <led number or ALL>");
    return;
  }

  // A manual ON/OFF command cancels any pattern that is running.
  currentPattern = PATTERN_NONE;

  if (wordList[1] == "ALL") {

    int i;
    for (i = 0; i < NUM_LEDS; i = i + 1) {
      if (turnOn == true) {
        setLed(i, MODE_ON);
      }
      else {
        setLed(i, MODE_OFF);
      }
    }

    if (turnOn == true) {
      Serial.println("All LEDs ON");
    }
    else {
      Serial.println("All LEDs OFF");
    }

    return;
  }

  int ledNumber = wordList[1].toInt();
  int ledIndex = ledNumber - 1;

  if (ledIndex < 0 || ledIndex >= NUM_LEDS) {
    Serial.println("Invalid LED number.");
    return;
  }

  if (turnOn == true) {
    setLed(ledIndex, MODE_ON);
    Serial.print("LED ");
    Serial.print(ledNumber);
    Serial.println(" ON");
  }
  else {
    setLed(ledIndex, MODE_OFF);
    Serial.print("LED ");
    Serial.print(ledNumber);
    Serial.println(" OFF");
  }
}

// -------------------- BLINK command --------------------

void handleBlink(String wordList[], int wordCount) {

  if (wordCount < 3) {
    Serial.println("Usage: BLINK <led number> <interval in milliseconds>");
    return;
  }

  int ledNumber = wordList[1].toInt();
  int ledIndex = ledNumber - 1;
  long blinkSpeed = wordList[2].toInt();

  if (ledIndex < 0 || ledIndex >= NUM_LEDS || blinkSpeed <= 0) {
    Serial.println("Invalid LED number or interval.");
    return;
  }

  currentPattern = PATTERN_NONE;

  ledMode[ledIndex] = MODE_BLINK;
  ledBlinkInterval[ledIndex] = blinkSpeed;
  ledLastToggleTime[ledIndex] = millis();
  ledIsOn[ledIndex] = true;
  digitalWrite(ledPins[ledIndex], HIGH);

  Serial.print("LED ");
  Serial.print(ledNumber);
  Serial.print(" blinking every ");
  Serial.print(blinkSpeed);
  Serial.println(" ms");
}

// -------------------- PATTERN command --------------------

void handlePattern(String wordList[], int wordCount) {

  if (wordCount < 2) {
    Serial.println("Usage: PATTERN CHASE <ms>  or  PATTERN ALT <ms>  or  PATTERN OFF");
    return;
  }

  if (wordList[1] == "OFF") {
    currentPattern = PATTERN_NONE;

    int i;
    for (i = 0; i < NUM_LEDS; i = i + 1) {
      setLed(i, MODE_OFF);
    }

    Serial.println("Pattern stopped.");
    return;
  }

  long speed = 300;
  if (wordCount >= 3) {
    speed = wordList[2].toInt();
  }
  if (speed <= 0) {
    speed = 300;
  }

  patternInterval = speed;
  lastPatternTime = millis();
  patternPosition = 0;
  alternateState = false;

  int i;
  for (i = 0; i < NUM_LEDS; i = i + 1) {
    setLed(i, MODE_OFF);
  }

  if (wordList[1] == "CHASE") {
    currentPattern = PATTERN_CHASE;
    Serial.println("Chase pattern started.");
  }
  else if (wordList[1] == "ALT") {
    currentPattern = PATTERN_ALT;
    Serial.println("Alternate pattern started.");
  }
  else {
    Serial.println("Unknown pattern. Use CHASE or ALT.");
  }
}

// -------------------- Handling blinking LEDs every loop --------------------

void updateBlinkingLeds() {

  unsigned long currentTime = millis();

  int i;
  for (i = 0; i < NUM_LEDS; i = i + 1) {

    if (ledMode[i] == MODE_BLINK) {

      unsigned long timeSinceLastToggle = currentTime - ledLastToggleTime[i];

      if (timeSinceLastToggle >= ledBlinkInterval[i]) {

        if (ledIsOn[i] == true) {
          ledIsOn[i] = false;
          digitalWrite(ledPins[i], LOW);
        }
        else {
          ledIsOn[i] = true;
          digitalWrite(ledPins[i], HIGH);
        }

        ledLastToggleTime[i] = currentTime;
      }
    }
  }
}

// -------------------- Handling the light show pattern every loop --------------------

void updatePattern() {

  if (currentPattern == PATTERN_NONE) {
    return;
  }

  unsigned long currentTime = millis();
  unsigned long timeSinceLastStep = currentTime - lastPatternTime;

  if (timeSinceLastStep < patternInterval) {
    return;
  }

  lastPatternTime = currentTime;

  if (currentPattern == PATTERN_CHASE) {

    int i;
    for (i = 0; i < NUM_LEDS; i = i + 1) {
      digitalWrite(ledPins[i], LOW);
    }

    digitalWrite(ledPins[patternPosition], HIGH);

    patternPosition = patternPosition + 1;
    if (patternPosition >= NUM_LEDS) {
      patternPosition = 0;
    }
  }
  else if (currentPattern == PATTERN_ALT) {

    if (alternateState == true) {
      alternateState = false;
    }
    else {
      alternateState = true;
    }

    int i;
    for (i = 0; i < NUM_LEDS; i = i + 1) {

      boolean thisIsAnEvenPosition = false;
      if (i == 0 || i == 2) {
        thisIsAnEvenPosition = true;
      }

      boolean turnThisLedOn = false;

      if (thisIsAnEvenPosition == true) {
        if (alternateState == true) {
          turnThisLedOn = true;
        }
        else {
          turnThisLedOn = false;
        }
      }
      else {
        if (alternateState == true) {
          turnThisLedOn = false;
        }
        else {
          turnThisLedOn = true;
        }
      }

      if (turnThisLedOn == true) {
        digitalWrite(ledPins[i], HIGH);
      }
      else {
        digitalWrite(ledPins[i], LOW);
      }
    }
  }
}

// --------------------  something  --------------------

void setLed(int ledIndex, int mode) {

  ledMode[ledIndex] = mode;

  if (mode == MODE_ON) {
    ledIsOn[ledIndex] = true;
    digitalWrite(ledPins[ledIndex], HIGH);
  }
  else {
    ledIsOn[ledIndex] = false;
    digitalWrite(ledPins[ledIndex], LOW);
  }
}

// -------------------- STATUS command --------------------

void printStatus() {

  int i;
  for (i = 0; i < NUM_LEDS; i = i + 1) {

    Serial.print("LED ");
    Serial.print(i + 1);
    Serial.print(": ");

    if (ledMode[i] == MODE_OFF) {
      Serial.println("OFF");
    }
    else if (ledMode[i] == MODE_ON) {
      Serial.println("ON");
    }
    else if (ledMode[i] == MODE_BLINK) {
      Serial.print("BLINK (");
      Serial.print(ledBlinkInterval[i]);
      Serial.println(" ms)");
    }
  }

  Serial.print("Pattern: ");

  if (currentPattern == PATTERN_NONE) {
    Serial.println("none");
  }
  else if (currentPattern == PATTERN_CHASE) {
    Serial.println("CHASE");
  }
  else if (currentPattern == PATTERN_ALT) {
    Serial.println("ALT");
  }
}

// -------------------- HELP command --------------------

void printHelp() {
  Serial.println("Commands:");
  Serial.println("  ON <n|ALL>");
  Serial.println("  OFF <n|ALL>");
  Serial.println("  BLINK <n> <ms>");
  Serial.println("  PATTERN CHASE <ms>");
  Serial.println("  PATTERN ALT <ms>");
  Serial.println("  PATTERN OFF");
  Serial.println("  STATUS");
}
