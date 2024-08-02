#ifndef __MAIN_H
#define __MAIN_H

#include "stm32l4xx_hal.h"

void Error_Handler(void);

#define S1_Pin GPIO_PIN_0
#define S1_GPIO_Port GPIOA
#define S2_Pin GPIO_PIN_1
#define S2_GPIO_Port GPIOA
#define D3_Pin GPIO_PIN_2
#define D3_GPIO_Port GPIOA
#define D4_Pin GPIO_PIN_3
#define D4_GPIO_Port GPIOA

#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA

#endif
