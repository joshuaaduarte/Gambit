/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-limit-switch
 */

#include <ezButton.h>

ezButton limitSwitch(7);  // create ezButton object that attach to pin 7;
ezButton limitSwitch2(8);

void setup() {
  Serial.begin(9600);
  limitSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitch2.setDebounceTime(50);
}

void loop() {
  while (Serial.available() ==0 );           //Wait for user input
  //String toRun = Serial.readString();
  String toRun = Serial.readString();
  int flag = 0;
  int flag2 = 0;

  if (toRun == "run\n"){
      while (flag == 0){
          Serial.println("Not Pressed");
          limitSwitch.loop();
          if(limitSwitch.isPressed()){
              Serial.println("Limit 1 Pressed");
                delay(1000);
            flag = 1;
          }
      }
        while (flag2 == 0){
            Serial.println("2 Not Pressed");
            limitSwitch2.loop();
            if(limitSwitch2.isPressed()){
                Serial.println("Limit 2 Pressed");
                    delay(1000);
                    flag2 = 1;

            }

      }
  }


}