//input this code to test UV sensor
void setup() {
    Serial.begin(115200);
    pinMode(A8, INPUT);
}
void loop() {
    int uvsensor = analogRead(A8);
    Serial.print("UV intensity: ");
    Serial.println(uvsensor);
    delay(50);
}
