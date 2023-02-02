//--- Includes ---//
#include "esp_log.h" // Log Printing
#include "freertos/FreeRTOS.h" // Main FreeRTOS Lib
#include "freertos/task.h" // FreeRTOS Task Lib

//--- Variables ---//
TaskHandle_t firstTaskHandle = NULL; // RTOS Task Handle
TaskHandle_t secondTaskHandle = NULL; // RTOS Task Handle
TaskHandle_t thirdTaskHandle = NULL; // RTOS Task Handle
const char* TAG = "RTOS"; // ESP_LOG TAG

//--- RTOS Tasks ---//
void first_task(void *arg)
{
    for(;;) // Infinite Loop
    {
        ESP_LOGI(TAG, "First task is running...");
        vTaskDelay(pdMS_TO_TICKS(1000)); // You can use "vTaskDelay(1000 / portTICK_RATE_MS);"
    }
    // Program should not be come here !
    vTaskDelete(firstTaskHandle); // Deleting the task
}

void second_task(void *arg)
{
    for(int i = 0; i < 10; i++)
    {
        ESP_LOGI(TAG, "Second task is running...");
        vTaskDelay(pdMS_TO_TICKS(500)); // You can use "vTaskDelay(500 / portTICK_RATE_MS);"
    }
    // Program will be deleted when (i == 10)
    ESP_LOGW(TAG, "Second task is deleted !");
    vTaskDelete(secondTaskHandle); // Deleting the task
}

void third_task(void *arg)
{
    for(int j = 0; j < 10; j++)
    {
        ESP_LOGI(TAG, "Third task is running at different core...");
        vTaskDelay(pdMS_TO_TICKS(250)); // You can use "vTaskDelay(250 / portTICK_RATE_MS);"
    }
    // Program will be deleted when (j == 10)
    ESP_LOGW(TAG, "Third task is deleted !");
    vTaskDelete(thirdTaskHandle); // Deleting the task
}

void app_main(void)
{
    xTaskCreate(first_task, "first_task", 1024*4, NULL, 1, &firstTaskHandle); // Creating first task
    xTaskCreate(second_task, "second_task", 1024*4, NULL, 6, &secondTaskHandle); // Creating second task
    xTaskCreatePinnedToCore(third_task, "third_task", 1024*4, NULL, 5, &thirdTaskHandle, 1); // Creating third task
}