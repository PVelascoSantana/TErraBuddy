
const int PIN_HUMEDAD = A0;

void setup() {
    Serial.begin(9600);
}

void loop() {
    int humedad = analogRead(PIN_HUMEDAD);

    Serial.print("Humedad (RAW): ");
    Serial.println(humedad);

    delay(1000);
}
