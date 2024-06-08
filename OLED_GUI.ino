/*   ****   A simple Graphical User Interface   ****
  featuring : 
              SPI OLED 320x240
              4 Touch screen buttons
              activating different programs
              
    NOTE: the OLED screen works on 3V logic!! it is intollerant to 5V
    touch the screen to choose between Mode0 to mode3.
    touch the arrow in order to proceed to the chosen mode

    
    by: Gal Arbel, 2024
*/
#include <Wire.h>
#include <ButtonIRQ.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <URTouch.h>
#include <Fonts/FreeSans9pt7b.h>


#define TFT_DC 8
#define TFT_CS 9
#define TFT_MOSI 11
#define TFT_CLK 13
#define TFT_RST 7
#define TFT_MISO 12
#define TFT_LED 6

ButtonIRQ rightbutton(2);//must be either 2 or 3 (UNO R3)
Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9, 8);
URTouch  myTouch(7, 6, 5, 4, 3);//tclk, tcs, tdin, dout, irq

bool togsw = false;
bool result;
char t[32];



int btn2pin = 2; //reserved for future use

int x, y;
int mode;

void setup(){
  Serial.begin(115200);
  rightbutton.begin(115200);
  Wire.begin();
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH); 
  tft.begin();
  tft.setRotation(3);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);

  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  drawButtons();
  while(!arrow()){
    checkTouch();
  }
  tft.fillScreen(ILI9341_BLACK);
}

void loop(){
  switch (mode){
    case 0:
      Serial.println("program 0 running");
      break;
    case 1:
      Serial.println("program 1 running");
      break;
    case 2:
      Serial.println("program 2 running");
      break;
    case 3:
      Serial.println("program 3 running");
      break;
  }
  //Serial.print("mode "); Serial.print(mode); Serial.println(" selected");
        //tft.fillScreen(ILI9341_BLACK);

}



void drawButtons(){
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&FreeSans9pt7b);
  tft.setCursor(30, 50);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);
  tft.print("Choose Mode");
  for (x = 0; x < 4; x++)
  {
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_GREEN);
    tft.fillRect(30+(x*62), 100, 60, 60, ILI9341_WHITE - (x*1000));//x,y,w,h,col
    tft.setCursor(37+(x*62), 120);
    tft.print("mode ");
    tft.setCursor(50+(x*62), 140);
    tft.print(x);
  }
  //Draw Arrow
  tft.fillRect(90, 190, 110, 15, ILI9341_MAGENTA);
  tft.fillTriangle(200, 180, 230, 197, 200, 215, ILI9341_MAGENTA);
  tft.setCursor(120, 202);
  tft.setTextColor(ILI9341_YELLOW);
  tft.print("proceed");
} 

void checkTouch(){
  if(myTouch.dataAvailable()){
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    //Serial.print("x = ");
    //Serial.println();
    //Serial.print("y = ");
    //Serial.println(y); 
    if(y > 100 && y < 160){
      if(x > 30 && x < 90){//mode 0 selected
        Serial.println("mode 0 Selected");
        drawButtons();
        buttonTouched(30, 100);
        mode = 0;
      } else if (x > 92 && x < 152){//mode 1 selected
          Serial.println("mode 1 Selected");
          drawButtons();
          buttonTouched(92, 100);
          mode = 1;
      } else if (x > 154 && x < 214){//mode 2 selected
          Serial.println("mode 2 Selected");
          drawButtons();
          buttonTouched(154, 100);
          mode = 2;
      }  else if (x > 216 && x < 276){//mode 3 selected
          Serial.println("mode 3 Selected");
          drawButtons();
          buttonTouched(216, 100);
          mode = 4;
      } 
    }          
  } 
  else {
    delay(100);
  }
}
void buttonTouched(int x, int y){
  int width = 60;
  tft.drawRect(x, y, width, width, ILI9341_BLUE);
  tft.drawRect(x+1, y+1, width-2, width-2, ILI9341_BLUE);
  tft.drawRect(x+2, y+2, width-4, width-4, ILI9341_BLACK);

}
bool arrow(){
  if(myTouch.dataAvailable()){
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    if(x > 90 && x < 230){
      if(y > 190 && x < 210){
        return true;
      }
    }
    else return false;
  }
}