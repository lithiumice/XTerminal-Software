#include "HAL/HAL.h"
#include "SPI.h"
#include "SD.h"
uint64_t cardSize;

bool HAL::SD_Init()
{
   pinMode(CONFIG_SD_DET_PIN, INPUT);

   SPIClass* sd_spi = new SPIClass(HSPI); // another SPI
   if (!SD.begin(15, *sd_spi, 80000000)) // SD-Card SS pin is 15
   {
       HAL::TerminalPrintln("Card Mount Failed");
       return false;
   }
   uint8_t cardType = SD.cardType();

   if (cardType == CARD_NONE)
   {
       HAL::TerminalPrintln("No SD card attached");
       return false;
   }

   HAL::TerminalPrint("SD Card Type: ");
   if (cardType == CARD_MMC)
   {
       HAL::TerminalPrintln("MMC");
   } else if (cardType == CARD_SD)
   {
       HAL::TerminalPrintln("SDSC");
   } else if (cardType == CARD_SDHC)
   {
       HAL::TerminalPrintln("SDHC");
   } else
   {
       HAL::TerminalPrintln("UNKNOWN");
   }

   cardSize = SD.cardSize() / (1024 * 1024);
   HAL::TerminalPrintln("SD Card Size(MB): "+String((int)cardSize));

   return true;
}

bool HAL::SD_GetReady()
{
    return !digitalRead(CONFIG_SD_DET_PIN);
}

float HAL::SD_GetCardSizeMB()
{
    return cardSize;
}

// static void SD_Check(bool isInsert)
// {

// }

void HAL::SD_SetEventCallback(SD_CallbackFunction_t callback)
{

}

void HAL::SD_Update()
{

}
