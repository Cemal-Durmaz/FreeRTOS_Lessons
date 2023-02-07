//--- Includes ---//
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "esp_log.h"

//--- Variables ---//
TaskHandle_t myTaskHandle = NULL;
TaskHandle_t myTaskHandle2 = NULL;
SemaphoreHandle_t xSemaphore = NULL;

static const char* TAG = "Semaphore";

//--- RTOS Tasks ---//
void Demo_Task(void *arg)
{
    for(;;)
    {
        ESP_LOGI(TAG, "Message Sent! [%d]", xTaskGetTickCount());
        xSemaphoreGive(xSemaphore);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void Demo_Task2(void *arg)
{
    for(;;)
    {
        if(xSemaphoreTake(xSemaphore, portMAX_DELAY))
        {
            ESP_LOGI(TAG, "Received Message[%d]", xTaskGetTickCount());
        }
    }
}

void app_main(void)
{
    xSemaphore = xSemaphoreCreateBinary();
    xTaskCreate(Demo_Task, "Demo_Task", 4096, NULL, 10, &myTaskHandle);
    xTaskCreatePinnedToCore(Demo_Task2, "Demo_Task2", 4096, NULL, 10, &myTaskHandle2, 1);
}