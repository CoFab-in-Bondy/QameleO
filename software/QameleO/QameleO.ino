#include "QameleO.h"

QameleO_measure data;

//////////////SETUP///////////////////////////////////////

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("start");
  delay(10000);

  
  setup_QameleO(data);
}


//////////////////////LOOP//////////////////////
void loop() {
  loop_QameleO(data);
}
