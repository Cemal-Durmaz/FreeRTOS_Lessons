//--- Includes ---//
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/queue.h"
#include "esp_log.h"

//--- Variables ---//
TaskHandle_t myTaskHandle = NULL;
TaskHandle_t myTaskHandle2 = NULL;
QueueHandle_t myQueue = NULL;
static const char* TAG = "Queue";

//--- RTOS Tasks ---//
void my_task_1(void *arg)
{
    char txBuffer[50];
    myQueue = xQueueCreate(5, sizeof(txBuffer));
    if(myQueue == 0)
        ESP_LOGI(TAG, "Failed to create queue");

    sprintf(txBuffer, "Hello from my_task_1 1");
    xQueueSend(myQueue, (void*)txBuffer, (TickType_t)0);

    sprintf(txBuffer, "Hello from my_task_1 2");
    xQueueSend(myQueue, (void*)txBuffer, (TickType_t)0);

    sprintf(txBuffer, "Hello from my_task_1 3");
    xQueueSend(myQueue, (void*)txBuffer, (TickType_t)0);

    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void my_task_2(void *arg)
{
    char rxBuffer[50];
    for(;;)
    {
        if(xQueueReceive(myQueue, &rxBuffer, (TickType_t)5))
        {
            ESP_LOGI(TAG, "Received data from queue == %s", rxBuffer);
            vTaskDelay(pdMS_TO_TICKS(1000));           
        }
    }
}


void app_main(void)
{
    xTaskCreate(my_task_1, "my_task_1", 4096, NULL, 10, &myTaskHandle);
    xTaskCreatePinnedToCore(my_task_2, "my_task_2", 4096, NULL, 10, &myTaskHandle2, 1);
}