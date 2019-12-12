/* RawFlashAccess.ino
 * 
 * This sketch demonstrates the raw API of the W25Q16DV class
 * whichallows for low level flash memory control.
 * 
 * Alexander Entinger
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "MKRMEM.h"

#undef max
#undef min
#include <array>
#include <algorithm>

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

W25Q16DV memory;

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  Serial.begin(9600);

  unsigned long const start = millis();
  for(unsigned long now = millis(); !Serial && ((now - start) < 5000); now = millis()) { };
  
  memory.begin();
  
  W25Q16DV_Id const id = memory.readId();

  char msg[32] = {0};
  snprintf(msg, sizeof(msg), "ID: %02X %02X %02X", id.manufacturer_id, id.memory_type, id.capacity);
  Serial.println(msg);

  /* Perform a complete chip erase (may last a little while) */
  memory.eraseChip();

  std::array<uint8_t, 256> data_write = {0},
                           data_read  = {0};
  
  /* Initialize data */
  std::transform(data_write.begin(), data_write.end(), data_write.begin(),
                [](uint8_t const elem)
                {
                  static uint8_t i = 0;
                  return i++;
                });
  
  printArray("WR: ", data_write);

  /* Write data to chip to first block */
  memory.programPage(0x000100, data_write.data(), data_write.size());

  /* Read data back in another array */
  memory.read(0x000100, data_read.data(), data_read.size());
  printArray("RD: ", data_read);

  /* Compare the two data buffers */
  if(std::equal(data_write.begin(), data_write.end(), data_read.begin())) {
    Serial.println("Comparison OK");
  } else {
    Serial.println("Comparison FAIL");
  }

  /* Erase the whole first sector (4 kB) */
  memory.eraseSector(0x000000);

  /* Set the comparison buffer to 0xFF since we now need to compare if every value is 0xFF */
  std::fill(data_write.begin(), data_write.end(), 0xFF);

  /* Read the data */
  memory.read(0x000100, data_read.data(), data_read.size());
  printArray("RD: ", data_read);

  /* Compare the two data buffers */
  if(std::all_of(data_read.begin(), data_read.end(), [](uint8_t const elem) { return (elem == 0xFF); })) {
    Serial.println("Comparison OK");
  } else {
    Serial.println("Comparison FAIL");
  }
}

void loop()
{ 

}

/**************************************************************************************
 * HELPER
 **************************************************************************************/

void printArray(char const * desc, std::array<uint8_t, 256> arr)
{
  Serial.print(desc);
  
  std::for_each(arr.begin(), arr.end(),
                [](uint8_t const elem)
                {
                  Serial.print(elem, HEX);
                  Serial.print(" ");
                });
  
  Serial.println();
}
