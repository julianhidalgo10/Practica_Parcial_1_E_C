#include "stm32l4xx_hal.h"

// Primero se definen los pines para los botones y LEDs

#define BUTTON_LEFT_Pin        GPIO_PIN_0
#define BUTTON_RIGHT_Pin       GPIO_PIN_1
#define LED_LEFT_Pin           GPIO_PIN_2
#define LED_RIGHT_Pin          GPIO_PIN_3
#define BUTTON_GPIO_Port       GPIOA
#define LED_GPIO_Port          GPIOB

// Ahora la variable global
UART_HandleTypeDef huart2;

// Y funciones de prototipo

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void Blink_LED(GPIO_TypeDef *LED_Port, uint16_t LED_Pin);

// Se hace el main principal

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    while (1) {
        // Se implementa el ciclo y se comprueba si se presiona el botón de la izquierda
        if (HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_LEFT_Pin) == GPIO_PIN_SET) {
            Blink_LED(LED_GPIO_Port, LED_LEFT_Pin);
            char msg[] = "Giro a la izquierda\n";
            HAL_UART_Transmit(&huart2, (uint8_t*)msg, sizeof(msg), HAL_MAX_DELAY);
            HAL_Delay(500);
        }

        // Ahora de igual manera se comprueba si se presiona el botón de la derecha
        if (HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_RIGHT_Pin) == GPIO_PIN_SET) {
            Blink_LED(LED_GPIO_Port, LED_RIGHT_Pin);
            char msg[] = "Giro a la derecha\n";
            HAL_UART_Transmit(&huart2, (uint8_t*)msg, sizeof(msg), HAL_MAX_DELAY);
            HAL_Delay(500);
        }
    }
}

// Se implementa la función para parpadear un LED a 2Hz 3 veces

void Blink_LED(GPIO_TypeDef *LED_Port, uint16_t LED_Pin) {
    for (int i = 0; i < 3; i++) {
        HAL_GPIO_WritePin(LED_Port, LED_Pin, GPIO_PIN_SET);
        HAL_Delay(250);
        HAL_GPIO_WritePin(LED_Port, LED_Pin, GPIO_PIN_RESET);
        HAL_Delay(250);
    }
}

// Finalmente la configuración del reloj del sistema

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Y la configuración del oscilador
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    }

    // Se inicializa la configuración del reloj

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
    }
}

// Inicialización de los GPIOs
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Se habilitan los relojes de los puertos GPIO
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Se configuran el GPIO para los botones
    GPIO_InitStruct.Pin = BUTTON_LEFT_Pin | BUTTON_RIGHT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

    // Se configuran el GPIO para los LEDs
    GPIO_InitStruct.Pin = LED_LEFT_Pin | LED_RIGHT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);
}

// Inicialización de USART2
static void MX_USART2_UART_Init(void) {
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
    }
}
