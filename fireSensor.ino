//Модуль опроса датчика пламени

const int fireSensorPin = A7; //контроль уровня топлива депульсатора



void readFireSensor(){
  if(analogReadFast(A7) < 900){
    globalState[4] = 1;
  }else{
    globalState[4] = 0;
  }
}


void fireStatusWatcher(){ 
  static int attempts  = 6;
  static bool flagTimer = false;
  
  if(globalState[0] == 1){ // если был переход на росжиг закрываем окно что бы стартовать
    flagTimer = false;
  } 

  if(globalState[0] == 2 && globalState[4] == 1 && globalState[7] == 0){
    attempts = 5; // востанавливаем попытки зажигания при удачном росжиге
    ignitionStop(); //вырубаем зажигание принудительно (на всякий)
  }else if(globalState[0] == 2 && globalState[4] == 0 && globalState[7] == 0){ //горение, нет пламени, нет ошибки
    if(globalState[2] == 1){ // ждем полного бака
       flagTimer = true;
    }
    if(flagTimer){
       attempts = attempts - ignitionRunAlt(); 
       if(attempts == 0){
        globalState[7] = 1; // уводим в ошибку
      }
    }
  }else{ 
    ignitionStop(); // принудительная остановка зажигания
  }
}
