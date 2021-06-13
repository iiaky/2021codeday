#include <U8g2lib.h>
#include <arduino-timer.h>

auto timer = timer_create_default();

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, SCL, SDA, U8X8_PIN_NONE);

unsigned long prevMillis;
unsigned long currentMillis;
boolean timerMode;
int timerTime;

void setup() {
  
  //for timer
  u8g2.begin();
  pinMode(6, INPUT); //buttonPin
  pinMode(4, OUTPUT); //ledPin
  pinMode(A0, INPUT); //rotaryPin
  pinMode(5, OUTPUT); //buzzerPin
  timerMode = false;
  auto everySecond = timer.every(1000, updateScreen); //calls updateScreen() every 1000 milliseconds

}


void loop() {
  
  startAnimation();
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(18,15, "Start timer?");
  u8g2.drawStr(20,40, "No");
  u8g2.drawStr(80,40, "Yes");
  u8g2.sendBuffer();
  bool choice = waitForButton();
  
  if (choice){
    trackMillis(1000);
    while(!timerMode){
      u8g2.clearBuffer();
      int input = analogRead(A0);
      timerTime = map(input, 0, 1023, 0, 60);
      u8g2.setFont(u8g2_font_ncenB10_tr);
      u8g2.drawStr(5,15, "Setting Timer...");
      u8g2.setCursor(55, 45);
      u8g2.print(timerTime);
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.drawStr(40,56, "seconds");
      u8g2.sendBuffer();
      int confirmTime = digitalRead(6);
      if (confirmTime){
        timerMode = true;
        break;
      } else{
        continue;
      }
    }
    while(timerMode){
      timer.tick();
    }
  } else{
    showInfo();
  }
  
}
 
bool updateScreen(void *){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB18_tr);
  u8g2.setCursor(50, 35);
  u8g2.print(timerTime);
  u8g2.sendBuffer();
  timerTime -= 1;
  if (timerTime == -1){
    timerMode = false;
    timerEnd();
    return false;
  }
  return true;
}

void timerEnd(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  int buttonPressed = digitalRead(6); 
  
  while (!buttonPressed){
    buttonPressed = digitalRead(6); 
    
    //turns evertything on
    u8g2.setCursor(25, 25);
    u8g2.print("TIMER");
    u8g2.setCursor(25, 45);
    u8g2.print("ENDED");
    u8g2.sendBuffer();
    analogWrite(5, 50);
    digitalWrite(4, HIGH);
    
    logMillis();
    trackMillis(1500);
    
    analogWrite(5, 0);
    digitalWrite(4, LOW);
    u8g2.clear();
    
    logMillis();
    trackMillis(1500);
    
    if(buttonPressed){
      analogWrite(5, 0);
      digitalWrite(4, LOW);
      u8g2.clearBuffer();
      u8g2.setCursor(25, 25);
      u8g2.print("TIMER");
      u8g2.setCursor(20, 45);
      u8g2.print("STOPPED");
      u8g2.sendBuffer();
      logMillis();
      break;
    }
  }
  trackMillis(1000);  //lets button reset to 0
  loop(); //triggers main arduino loop
}

bool waitForButton(){
  while(true){
    int buttonState = digitalRead(6);
    if (buttonState == HIGH){
      int rotaryChoice = analogRead(A0);
      if (rotaryChoice >= 512){
        return false; //choice "don't set timer"
      }else{
        logMillis();
        return true; //choice "set timer"
      }
    }
  }
}

void showInfo(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.setCursor(25, 25);
  u8g2.print("Wrong");
  u8g2.setCursor(15, 40);
  u8g2.print("Answer");
  u8g2.sendBuffer();
  logMillis();
  trackMillis(1000);
}

void logMillis(){
  currentMillis = millis();
  prevMillis = currentMillis;
}

void trackMillis(int x){ //delays for x seconds
  while(true){
    currentMillis = millis();
    if (currentMillis - prevMillis >= x){
      break;
    }
  }
  return;
}

void startAnimation(){
  u8g2.clearBuffer();
  u8g2.drawCircle(40, 48, 10, U8G2_DRAW_ALL);
  u8g2.sendBuffer();
  logMillis();
  trackMillis(165);

  u8g2.clearBuffer();
  u8g2.drawEllipse(40, 52, 15, 8, U8G2_DRAW_ALL);
  u8g2.sendBuffer();
  logMillis();
  trackMillis(70);

  u8g2.clearBuffer();
  u8g2.drawCircle(40, 40, 10, U8G2_DRAW_ALL);
  u8g2.sendBuffer();
  logMillis();
  trackMillis(130);
  
  u8g2.clearBuffer();
  u8g2.drawCircle(40, 25, 10, U8G2_DRAW_ALL);
  u8g2.sendBuffer();
  logMillis();
  trackMillis(200);

  u8g2.clearBuffer();
  u8g2.drawCircle(40, 35, 10, U8G2_DRAW_ALL);
  u8g2.sendBuffer();
  logMillis();
  trackMillis(130);

  u8g2.clearBuffer();
  u8g2.drawCircle(40, 45, 10, U8G2_DRAW_ALL);
  u8g2.sendBuffer();
  logMillis();
  trackMillis(130);

  u8g2.clearBuffer();
  u8g2.drawEllipse(40, 52, 15, 8, U8G2_DRAW_ALL);
  u8g2.sendBuffer();
  logMillis();
  trackMillis(100);

  u8g2.clearBuffer();
  u8g2.drawCircle(40, 48, 10, U8G2_DRAW_ALL);
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(55,30,"blob");
  u8g2.sendBuffer();
  logMillis();
  trackMillis(850);
}
