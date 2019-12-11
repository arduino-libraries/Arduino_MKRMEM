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


  uint8_t data_write[256] = {0},
          data_read [256] = {0};
  
  /* Initialize data */
  std::for_each(data_write,
                data_write + sizeof(data_write),
                [&](uint8_t & elem)
                {
                  static uint8_t i = 0;
                  elem = i;
                  i++;
                });
  
  printArray("WR: ", data_write, sizeof(data_write));

  /* Write data to chip to first block */
  memory.programPage(0x000100, data_write, sizeof(data_write));

  /* Read data back in another array */
  memory.read(0x000100, data_read, sizeof(data_read));
  printArray("RD: ", data_read, sizeof(data_read));

  /* Compare the two data buffers */
  if(std::equal(data_write, data_write + sizeof(data_write), data_read)) {
    Serial.println("Comparison OK");
  } else {
    Serial.println("Comparison FAIL");
  }

  /* Erase the whole first sector (4 kB) */
  memory.eraseSector(0x000000);

  /* Set the comparison buffer to 0xFF since we now need to compare if every value is 0xFF */
  memset(data_write, 0xFF, sizeof(data_write));

  /* Read the data */
  memory.read(0x000100, data_read, sizeof(data_read));
  printArray("RD: ", data_read, sizeof(data_read));

  /* Compare the two data buffers */
  if(std::all_of(data_read,
                 data_read + sizeof(data_read),
                 [](uint8_t const elem) { return (elem == 0xFF); })) {
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

void printArray(char const * desc, uint8_t const * arr, size_t const arr_size)
{
  Serial.print(desc);
  
  std::for_each(arr,
                arr + arr_size,
                [](uint8_t const elem)
                {
                  Serial.print(elem, HEX);
                  Serial.print(" ");
                });
  
  Serial.println();
}
