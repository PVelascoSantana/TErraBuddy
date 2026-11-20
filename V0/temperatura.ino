
#include <Wire.h>
#include <Adafruit_SHT31.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando sensor SHT3x...");

  if (sht31.begin(0x44)) {
    Serial.println("Sensor detectado en 0x44");
  }
  else if (sht31.begin(0x45)) {
    Serial.println("Sensor detectado en 0x45");
  }
  else {
    Serial.println("ERROR: Sensor no detectado");
    while (1) {
      delay(1000);
    }
  }
}

void loop() {
  float temperatura = sht31.readTemperature();
  float humedad = sht31.readHumidity();

  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println("Error de lectura");
  }
  else {
    Serial.print("Temperatura: ");
    Serial.print(temperatura, 2);
    Serial.print(" C | Humedad: ");
    Serial.print(humedad, 2);
    Serial.println(" %");
  }

  delay(200);
}
