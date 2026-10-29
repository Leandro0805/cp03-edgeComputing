# CP 03 - Alarme de Luminosidade com ESP32 e MQTT
**Disciplina:** Edge Computing  
**Curso:** Ciência da Computação - 2º Ano  
**Turma:** CCPB

## Integrantes do Grupo
* **Leandro Filippini Aguiar Alves** - RM: 568157


---

## Visão Geral do Projeto
Este projeto consiste em um sistema de monitoramento de iluminação em tempo real utilizando um ESP32 e um sensor LDR. Quando a ausência de luz é detectada:
1. O ESP32 dispara localmente um alerta sonoro através de um Buzzer.
2. O ESP32 envia via protocolo MQTT a mensagem `"Alarme disparado"`.
3. Um programa em Python (Subscriber) que está inscrito no mesmo tópico MQTT recebe e exibe o alerta em tempo real no terminal.

---

## Estrutura e Localização dos Arquivos no Repositório

Com base na organização do repositório, os arquivos estão divididos nas seguintes pastas:

```text
cp03-edgecomputing/
├── codigo-python/
│   └── subscriber.py        # Script em Python para escutar o tópico MQTT
├── codigos-esp32/
│   ├── sketch/
│   │   └── sketch.ino       # Código fonte principal do ESP32 (C++/Arduino)
│   ├── diagram.json         # Mapeamento do circuito no Wokwi
│   ├── libraries.txt        # Bibliotecas utilizadas na simulação (PubSubClient)
│   └── wokwi-project.txt    # Arquivo de configuração do projeto Wokwi
└── README.md                # Documentação do projeto
```
## Esquema do Circuito e Pinos Utilizados

O circuito foi montado no Wokwi integrando o ESP32, o sensor LDR (divisor de tensão/módulo) e o Buzzer piezoelétrico.

### Tabela de Mapeamento de Pinos (ESP32)

| Componente | Pino do Componente | Pino do ESP32 | Tipo de Pino | Descrição / Função |
| :--- | :--- | :--- | :--- | :--- |
| **LDR Sensor** | `VCC` | `3V3` | Alimentação | Tensão de operação de 3.3V |
| **LDR Sensor** | `GND` | `GND` | Terra | Referência do circuito |
| **LDR Sensor** | `AO` (Analog Output) | `GPIO 34` | Entrada Analógica (ADC1_CH6) | Leitura do sinal de variação de luminosidade |
| **Buzzer** | `+` (Anodo / Sinal) | `GPIO 25` | Saída Digital / PWM (DAC1) | Sinal de acionamento do alerta sonoro |
| **Buzzer** | `-` (Catodo / GND) | `GND` | Terra | Referência do circuito |

---

##  Instruções de Execução

Siga os passos abaixo para rodar a simulação do ESP32 e o receptor em Python de forma integrada.

### 1. Executando a Simulação do ESP32 no Wokwi
1. Acesse o link do projeto no **Wokwi**: https://wokwi.com/projects/479225297784763393
2. Certifique-se de que os arquivos do projeto estejam organizados na pasta `codigos-esp32`:
   * `sketch/sketch.ino`: Código C++ do ESP32.
   * `diagram.json`: Estrutura do circuito.
   * `libraries.txt`: Deve conter a biblioteca `PubSubClient`.
3. Clique no botão **Play** (Iniciar simulação) no Wokwi.
4. Observe o **Serial Monitor**: o ESP32 se conectará automaticamente à rede Wi-Fi virtual (`Wokwi-GUEST`) e, em seguida, ao Broker MQTT (`broker.hivemq.com`).

### 2. Configurando e Executando o Cliente Python (Subscriber)
1. Certifique-se de ter o **Python 3.x** instalado na sua máquina.
2. Instale a biblioteca do cliente MQTT via terminal/PowerShell:
   ```bash
   pip install paho-mqtt

   ````
3.Após iniciar o código em python, quando a luz não for detectada pelo sensor LDR, uma mensagem de "Alarme disparado" aparecerá no console
