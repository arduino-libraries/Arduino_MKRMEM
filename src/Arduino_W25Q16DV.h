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

#ifndef ARDUINO_W25Q16DV_H_
#define ARDUINO_W25Q16DV_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <Arduino.h>

#include <SPI.h>

extern "C"
{
#include "spiffs.h"
}

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

enum class W25Q16DV_Command : uint8_t
{
  PageProgram    = 0x02,
  ReadData       = 0x03,
  ReadStatusReg1 = 0x05,
  WriteEnable    = 0x06,
  SectorErase    = 0x20,
  ChipErase      = 0x60,
  ReadJedecId    = 0x9F,
};

typedef struct
{
  uint8_t manufacturer_id;
  uint8_t memory_type;
  uint8_t capacity;
} W25Q16DV_Id;

typedef union
{
  struct {
    uint8_t BUSY : 1;
    uint8_t WEL  : 1;
    uint8_t BP0  : 1;
    uint8_t BP1  : 1;
    uint8_t BP2  : 1;
    uint8_t TB   : 1;
    uint8_t SEC  : 1;
    uint8_t SRP0 : 1;
  } bit;
  uint8_t byte;
} W25Q16DV_StatusReg1;

/**************************************************************************************
 * CONSTANTS
 **************************************************************************************/
#if defined(ARDUINO_EDGE_CONTROL)
static int const EDGE_CONTROL_W25Q16DV_CS_PIN = 28;
#else
static int const MKRMEM_W25Q16DV_CS_PIN = 5;
#endif

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class Arduino_W25Q16DV
{

public:

  Arduino_W25Q16DV(SPIClass & spi, int const cs_pin);

  void begin();

  W25Q16DV_Id readId();

  bool isBusy     ();  
  void read       (uint32_t const addr, uint8_t       * buf, uint32_t const size);
  void programPage(uint32_t const addr, uint8_t const * buf, uint32_t const size);
  void eraseSector(uint32_t const addr);
  void eraseChip  ();


  static uint32_t const PAGE_SIZE   = 256;
  static uint32_t const SECTOR_SIZE = 4096;
  static uint32_t const MEMORY_SIZE = 4096 * 16 * 32; /* 16 sectors per block / 32 blocks per chip */

private:

  SPIClass & _spi;
  int const _cs_pin;

  void select();
  void deselect();

  uint8_t readStatusReg1();
  void    enableWrite();

};

/**************************************************************************************
 * EXTERN DEFINITION
 **************************************************************************************/

extern Arduino_W25Q16DV flash;

/**************************************************************************************
 * FREE FUNCTION DECLARATION
 **************************************************************************************/

s32_t w25q16_spi_read (u32_t addr, u32_t size, u8_t * buf);
s32_t w25q16_spi_write(u32_t addr, u32_t size, u8_t * buf);
s32_t w25q16_spi_erase(u32_t addr, u32_t size);

#endif /* ARDUINO_W25Q16DV_H_ */
