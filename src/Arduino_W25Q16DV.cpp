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

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "Arduino_W25Q16DV.h"

/**************************************************************************************
 * CONSTANTS
 **************************************************************************************/

static uint32_t const W25Q16DV_MAX_SPI_CLK = 104 * 1000 * 1000; /* 104 MHz */
static SPISettings const W25Q16DV_SPI_SETTINGS{W25Q16DV_MAX_SPI_CLK, MSBFIRST, SPI_MODE0};

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

Arduino_W25Q16DV::Arduino_W25Q16DV(SPIClass & spi, int const cs_pin)
: _spi   (spi)
, _cs_pin(cs_pin)
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void Arduino_W25Q16DV::begin()
{
  _spi.begin();
  pinMode(_cs_pin, OUTPUT);
  deselect();
}

W25Q16DV_Id Arduino_W25Q16DV::readId()
{
  W25Q16DV_Id id;
  
  select();
  _spi.beginTransaction(W25Q16DV_SPI_SETTINGS);
  _spi.transfer(static_cast<uint8_t>(W25Q16DV_Command::ReadJedecId));
  id.manufacturer_id = _spi.transfer(0);
  id.memory_type     = _spi.transfer(0);
  id.capacity        = _spi.transfer(0);
  deselect();

  return id;
}

bool Arduino_W25Q16DV::isBusy()
{
  W25Q16DV_StatusReg1 status_reg_1;
  status_reg_1.byte = readStatusReg1();
  return (status_reg_1.bit.BUSY == 1);
}

void Arduino_W25Q16DV::read(uint32_t const addr, uint8_t * buf, uint32_t const size)
{
  while(isBusy()) { delayMicroseconds(1); }

  select();
  _spi.beginTransaction(W25Q16DV_SPI_SETTINGS);
  /* Command */
  _spi.transfer(static_cast<uint8_t>(W25Q16DV_Command::ReadData));
  /* Address */
  _spi.transfer(static_cast<uint8_t>(addr >> 16));
  _spi.transfer(static_cast<uint8_t>(addr >>  8));
  _spi.transfer(static_cast<uint8_t>(addr >>  0));
  /* Data */
  for(uint32_t bytes_read = 0; bytes_read < size; bytes_read++)
  {
    buf[bytes_read] = _spi.transfer(0);
  }
  deselect();
}

void Arduino_W25Q16DV::programPage(uint32_t const addr, uint8_t const * buf, uint32_t const size)
{
  while(isBusy()) { delayMicroseconds(1); }

  enableWrite();

  select();
  _spi.beginTransaction(W25Q16DV_SPI_SETTINGS);
  /* Command */
  _spi.transfer(static_cast<uint8_t>(W25Q16DV_Command::PageProgram));
  /* Address */
  _spi.transfer(static_cast<uint8_t>(addr >> 16));
  _spi.transfer(static_cast<uint8_t>(addr >>  8));
  _spi.transfer(static_cast<uint8_t>(addr >>  0));
  /* Data */
  for(uint32_t bytes_written = 0; bytes_written < size; bytes_written++)
  {
    _spi.transfer(buf[bytes_written]);
  }
  deselect();
}

void Arduino_W25Q16DV::eraseSector(uint32_t const addr)
{
  while(isBusy()) { delayMicroseconds(1); }

  enableWrite();

  select();
  _spi.beginTransaction(W25Q16DV_SPI_SETTINGS);
  /* Command */
  _spi.transfer(static_cast<uint8_t>(W25Q16DV_Command::SectorErase));
  /* Address */
  _spi.transfer(static_cast<uint8_t>(addr >> 16));
  _spi.transfer(static_cast<uint8_t>(addr >>  8));
  _spi.transfer(static_cast<uint8_t>(addr >>  0));
  deselect();
}

void Arduino_W25Q16DV::eraseChip()
{
  while(isBusy()) { delayMicroseconds(1); }

  enableWrite();

  select();
  _spi.beginTransaction(W25Q16DV_SPI_SETTINGS);
  _spi.transfer(static_cast<uint8_t>(W25Q16DV_Command::ChipErase));
  deselect();

  /* In this instance wait within this function since it's so time consuming */
  while(isBusy()) { delayMicroseconds(1); }
}

/**************************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 **************************************************************************************/

void Arduino_W25Q16DV::select()
{
  digitalWrite(_cs_pin, LOW);
}

void Arduino_W25Q16DV::deselect()
{
  digitalWrite(_cs_pin, HIGH);
}

uint8_t Arduino_W25Q16DV::readStatusReg1()
{
  select();
  _spi.beginTransaction(W25Q16DV_SPI_SETTINGS);
  /* Command */
  _spi.transfer(static_cast<uint8_t>(W25Q16DV_Command::ReadStatusReg1));
  /* Read Status Reg 1 */
  uint8_t const status_reg_1 = _spi.transfer(0);
  deselect();

  return status_reg_1;
}

void Arduino_W25Q16DV::enableWrite()
{
  select();
  _spi.beginTransaction(W25Q16DV_SPI_SETTINGS);
  _spi.transfer(static_cast<uint8_t>(W25Q16DV_Command::WriteEnable));
  deselect();
}

/**************************************************************************************
 * EXTERN DECLARATION
 **************************************************************************************/

Arduino_W25Q16DV flash(SPI, MKRMEM_W25Q16DV_CS_PIN);

/**************************************************************************************
 * FREE FUNCTION DEFINITION
 **************************************************************************************/

s32_t w25q16_spi_read(u32_t addr, u32_t size, u8_t * buf)
{
  flash.read(addr, buf, size);
  return SPIFFS_OK;
}

s32_t w25q16_spi_write(u32_t addr, u32_t size, u8_t * buf)
{
  flash.programPage(addr, buf, size);
  return SPIFFS_OK;
}

s32_t w25q16_spi_erase(u32_t addr, u32_t size)
{
  flash.eraseSector(addr);
  return SPIFFS_OK;
}
