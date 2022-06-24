#define motorPin 13

const int moisturePin = A0; 
unsigned long interval = 10000;
unsigned long previousMillis = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  float  moisturePercentage = ( 100.00 - ( (analogRead(moisturePin) / 1023.00) * 100.00 ) );
  /*moisturePercentage=40.00;*/
  Serial.print("Soil Moisture is  = ");
  Serial.print(moisturePercentage);
  Serial.println("%");
  if (moisturePercentage < 25) {
    digitalWrite(motorPin, HIGH);         // tun on motor
  }
  if (moisturePercentage > 25 && moisturePercentage < 30) {
    digitalWrite(motorPin, HIGH);        //turn on motor pump
  }
  if (moisturePercentage > 30) {
    digitalWrite(motorPin, LOW);          // turn off mottor
  }
  delay(1000);
}
