#include <AFMotor.h>

// Definição dos motores nos canais M2 e M3
AF_DCMotor motorEsquerdo(2);  // Motor esquerdo conectado no M2
AF_DCMotor motorDireito(3);   // Motor direito conectado no M3

// Definição dos sensores infravermelhos
#define sensorEsquerda A2  // Sensor esquerdo
#define sensorDireita  A3  // Sensor direito

// Velocidade dos motores
int velocidade = 100;

// Correção de velocidade para ajuste fino dos motores, caso um seja mais rápido que o outro
int correcaoMotorEsquerdo = 0;
int correcaoMotorDireito = 0;

// Variáveis para salvar leituras anteriores, para caso o robô saia da linha e precise recuperar
boolean leituraAnteriorEsquerda = false;
boolean leituraAnteriorDireita = false;

void setup() {
  Serial.begin(9600);

  // Define os pinos dos sensores como entrada
  pinMode(sensorEsquerda, INPUT_PULLUP);
  pinMode(sensorDireita, INPUT_PULLUP);
  
  // Inicia os motores para frente, com velocidade constante
  motorEsquerdo.setSpeed(velocidade + correcaoMotorEsquerdo);
  motorDireito.setSpeed(velocidade + correcaoMotorDireito);
  motorEsquerdo.run(FORWARD);
  motorDireito.run(FORWARD);
  
  Serial.println("Aguardando 3 segundos para iniciar o movimento...");
  delay(3000);
}

void loop() {
  // Leitura dos sensores (ajuste para sensores analógicos convertidos para digital)
  boolean valorSensorEsquerda = (analogRead(sensorEsquerda) > 200);
  boolean valorSensorDireita = (analogRead(sensorDireita) > 200);
  
  // Mostra os valores dos sensores no serial monitor
  Serial.print("Sensor esquerda: ");
  Serial.print(valorSensorEsquerda);
  Serial.print(" Sensor direita: ");
  Serial.println(valorSensorDireita);

  // Recupera a leitura anterior se ambos os sensores saírem da linha
  if (!valorSensorEsquerda && !valorSensorDireita) {
    valorSensorEsquerda = leituraAnteriorEsquerda;
    valorSensorDireita = leituraAnteriorDireita;
  }

  // Controle dos motores baseado nas leituras dos sensores
  if (valorSensorEsquerda && valorSensorDireita) {  // Ambos na linha
    motorEsquerdo.setSpeed(velocidade + correcaoMotorEsquerdo);
    motorDireito.setSpeed(velocidade + correcaoMotorDireito);
    motorEsquerdo.run(FORWARD);
    motorDireito.run(FORWARD);
  } 
  else if (valorSensorEsquerda && !valorSensorDireita) {  // Apenas o sensor esquerdo na linha
    motorEsquerdo.setSpeed(0);  // Para o motor esquerdo
    motorDireito.setSpeed(velocidade + correcaoMotorDireito);
    motorDireito.run(FORWARD);
  } 
  else if (!valorSensorEsquerda && valorSensorDireita) {  // Apenas o sensor direito na linha
    motorDireito.setSpeed(0);  // Para o motor direito
    motorEsquerdo.setSpeed(velocidade + correcaoMotorEsquerdo);
    motorEsquerdo.run(FORWARD);
  }

  // Salva o valor atual dos sensores como leitura anterior
  leituraAnteriorEsquerda = valorSensorEsquerda;
  leituraAnteriorDireita = valorSensorDireita;
}
