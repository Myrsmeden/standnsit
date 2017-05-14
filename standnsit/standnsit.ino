int led1 = 11;
int led2 = 12;
#define trigPin 9         // Pin for trig signal
#define echoPin 8         // Pin for echo signal
#define sitHeight 70      // Height of table when sitting [cm]
#define standHeight 95   // Height of table when standing [cm]
#define touchSensor 10

int mode = 0;
int modes[][2] = {{20*60,20*60},{20*60,20*60},{20*60,20*60},{20*60,20*60}};

int standing = 0;
int timer = 0;

int setting = 0;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(touchSensor, INPUT);
  timer = modes[mode][standing];
  Serial.println("Setting up");
}

int getDistance() {
  long duration, distance;
  digitalWrite(trigPin, LOW);         // Send LOW signal to ultrasonic sensor
  delayMicroseconds(2);               // Wait two microseconds
  digitalWrite(trigPin, HIGH);        // Send HIGH signal to ultrasonic sensor
  delayMicroseconds(10);              // Wait ten microseconds
  digitalWrite(trigPin, LOW);         // Send LOW signal to ultrasonic sensor
  duration = pulseIn(echoPin, HIGH);  // Fetch duration from echo pin
  distance = (duration/2) / 29.1;     // Calculate distance
  //Serial.print(distance);
  //Serial.println(" cm");
  return distance;
}

void blink() {
  digitalWrite(led1, HIGH);
  delay(100);
  digitalWrite(led1, LOW);
  delay(100);
}

void alarm() {
  if(standing) {
      while(getDistance() < standHeight) {
        blink();
      }
    } else {
      while(getDistance() > sitHeight) {
        blink();
      }
    }
}

void lightLeds(int mode) {
  Serial.println(mode);
  if ( mode % 2 == 1 ) {
    digitalWrite(led1, HIGH);
  } else {
    digitalWrite(led1, LOW);
  }

  if ( mode > 1 ) {
    digitalWrite(led2, HIGH);
  } else {
    digitalWrite(led2, LOW);
  }
}

void checkModeChange() {
  if (digitalRead(touchSensor) == HIGH) {
    mode++;
    Serial.println("Mode is:");
    Serial.println(mode);
    if ( mode > 3) {
      mode -= 4;
    }
    lightLeds(mode);
    delay(400);
  }
  
}

void loop() {
  while(timer > 0) {
    checkModeChange();
    if(standing) {
      if(getDistance() < standHeight) {
        alarm();
        lightLeds(mode);
      }
    } else {
      if(getDistance() > sitHeight) {
        alarm();
        lightLeds(mode);
      }
    }
    delay(1000);
    timer--;
  }
  standing = 1 - standing;
  alarm();
  lightLeds(mode);
  timer = modes[mode][standing];
  
}
