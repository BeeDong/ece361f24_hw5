/**
 * iom361.c - Source file for ECE 361 I/O module emulator
 *
 * @file:		iom361_r2.c
 * @author:		Roy Kravitz (roy.kravitz@pdx.edu)
 * @date:		05-Nov-2033
 * @version:	2.0
 *
 * This is the source code for the ECE 361 I/O module emulation.  The I/O module
 * emulates a memory-mapped I/O system with a number of "typical" peripheral registers.
 *
 * This version uses an array of uint3t_ instead of a struct.  More accurate way to
 * model memory mapped I/O registers
 *
 */
 #include <stdlib.h>
 #include <stdio.h>
 #include <math.h>
 #include <time.h>
 #include <float.h>
 
 #include "float_rndm.h"
 #include "iom361_r2.h"
 
 // constants
 //#define _DEBUG_ 1
 
 // global variables
 static uint32_t IOSpace[sizeof(ioreg_t)/ sizeof(uint32_t)];
 static uint32_t* IOSpacePtr;
 static int nsw;						// number of switches
 static int nleds;						// number of LEDs
 static bool isInitialized = false;		// true if IOM361 has been initialized
 static uint32_t errValue = 0xDEADBEEF;	// value returned on error
 
 // Helper function prototypes
 static void display_leds(uint32_t value, int num_leds);
 static void display_rgb_leds(uint32_t value);
 
 // API functions
 
 /* iom361_initialize() */
 uint32_t* iom361_initialize(int num_switches, int num_leds, int* rtn_code) {
	// initialize global variables
	nsw = num_switches;
	nleds = num_leds;
	IOSpacePtr = (uint32_t*) &IOSpace;
	
	#ifdef _DEBUG_
		printf("INFO [iom361_initialize()]: IOSpacePtr=%p, IOSpace Length=%d\n",
			IOSpacePtr, sizeof(IOSpace));
	#endif
	
	// initialize the I/O registers
	iom361_writeReg(IOSpacePtr, LEDS_REG, 0x00000000, NULL);
	iom361_writeReg(IOSpacePtr, RGB_LED_REG, 0x00000000, NULL);
	iom361_writeReg(IOSpacePtr, RSVD1_REG, 0x11111111, NULL);
	iom361_writeReg(IOSpacePtr, RSVD2_REG, 0x22222222, NULL);
	iom361_writeReg(IOSpacePtr, RSVD3_REG, 0x33333333, NULL);
		
	_iom361_setSwitches(0x00000000);
	_iom361_setSensor1(23.5, 75.0);
	
	// randomize rand()
	srand(time(NULL));
	
	if (rtn_code != NULL)
		*rtn_code = 0;
	isInitialized = true;
	return IOSpacePtr;
 }
 
 /* iom361_readReg(g) */
 uint32_t iom361_readReg(uint32_t* base, uint32_t offset, int* rtn_code) {
	uint32_t value;
	uint32_t* ioreg_ptr;
	
	if (base != IOSpacePtr) {
		// not pointing to base of IO space
		if (rtn_code != NULL)
			*rtn_code = 1;
		return errValue;
	}
	
	if ((offset < 0) || (offset > (sizeof(IOSpace) - sizeof(uint32_t)))) {
		// offset is out of range
		if (rtn_code != NULL)
			*rtn_code = 2;
		return errValue;
	}
	
	// calculate address and get the value
	ioreg_ptr = base + (offset / sizeof(uint32_t));
	value = *ioreg_ptr;
	
	#ifdef _DEBUG_
		printf("INFO[iom361_readReg()]: base = %p, offset = %d, ioreg_ptr=%p, value=%08X\n",
			base, offset, ioreg_ptr, value);
	#endif
	
	if (rtn_code != NULL)
		*rtn_code = 0;
	return value;
 }
 
 /* iom361_writeReg() */
 uint32_t iom361_writeReg(uint32_t* base, int offset, uint32_t value, int* rtn_code) {
	uint32_t* ioreg_ptr;
	
	if (base != IOSpacePtr) {
		// not pointing to base of IO space
		if (rtn_code != NULL)
			*rtn_code = 1;
		return errValue;
	}
	
	if ((offset < 0) || (offset > (sizeof(IOSpace) - sizeof(uint32_t)))) {
		// offset is out of range
		if (rtn_code != NULL)
			*rtn_code = 2;
		return errValue;
	}
	
	if ((offset % sizeof(uint32_t)) != 0) {
		// offset does not point to start of an I/O register
		if (rtn_code != NULL)
			*rtn_code = 3;
		return errValue;
	}
	
	// OK, we're in range and on a I/O register boundary
	// Organize code so we can do something different w/ each register
	// NOTE: This code would have to change if we changed I/O register map
	if (rtn_code != NULL)
		*rtn_code = 0;
	
	ioreg_ptr = base + (offset/sizeof(uint32_t));
	
	#ifdef _DEBUG_
		printf("INFO[iom361_writeReg()]: base = %p, offset = %d, ioreg_ptr=%p, value=%08X\n",
			base, offset, ioreg_ptr, value);
	#endif
	
	switch (offset) {
		case SWITCHES_REG:	break; // switches are a read-only input
					
		case LEDS_REG:		*ioreg_ptr = value;
							display_leds(value, nleds);
							break;
				
		case RGB_LED_REG:	*ioreg_ptr = value;
							display_rgb_leds(value);
							break;
					
		case TEMP_REG:		break;	// temperature is a read-only input

		case HUMID_REG:		break;	// humidity is a read-only input
		
		case RSVD1_REG:		*ioreg_ptr = value;
							break;
					
		case RSVD2_REG:		*ioreg_ptr = value;
							break;
					
		case RSVD3_REG:		*ioreg_ptr = value;
							break;
					
		default:	if (rtn_code != NULL)	// shouldn't get here	
						*rtn_code = 4;
					value = errValue;
					break;
	}
	return value;
 }
 
 
