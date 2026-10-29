import paho.mqtt.client as mqtt

# Configurações do Broker e Tópico (Devem ser EXATAMENTE iguais às do ESP32)
BROKER = "broker.hivemq.com"
PORTA = 1883
TOPICO = "fiap/edge/cp03/alarme" # Ajuste para o mesmo tópico definido no ESP32

# Função chamada quando a conexão com o Broker é estabelecida
def on_connect(client, userdata, flags, rc, properties=None):
    if rc == 0:
        print(f"Conectado ao Broker MQTT ({BROKER}) com sucesso!")
        # Inscreve-se (subscribes) no tópico para escutar mensagens
        client.subscribe(TOPICO)
        print(f"Aguardando mensagens no tópico: {TOPICO}...\n")
    else:
        print(f"Falha na conexão. Código de retorno: {rc}")

# Função chamada automaticamente quando uma nova mensagem é recebida
def on_message(client, userdata, msg):
    mensagem_recebida = msg.payload.decode("utf-8")
    print(f"[ALERTA RECEBIDO] Tópico '{msg.topic}': {mensagem_recebida}")

# Inicialização do cliente MQTT (paho-mqtt v2.x)
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_message = on_message

# Conecta ao broker e entra no loop contínuo de escuta
print("Conectando ao broker...")
client.connect(BROKER, PORTA, 60)

try:
    client.loop_forever()
except KeyboardInterrupt:
    print("\nPrograma encerrado pelo utilizador.")
    client.disconnect()