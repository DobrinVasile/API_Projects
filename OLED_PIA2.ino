#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

// Definire pinii
const int flameSensorPin = 2; // Pinul senzorului de flacără
const int buzzerPin = 4;      // Pinul buzzer-ului
const int ledPin = 3;         // Pinul LED-ului pentru flacără
const int trigPin = 7;        // Pinul trig pentru senzorul ultrasonic
const int echoPin = 8;        // Pinul echo pentru senzorul ultrasonic
const int led1Pin = 11;       // LED 1
const int led2Pin = 12;       // LED 2
const int led3Pin = 13;       // LED 3

// Melodia "Pirates of the Caribbean"
int melody[] = {
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_A4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_B4,
  NOTE_C5, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_A4, NOTE_A4,
  NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_A4,
  NOTE_D5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4,
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_D5,
  NOTE_B4, NOTE_A4, NOTE_G4
};

int noteDurations[] = {
  8, 8, 4, 8, 8, 4, 8, 8,
  8, 8, 4, 8, 8, 4, 8, 8,
  4, 8, 8, 4, 8, 8, 8, 8,
  8, 4, 8, 8, 8, 4, 8, 8,
  4, 8, 8, 8, 4, 8, 8, 8,
  4, 8, 8, 4, 8, 8, 4, 8,
  8, 8, 4, 8, 8, 4, 8, 8,
  4, 8, 8, 8, 4, 8, 8, 8,
  4, 8, 8, 4, 8, 8, 4, 8,
  8, 8, 4, 8, 8, 4, 8, 8
};

void setup() {
  pinMode(flameSensorPin, INPUT);  // Senzor de flacără ca intrare
  pinMode(buzzerPin, OUTPUT);      // Buzzer ca ieșire
  pinMode(ledPin, OUTPUT);         // LED pentru flacără ca ieșire
  pinMode(trigPin, OUTPUT);        // Trig pentru senzor ultrasonic ca ieșire
  pinMode(echoPin, INPUT);         // Echo pentru senzor ultrasonic ca intrare
  pinMode(led1Pin, OUTPUT);        // LED 1
  pinMode(led2Pin, OUTPUT);        // LED 2
  pinMode(led3Pin, OUTPUT);        // LED 3
  Serial.begin(9600);              // Inițializare serială pentru debugging
}

long readUltrasonicDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration / 2) / 29.1; // Conversie la cm
  return distance;
}

void loop() {
  int flameState = digitalRead(flameSensorPin);
  long distance = readUltrasonicDistance(trigPin, echoPin);
  
  Serial.print("Flame state: ");
  Serial.println(flameState);
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Control LED-uri pe baza distanței
  if (distance < 10) {
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, HIGH);
    digitalWrite(led3Pin, HIGH);
  } else if (distance >= 10 && distance < 20) {
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led3Pin, LOW);
  } else if (distance >= 20 && distance < 30) {
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, HIGH);
    digitalWrite(led3Pin, LOW);
  } else if (distance >= 30 && distance < 40) {
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led3Pin, HIGH);
  } else {
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led3Pin, LOW);
  }
  
  // Dacă este detectată o flacără
  if (flameState == LOW) { 
    digitalWrite(ledPin, HIGH); // Aprindere LED pentru flacără
    
    for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(melody[0]); thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(buzzerPin, melody[thisNote], noteDuration);
      
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(buzzerPin); // Oprire ton
    }
    
    digitalWrite(ledPin, LOW); // Stingere LED pentru flacără
    delay(1000); // Așteaptă 10 secunde pentru a permite redarea completă a melodiei
  } else {
    digitalWrite(ledPin, LOW); // Stingere LED pentru flacără
  }
  
  delay(100); // Mică întârziere pentru stabilitate
}
