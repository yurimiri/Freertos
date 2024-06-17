#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

void vTaskFunction(void *pvParameters);
void vPrintString(const char *pcString);
void vControlTask(void *pvParameters);

static const char *pcTextForTask1 = "Task 1 is running\r\n";
static const char *pcTextForTask2 = "Task 2 is running\r\n";

TaskHandle_t xTask1Handle = NULL;

int main(void)
{
    
    xTaskCreate(vTaskFunction, "Task 1", 1000, (void*)pcTextForTask1, 1, &xTask1Handle);
    xTaskCreate(vTaskFunction, "Task 2", 1000, (void*)pcTextForTask2, 2, NULL);
    xTaskCreate(vControlTask, "Control Task", 1000, NULL, 3, NULL);

    vTaskStartScheduler();

    return 0;
}

void vTaskFunction(void *pvParameters)
{
    char *pcTaskName;
    TickType_t xLastWakeTime;

    pcTaskName = (char *) pvParameters;

    xLastWakeTime = xTaskGetTickCount();

    for(;;)
    {
        vPrintString(pcTaskName);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(250));
    }
}

void vPrintString(const char *pcString)
{
    taskENTER_CRITICAL();
    {
        printf("%s\n", pcString);
        fflush(stdout);
    }
    taskEXIT_CRITICAL();
}

void vControlTask(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        vTaskSuspend(xTask1Handle);
        vPrintString("Task 1 suspended");

        vTaskDelay(pdMS_TO_TICKS(2000));

        vTaskResume(xTask1Handle);
        vPrintString("Task 1 resumed");

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
