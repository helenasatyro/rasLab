const int pingPin = 7;
// inicializa variaveis para os pinos
int choice = 0;
int ativa1 = 2;
int motor1A = 5;
int motor1B = 3;
int ativa2 = 13;
int motor2A = 10;
int motor2B = 11;
int speed = 0;

void setup()
{
  //inicializa os pins de output

  pinMode(ativa1, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor1A, OUTPUT);
  pinMode(ativa2, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  // Configura a taxa de transferência em bits por segundo
  Serial.begin(9600);
  
  Serial.print("Velocidade: ");
 	while (Serial.available() == 0) {
 	}
   	speed = Serial.parseInt(SKIP_ALL, '\n');
  // não consigo imprimir a velocidade
   	Serial.print(speed);
  // quando eu seto a velocidade como menor 
  // que zero o segundo motor não roda e 
  	if (speed >= 0) { choice = 0;} else { choice = 1;}
  	speed = abs(speed);
    if (speed > 255) { speed = 255;}
    delay(200);

  
  // seta os pins de ativação para para alto, ativando 3 e 4 (AE e BE por ex)
  digitalWrite(ativa1, HIGH);
  digitalWrite(ativa2, HIGH);

}

void loop()
{ long duration, cm;

  // ativamos o módulo com um pulso de 2 microssegundos
  // enviamos um low para "limpar" o buffer
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
 
  cm = medicaoDistancia(duration, cm);

 // evita colisões parando ou reduzindo a velocidade
  if (cm <= 5) 
  {
    stop();
  } else if (cm <= 20 && speed > 150 && choice == 1) {
    speed = speed - 70;
  }
  // exibe a dustância medida pelo sensor
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(100);
  
  // se houver informação no buffer (velocidade) delimita a velocidade
 	if (Serial.available() ){
   	speed = Serial.parseInt(SKIP_ALL, '\n');
  	if (speed >= 0) { choice = 0;} else { choice = 1;}
  	speed = abs(speed);
    if (speed > 255) { speed = 255;}
    delay(200);
  }
   
  if (choice == 0){
    analogWrite(motor1B, speed); // liga o motor
    digitalWrite(motor1A, LOW);
    
    analogWrite(motor2B, speed); // liga o motor
    digitalWrite(motor2A, LOW);
  }
  else if (choice == 1){
    analogWrite(motor1A, speed); // anda de ré
    digitalWrite(motor1B, LOW); 
    
    analogWrite(motor2A, speed); // anda de ré
    digitalWrite(motor2B, LOW); 

  }
}

void stop() {
   digitalWrite(ativa1, LOW);
   digitalWrite(ativa2, LOW);
}

int medicaoDistancia(long duration, long cm) {
  // enviamos um pulso e medimos o tempo até que seja detectado o retorno (LOW)
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // converte tempo em distância
  // Tvelocidade do som: 340 m/s ou 29 microssegundos por centímetro
  // Como o sinal tem que ir e voltar, dividimos o tempo ou distância por 2.
  return duration / 29 / 2;
}

