//by Małgorzata Kosakowska and Michał Ptak
#include <Adafruit_MLX90640.h>
#include "gest1.h"
Adafruit_MLX90640 mlx;
float frame[32*24]; // buffer for full frame of temperatures

void setup() {
  mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire);
  mlx.setMode(MLX90640_CHESS);
  mlx.setResolution(MLX90640_ADC_18BIT);
  mlx.setRefreshRate(MLX90640_1_HZ);

  Serial1.begin(28800); //baud rate
  Serial1.println(0); 
  delay(100);
}

void por()
{
  uint8_t count_skos_LP=0;
  uint8_t count_skos_PL=0;
  uint8_t count_dwie=0;
  uint8_t count_raczka=0;
  char c = ' ';
  
  for (uint8_t h=4; h<20; h++){ //4- 20
    for (uint8_t w=3; w<29; w++) { //6-26
        float t = frame[h*32 + w];
        if (t < 26) c = '.';
        else c = 'X';

        if ((c=='X') && (c==dwie[h*32 + w]))
        count_dwie++;

        if  ((c=='X') && (c==raczka[h*32 + w]))
        count_raczka++;
        
        if  ((c=='X') && (c==skos_PL[h*32 + w]))
        count_skos_PL++;
        
        if  ((c=='X') && (c==skos_LP[h*32 + w]))
        count_skos_LP++;
        }
  }
  if (count_dwie>=150){
      Serial1.println(1);
      delay(8000);
  }
  else if(count_raczka>=150){
      Serial1.println(2);
      delay(7000);
  }
  else if(count_skos_PL>=110){
      Serial1.println(3);
      delay(7000);
  }
  else if(count_skos_LP>=110){
      Serial1.println(4);
      delay(5000);
  }
  else {
    Serial1.println(0);
    delay(1000); 
  }
}

void loop() {
  delay(500);
  mlx.getFrame(frame);
  por(); 
}
