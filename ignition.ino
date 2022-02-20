//модуль зажыгания
const int fireStartPin = 4;

void initIgnition(){
  pinMode(fireStartPin, OUTPUT);
  digitalWrite(fireStartPin, LOW);
}

int ignitionRunAlt(){
  PWM_20KHZ_D3(0); //закрываем пневмаклапан
  digitalWrite(externalPumpPin, LOW); //вырубаем насос на время пуска
  runStepMotor(LOW);
  delay(2500); //ждем пока давление стравится
  globalState[6] = 1; // ставим статус открытой пневмы 
  PWM_20KHZ_D3(255); //открываем Клапан
  //delay(100); //задержка перед зажиганием, ждем потока 
  digitalWrite(fireStartPin, HIGH); //зажигаем дугу
  delay(200); // держим уровень 100мс
  digitalWrite(fireStartPin, LOW); //выключем зажигание
  delay(2000); //ждем пламя
  PWM_20KHZ_D3(122); // понижаем напряжение на клапане 
  return 1;
}


void ignitionStop(){
  digitalWrite(fireStartPin, LOW);
}
