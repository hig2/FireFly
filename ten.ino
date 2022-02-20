


const int tenPin = 2;

void initTen(){
  pinMode(tenPin, OUTPUT);
  digitalWrite(tenPin, LOW);
}

void tenRun(){
  digitalWrite(tenPin, HIGH);
}

void tenStop(){
  digitalWrite(tenPin, LOW);
}


void tenWatcher(){
  static bool flagTen = false;
  static unsigned long t = 0;
  const long defTimer = 30000;
  const int defTemp = globalState[8] + 20;

  if(globalState[2] == 1){ // депульсатор заполнен 
    t = millis();
    flagTen = true;
  }else if(millis() - t > defTimer){ // добавляем паузу гистерезиса 
   flagTen = false;
  }
  
  if((globalState[0] == 1 || globalState[0] == 2) && globalState[7] == 0 && flagTen){  
    if(globalState[1] < globalState[8]){
      tenRun();
    }else if(globalState[1] > globalState[8]){
      tenStop();
    }
    
    if(globalState[1] > defTemp){
      globalState[7] = 3; // перегрев топлива
    }
  }else{
    tenStop();
  }
}
