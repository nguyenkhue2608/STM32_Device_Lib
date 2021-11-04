#include "FLASH.h"

void FLASH_WritePage(uint32_t startPage, uint32_t endPage, uint32_t data)
{
  HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef EraseInit;
	EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInit.PageAddress = startPage;
	EraseInit.NbPages = (endPage - startPage)/FLASH_PAGE_SIZE;
	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&EraseInit, &PageError);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startPage, data);
  HAL_FLASH_Lock();
}
uint32_t FLASH_ReadData(uint32_t addr)
{
	uint32_t data = *(__IO uint32_t *)(addr);
	return data;
}
