# MKRMEM
This library provides a driver for the Arduino MKR MEM shield W25Q16DV SPI flash (with a capacity of 2 MByte) complete with integration for the [SPIFFS](https://github.com/pellepl/spiffs) embedded flash file system (EFFS).

## Why use a EFFS?
The usage of semiconductor based flash memories differs fundamentally from other devices such as magnetic based hard discs. In flash terminology you don't write to memory but you **program** it. When a flash memory is programmed the default state of the flash memory (all bits equal to 1) is changed to the desired value. Note that you can only change bits from 1 to 0, in order to change a bit to 1 again an **erase operation** has to be performed. Such an erase operation can not be performed for each individual bit, only a bit block of bits and bytes can be "erased" together. The smallest possible erase size for the W25Q16DV is 4 kByte (sector erase). Also you can not just programm any address on the flash, programming is usually performed **page-by-page**. The W25Q16DV page size is 256 bytes.

If data stored on the flash memory needs to be changed the data has to be read, modified and written to another address while the the old entry has to be marked as as invalid. Since this tends to consume the available space fairly quickly a process known as **garbage collection** has to be performed periodically. During garbage collection all valid data are copied to fresh blocks and the old blocks are erased.

When a maximum number of program / erase cycles (≥ 10^5) are exceeded the flash memory will start to wear down, causing the flash memory to no longer function reliably. As a countermeasure **wear leveling** techniques are used which distribute the data evenly across all sectors of the flash memory to minimize the number of erase cycles per sector and thus extend the life of the flash memory. **Dynamic** Wear Leveling refers to a wear-leveling strategy which works only with those data **Static** Wear Leveling refers to a wear-leveling strategy that looks at all data, including those already written to the flash memory.

The goals of garbage collection (maximizing the free sectors) and wear-leveling (even utilization of all sectors) are in conflict with one another. To get both sufficient performance and endurance, a good trade-off must be found between these two tasks.

So while the usage of a embedded flash file system comes with a bit of overhead it is the only way to reasonably ensure successful long-term operation of flash memories.
