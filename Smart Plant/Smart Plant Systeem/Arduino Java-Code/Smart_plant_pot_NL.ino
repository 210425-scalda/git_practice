
#include "FastLED.h"
 
// How many leds in your strip?
#define NUM_LEDS 6

 //Data pin waar de ledstrip op is aangesloten
#define DATA_PIN 5
 
CRGB leds[NUM_LEDS];

// Deze constanten worden niet veranderd, het zijn de namen voor de pins die in gebruik zijn:
const int ledPin = 2;                                             // Digitale output pin waar de LED aan verbonden is.
const int pumpPin = 12;                                           // Digitale output pin waar de water pomp aan verbonden is
const int waterLevelPin = A3;                                      // Analoge pin waar de water level sensor aan verbonden is
const int moistureSensorPin = 7;                               // Digitale input pin waar de grondvochtigheid mee gemeten word


// These are the values to edit:

double checkInterval = 100;                                      //Tijdsintervallen waarop de grondvochtigheids word gecheckt -                    het is standaard op een half uur ingestelt = 900000
int waterLevelThreshold = 170;                          // Drempelwaarde waarop de LED gaat knipperen om voor laag                                              laag water niveau te waarschuwen – verander dit op basis van de test
int emptyReservoirTimer = 15;                   // Hoe lang de LED gaat knipperen als de watertank bijgevuld moet worden            standaard staat dit op = 30mins
int amountToPump = 2000;                                     // Hoe lang de pomp aan gaat als de plant water nodig heeft

// Global temp values

int sensorWaterLevelValue = 0;                            // Hier word de waarde van de waterlevel sensor tijdelijk opgeslagen
int moistureSensorValue = 0;                   // Hier word de waarde van de grondvochtigheids sensor tijdelijk opgeslagen 

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);  
    pinMode(ledPin, OUTPUT);
    pinMode(pumpPin, OUTPUT);
    pinMode(moistureSensorPin, INPUT);
    FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
                                                             //De led knippert 5 keer om te laten zien dat hij aan staat en dat de code werkt:
     for (int i=0; i <= 4; i++){
        digitalWrite(ledPin, HIGH);
        delay(300);
        digitalWrite(ledPin, LOW);
        delay(300);
      }
      delay(2000);

    digitalWrite(ledPin, HIGH);                                   // De LED word aan gezet 
}

void loop() {
  // De code die herhaaldelijk moet draaien staat hier onder:
  
  sensorWaterLevelValue = analogRead(waterLevelPin);              //Lees de waarde van de waterlevel sensor
  Serial.print("Water level sensor value: ");                        //print het naar de monitor
  Serial.println(sensorWaterLevelValue);

  if (sensorWaterLevelValue < waterLevelThreshold){                   //check of het waterniveau voldoende is        

      for (int i=0; i <= emptyReservoirTimer; i++){  
        digitalWrite(ledPin, LOW);                                    //lamp gaat uit als reservoir leeg is
        delay(1000);
        digitalWrite(ledPin, HIGH);                                   //Lamp gaat aan als reservoir vol is
        delay(1000);  
      }
  }
  else {
    digitalWrite(ledPin, HIGH);
    delay(checkInterval);                                        //zorgt voor een Delay voordat het soil moisture level gecheckt word
  }
      for(int i=0; i<NUM_LEDS; i++){                              //Dit is de code waarin de RGB kleur word gedefinieerd.
        leds[i] = CHSV(160, 255, 128);                            //Deze rij definieerd de kleur die pulseerd tijdens elke soil moisture check
        FastLED.show();
        delay(100);                                               //dit is de delay tussen elke pulse
        leds[i] = CHSV(200,255,255);                              //Deze rij definieerd de kleur die blijft branden
        FastLED.show();
    }


// check soil moisture level

      moistureSensorValue = digitalRead(moistureSensorPin);  //leest de soil moisture sensor af en slaat de waarde op
      Serial.print("Soil moisture sensor is currently: ");
      Serial.print(moistureSensorValue);
      Serial.println(" ('1' means soil is too dry and '0' means the soil is moist enough.)");

      if (moistureSensorValue ==1){
                                                                 //zet de pomp aan 
        digitalWrite(pumpPin, HIGH);
          Serial.println("pump on");
        delay(amountToPump);                                      //blijf water pompen
        digitalWrite(pumpPin, LOW);
         Serial.println("pump off");
        delay(800);                                              //wachten op nieuwe reading van de soil moisture sensor
      }



}