// Functions used for testing - set register values for read-only registers
  
/* _iom361_setSwitches() */
void _iom361_setSwitches(uint32_t value){
	uint32_t* ioreg_ptr = IOSpacePtr + (SWITCHES_REG / sizeof(uint32_t));
	
	// this one is straightforward - just write value to switch register
	*ioreg_ptr = value;
	return;
}
  
  
/* _iom361_setSensor1() */
void _iom361_setSensor1(float new_temp, float new_humid){
	float temp_float, humid_float;
	uint32_t temp_value, humid_value;
	
	uint32_t* ioreg_ptr;
	
	static float temp_const = powf(2, 20) / 200.0;
	static float rh_const = powf(2,20) / 100.0;
	
	// per AHT20 data sheet, Temp(C) = (ST/2**20)* 200 - 50
	// so ST = (2**20/200) * (Temp(C) + 50)
	temp_float = temp_const * (new_temp + 50.0);
	temp_value = (uint32_t) temp_float;
	
	// per AHT20 data sheet, RH(%) = (SRH/2**20)* 100%
	// so SRH = (2**20/100) * RH(%)
	humid_float = rh_const * new_humid;
	humid_value = (uint32_t) humid_float;
	
	// write the I/O registers
	ioreg_ptr = IOSpacePtr + (TEMP_REG / sizeof(uint32_t));
	*ioreg_ptr = temp_value;
	
	ioreg_ptr = IOSpacePtr + (HUMID_REG / sizeof(uint32_t));
	*ioreg_ptr = humid_value;
	return;
}


/* _iom361_setSensor1_rndm() */
void _iom361_setSensor1_rndm(float temp_low, float temp_hi,
	float humid_low, float humid_hi) {
	float new_temp = 0.0, new_humid = 0.0;
	
	new_temp = (float) float_rand_in_range(temp_low, temp_hi);
	new_humid = (float) float_rand_in_range(humid_low, humid_hi);
	_iom361_setSensor1(new_temp, new_humid);
}
	
		
  
// Helper Functions
 
/**
 * display_leds() - displays the LED register
 *
 * Displays the LED values in a readable format
 *
 * @param	value is the leds to display
 * @param	num_leds is the number of LEDs to display
 *
 */ 
static void display_leds(uint32_t value, int num_leds) {
	char leds[32];

	// put either '0' (on) or 'x' (off) for each LED	  
	for (int i = 0; i < num_leds; i++) {
		leds[i] = (0x1 << i) & value ? 'o' : '_';
	}
	  
	// and put to display in reverse order
	// break into 4 led groups
	for (int i = num_leds - 1; i >= 0; i--) {
		if ((num_leds - 1 - i) % 4 == 0) {
			putchar(' ');
			putchar(' ');
		}
		putchar(leds[i]);
	}
	printf("\n");
	return;
}	


/**
 * display_rgb_leds() - displays the duty cycles for an RGB LED
 *
 * Displays the red, green, and blue duty cycles and the enable bit
 * from the RBG LED I/O register.  The RGB LED I/O register has this format:
 *	bits[31:31]:  	Enable - true if RGB outputs are enabled
 *	bits[30:24]:	*reserved*
 *	bits[23:16]:	8-bit duty cycle for Red segment
 *	bits[15:8]:		8-bit duty cycle for Green segment
 *	bits[7:0]:		8-bit duty cycle for Blue segment
 *
 * @param	value is the RGB control register
 *
 */   
static void display_rgb_leds(uint32_t value) {
	uint8_t red_dc, green_dc, blue_dc;
	uint8_t enable;
	int reddc, grndc, bludc;
		
	// get the duty cycles and enable from control reg
	blue_dc  = (value >> 0) & 0xFF;
	green_dc = (value >> 8) & 0xFF;
	red_dc   = (value >> 16)& 0xFF;
	enable   = (value >> 31)& 0x01;
		
	// calculate the duty cycle %
	reddc = (red_dc * 100 / 255);
	grndc = (green_dc * 100 / 255);
	bludc = (blue_dc * 100 / 255);
		
	printf("RedDC=%2d%% (%3d), GrnDC=%2d%% (%3d), BluDC=%2d%% (%3d)\tEnable=%s\r\n",
			reddc, red_dc,
			grndc, green_dc,
			bludc, blue_dc,
			(enable ? "ON" : "OFF")
	);
}
  
  
					
		
		
	 

		
	
	
	
		
		
	
	
	
 
 
 
 