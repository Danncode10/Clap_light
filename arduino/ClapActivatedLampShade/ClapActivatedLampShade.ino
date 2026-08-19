/*
  Clap-Activated Lamp Shade

  Clap 1: LED 1 on
  Clap 2: LED 1 and LED 2 on
  Clap 3: LED 1, LED 2, and LED 3 on
  Clap 4: all LEDs off, then the clap count resets to zero
*/

// Pin assignments
const int SOUND_SENSOR_PIN = 2;  // Sound sensor digital-output (DO) pin
const int LED1_PIN = 9;
const int LED2_PIN = 10;
const int LED3_PIN = 11;

// Most KY-038/KY-037-style modules output LOW when sound is detected.
const int SOUND_ACTIVE_STATE = LOW;
const unsigned long CLAP_WINDOW_MS = 2000;
const unsigned long DEBOUNCE_MS = 80;

int clapCount = 0;
int previousSensorState;
unsigned long lastClapTime = 0;

void setup() {
  pinMode(SOUND_SENSOR_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  turnOffAllLEDs();
  previousSensorState = digitalRead(SOUND_SENSOR_PIN);

  Serial.begin(9600);
  Serial.println("Clap light ready: 1, 2, 3 LEDs on; 4 turns all off.");
}

void loop() {
  unsigned long now = millis();
  int sensorState = digitalRead(SOUND_SENSOR_PIN);

  // Count only the start of a sensor pulse, not every loop while it is active.
  bool clapStarted = previousSensorState != SOUND_ACTIVE_STATE &&
                     sensorState == SOUND_ACTIVE_STATE;

  if (clapStarted && now - lastClapTime >= DEBOUNCE_MS) {
    // A long pause starts a new clap sequence.
    if (clapCount > 0 && now - lastClapTime > CLAP_WINDOW_MS) {
      clapCount = 0;
    }

    lastClapTime = now;
    clapCount++;
    updateLights();
  }

  previousSensorState = sensorState;

  // Reset an unfinished sequence after two seconds, while leaving its LEDs on.
  if (clapCount > 0 && now - lastClapTime > CLAP_WINDOW_MS) {
    clapCount = 0;
    Serial.println("Clap sequence reset.");
  }
}

void updateLights() {
  if (clapCount == 1) {
    setLEDs(HIGH, LOW, LOW);
  } else if (clapCount == 2) {
    setLEDs(HIGH, HIGH, LOW);
  } else if (clapCount == 3) {
    setLEDs(HIGH, HIGH, HIGH);
  } else {  // Fourth clap
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
  setLEDs(LOW, LOW, LOW);
}
