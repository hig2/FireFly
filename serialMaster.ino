//Модуль реализации протокола связи



void serialMaster(){
  static unsigned long timerConnect = 0;
  static bool flagConnect = false;
  
  static int lengthGlobalState = sizeof(globalState) / sizeof(globalState[0]);
  String acc = "$";
  unsigned long crc = 0;
  static int lengthBufferComand = 3;
  int bufferComand[lengthBufferComand]; // буфер команд управления всегда+1 для CRC
  
  for(byte i = 0; i < lengthGlobalState; i++){ // расчет CRC
    crc += globalState[i];
  }
  
  for(byte i = 0; i < lengthGlobalState; i++){ // формируем посылку 
    if(lengthGlobalState - 1 == i){
      acc+= crc;
      acc+= ";";
    }else{
      acc+= globalState[i];
      acc+= " ";
    }  
  }
  uart.println(acc); // отпровляем посылку

  //проверка на разрыв подключения

  if(flagConnect && (millis() - timerConnect > 60000)){ // если было осуществленно подключение и таймер обмена превысил 1 минут
    globalState[7] = 6;
  }
  
   if(uart.parsePacket((int*)bufferComand)){
    crc = 0; //очищаем преведущую CRC
    timerConnect = millis(); 
    for(byte i = 0; i < lengthBufferComand - 1; i++){ // расчет CRC
      crc += bufferComand[i];
    }
    if(bufferComand[lengthBufferComand - 1] == crc){ //проверка контрольной суммы пройдена
      flagConnect = true; // после проверки пакета на валидность ставим флаг удачного обмена
      
      dirFun(bufferComand[0]);
      //globalState[8] = bufferComand[1];// вытаскиваем команды из буфера  и преносим их в глобальное состояние температуру топлива

      if(globalState[8] > 100){// ограничение на макс температуру 
        globalState[8] = 100;
      }
    }else{
      //обработка ошибки
    } 
  }
}

void dirFun(int dir){
  switch(dir){
    case 0: globalState[0] = 0;
    break;
    case 1: 
    if(globalState[0] == 2){
      break;
    }else{
      globalState[0] = 1;
    }
      break;
    case 27: globalState[7] = 7;
    break;
  }
}



void monitorStage(){
 static bool testModeFlag = false;
 static long numberList = 0;
 if(globalState[0] == 11){
  testModeFlag = true;
  globalState[0] = 1;
 }
 if(testModeFlag){ 
   numberList++;
   uart.println("------------------------------------");
   uart.print("  Номер справки: ");
   uart.println(numberList);
   uart.println("----------------");
   
   uart.print("Статус: ");
   uart.println(globalState[0]);
  
   uart.print("Температура топлива: ");
   uart.println(globalState[1]);
  
   uart.print("Датчик уровня топлива 1: ");
   uart.println(globalState[2]);
  
   uart.print("Датчик уровня топлива 2: ");
   uart.println(globalState[3]);
  
   uart.print("Датчик пламени: ");
   uart.println(globalState[4]);
  
   uart.print("Ток мотора: ");
   uart.println(globalState[5]);
  
   uart.print("Пневма реле: ");
   uart.println(globalState[6]);
  
   uart.print("Статус ошибки: ");
   uart.println(globalState[7]);
  
   uart.print("Поддерживаемая температура топлива: ");
   uart.println(globalState[8]);
  
   uart.print("Версия прошивки: ");
   uart.println(globalState[9]);
  
   uart.print("ID: ");
   uart.println(globalState[10]);
  
   uart.print("Последняя контрольная сумма: ");
   uart.println(globalState[11]);
   
   
   uart.println("------------------------------------");
  } 
}
