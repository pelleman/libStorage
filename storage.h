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


byte readEepromByte();
int readEepromInt ();
long readEepromLong ();
void writeEepromByte(byte);
void writeEepromInt(int);
void writeEepromLong(long);
bool storageStartRead (long);
void storageStartWrite (long);
void storageEndWrite (long);

