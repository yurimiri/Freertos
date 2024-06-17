#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#define MAX_TIME_TO_WAIT pdMS_TO_TICKS(200)
#define UART_BUFFER_SIZE 128

size_t UART_bytes_in_rx_buffer(void *pxUARTInstance);
size_t UART_read_from_receive_buffer(void *pxUARTInstance, uint8_t *pucBuffer, size_t uxWantedBytes);
void UART_receive_interrupt_simulator(void *pvParameters);

size_t xUART_Receive(uint8_t *pucBuffer, size_t uxWantedBytes);

void vUARTTask(void *pvParameters);

void main()
{
    xTaskCreate(vUARTTask, "UART Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    TimerHandle_t xTimer = xTimerCreate("UART Timer", pdMS_TO_TICKS(200), pdTRUE, NULL, UART_receive_interrupt_simulator);
    xTimerStart(xTimer, 0);

    vTaskStartScheduler();
}

void vUARTTask(void *pvParameters)
{
    uint8_t pucBuffer[UART_BUFFER_SIZE];
    size_t uxWantedBytes = 50; 
    size_t uxReceivedBytes;

    for (;;)
    {
        uxReceivedBytes = xUART_Receive(pucBuffer, uxWantedBytes);
        if (uxReceivedBytes > 0)
        {
            printf("Received %d bytes: ", uxReceivedBytes);
            for (size_t i = 0; i < uxReceivedBytes; i++)
            {
                printf("%02X ", pucBuffer[i]);
            }
            printf("\n");
        }
        else
        {
            printf("Receive timed out\n");
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

size_t xUART_Receive(uint8_t *pucBuffer, size_t uxWantedBytes)
{
    size_t uxReceived = 0;
    TickType_t xTicksToWait = MAX_TIME_TO_WAIT;
    TimeOut_t xTimeOut;
    void *pxUARTInstance = NULL; 
    vTaskSetTimeOutState(&xTimeOut);

    while (UART_bytes_in_rx_buffer(pxUARTInstance) < uxWantedBytes)
    {
        if (xTaskCheckForTimeOut(&xTimeOut, &xTicksToWait) != pdFALSE)
        {
            break;
        }

        ulTaskNotifyTake(pdTRUE, xTicksToWait);
    }

    uxReceived = UART_read_from_receive_buffer(pxUARTInstance, pucBuffer, uxWantedBytes);

    return uxReceived;
}

size_t UART_bytes_in_rx_buffer(void *pxUARTInstance)
{
    return rand() % 10;
}

size_t UART_read_from_receive_buffer(void *pxUARTInstance, uint8_t *pucBuffer, size_t uxWantedBytes)
{
    size_t uxBytesAvailable = UART_bytes_in_rx_buffer(pxUARTInstance);
    size_t uxBytesToRead = (uxBytesAvailable < uxWantedBytes) ? uxBytesAvailable : uxWantedBytes;

    for (size_t i = 0; i < uxBytesToRead; i++)
    {
        pucBuffer[i] = rand() % 256;
    }

    return uxBytesToRead;
}

void UART_receive_interrupt_simulator(void *pvParameters)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(xTaskGetHandle("UART Task"), &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
