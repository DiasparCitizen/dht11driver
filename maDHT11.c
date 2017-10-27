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

 #include "maDHT11.h"
 #include <stdio.h>

 /* PROTOTYPES */

 byte get_level_us_length(byte level); 

 /* IMPLEMENTATIONS */

 byte get_level_us_length(byte level){

	byte uss = 0;
	while (_get_input() == level && uss++ < 0xFF)
		_delay_us(1);
	return uss;

 }
 
 byte read_byte_from_array(byte *array){

	byte b = 0x00, i;
	for (i = 0; i < 8; i++)
		b |= array[7-i] << i;
	return b;

 }

 DHT11_reading read_DHT11(){

	byte lengths[BIT_COUNT];
	byte i, error;
	DHT11_raw_data raw_data;
	DHT11_reading reading;
	
	// Request reading
	_set_pin_as_output();
	_delay_ms(OUTPUT_SAFE_WAIT);

	_set_0();
	_delay_ms(REQUEST_LOW_MS);
	_set_1();
	_delay_us(REQUEST_HIGH_US);

	// Prepare to receive data
	_set_pin_as_input();
	while (_get_input() == 1);	// Wait for signal to change

	// Get confirmation
	lengths[0] = get_level_us_length(0);
	lengths[1] = get_level_us_length(1);

	// Get all 40 bits
	i = 0;
	while (i < BIT_COUNT){
		get_level_us_length(0);
		lengths[i] = get_level_us_length(1);
		i++;
	}

	// Replace with 1's and 0's
	error = FALSE;
	i = 0;
	while (i < 40 && !error){
		if ( _is_1(lengths[i]) ) lengths[i] = 0x01;
		else if ( _is_0(lengths[i]) ) lengths[i] = 0x00;
		else error = TRUE;
		i++;
	}

	// Interpret data
	raw_data.humidity_integer	= read_byte_from_array(&lengths[0]);
	raw_data.humidity_dec		= read_byte_from_array(&lengths[8]);
	raw_data.temp_integer		= read_byte_from_array(&lengths[16]);
	raw_data.temp_dec			= read_byte_from_array(&lengths[24]);
	raw_data.checksum			= read_byte_from_array(&lengths[32]);

	// Build results
	reading.temperature = raw_data.temp_integer;
	reading.humidity = raw_data.humidity_integer;
	reading.valid = !error
		&& raw_data.checksum == (raw_data.temp_integer + raw_data.temp_dec + raw_data.humidity_integer + raw_data.humidity_dec);
	
	return reading;

 }

 

