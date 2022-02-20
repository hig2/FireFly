#include "avdweb_AnalogReadFast.h" 
#include <GyverPWM.h>
#include "GyverWDT.h"
#include <avr/eeprom.h>

int globalState[12]={
  1, //[0] Общее состояние : 0 - ожидание, 1 - росжиг, 2 - горение 
  0, //[1] Текущая температура топлива
  0, //[2] Статус уровня топлива депульсатора (0,1)
  0, //[3] Статус уровня топлива буферного бака (0,1)
  0, //[4] Статус горения (0,1)
  1, //[5] Показания потребляймого тока внутреннего двигателя 
  0, //[6] Пневма реле открыто
  0, //[7] Номер ошибки(1 - не удалось зажечь,2 - не удалось заполнить бак топливом, 3 - перегрев топлива, 4 - блокировка, 5 - клин двигателя, 6 - разрыв обменна данными с пультом,7 -  Обрыв датчика температуры котла. )
  85, //[8] Команда на выполднение (Удерживаймая температура)
  102, //[9] Версия прошивки
  1, //[10] ID
  0  //[11] CRC(контрольная сумма пакета)
};


void setup(){
  uart.begin(19200);
  ledInit();
  initCtrlLevel();
  initExternalPump();
  initStepMotor();
  initIgnition();
  initAir();
  initTen();
  //readLockByte();
  Watchdog.enable(RESET_MODE, WDT_PRESCALER_1024);
}

void loop(){
  readTimer_1(500);
  readTimer_2(100);
  readTimer_3(2000);
  planTimerReboot();
  
  ledWatcher();
  externalPumpWatcher(); //ватчер работы с внешним насосом
  stepMotorWatcher(); // ватчер для работы насоса
  tenWatcher(); // нагрев топлива 
  termoWatcher(); // переход фазы на основании удерживаемой температуры
  fireStatusWatcher(); // ватчер показания датчика , статуса горения(2), + моментальный росжиг (5 попыток )
  airWatcher();
  //motoTimeWatcher();
  Watchdog.reset();
}

void readTimer_1(int timer){
  static unsigned long t = 0;

  if(millis() - t > timer){
    t = millis();
    readLevelFuel(); // чтение уровней топливных баков
    readTermoFuel(); // опрос термодатчика топлива и модификация глобального состояния
  }
}



void readTimer_2(int timer){
  static unsigned long t = 0;
  
  if(millis() - t > timer){
    t = millis();
    readFireSensor();
   }
}

void readTimer_3(int timer){
  static unsigned long t = 0;

  if(millis() - t > timer){
    t = millis();
    serialMaster();
    monitorStage();
  }
}


void planTimerReboot(){
  while(millis() > 86400000 || globalState[0] == 69){
    // бесконечный луп для перезагрузки по вач догу 
  }
}
