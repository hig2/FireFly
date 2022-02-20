

void readLockByte(){
  byte firstLockByte = eeprom_read_byte(1);
  
  if(firstLockByte == 255){ // если память свежая
    eeprom_write_byte(1,0);
    delay(5);
    firstLockByte = eeprom_read_byte(1); // пишем актуальное значение 
  }

  if(firstLockByte > 144){ // если больше 144 часов то уводим в ошибку 4
      globalState[7] = 4;
  }
  
}


void motoTimeWatcher(){
  static unsigned long t = 0;

  if((millis() - t) > 3600000){
    byte firstLockByte = eeprom_read_byte(1); //читаем текущее значение в памяти
    if(firstLockByte > 144){
      globalState[7] = 4;
    }
    eeprom_write_byte(1, firstLockByte + 1);
    t = millis();
  }
}




void eepromWatcher(){
  if(globalState[0] == 342){
    eeprom_write_byte(1,1);
    uart.println("Разблокированно !");
    globalState[0] = 69;
  }
  if(globalState[0] == 341){
    eeprom_write_byte(1,0);
    uart.println("Заблокированно!");
    globalState[0] = 69;
  }
}


































//Модуль для работы энерго независимой памятью

//1) переход со второй фазы на первую при потере пламени и понижении температуры 

 
 



//+1. замер тока в течении 4 секунды и получение среднего значения(10 раз каждые 0.2 сек в течении 4сек, общее количество опросов 200 раз)
//+2. запрет на работу насоса во время фазы зажигания
//+3. Реализация разгона мотора с плавным разгоном с 100 до 300 рпм 4 сек потом до плавный разгон до 900 рпм работа относительно среднего значения тока для этой скорости
//+4. Реализация продседурного способа зажигания (в начале поджиг потом плювок на горящую дугу) включая реализацию востоновления горения
