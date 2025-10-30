

const int DHTpin = 7;

byte data[5];

void setup(){
  Serial.begin(9600);
  pinMode(DHTpin, INPUT);
}

void loop(){

  uint8_t i, j;

  ReadDHT();
  for(i = 0; i < 5; i++){
    Serial.println(data[i]);
  }
  delay(10000);

}


bool ReadDHT(){

  uint32_t endTime,startTime;
  uint8_t i, j;
  unsigned long signalTime;

  // Start Process
  pinMode(DHTpin, OUTPUT);
  Serial.println("Start Signal");
  digitalWrite(DHTpin, LOW);
  delay(20);
  digitalWrite(DHTpin, HIGH);
  delayMicroseconds(40);
  //Change pin to input
  pinMode(DHTpin, INPUT);

  // Confirmation to Start
  if (digitalRead(DHTpin) == 0){
    Serial.println("Signal Recieved");
    startTime = micros();
    while (digitalRead(DHTpin) == 0){
      if (micros - startTime > 100) return false;
    }

    //Start data sending process
    if (digitalRead(DHTpin) == HIGH){
      endTime = micros();
      signalTime = endTime - startTime;
      Serial.print("Signal Time was: ");
      Serial.print(signalTime);
      Serial.println(" microseconds");
      startTime = micros();
      Serial.println("Preparing Data");
      Serial.println();
      startTime = micros();
      while (digitalRead(DHTpin) == 1){
        if (micros - startTime > 100) return false;
      }

    }
  }
  else {
    
    Serial.println("Signal Failed"); 
    return false;
  }

  //Step before Data starts to send
  if (digitalRead(DHTpin) == 0){
    endTime = micros();
    signalTime = endTime - startTime;
    Serial.print("Prep Signal was ");   // print text
    Serial.print(signalTime);           // print variable
    Serial.println(" ms");
    Serial.println("Data now Sending");
    Serial.println();
    while (digitalRead(DHTpin) == 0);
  }

  //Reading 40 bits, 5 bytes, 5 numbers
  //
  for (i = 0; i < 5; i++){
    data[i] = 0;

    for (j = 0; j < 8; j++){
      //Starts with Low for 50 microseconds
      startTime = micros();
      while (digitalRead(DHTpin) == 0){
        if ((micros() - startTime) > 100) return false;
      }

      startTime = micros();
      while (digitalRead(DHTpin) == 1) {
        if (micros() - startTime > 100) return false;
      }
      signalTime = micros() - startTime;
       if (signalTime > 50){
            data[i] |= (1 << (7 - j));
        }


    }


  }

  return true;

}
