#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <windows.h>

static volatile uint32_t ulHighFrequencyTimerTicks = 0;

void vConfigureTimerForRunTimeStats(void)
{
}

uint32_t ulGetRunTimeCounterValue(void)
{
    LARGE_INTEGER liCounter, liFrequency;
    QueryPerformanceFrequency(&liFrequency);
    QueryPerformanceCounter(&liCounter);

    return (uint32_t)((liCounter.QuadPart * 1000) / liFrequency.QuadPart);
}

#define BUFFER_SIZE 1024

void vTaskFunction(void *pvParameters);
void vPrintString(const char *pcString);
void vPrintRunTimeStats(void);

static const char *pcTextForTask1 = "Task 1 is running\r\n";
static const char *pcTextForTask2 = "Task 2 is running\r\n";

int main(void)
{
    vConfigureTimerForRunTimeStats();

    xTaskCreate(vTaskFunction, "Task 1", 1000, (void*)pcTextForTask1, 1, NULL);
    xTaskCreate(vTaskFunction, "Task 2", 1000, (void*)pcTextForTask2, 2, NULL);

    vTaskStartScheduler();

    return 0;
}

void vTaskFunction(void *pvParameters)
{
    char *pcTaskName;
    TickType_t xLastWakeTime;

    pcTaskName = (char *)pvParameters;

    xLastWakeTime = xTaskGetTickCount();

    for(;;)
    {
        vPrintString(pcTaskName);
        vPrintRunTimeStats();
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

void vPrintRunTimeStats(void)
{
    static char cBuffer[BUFFER_SIZE];

    taskENTER_CRITICAL();
    {
        vTaskGetRunTimeStats(cBuffer);
        printf("%s\n", cBuffer);
        fflush(stdout);
    }
    taskEXIT_CRITICAL();
}
