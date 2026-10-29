#include <WiFi.h>
#include <PubSubClient.h>

// Configurações dos Pinos
const int PIN_LDR = 34;    // Pino de entrada analógica do LDR
const int PIN_BUZZER = 25; // Pino de saída digital/PWM do Buzzer

// Configurações de Rede e MQTT
const char* SSID_WIFI = "Wokwi-GUEST"; // Rede padrão do Wokwi
const char* SENHA_WIFI = "";

// Usaremos o broker público do HiveMQ como exemplo
const char* BROKER_MQTT = "broker.hivemq.com";
const int PORTA_MQTT = 1883;
const char* TOPICO_ALARME = "fiap/edge/cp03/alarme"; // Modifique para um tópico único

// Limiar do LDR (Ajuste conforme os testes com luz/escuridão no Wokwi)
const int LIMIAR_LUZ = 2000; 

WiFiClient espClient;
PubSubClient client(espClient);

void conectaWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  
  Serial.print("Conectando ao WiFi...");
  WiFi.begin(SSID_WIFI, SENHA_WIFI);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectado!");
}

void conectaMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao Broker MQTT...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado ao MQTT!");
    } else {
      Serial.print("Falha na conexão. Código: ");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 2 segundos...");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_LDR, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  conectaWiFi();
  client.setServer(BROKER_MQTT, PORTA_MQTT);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    conectaWiFi();
  }
  if (!client.connected()) {
    conectaMQTT();
  }
  client.loop();

  // Leitura analógica do LDR (retorna valores entre 0 e 4095)
  int valorLDR = analogRead(PIN_LDR);
  Serial.print("Leitura LDR: ");
  Serial.println(valorLDR);

  // Lógica da Atividade
  // No Wokwi, no sensor LDR: valor alto representa ausência de luz (escuridão)
  if (valorLDR > LIMIAR_LUZ) { 
    // SEM LUZ: Dispara o buzzer e publica via MQTT
    tone(PIN_BUZZER, 1000); // Emite um som de 1000Hz no buzzer
    
    Serial.println("Alerta: Sem luz! Publicando MQTT...");
    client.publish(TOPICO_ALARME, "Alarme disparado");
    
    delay(1000); // Evita o envio excessivo de mensagens contínuas
  } else {
    // COM LUZ: Buzzer desligado
    noTone(PIN_BUZZER);
    digitalWrite(PIN_BUZZER, LOW);
  }

  delay(200);
}