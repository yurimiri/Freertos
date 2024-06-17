#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>

void vTaskFunction(void *pvParameters);
void vPrintString(const char *pcString);
void vAnExampleISR(void);

static const char *pcTextForTask1 = "Task 1 is running";
static const char *pcTextForTask2 = "Task 2 is running";
static const char *pcISRTriggered = "ISR triggered, Task 1 resumed";

TaskHandle_t xHandle;

int main(void)
{
    xTaskCreate(vTaskFunction, "Task 1", 1000, (void *)pcTextForTask1, 1, &xHandle);
    xTaskCreate(vTaskFunction, "Task 2", 1000, (void *)pcTextForTask2, 2, NULL);

    vTaskStartScheduler();

    for (;;);

    return 0;
}

void vTaskFunction(void *pvParameters)
{
    char *pcTaskName;
    TickType_t xLastWakeTime;
    static int count = 0;

    pcTaskName = (char *)pvParameters;
    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        vPrintString(pcTaskName);

        if (strcmp(pcTaskName, pcTextForTask1) == 0)
        {
            vTaskSuspend(NULL);
        }
        else
        {
            vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(250));

            count++;
            if (count == 4) 
            {
                vAnExampleISR();
                count = 0;
            }
        }
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

void vAnExampleISR(void)
{
    BaseType_t xYieldRequired;

    vPrintString(pcISRTriggered);

    xYieldRequired = xTaskResumeFromISR(xHandle);

    if (xYieldRequired == pdTRUE)
    {
        portYIELD_FROM_ISR(xYieldRequired);
    }
}
