/*
  CP 5 - JOGO GENIUS

  Caio Rocha Fernandes
  Florbela Freitas Oliveira
  Jaquelline Barbosa
*/

// Variáveis Globais
String recebido;
String sequenciaNumerica = "";
int velocidade = 1000;
int buttonState = 0;
//amarelo, azul, verde e vermelho
const int LED_PINS[] = {8, 9, 10, 11}; // Array de LEDs
//amarelo, azul, branco e preto
const int BOTOES_PINS[] = {5, 4, 3, 2}
const long SERIAL_TIMEOUT = 10000; // Timeout de 10 segundos para leitura do Serial
const int NUM_LEDS = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

long numeroGerado;
int sequencia;

// Definindo o enum para os estados do jogo
enum GameState {
  START_GAME,
  LEVEL_1,
  LEVEL_2,
  LEVEL_3,
  LEVEL_4,
  GAME_OVER,
  PLAY_AGAIN
};
GameState stateGame = START_GAME;

void setup() {
  // Iniciar comunicacao serial
  Serial.begin(9600);

  // Definir LEDs como OUTPUTs pinos 8, 9, 10 e 11
  for(int i = 0; i < NUM_LEDS; i++){
    pinMode(LED_PINS[i], OUTPUT);
  }

  // Definir Botões como INPUTs pinos 5, 4, 3 e 2
  for(int i = 0; i < NUM_LEDS; i++){
    pinMode(BOTOES_PINS[i], INPUT_PULLUP);
  }

  //Definir os botões como input
  pinMode();

  // Definir a seed do gerador de números aleatórios
  randomSeed(analogRead(0));
}

void nivelDificuldade() {
	Serial.println("Escolha o nível da dificuldade:");
	Serial.println("(1) Iniciante");
	Serial.println("(2) Médio");
	Serial.println("(3) Hard");

	leserial();
	
	if (recebido.equals("1")) {    
	  velocidade = 1000;  
	  Serial.println("Você escolheu o nível Iniciante.");  
  } else if (recebido.equals("2")) {    
	  velocidade = 500;
	  Serial.println("Você escolheu o nível Médio.");  
  } else if (recebido.equals("3")) {    
	  velocidade = 100;   
	  Serial.println("Você escolheu o nível Hard.");  
  } else {        
	  velocidade = 1000;    
	  Serial.println("Você não escolheu nenhum nível de dificuldade. Por padrão, ficará Iniciante.");  
  }
}

void loop() {
  switch (stateGame) {
    case START_GAME:
      Serial.println("* Início*");
      nivelDificuldade();
      Serial.println("Comecar? (s/n)");
      leserial();
      if (recebido.equalsIgnoreCase("s")){
        stateGame = LEVEL_1;
        Serial.println("Jogo começando...");
        piscaled(1000, 3);
      } else {
        stateGame = GAME_OVER;
        Serial.println("Jogo não iniciado"); 
        piscaled(300, 5);     
      }
      break;

    case LEVEL_1:
      Serial.println("* Nível 1 *");
      geraSequencia(velocidade, 3); 
      leserial();
      //buttonState = digitalRead(BOTOES_PINS[]);
      if (recebido.equals(sequenciaNumerica)){
        stateGame = LEVEL_2;
        Serial.println("Parabéns! Próximo nível.");
        piscaled(velocidade, 3);
      } else {
        stateGame = GAME_OVER;
        Serial.println("Errooooo!!!"); 
        piscaled(300, 5);     
      }
      break;

    case LEVEL_2:
      Serial.println("* Nível 2 *");
      geraSequencia(velocidade, 5); 
      leserial();
      if (recebido.equals(sequenciaNumerica)){
        stateGame = LEVEL_3;
        Serial.println("Parabéns! Próximo nível.");
        piscaled(velocidade, 3);
      } else {
        stateGame = GAME_OVER;
        Serial.println("Errooooo!!!"); 
        piscaled(300, 5);     
      }
      break;

      case LEVEL_3:
        Serial.println("* Nível 3 *");
        geraSequencia(velocidade, 7); 
        leserial();
        if (recebido.equals(sequenciaNumerica)){
          stateGame = LEVEL_4;
          Serial.println("Parabéns! Próximo nível.");
          piscaled(velocidade, 3);
        } else {
          stateGame = GAME_OVER;
          Serial.println("Errooooo!!!"); 
          piscaled(300, 5);
        }
        break;

        case LEVEL_4:
        Serial.println("* Nível 4 *");
        geraSequencia(velocidade, 10); 
        leserial();
        if (recebido.equals(sequenciaNumerica)){
          stateGame = GAME_OVER;
          Serial.println("Parabéns! Você venceu!");
          piscaled(velocidade, 3);
        } else {
          stateGame = GAME_OVER;
          Serial.println("Errooooo!!!"); 
          piscaled(300, 5);
        }
        break;

    case GAME_OVER:
      Serial.println("Game Over"); 
      piscaled(100,5);
      stateGame = PLAY_AGAIN;
      break;

    case PLAY_AGAIN:
      Serial.println("jogar novamente? (s/n)"); 
      leserial();
      if (recebido.equalsIgnoreCase("s")){
        stateGame = LEVEL_1;
        Serial.println("Jogo comecando...");
        piscaled(1000,3);
      } else {
        stateGame = GAME_OVER;
        Serial.println("Jogo nao iniciado"); 
        piscaled(300,5);     
      }
      break;
  }
}

void leserial(){

  Serial.println("* Insera sua resposta *");

  // Aguardar a resposta do usuario pelo tempo definido em SERIAL_TIMEOUT
  long startTime = millis();
  while (Serial.available() == 0 && millis() - startTime < SERIAL_TIMEOUT)
  {}

  // guarda o valor digitado pelo usuario em recebido
  if (Serial.available()) {
    recebido = Serial.readString();
    recebido.trim();
    Serial.println(recebido);
  } else {
    Serial.println("Timeout! Resposta não recebida.");
  }
}

void piscaled(int tempo, int vezes){
  for(int i = 0; i < vezes; i++){
    for(int j = 0; j < NUM_LEDS; j++){
      digitalWrite(LED_PINS[j], HIGH);
      tone(1, 440);
    }
    delay(tempo);
    for(int j = 0; j < NUM_LEDS; j++){
      digitalWrite(LED_PINS[j], LOW);
    }
    delay(tempo);
  }
}

void geraSequencia (int tempo, int sequencia){
  // Criar uma lista de inteiros com o tamanho que e passado como argumento
  int ordemLeds[sequencia];

  // Gerar sequencia aleatoria
  for (int i = 0; i < sequencia; i++){
    ordemLeds[i] = random(1, NUM_LEDS + 1);
  }

  // Inicialmente, a String sequenciaNumerica é uma String vazia
  sequenciaNumerica = "";

  // Pisca os LEDs na sequencia gerada
  for (int j = 0; j < sequencia; j++){
    int ledIndex = ordemLeds[j] - 1;
    digitalWrite(LED_PINS[ledIndex], HIGH);
    delay(tempo);
    digitalWrite(LED_PINS[ledIndex], LOW);
    delay(tempo);
    // Converte a lista numa String   
    sequenciaNumerica += String(ordemLeds[j]);
  }
}