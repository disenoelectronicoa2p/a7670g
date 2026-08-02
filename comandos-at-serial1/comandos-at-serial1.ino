  void setup(){
    Serial.begin(115200);
    Serial1.begin(115200);
    Serial.println("Start!...");
    Serial1.println("AT");  
    
  }

  void loop(){  
   if (Serial1.available())
     Serial.write(Serial1.read());    
    if(Serial.available()) 
    Serial1.write(Serial.read());
  }