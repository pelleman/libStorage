/**********************************************
 *
 * Storage.cpp
 *
 * Will serialize settings, sort of, on Arduino
 * This is a slighly flawed beta.
 * Features wear leveling and broken data redundancy.
 * CRC is not implemented yet.
 *
 * Per-Ola Gustavsson
 * http://marsba.se
 * pelle@marsba.se
 *
 *
 *
 * Copyright 2015 Per-Ola Gustavsson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *********************************************/


#include <EEPROM.h>
#include <Arduino.h>
#include "misc.h"

const byte STORAGE_MAX_REDUNDANCY = 10;
const byte STORAGE_BLOCK_SIZE = 32;
const int STORAGE_SIZE = 1024;
byte storageCurrentBlock = 255;
byte storagePreviousBlock = 255;
int eepromPos = STORAGE_SIZE;
long storageHeader = 0;

/* struct storageBlock		// Only informational
{
	long header;
	byte next
	byte previous;
	byte first;
	byte crc;				// CRC is not implemented
	// byte data[STORAGE_BLOCK_SIZE - 8];
};*/

byte readEepromByte()
{
	byte read = 0;
	EEPROM.get(eepromPos, read);
	eepromPos+=sizeof(read);
	DEBUG_PRINT(read);
	return read;
}
int readEepromInt ()
{
	int read = 0;
	EEPROM.get(eepromPos, read);
	eepromPos+=sizeof(read);
	DEBUG_PRINT(read);
	return read;
}
long readEepromLong ()
{
	long read = 0;
	EEPROM.get(eepromPos, read);
	eepromPos+=sizeof(read);
	DEBUG_PRINT(read);
	return read;
}
void writeEepromByte(byte data)
{
	EEPROM.put(eepromPos, data);
	eepromPos+=sizeof(data);
	DEBUG_PRINT(data);
}
void writeEepromInt(int data)
{
	EEPROM.put(eepromPos, data);
	eepromPos+=sizeof(data);
	DEBUG_PRINT(data);
}
void writeEepromLong(long data)
{
	EEPROM.put(eepromPos, data);
	eepromPos+=sizeof(data);
	DEBUG_PRINT(data);
}
bool storageStartRead (long header) 
{
	int i = STORAGE_BLOCK_SIZE;
	do {
		long inLong = 0;
		EEPROM.get(i, inLong);
		if (inLong == header) {
			byte inByte = 0;
			EEPROM.get(i + 4, inByte);
			if (inByte == 255) {
				eepromPos = i + 8;
				return true;
			}
			i = inByte * STORAGE_BLOCK_SIZE;
		} else 
			i += STORAGE_BLOCK_SIZE;
	} while (i < 1024);
	return false;
}
void storageStartWrite (long header)
{
	storageHeader = header;
	byte block;
	int i = STORAGE_BLOCK_SIZE;
	do {
		long inLong = 0;
		EEPROM.get(i, inLong);
		if (inLong == header) {
			byte inByte = 0;
			EEPROM.get(i + 4, inByte);
			if (inByte == 0xFF) {
				storagePreviousBlock = i / STORAGE_BLOCK_SIZE;
				i += STORAGE_BLOCK_SIZE;
				while (i < STORAGE_SIZE) {
					EEPROM.get(i + 4, inLong);
					if (inLong == 0xFFFFFFFF) {
						storageCurrentBlock = i / STORAGE_BLOCK_SIZE;
						eepromPos = i + 8;
						return;
					}
				}
				EEPROM.get(storagePreviousBlock * STORAGE_BLOCK_SIZE + 6, 
					storageCurrentBlock);
				eepromPos = storageCurrentBlock * STORAGE_BLOCK_SIZE + 8;
				return;
			}
			i = inByte * STORAGE_BLOCK_SIZE;
		} else 
			i += STORAGE_BLOCK_SIZE;
	} while (i < STORAGE_SIZE);
	
	if (i >= STORAGE_SIZE) {
		storageCurrentBlock = 1;
		eepromPos = STORAGE_BLOCK_SIZE + 8;
		return;
	}
	
	
}
void storageEndWrite ()
{
	EEPROM.put(storageCurrentBlock * STORAGE_BLOCK_SIZE, storageHeader);
	EEPROM.put(storageCurrentBlock * STORAGE_BLOCK_SIZE + 4, 0xFF);
	EEPROM.put(storageCurrentBlock * STORAGE_BLOCK_SIZE + 5, storagePreviousBlock);
	EEPROM.put(storageCurrentBlock * STORAGE_BLOCK_SIZE + 7, 0xFF);

	byte first = 0xFF;
	if (storagePreviousBlock != 0xFF) {
		EEPROM.get(storagePreviousBlock * STORAGE_BLOCK_SIZE + 4, first);
		if (storageCurrentBlock == first)
			EEPROM.get(storageCurrentBlock * STORAGE_BLOCK_SIZE + 4, first);
		EEPROM.put(storagePreviousBlock * STORAGE_BLOCK_SIZE + 4, storageCurrentBlock);
	}
	EEPROM.put(storageCurrentBlock * STORAGE_BLOCK_SIZE + 6, first);
	
	eepromPos = STORAGE_SIZE;
}
