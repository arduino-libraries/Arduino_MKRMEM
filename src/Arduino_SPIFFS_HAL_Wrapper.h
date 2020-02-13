/*
  Copyright (c) 2019 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef ARDUINO_SPIFFS_HAL_WRAPPER_H_
#define ARDUINO_SPIFFS_HAL_WRAPPER_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

extern "C"
{
#include "spiffs.h"
}

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class Arduino_SpiffsHalWrapper
{

public:

  virtual ~Arduino_SpiffsHalWrapper() { }


  virtual s32_t read (u32_t addr, u32_t size, u8_t * buf) = 0;
  virtual s32_t write(u32_t addr, u32_t size, u8_t * buf) = 0;
  virtual s32_t erase(u32_t addr, u32_t size)             = 0;

};

#endif /* ARDUINO_SPIFFS_HAL_WRAPPER_H_ */
