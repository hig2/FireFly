// модуль шагового двигателя
// ! в качестве значения PRM допускаются целые 

const int faultPin = 7; // пин ошибки
const int enablePin = 8; // пин включения драйвера
const int currentPin = A6; //  датчик тока
////////
const int startPRM = 300;
const int maxPRM = 600;
volatile unsigned int timeStepMin = 1200000/maxPRM;
volatile unsigned int timeStep =    1200000/startPRM;



void runStepMotor(bool flagRun){
  static int attempts = 3;
  static unsigned long timeBoost = 0;
  static unsigned long timerLevel = 0;
  static bool timerFlag = false;
  
  if(flagRun){
    if(!timerFlag){
      digitalWrite(enablePin, LOW);
      OCR1A = timeStep;
      timerLevel = millis();
      timerFlag = true;
    }else if(millis() - timerLevel > 1000 ){ // переключение на повышенную передачу
      TIMSK1 |= (1<<OCIE1A);
    }

    
    if(timeStepMin >= timeStep){ // дэтектирование остановки двигателя 
      readCurrentStepMotorTimer(); // опрос датчика тока внутреннего двигателя и модификация глобального состояния
      if(attempts == 0){
        globalState[7] = 5; // уводим в ошибку 
      }
      
      if(globalState[5] == 0){
        timeStep = 1200000/startPRM;
        timerFlag = false; // следующий раз стартует с пониженной передачи
        globalState[5] = 1; // кастыль сбрасываем значение защиты по току
        attempts--;
      }else{
        attempts = 3;
      }
    }
    
  }else{ //остановка двигателя
    //startPRM = 200;
    digitalWrite(enablePin, HIGH); // выключаем драйвер когда PRM == 0
    OCR1A = 0;
    timeStep = 1200000/startPRM;
    timerFlag = false; // следующий раз стартует с пониженной передачи
  }
}



void readCurrentStepMotorTimer(){
  static unsigned long t = 0;
  static bool flagTimer = false;
  static int acc = 0;

  if(!flagTimer ){
     t = millis();
     flagTimer = true;
  }else if(millis() - t > 50){
    flagTimer = false;
    if(512 > analogReadFast(currentPin)){
      acc += 1;
      if(acc > 10){
        globalState[5] = 0;
        acc = 0;
      }
    }else{
      acc = 0;
      globalState[5] = 1;
    }
  }
}



void initStepMotor(){
  pinMode(9, OUTPUT);
  pinMode(faultPin, INPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(currentPin, INPUT);
  digitalWrite(enablePin, HIGH);

  TCCR1A = 0;   // установить регистры в 0
  TCCR1B = 0;
  TCCR1A |= (1 << WGM10);
  TCCR1B |= (1 << WGM13); 
  TCCR1B |= (1 << CS10); // Установить биты на коэффициент деления 1
  TCCR1A |= (1 << COM1A0);
}

void stepMotorWatcher(){
  const int botGistTime = 4000; // гистерезис на отпускание
  const int upGistTime = 2000; // гистерезис на подъем 
  const long errorTime = 120000; 
  static bool fuelTankFlag = true ;//признак начала работы для насоса
  static unsigned long timerTankUp = 0;
  static unsigned long timerTankBot = 0;
  static bool timerFlagUp = false;
  static bool timerFlagBot = false;

  if((globalState[0] == 1 || globalState[0] == 2) && globalState[7] == 0){ // статус начало росжига либо горение и нет ошибки
    if(globalState[2] == 1){ // депульсатор заполнен 
      if(!timerFlagUp){
        timerTankUp = millis();
        timerFlagUp = true;
      }else if(millis() - timerTankUp > upGistTime){
        fuelTankFlag = false;
        timerFlagBot = false;
      }
    }else if(globalState[2] == 0){
      if(!timerFlagBot){
        timerTankBot = millis();
        timerFlagBot = true;
      }else if(millis() - timerTankBot > errorTime){
        globalState[7] = 2; //не смог наполнить  отлов ошибки
      }else if(millis() - timerTankBot > botGistTime){
        fuelTankFlag = true;
        timerFlagUp = false;
      }
    }    
    runStepMotor(fuelTankFlag); // пуск двигателя   
  }else{
    runStepMotor(LOW);
    timerTankBot = millis();
  }
}

ISR (TIMER1_COMPA_vect){  // функция прерывания, разгоняет мотор и выключается
  
  if(timeStep > timeStepMin){
    timeStep--;
    OCR1A = timeStep;
  }else{
    TIMSK1=0;
  }
}
