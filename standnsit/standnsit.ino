/*
 * 
 * Code for Stand'n'sit application
 * An utility to remind you it is time to change working position
 * @author Johan Myrsmeden
 * GitHub: @myrsmeden
 * 
 */

// Define the purpose of the GPIO pins
#define led0 11         // First led pin, bit 0
#define led1 = 12       // Second led pin, bit 1
#define trigPin 9       // Pin for trig signal for distance sensor
#define echoPin 8       // Pin for echo signal for distance sensor
#define sitHeight 70    // Height of table when sitting [cm]
#define standHeight 95  // Height of table when standing [cm]
#define touchSensor 10  // Pin for the out signal of the touch sensor

// Define the different modes that you can have
// It is an array of four tuples, where 
// the first value in each tuple is the time you should be sitting and 
// the second value is the time you should be standing
int modes[][4] = {{20*60,20*60},{20*60,20*60},{20*60,20*60},{20*60,20*60}};

// Variable to remember which mode you are in
int mode = 0;

// Boolean for standing or sitting
int standing = 0;

// The timer, our counter variable
int timer = 0;

// Setup function
// Set output and input pins and set the timer
void setup() {
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(touchSensor, INPUT);
  timer = modes[mode][standing];
  Serial.println("Setting up");
}

// Function to get the distance to the floor from the sensor
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

// Function to blink with leds
void blink() {
  digitalWrite(led0, HIGH);
  delay(100);
  digitalWrite(led0, LOW);
  delay(100);
}

// The alarm function
// Blink as long as the distance is not correct
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

// Function to light the leds to show the (binary) value
// stored in mode, to tell the user which mode that is active
void lightLeds(int mode) {
  // The least significant bit
  if ( mode % 2 == 1 ) {
    digitalWrite(led0, HIGH);
  } else {
    digitalWrite(led0, LOW);
  }
  // The most significant bit
  if ( mode > 1 ) {
    digitalWrite(led1, HIGH);
  } else {
    digitalWrite(led1, LOW);
  }
}

// Function to ping the touch sensor 
// and change the mode variable and update the leds
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

// Our loop function
// Decrease the timer until it reaches zero
// Then change standing boolean and update the timer
// NOTE: For now, the timer variable only changes when it reaches zero, 
// you cannot change the timer when running
// It is a temporary solution to prevent cheating
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
