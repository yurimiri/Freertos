#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#define STACK_SIZE 200
#define PRIORITY 2

void vTaskFunction(void *pvParameters);
void vAnotherFunction(void *pvParameters);
void vPrintString(const char *pcString);

static const char *pcTextForTask1 = "Task 1 is running\r\n";
static const char *pcTextForTask2 = "Task 2 is running\r\n";
static const char *pcTextForDemoTask = "Demo Task is running\r\n";
static const char *pcTaskDeleted = "Demo Task has been deleted\r\n";

int main(void)
{
    xTaskCreate(vTaskFunction, "Task 1", STACK_SIZE, (void*)pcTextForTask1, 1, NULL);
    xTaskCreate(vAnotherFunction, "Task 2", STACK_SIZE, (void*)pcTextForTask2, 2, NULL);
    
    vTaskStartScheduler();

    return 0;
}

void vAnotherFunction(void *pvParameters)
{
    TaskHandle_t xHandle;

    vPrintString((char *)pvParameters);

    if (xTaskCreate(vTaskFunction, "Demo task", STACK_SIZE, (void*)pcTextForDemoTask, PRIORITY, &xHandle) != pdPASS)
    {
    }
    else
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        
        vTaskDelete(xHandle);

        vPrintString(pcTaskDeleted);
    }

    vTaskDelete(NULL);
}

void vTaskFunction(void *pvParameters)
{
    char *pcTaskName;
    TickType_t xLastWakeTime;

    pcTaskName = (char *)pvParameters;
    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        vPrintString(pcTaskName);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(250));
    }
}

void vPrintString(const char *pcString)
{
    taskENTER_CRITICAL();
    {
        printf("%s", pcString);
        fflush(stdout);
    }
    taskEXIT_CRITICAL();
}
