/*
  Clap-Activated Lamp Shade

  Clap 1: LED 1 on
  Clap 2: LED 1 and LED 2 on
  Clap 3: LED 1, LED 2, and LED 3 on
  Clap 4: all LEDs off, then the clap count resets to zero
*/

// Pin assignments
const int SOUND_ANALOG_PIN = A0;  // Sound sensor analog-output (AO) pin
const int LED1_PIN = 9;
const int LED2_PIN = 10;
const int LED3_PIN = 11;

// Standard LED wiring: Arduino pin -> resistor -> LED anode, LED cathode -> GND.
const int LED_ON = HIGH;
const int LED_OFF = LOW;

// A clap produces a short analog peak. Raise this value if normal room noise
// counts as a clap; lower it (for example, to 25) if normal claps are missed.
const int CLAP_THRESHOLD = 35;
const unsigned long SAMPLE_WINDOW_MS = 25;
const unsigned long CLAP_LOCKOUT_MS = 600;
const unsigned long SENSOR_RELEASE_MS = 250;
const unsigned long DEBUG_INTERVAL_MS = 250;

int clapCount = 0;
int lastSoundPeak = 0;
bool previousSoundActive = false;
unsigned long lastClapTime = 0;
unsigned long lastInactiveTime = 0;
unsigned long lastDebugTime = 0;
bool sensorReady = false;

void setup() {
  pinMode(SOUND_ANALOG_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  turnOffAllLEDs();
  lastInactiveTime = millis();

  Serial.begin(9600);
  Serial.println("Clap light debug ready.");
  Serial.println("Columns: time | peak | active | ready | clap count | LED1 LED2 LED3");
}

void loop() {
  unsigned long now = millis();
  lastSoundPeak = readSoundPeakToPeak();
  bool soundActive = lastSoundPeak >= CLAP_THRESHOLD;

  // Count only the beginning of a loud, short sound peak.
  bool clapStarted = !previousSoundActive && soundActive;

  // Require 250 ms of quiet before accepting another clap.
  if (!soundActive) {
    if (previousSoundActive) {
      lastInactiveTime = now;
    }

    if (now - lastInactiveTime >= SENSOR_RELEASE_MS) {
      sensorReady = true;
    }
  }

  // Ignore new sound for 600 ms after each accepted clap. This prevents a
  // sustained blow or noisy pulse from quickly cycling the LEDs.
  if (sensorReady && clapStarted && now - lastClapTime >= CLAP_LOCKOUT_MS) {
    lastClapTime = now;
    sensorReady = false;
    clapCount++;
    updateLights();
  }

  previousSoundActive = soundActive;

  // Print a steady, paste-friendly record for diagnosing the sensor.
  if (now - lastDebugTime >= DEBUG_INTERVAL_MS) {
    printDebugStatus(now, soundActive);
    lastDebugTime = now;
  }
}

void updateLights() {
  if (clapCount == 1) {
    setLEDs(LED_ON, LED_OFF, LED_OFF);
  } else if (clapCount == 2) {
    setLEDs(LED_ON, LED_ON, LED_OFF);
  } else if (clapCount == 3) {
    setLEDs(LED_ON, LED_ON, LED_ON);
  } else {  // Fourth clap: turn every LED off and begin again at zero.
    turnOffAllLEDs();
    clapCount = 0;
  }

  Serial.print("Clap count: ");
  Serial.println(clapCount);
}

void setLEDs(int led1State, int led2State, int led3State) {
  digitalWrite(LED1_PIN, led1State);
  digitalWrite(LED2_PIN, led2State);
  digitalWrite(LED3_PIN, led3State);
}

void turnOffAllLEDs() {
  setLEDs(LED_OFF, LED_OFF, LED_OFF);
}

int readSoundPeakToPeak() {
  int minimum = 1023;
  int maximum = 0;
  unsigned long sampleStart = millis();

  while (millis() - sampleStart < SAMPLE_WINDOW_MS) {
    int reading = analogRead(SOUND_ANALOG_PIN);
    if (reading < minimum) minimum = reading;
    if (reading > maximum) maximum = reading;
  }

  return maximum - minimum;
}

void printDebugStatus(unsigned long now, bool soundActive) {
  Serial.print(now);
  Serial.print(" ms | peak=");
  Serial.print(lastSoundPeak);
  Serial.print(" | active=");
  Serial.print(soundActive ? "YES" : "NO");
  Serial.print(" | ready=");
  Serial.print(sensorReady ? "YES" : "NO");
  Serial.print(" | claps=");
  Serial.print(clapCount);
  Serial.print(" | LEDs=");
  Serial.print(digitalRead(LED1_PIN) == LED_ON ? "ON " : "OFF ");
  Serial.print(digitalRead(LED2_PIN) == LED_ON ? "ON " : "OFF ");
  Serial.println(digitalRead(LED3_PIN) == LED_ON ? "ON" : "OFF");
}
