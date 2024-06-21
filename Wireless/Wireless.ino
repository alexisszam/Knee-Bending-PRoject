#include <WiFi.h>

const char *ssid = "ESP32-AP";     // Nombre de la red WiFi
const char *password = "password"; // Contraseña de la red WiFi
const char *host = "192.168.4.1";  // Dirección IP del servidor Arduino (obtenida del Monitor Serie del Arduino inalámbrico)

WiFiClient client;

int previousSensorValue = 0;
int currentSensorValue = 0;
int nextSensorValue = 0;
bool peakDetected = false;

void setup() {
  Serial.begin(115200); // Inicializar la comunicación serial a 115200 bps

  // Conectar el ESP32 a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando al Arduino...");
  }
  Serial.println("Conectado al Arduino");

  // Establecer conexión con el servidor en el punto de acceso
  if (!client.connect(host, 80)) {
    Serial.println("Error al conectar al Arduino");
  }
}

void loop() {
  // Leer el valor del sensor en el pin A0
  currentSensorValue = analogRead(A0);

  // Ajustar el valor leído para que el voltaje máximo de 3.5V corresponda a 1023
  float maxVoltage = 3.5;
  float adcMaxVoltage = 3.3;
  int adjustedSensorValue = currentSensorValue * (maxVoltage / adcMaxVoltage);

  // Asegurarse de que el valor ajustado no exceda 1023
  if (adjustedSensorValue > 1023) {
    adjustedSensorValue = 1023;
  }

  // Detectar un pico comparando con el valor anterior y posterior
  if (adjustedSensorValue > previousSensorValue && adjustedSensorValue > nextSensorValue) {
    // Enviar datos al servidor en el punto de acceso solo si se detecta un pico
    if (client.connected() && !peakDetected) {
      Serial.println(adjustedSensorValue);
      client.print(adjustedSensorValue);  // Enviar el valor del sensor como texto
      client.println();                   // Enviar un retorno de carro y nueva línea
      peakDetected = true;                // Marcar que se detectó un pico
    }
  } else {
    peakDetected = false;  // Resetear la bandera si no se detecta un pico
  }

  // Actualizar valores para la próxima iteración
  previousSensorValue = currentSensorValue;
  // Leer el siguiente valor del sensor para la próxima iteración
  delay(10); // Pequeña pausa para asegurar la lectura correcta
  nextSensorValue = analogRead(A0);

  delay(100); // Esperar 100 milisegundos antes de la siguiente iteración del bucle
}