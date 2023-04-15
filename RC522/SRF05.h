/*****************************************************************************************************
@tac gia: khuenguyen
@website: khuenguyencreator.com
@youtube: https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
@huong dan su dung:
- Cau hinh chan TRIG output no pull, Echo input no pull
- Tao mot bien luu gia tri doc duoc kieu SRF05_Typedef 
VD: SRF05_Typedef SRF05_01;
- Truyen dia chi 2 chan vao SRF05_Init
VD: SRF05_Init(&SRF05_01, ECHO_GPIO_Port, ECHO_Pin, TRIG_GPIO_Port, TRIG_Pin);
- Doc gia tri nhan duoc
VD: Value = SRF05_Read(&SRF05_01);

***Note: #include va cau hinh ham delay truyen vao
*********************** *****************************************************************************/
#ifndef __SRF05_H
#define __SRF05_H

#include "stm32f1xx_hal.h"
#define SRF05_TIMEOUT			1000000
#define SRF05_NUMBER			((float)0.0171821)
	
typedef struct {          /*!< Distance measured from sensor in centimeters */
	GPIO_TypeDef* ECHO_GPIOx;    /*!< Pointer to GPIOx PORT for ECHO pin. Meant for private use only */
	uint16_t ECHO_GPIO_Pin;      /*!< GPIO Pin for ECHO pin. Meant for private use only */
	GPIO_TypeDef* TRIGGER_GPIOx; /*!< Pointer to GPIOx PORT for TRIGGER pin. Meant for private use only */
	uint16_t TRIGGER_GPIO_Pin;   /*!< GPIO Pin for ECHO pin. Meant for private use only */
	float Distance;    
}SRF05_Device_Name;

uint8_t SRF05_Init(SRF05_Device_Name* SRF05, GPIO_TypeDef* ECHO_GPIOx, uint16_t ECHO_GPIO_Pin, GPIO_TypeDef* TRIGGER_GPIOx, uint16_t TRIGGER_GPIO_Pin);
float SRF05_Read(SRF05_Device_Name* );
#endif
