/*
 *  Copyright (C) 2015 Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
   
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
  
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Version:		2.1
 *  Design:			David Gascón
 *  Implementation:	Yuri Carmona
 */
  

#include <Waspmote.h>

//define global variable for Waspmote serial id
volatile unsigned long _serial_id;

int main(void)
{
	init();

	// proceed depending on the bootloader version
	if( Utils.getBootVersion() >= 'E')
	{
		pinMode(RTC_SLEEP, OUTPUT);
		digitalWrite(RTC_SLEEP, HIGH);
	}
	
	// Check OTA EEPROM flag and mark flag in Waspmote 
	// Control Register if corresponds to
	if( Utils.readEEPROM(0x01) == 0x01 )
	{
		// set register flag
		WaspRegister |= REG_OTA;
		
		// clear eeprom flag
		eeprom_write_byte((unsigned char *) 0x01, 0x00);
	}
	
	delay(3);	
	if( WaspRegister & REG_SX )
	{	
		delay(3);	
		// Powering the module
		pinMode(XBEE_PW,OUTPUT);
		delay(3);
		digitalWrite(XBEE_PW,HIGH);
		delay(3);
		//Configure the MISO, MOSI, CS, SPCR.
		SPI.begin();
		delay(3);
		//Set Most significant bit first
		SPI.setBitOrder(MSBFIRST);
		delay(3);
		//Divide the clock frequency
		SPI.setClockDivider(SPI_CLOCK_DIV2);
		delay(3);
		//Set data mode
		SPI.setDataMode(SPI_MODE0);		
		delay(3);
		//finish
		SPI.end();		
		delay(3);
		// disable all SPI slaves 
		SPI.setSPISlave(ALL_DESELECTED);	
		delay(3);
		// Powering the module
		pinMode(XBEE_PW,OUTPUT);
		digitalWrite(XBEE_PW,LOW);	
		delay(3);
	}	
	
	delay(3);	
	// get serial id
	_serial_id = Utils.readSerialID();
	
	// set random seed
	srand(_serial_id);
	
	setup();
    
	for (;;) {
		loop();

	}
        
	return 0;
}
