// модуль работы с пневмореле

const int airRunPin = 3;

void initAir(){
  pinMode(airRunPin, OUTPUT);
  PWM_20KHZ_D3(0);
}

void airWatcher(){
  if(globalState[0] != 2 || globalState[7] != 0){
    PWM_20KHZ_D3(0);
    globalState[6] = 0;
  }
}
