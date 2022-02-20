
const int ledGreenPin = A1;
const int ledRedPin = A2;


void ledInit(){
  pinMode(ledGreenPin, OUTPUT);
  digitalWrite(ledGreenPin, LOW);
  pinMode(ledRedPin, OUTPUT);
  digitalWrite(ledRedPin, LOW);
}

void ledWatcher(){
  if(globalState[7] == 0){
    switch (globalState[0]){
    case 0:
      digitalWrite(ledGreenPin, LOW);
      digitalWrite(ledRedPin, HIGH);
    break;
    case 1:
      greenBlink(800);
      digitalWrite(ledRedPin, LOW);
    break;
    case 2:
      digitalWrite(ledGreenPin, HIGH);
      digitalWrite(ledRedPin, LOW);
    break;
    }
  }else{
    digitalWrite(ledGreenPin, LOW);
    errorBlink(1000, 3000);
  }
}



void greenBlink(int timeLedBlink){
  static unsigned long t = 0;
  static bool flagLed = LOW;
  
  if(millis() - t > timeLedBlink){
    t = millis();
    flagLed = !flagLed;
 }
 
 digitalWrite(ledGreenPin, flagLed);
}


void errorBlink(int timeBlink , int pause){
  static int iter = globalState[7];
  static unsigned long t;
  static bool flagT = false;
  static bool flagLed = LOW;
 if(iter == 0){ 
  if(flagT){
    t = millis();
    flagT = false;
  }else if(millis() - t > pause){
    iter = globalState[7];
  }  
 }else{
   if(!flagT){
    t = millis();
    flagT = true;
  }else if(millis() - t > timeBlink){
    flagT = false;
    flagLed = !flagLed;
    if(!flagLed){
      iter--;
    }
  }
 }
  digitalWrite(ledRedPin, flagLed);  
}
