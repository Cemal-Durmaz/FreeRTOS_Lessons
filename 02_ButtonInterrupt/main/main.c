//--- Includes ---//
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio.h"
#include "esp_log.h"

//--- Defines ---//
#define ESP_INR_FLAG_DEFAULT 0
#define LED_PIN 27
#define BUTTON_PIN 33

//--- Variables ---//
const char* TAG = "ISR";
TaskHandle_t ISR = NULL; // Interrupt Service Routine

//--- Tasks ---//
void interrupt_task(void *arg)
{
    bool led_stat = false;
    for(;;)
    {
        vTaskSuspend(NULL);
        led_stat = !led_stat;
        gpio_set_level(LED_PIN, led_stat);
        ESP_LOGI(TAG, "Button Pressed !");
    }
}

void IRAM_ATTR button_isr_handler(void *arg)
{
    xTaskResumeFromISR(ISR);
}

void app_main(void)
{
    gpio_pad_select_gpio(BUTTON_PIN);
    gpio_pad_select_gpio(LED_PIN);

    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_POSEDGE);

    gpio_install_isr_service(ESP_INR_FLAG_DEFAULT);

    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

    xTaskCreate(interrupt_task, "interrupt_task", 1024 * 4, NULL, 10, &ISR);
}