#define vermelho_norte 8
#define barulho 9
#define sensor_umidade_norte A0
#define amarelo_norte 7
#define verde_norte 6

#define vermelho_sul 12
#define sensor_umidade_sul A1
#define amarelo_sul 10
#define verde_sul 11
#define motor 5
int status_motor;
int status_chuva;
#define sensor_chuva A2

unsigned long ultimoBeep = 0;
const unsigned long intervaloBeep = 2000;

void tocarAlarme() {
  if (millis() - ultimoBeep >= intervaloBeep) {
    ultimoBeep = millis();

    digitalWrite(barulho, HIGH);
    delay(200);
    digitalWrite(barulho, LOW);
  }
}

void setup() {
  pinMode(vermelho_norte, OUTPUT);
  pinMode(barulho, OUTPUT);
  pinMode(amarelo_norte, OUTPUT);
  pinMode(verde_norte, OUTPUT);

  pinMode(vermelho_sul, OUTPUT);
  pinMode(amarelo_sul, OUTPUT);
  pinMode(verde_sul, OUTPUT);

  pinMode(motor, OUTPUT);

  // Relé ativo em LOW: começa desligado
  digitalWrite(motor, HIGH);

  Serial.begin(9600);

  //Serial.println("Sistema Iniciado");
  delay(3000);
}

void loop() {

  int analog_chuva = analogRead(sensor_chuva);
  float umidade_chuva = map(analog_chuva, 1023, 300, 0, 100);

  int analog_umidade_norte = analogRead(sensor_umidade_norte);
  float umidade_norte = map(analog_umidade_norte, 1023, 300, 0, 100);

  int analog_umidade_sul = analogRead(sensor_umidade_sul);
  float umidade_sul = map(analog_umidade_sul, 1023, 300, 0, 100);

 // Serial.println("------------------------------------");
  //Serial.print("Valor do Sensor Norte: ");
  //Serial.println(analog_umidade_norte);

 // Serial.print("Valor do Sensor Sul: ");
 // Serial.println(analog_umidade_sul);

  //Serial.print("Umidade Norte: ");
  //Serial.print(umidade_norte);
  //Serial.println("%");

  //Serial.print("Umidade Sul: ");
  //Serial.print(umidade_sul);
  //Serial.println("%");

  bool soloSeco = false;

  // NORTE
  if (umidade_norte < 50) {
    digitalWrite(vermelho_norte, HIGH);
    digitalWrite(amarelo_norte, LOW);
    digitalWrite(verde_norte, LOW);

    soloSeco = true;

    tocarAlarme();

    //Serial.println("Sensor Norte: Critico (seco)");
  }
  else if (umidade_norte <= 75) {
    digitalWrite(vermelho_norte, LOW);
    digitalWrite(amarelo_norte, LOW);
    digitalWrite(verde_norte, HIGH);

    //Serial.println("Sensor Norte: Nivel Ideal");
  }
  else {
    digitalWrite(vermelho_norte, LOW);
    digitalWrite(amarelo_norte, HIGH);
    digitalWrite(verde_norte, LOW);

    //Serial.println("Sensor Norte: Excesso Água");
  }

  // SUL
  if (umidade_sul < 50) {
    digitalWrite(vermelho_sul, HIGH);
    digitalWrite(amarelo_sul, LOW);
    digitalWrite(verde_sul, LOW);

    soloSeco = true;

    tocarAlarme();

    //Serial.println("Sensor Sul: Critico (seco)");
  }
  else if (umidade_sul <= 75) {
    digitalWrite(vermelho_sul, LOW);
    digitalWrite(amarelo_sul, LOW);
    digitalWrite(verde_sul, HIGH);

    //Serial.println("Sensor Sul: Nivel Ideal");
  }
  else {
    digitalWrite(vermelho_sul, LOW);
    digitalWrite(amarelo_sul, HIGH);
    digitalWrite(verde_sul, LOW);

    //Serial.println("Sensor Sul: Excesso de Água");
  }

  // CONTROLE DO MOTOR
  if (umidade_chuva > 50) {
    // Chovendo = desliga motor
    digitalWrite(motor, HIGH);
    status_chuva = 1;
    status_motor = 0;
    //Serial.println("-----------------------------------");
    //Serial.println("ESTA CHOVENDO | MOTOR DESLIGADO!!");
  }
  else if (soloSeco) {
    // Solo seco = liga motor
    digitalWrite(motor, LOW);
    status_motor = 1;
    status_chuva = 0;
    //Serial.println("-----------------------------------");
    //Serial.println("PAROU DE CHOVER OU NÃO ESTÁ CHOVENDO!! | MOTOR LIGADO");
  }
  else {
    // Verde ou amarelo = desliga motor
    digitalWrite(motor, HIGH);

    //Serial.println("-----------------------------------");
    //Serial.println("MOTOR DESLIGADO");
  }
  Serial.print(analog_umidade_norte); 
  Serial.print(","); 
  Serial.print(analog_umidade_sul); 
  Serial.print(","); 
  Serial.print(status_chuva); // 0 ou 1
  Serial.print(","); 
  Serial.println(status_motor);  // 0 ou 1 Último valor com println quebrando a linha
  delay(2000);
}