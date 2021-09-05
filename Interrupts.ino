#include <Keyboard.h>

void key1Interrupt(){
  if(PIND & KEY_1_PORT) Keyboard.press(KEY_1_CHAR);
  else                Keyboard.release(KEY_1_CHAR);
}

void key2Interrupt(){
  if(PIND & KEY_2_PORT) Keyboard.press(KEY_2_CHAR);
  else                Keyboard.release(KEY_2_CHAR);
}

void key3Interrupt(){
  if(PIND & KEY_3_PORT){
    Keyboard.press(KEY_3_CHAR);
  }
  else {
    Keyboard.release(KEY_3_CHAR);
  }
}
