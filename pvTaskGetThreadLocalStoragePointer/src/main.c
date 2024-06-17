#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

void vTaskFunction(void *pvParameters);

void vPrintTaskName(const char *pcTaskName);

int main(void)
{
    xTaskCreate(vTaskFunction, "Task 1", 1000, (void*)"Task 1", 1, NULL);
    xTaskCreate(vTaskFunction, "Task 2", 1000, (void*)"Task 2", 2, NULL);

    vTaskStartScheduler();

    return 0;
}

void vTaskFunction(void *pvParameters)
{
    const char *pcTaskName = (const char *)pvParameters;

    uint32_t ulValueToStore = (uint32_t)pcTaskName;
    vTaskSetThreadLocalStoragePointer(NULL, 5, (void *)ulValueToStore);

    vPrintTaskName(pcTaskName);

    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

void vPrintTaskName(const char *pcTaskName)
{
    taskENTER_CRITICAL();
    
    printf("%s - ", pcTaskName);
    
    uint32_t ulStoredValue = (uint32_t)pvTaskGetThreadLocalStoragePointer(NULL, 5);
    
    printf("Thread Local Storage Value: %lu\n", ulStoredValue);
    
    fflush(stdout);
    
    taskEXIT_CRITICAL();
}
