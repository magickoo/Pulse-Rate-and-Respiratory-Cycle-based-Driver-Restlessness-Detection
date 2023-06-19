#include <DallasTemperature.h>

#include <OneWire.h>

   int LED = 9;  
       int BUZZER = 10;
      int ALCOHOL_sensor = 2;// MQ-6 SENSOR  
      int ALCOHOL_detected;  
      #include <Wire.h>
#include <MAX30100_PulseOximeter.h> //library initialization
 
#define REPORTING_PERIOD_MS     1000 //update frequency 1000ms
 
PulseOximeter pox;
uint32_t tsLastReport = 0;
int greenLedPin = 5;
int whiteLedPin = 3;
int redLedPin = 4;

int temp_sensor = 1;

float temperature = 0;
int lowerLimit = 15;
int higherLimit = 35;
OneWire oneWirePin(temp_sensor);
DallasTemperature sensors(&oneWirePin);
       void onBeatDetected()
{
    Serial.println("Beat!");
}
      void setup()  
       {  
         Serial.begin(9600);  
         pinMode(LED, OUTPUT);  
         pinMode(BUZZER, OUTPUT);  
         pinMode(ALCOHOL_sensor, INPUT);  
         
  
  //Setup the LEDS to act as outputs
  pinMode(redLedPin,OUTPUT);
  pinMode(greenLedPin,OUTPUT);
  pinMode(whiteLedPin,OUTPUT);
  
  sensors.begin();
  Serial.begin(11520);
    Serial.print("Initializing pulse oximeter..");
 
    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
    Serial.begin(9600);
       }  

 
       void loop()  
       {  
         ALCOHOL_detected = digitalRead(ALCOHOL_sensor);  
          Serial.println(ALCOHOL_detected);  
         if (ALCOHOL_detected == 1)  
         {  
           Serial.println("ALCOHOL detected...");  
           digitalWrite(LED, HIGH);  
           digitalWrite(BUZZER, HIGH);  
         }  
         else  
         {  
           Serial.println("No ALCOHOL detected ");  
           digitalWrite(LED, LOW);  
           digitalWrite(BUZZER, LOW);  
         }  
         // Make sure to call update as fast as possible
   
    Serial.print("Requesting Temperatures from sensors: ");
  sensors.requestTemperatures(); 
  Serial.println("DONE");
  
  temperature = sensors.getTempCByIndex(0);
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
  digitalWrite(whiteLedPin, LOW);
  
  Serial.print("Temperature is ");
  Serial.print(temperature);
    if(temperature <= lowerLimit){ 
    Serial.println(", greenish Yellow LED is Activated"); 
    digitalWrite(whiteLedPin, HIGH); } 
  else if(temperature > lowerLimit && temperature < higherLimit){ 
    Serial.println(", Green LED is Activated"); 
    digitalWrite(greenLedPin, HIGH);
  } 
  else if(temperature >= higherLimit){
    Serial.println(", Red LED is Activated");
    digitalWrite(redLedPin, HIGH);
  }
  delay(500);
   pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
 
        tsLastReport = millis();
    }
}
       


   
