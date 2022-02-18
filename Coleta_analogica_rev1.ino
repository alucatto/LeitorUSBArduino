//------------------------------------------------
// SETUP INICIAL
//------------------------------------------------
// Definição de constantes de comunicação
const int HANDSHAKE = 0;
const int VOLTAGE_REQUEST = 1;
const int ON_REQUEST = 2;
const int STREAM = 3;

// Definição de variáveis
long loop_timer_now;          //tempo atual (em milissegundos)
long previous_millis;         //tempo anterior (em milissegundos)
long loop_time;               //tempo do loop
int value = 0;  // variavel com o valor da leitura
int inByte = 0; //Variavel de comunicação
int daqMode = ON_REQUEST; //Status da comunicação

// Definição de portas físicas do Arduino
int analogPin = A0; // Pino analógico

//Funções
void printVoltage(){
  // lê o valor do pino analógico
  value = analogRead(analogPin);
  // contador do tempo de loop
  previous_millis = loop_timer_now; loop_timer_now = millis();
  loop_time = loop_timer_now - previous_millis;

  // escreve o resultado
  if (Serial.availableForWrite()) {
    String outstr = String(String(loop_timer_now, DEC) + "," +String(loop_time, DEC) + "," + String(value, DEC));
    Serial.println(outstr);
  }
}

// Inicialização do código
void setup() {
  Serial.begin(115200);           //  setup serial
  loop_timer_now = millis();
}

//----------------------------------------------
//CÓDIGO
//----------------------------------------------
void loop() {
 // Se está transferindo dados continuamente (streaming mode)
  if (daqMode == STREAM) {
    printVoltage();    
  }
  // Checa se algum dado foi enviado ao Arduino e responde de acordo
  if (Serial.available() > 0) {
    // Lê sob demanda
    inByte = Serial.read();

    // Handshake
    if (inByte == HANDSHAKE){
      if (Serial.availableForWrite()) {
          Serial.println("Handshake message received.");
      }
    }
  // Se os dados estão sendo solicitados, coleta e escreve
    else if (inByte == VOLTAGE_REQUEST) printVoltage();

    // Modifica o modo de aquisição
    else if (inByte == ON_REQUEST) daqMode = ON_REQUEST;
    else if (inByte == STREAM) daqMode = STREAM;
}
}
