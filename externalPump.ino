// модуль работы с внешним насосом

const int externalPumpPin = A3;

void initExternalPump(){
  pinMode(externalPumpPin, OUTPUT);
  digitalWrite(externalPumpPin, LOW);
}

void externalPumpWatcher(){
  const int gistTime = 10000; // гистерезис 
  static bool fuelTankFlag = true; //признак начала работы для насоса
  static unsigned long timerTank = 0;
  
    if(globalState[7] == 0){ //  нет ошибки
      if(globalState[3] == 1){ // депульсатор заполнен 
        timerTank = millis();
        fuelTankFlag = false;
      }else if(millis() - timerTank > gistTime){ // добавляем паузу гистерезиса 
        fuelTankFlag = true; 
      }
  
      
      digitalWrite(externalPumpPin, fuelTankFlag);
    }else{
      digitalWrite(externalPumpPin, LOW); // принудительная остановка при аварии
    }
}
