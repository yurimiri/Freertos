#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

void vTaskFunction1(void *pvParameters);
void vTaskFunction2(void *pvParameters);
void vTaskFunction3(void *pvParameters);
void vTaskFunction4(void *pvParameters);
void vControllerTask(void *pvParameters);
void vPrintString(const char *pcString);

static TaskHandle_t xTask1Handle = NULL;
static TaskHandle_t xTask2Handle = NULL;
static TaskHandle_t xTask3Handle = NULL;
static TaskHandle_t xTask4Handle = NULL;

int main(void)
{
    xTaskCreate(vTaskFunction1, "Task 1", 1000, NULL, 1, &xTask1Handle);
    xTaskCreate(vTaskFunction2, "Task 2", 1000, NULL, 1, &xTask2Handle);
    xTaskCreate(vTaskFunction3, "Task 3", 1000, NULL, 1, &xTask3Handle);
    xTaskCreate(vTaskFunction4, "Task 4", 1000, NULL, 1, &xTask4Handle);
    xTaskCreate(vControllerTask, "Controller", 1000, NULL, 2, NULL);

    vTaskStartScheduler();

    return 0;
}

void vTaskFunction1(void *pvParameters)
{
    uint32_t ulNotificationValue;

    for (;;)
    {
        xTaskNotifyWait(0, 0, &ulNotificationValue, portMAX_DELAY);
        printf("Task 1 notified with value: %lu\n", ulNotificationValue);
    }
}

void vTaskFunction2(void *pvParameters)
{
    uint32_t ulNotificationValue;

    for (;;)
    {
        xTaskNotifyWait(0, 0, &ulNotificationValue, portMAX_DELAY);
        printf("Task 2 notified with value: %lu\n", ulNotificationValue);
    }
}

void vTaskFunction3(void *pvParameters)
{
    uint32_t ulNotificationValue;

    for (;;)
    {
        xTaskNotifyWait(0, 0, &ulNotificationValue, portMAX_DELAY);
        printf("Task 3 notified with value: %lu\n", ulNotificationValue);
    }
}

void vTaskFunction4(void *pvParameters)
{
    uint32_t ulNotificationValue;

    for (;;)
    {
        xTaskNotifyWait(0, 0, &ulNotificationValue, portMAX_DELAY);
        printf("Task 4 notified with value: %lu\n", ulNotificationValue);
    }
}

void vControllerTask(void *pvParameters)
{
    uint32_t ulPreviousValue;
    uint32_t ulTask1Value = 1UL << 8UL; 
    uint32_t ulTask2Value = 0;
    uint32_t ulTask3Value = 0x50;
    uint32_t ulTask4Value = 0xfff;

    for (;;)
    {
        xTaskNotifyAndQuery(xTask1Handle, ulTask1Value, eSetBits, NULL);
        ulTask1Value <<= 1;
        if (ulTask1Value == 0) ulTask1Value = 1UL << 8UL; 

        xTaskNotifyAndQuery(xTask2Handle, 0, eNoAction, &ulPreviousValue);
        printf("Task 2 previous notification value: %lu\n", ulPreviousValue);
        ulTask2Value++; 

        xTaskNotifyAndQuery(xTask3Handle, ulTask3Value, eSetValueWithOverwrite, &ulPreviousValue);
        printf("Task 3 previous notification value: %lu\n", ulPreviousValue);
        ulTask3Value += 0x10; 

        if (xTaskNotifyAndQuery(xTask4Handle, ulTask4Value, eSetValueWithoutOverwrite, &ulPreviousValue) == pdPASS)
        {
            printf("Task 4 notification value updated to %lu\n", ulTask4Value);
        }
        else
        {
            printf("Task 4 notification value not updated, previous value: %lu\n", ulPreviousValue);
        }
        ulTask4Value -= 0x100; 

        vTaskDelay(pdMS_TO_TICKS(1000));
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
