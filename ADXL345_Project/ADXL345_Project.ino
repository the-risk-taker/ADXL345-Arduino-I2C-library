/*
* ADXL345_Project.ino
*
* Created: 9/1/2018 12:31:42 PM
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
*/

#include <Wire.h>

#include "Headers/Status.h"
#include "Headers/I2C_Sensor.h"
#include "Headers/ADXL345.h"

static long previousTime		= 0;
long actualTime					= 0;

#define PRINT_DATA				0x01 // 0x01 = Enable; Other = Disable
#define PRINT_EVENTS			0x00 // 0x01 = Enable; Other = Disable

void setup()
{
	Wire.begin();
	
	Status.begin();
	Status.checkStatus(Accelerometer);
	
	// Set your Data Rate and Range or use defaults from ADXL345::init()
	//		Accelerometer.setDataRate(->your_setting<-);
	//		Accelerometer.setRange(->your_setting<-);
}

void loop()
{
	actualTime = millis();
	
	if((actualTime - previousTime) >= 10)
	{
		Accelerometer.read();
		
		#if( PRINT_DATA == 0x01)
		Status.printMessage("AccX = " + String(Accelerometer.getNormalized().X) +
							"\tAccY = " + String(Accelerometer.getNormalized().Y) +
							"\tAccZ = " + String(Accelerometer.getNormalized().Z) +
							"\tm/s^2" + 
							"\tpitch = " + String(Accelerometer.getPitchRoll().pitch) +
							"\troll = " + String(Accelerometer.getPitchRoll().roll)
							);
		#endif
		
		#if( PRINT_EVENTS == 0x01)
		if(Accelerometer.getEventByName(FREE_FALL))
		{
			Status.printMessage("Free Fall");
		}
		if(Accelerometer.getEventByName(SINGLE_TAP))
		{
			Status.printMessage("Single Tap");
		}
		if(Accelerometer.getEventByName(DOUBLE_TAP))
		{
			Status.printMessage("Double Tap");
		}
		#endif
		
		previousTime = actualTime;
	}
}
