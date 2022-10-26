/* CONEXÕES:
     MPPT        Arduino pin
     GND         GND
     RX          -
     TX          RX2
     Power       -
 */


#include "config_mppt.h"
void setup()  
{
setup_mppt();
} 

void loop() 
{
  if (Serial1.available())
   {
        loop_mppt();
   }
}
