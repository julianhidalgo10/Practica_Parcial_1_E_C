#include "main.h"
#include "stm32l4xx_hal.h"
#include <string.h>

// Primero se declaran las variables globales

uint32_t last_left_press_tick = 0;
uint32_t last_right_press_tick = 0;
uint8_t left_toggle_count = 0;
uint8_t right_toggle_count = 0;
UART_HandleTypeDef huart2;

// Se implementa la función para enviar un mensaje por UART
void send_message(char* message) {
    HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
}

// Ahora la función de callback para el manejo de interrupciones EXTI

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    uint32_t current_tick = HAL_GetTick();

    if (GPIO_Pin == S1_Pin && (current_tick > last_left_press_tick + 200)) {
        last_left_press_tick = current_tick;
        toggle_led(D3_GPIO_Port, D3_Pin, &left_toggle_count);
    } else if (GPIO_Pin == S2_Pin && (current_tick > last_right_press_tick + 200)) {
        last_right_press_tick = current_tick;
        toggle_led(D4_GPIO_Port, D4_Pin, &right_toggle_count);
    }
}

// Función para alternar el LED
void toggle_led(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t* toggle_count) {
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
    (*toggle_count)++;

    char message[50];
    sprintf(message, "LED %d toggled %d times\r\n", (GPIO_Pin == D3_Pin ? 3 : 4), *toggle_count);
    send_message(message);
}

// Ahora el main principal
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    // Se implementa el ciclo principal del programa
    while (1) {
        __WFI();
    }
}

void SystemClock_Config(void) {
}

// Ahora se procede con la inicialización de los GPIO
void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOA, D3_Pin | D4_Pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = D3_Pin | D4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = S1_Pin | S2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void MX_USART2_UART_Init(void) {
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        Error_Handler();
    }
}

// Funcion buscada para manejo de errores
void Error_Handler(void) {
    while(1) {
        // Se queda atrapado aquí si ocurre un error
    }
}
