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

#include "W25Q16DV.h"

#include <SPI.h>

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

W25Q16DV::W25Q16DV(int const cs_pin)
: _cs_pin(cs_pin)
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void W25Q16DV::begin()
{
  SPI.begin();
  pinMode(_cs_pin, OUTPUT);
  deselect();
}

W25Q16DV_Id W25Q16DV::readId()
{
  W25Q16DV_Id id;
  
  select();
  SPI.transfer(static_cast<uint8_t>(W25Q16DV_Command::ReadJedecId));
  id.manufacturer_id = SPI.transfer(0);
  id.memory_type     = SPI.transfer(0);
  id.capacity        = SPI.transfer(0);
  deselect();

  return id;
}

bool W25Q16DV::isBusy()
{
  W25Q16DV_StatusReg1 status_reg_1;
  status_reg_1.byte = readStatusReg1();
  return (status_reg_1.bit.BUSY == 1);
}

void W25Q16DV::read(uint32_t const addr, uint8_t * buf, uint32_t const size)
{
  while(isBusy()) { delayMicroseconds(1); }

  isBusy();
  
  select();
  /* Command */
  SPI.transfer(static_cast<uint8_t>(W25Q16DV_Command::ReadData));
  /* Address */
  SPI.transfer(static_cast<uint8_t>(addr >> 16));
  SPI.transfer(static_cast<uint8_t>(addr >>  8));
  SPI.transfer(static_cast<uint8_t>(addr >>  0));
  /* Data */
  for(uint32_t bytes_read = 0; bytes_read < size; bytes_read++)
  {
    buf[bytes_read] = SPI.transfer(0);
  }
  deselect();
}

void W25Q16DV::programPage(uint32_t const addr, uint8_t const * buf, uint32_t const size)
{
  while(isBusy()) { delayMicroseconds(1); }

  enableWrite();

  select();
  /* Command */
  SPI.transfer(static_cast<uint8_t>(W25Q16DV_Command::PageProgram));
  /* Address */
  SPI.transfer(static_cast<uint8_t>(addr >> 16));
  SPI.transfer(static_cast<uint8_t>(addr >>  8));
  SPI.transfer(static_cast<uint8_t>(addr >>  0));
  /* Data */
  for(uint32_t bytes_written = 0; bytes_written < size; bytes_written++)
  {
    SPI.transfer(buf[bytes_written]);
  }
  deselect();
}

void W25Q16DV::eraseSector(uint32_t const addr)
{
  while(isBusy()) { delayMicroseconds(1); }

  enableWrite();

  select();
  /* Command */
  SPI.transfer(static_cast<uint8_t>(W25Q16DV_Command::SectorErase));
  /* Address */
  SPI.transfer(static_cast<uint8_t>(addr >> 16));
  SPI.transfer(static_cast<uint8_t>(addr >>  8));
  SPI.transfer(static_cast<uint8_t>(addr >>  0));
  deselect();
}

/**************************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 **************************************************************************************/

void W25Q16DV::select()
{
  digitalWrite(_cs_pin, LOW);
}

void W25Q16DV::deselect()
{
  digitalWrite(_cs_pin, HIGH);
}

uint8_t W25Q16DV::readStatusReg1()
{
  select();
  /* Command */
  SPI.transfer(static_cast<uint8_t>(W25Q16DV_Command::ReadStatusReg1));
  /* Read Status Reg 1 */
  uint8_t const status_reg_1 = SPI.transfer(0);
  deselect();

  return status_reg_1;
}

void W25Q16DV::enableWrite()
{
  select();
  SPI.transfer(static_cast<uint8_t>(W25Q16DV_Command::WriteEnable));
  deselect();
}