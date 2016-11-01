#include "mbed.h"
#include "DRV8835.h"
#include "Adafruit_ST7735.h"
#include "QEI.h"
#include "LSM303D.h"
#include "L3GD20H.h"
#include "MSCFileSystem.h"
#include "MODSERIAL.h"
#include "GPS.h"
#include "SweptIR.h"
#include "stdlib.h"
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
DRV8835 motors(p23,p18,p24,p11);
LSM303D comp(p5,p6,p7,p15);
L3GD20H gyro(p5,p6,p7,p22);
Adafruit_ST7735 tft(p5,p6,p7,p12,p8);
QEI Left(p30,p29,NC,909.72,QEI::X4_ENCODING);  // encoder object for Left wheel
QEI Right(p17,p16,NC,909.72,QEI::X4_ENCODING);  // encoder object for Right wheel
Serial pc(USBTX, USBRX); // tx, rx read from bluetooth module
GPS gps(p13,p14);
AnalogIn batt(p19);
SweptIR sweep(p25,p20);
float turnangle; // Read turn angle from MATLAB
char speed[2]; // Read speed component from MATLAB

float V ; // Constant velocity component for control loop
float K ; // Gain component for control loop

int main() {
    
    
    while(1)
    {
        tft.fillScreen(0x0000); //Makes screen black
        tft.setCursor(0,0);  
        
        //Read values from MATLAB
        pc.scanf("%f",&turnangle);
        speed[1] = pc.getc(); 
        
        // Print values to TFT  
        tft.printf(" turnangle:%f\n\r speed:%s\n\r",turnangle,speed[1]);
           
          // Control loop to drive 
            if ( speed[1] == 'L')
                { V = 0.2;
                K = 0.00222222222;
                    motors.motorR_fwd(V +(K*turnangle));
                    motors.motorL_fwd(V -(K*turnangle)); 
                
                }
            else if  ( speed[1] == 'H') 
               { V = 0.4;
               K = 0.00444444444;
                    motors.motorR_fwd(V +(K*turnangle));
                    motors.motorL_fwd(V -(K*turnangle)); 
                }
            else if   ( speed[1] == 'S') 
                {  motors.motorR_fwd(0);
                    motors.motorL_fwd(0); 
                }
    }
    
}