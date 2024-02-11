#include <ThingSpeak.h>               // add library
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <Stepper.h>
#include <DHT11.h>

WiFiClient  client;
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

unsigned long counterChannelNumber = 2353863;                // Channel ID
const char * myCounterReadAPIKey = "9S1YMDLG40TTPC0T";      // Read API Key
const char * myCounterWriteAPIKey = "PLWSYZEHV69DLP3Q";      // Write API Key

int pos = 0;    // variable to store the servo position

const int buzzer = D5; //buzzer to ESP8266 pin D5
const int LED_RED = D6;

// Defines the number of steps per rotation
const int stepsPerRevolution = 2038;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, D1, D3, D2, D4);

DHT11 dht11(D7);


void setup()
{
  Serial.begin(115200);
  myservo.attach(D0);  // attaches the servo on pin 0 to the servo object
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 5 as an output
  pinMode(LED_RED, OUTPUT);
  Serial.println();

  WiFi.begin("HOKAZONO_FIBER_2", "KeanReikaClyde2?");                 // write wifi name & password

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}

void loop()
{
  int A = ThingSpeak.readLongField(counterChannelNumber, 1, myCounterReadAPIKey);
  Serial.println(A);
  if (A == 1){
    Serial.println(A);
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15); 
    }
  }
  int B = ThingSpeak.readLongField(counterChannelNumber, 2, myCounterReadAPIKey);
  Serial.println(B);
  if (B == 1){
    Serial.println(B);
    myStepper.setSpeed(5);
	  myStepper.step(stepsPerRevolution);
	  delay(1000);
  }
  int C = ThingSpeak.readLongField(counterChannelNumber, 3, myCounterReadAPIKey);
  Serial.println(B);
  if (C == 1){
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);
  }
  int D = ThingSpeak.readLongField(counterChannelNumber, 4, myCounterReadAPIKey);
  Serial.println(D);
  if (D == 1){
    digitalWrite(LED_RED, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_RED, LOW);   // turn the LED off by making the voltage LOW
    delay(1000); 
  }
  int E = ThingSpeak.readLongField(counterChannelNumber, 7, myCounterReadAPIKey);
  Serial.println(E);
  if (E == 1){
    int temperature = dht11.readTemperature();
    delay(50);
    int humidity = dht11.readHumidity();
    Serial.println("Temperature: ");
    Serial.println(temperature);
    Serial.println("Humidity: ");
    Serial.println(humidity);
    delay(1000);
    ThingSpeak.writeField(counterChannelNumber, 5, temperature, myCounterWriteAPIKey); //Update Temperature in ThingSpeak
    delay(5000);
    ThingSpeak.writeField(counterChannelNumber, 6, humidity, myCounterWriteAPIKey); //Update Humidity in ThingSpeak
    delay(100);
  }
}