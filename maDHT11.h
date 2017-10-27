/*
 
DHT11 microaddict library 1.0
Copyright (C) 2017 Ismael García-Marlowe

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

 */ 

#define F_CPU 16000000
#include <util/delay.h>
#include <asf.h>

#ifndef MADHT11_H_
#define MADHT11_H_

/************************************************************/
/************** CUSTOMIZE HERE ******************************/

#define DHT11_PORT				PORTB
#define DHT11_PORT_BIT			1
#define DHT11_PORT_CONFIG		DDRB
#define DHT11_PORT_READ_REG		PINB 

/************************************************************/

// Timing constants and macros

#define LOW_US_WIDTH			54
#define HIGH_1_US_WIDTH			70
#define HIGH_0_US_WIDTH			24
#define RADIUS_0				15
#define RADIUS_1				30

#define MIN_1_WIDTH				(HIGH_1_US_WIDTH - RADIUS_1)
#define MAX_1_WIDTH				(HIGH_1_US_WIDTH + RADIUS_1)
#define MIN_0_WIDTH				(HIGH_0_US_WIDTH - RADIUS_0)
#define MAX_0_WIDTH				(HIGH_0_US_WIDTH + RADIUS_0)

#define _is_1(us)				( us > MIN_1_WIDTH && us < MAX_1_WIDTH )
#define _is_0(us)				( us > MIN_0_WIDTH && us < MAX_0_WIDTH )

// Other constants

#define BIT_COUNT				40
#define REQUEST_LOW_MS			20	// Should be > 18 ms
#define REQUEST_HIGH_US			40  // Should be 40 us
#define OUTPUT_SAFE_WAIT		20	// For safety's sake, wait a few ms after pin is changed to output mode

#define TRUE	0x01
#define FALSE	0x00

// I/O macros

#define _set_pin_as_output()	DHT11_PORT_CONFIG	|= 0x01 << DHT11_PORT_BIT;
#define _set_pin_as_input()		DHT11_PORT_CONFIG	&= ~(0x01 << DHT11_PORT_BIT); DHT11_PORT	&= ~(0x01 << DHT11_PORT_BIT)
#define _set_1()				DHT11_PORT			|= 0x01 << DHT11_PORT_BIT;
#define _set_0()				DHT11_PORT			&= ~(0x01 << DHT11_PORT_BIT);
#define _get_input()			( 0x01 & (PINB >> DHT11_PORT_BIT) )

// Types

typedef unsigned char byte;
typedef struct _DHT11_reading {
	byte temperature;
	byte humidity;
	byte valid;
} DHT11_reading;

typedef struct _DHT11_raw_data {
	byte temp_integer;
	byte temp_dec;
	byte humidity_integer;
	byte humidity_dec;
	byte checksum;
} DHT11_raw_data;

/* Prototypes */

DHT11_reading read_DHT11();

#endif /* MADHT11_H_ */