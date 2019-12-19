/* SPIFFSUsage.ino
 * 
 * This sketch demonstrates various file operations utilizing
 * the Arduino MKR MEM shield port for the SPIFFS.
 * 
 * Alexander Entinger
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <Arduino_MKRMEM.h>

/**************************************************************************************
 * CONSTANTS
 **************************************************************************************/

/* A pangram is a sentence using every letter of a given alphabet at least once. */
static char const PANGRAM[] = "The quick brown fox jumps over the lazy dog.";

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

Arduino_SPIFFS filesystem;

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup() {
  Serial.begin(9600);

  unsigned long const start = millis();
  for(unsigned long now = millis(); !Serial && ((now - start) < 5000); now = millis()) { };
  
  w25q16dv.begin();

  Serial.println("Mounting ...");
  if(SPIFFS_OK != filesystem.mount()) {
    Serial.println("mount() failed with error code "); Serial.println(filesystem.err()); return;
  }


  Serial.println("Checking ...");
  if(SPIFFS_OK != filesystem.check()) {
    Serial.println("check() failed with error code "); Serial.println(filesystem.err()); return;
  }


  Serial.println("Writing ...");
  /* Create file if it doesn't exist (SPIFFS_CREAT) and open in 
   * write only mode (SPIFFS_WRONLY ). If the file does exist
   * delete the existing content (SPIFFS_TRUNC).
   */
  spiffs_file file = filesystem.open("fox.txt", SPIFFS_CREAT | SPIFFS_WRONLY | SPIFFS_TRUNC);

  s32_t const bytes_to_write = sizeof(PANGRAM);
  s32_t const bytes_written = filesystem.write(file, (void *)PANGRAM, bytes_to_write);
  
  if(bytes_written != bytes_to_write) {
    Serial.println("write() failed with error code "); Serial.println(filesystem.err()); return;
  }

  filesystem.close(file);


  Serial.println("Reading ...");

  file = filesystem.open("fox.txt", SPIFFS_RDONLY);

  char buf[64] = {0};
  s32_t const bytes_read = filesystem.read(file, buf, sizeof(buf));
  buf[bytes_read] = '\0';
  
  filesystem.close(file);
  Serial.print("["); Serial.print(bytes_read); Serial.print("] ");
  Serial.println(buf);

  Serial.println("Unmounting ...");
  filesystem.unmount();
}

void loop() {

}
