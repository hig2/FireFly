//Модуль опроса датчиков уровня

const int fuelLevel_1_Pin = 6; //контроль уровня топлива депульсатора
const int fuelLevel_2_Pin = 5; //контроль уровня топлива топливного бака

void initCtrlLevel(){
  pinMode(fuelLevel_1_Pin, INPUT);
  pinMode(fuelLevel_2_Pin, INPUT);  
}

void readLevelFuel(){
  if(digitalRead(fuelLevel_1_Pin)){
    globalState[2] = 0;   
  }else{
    globalState[2] = 1;  
  }

  if(digitalRead(fuelLevel_2_Pin)){
    globalState[3] = 0;
  }else{
    globalState[3] = 1;
  }
}
