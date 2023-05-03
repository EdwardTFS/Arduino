#define LED 2

void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
}

void loop() {
    digitalWrite(LED, HIGH);
    Serial.print("ON\r\n"); 
    delay(1000);
    digitalWrite(LED, LOW);
    Serial.print("OFF\r\n"); 
    delay(1000);
}

