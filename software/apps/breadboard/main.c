#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "microbit_v2.h"
#include "nrfx_uarte.h"

// Pin configurations
#define UART_RX              EDGE_P1
#define UART_TX              EDGE_P2
#define UART_TX_BUF_SIZE     1024
#define UART_RX_BUF_SIZE     1024
#define UART_BUF_SIZE     1024


nrfx_uarte_t g_Uarte = NRFX_UARTE_INSTANCE(1);
nrfx_uarte_config_t g_UarteCfg = NRFX_UARTE_DEFAULT_CONFIG;


// error handler for UART
void uart_error_handle (uint32_t p_event) {
  switch(p_event) {
      case NRFX_ERROR_BUSY :
         printf("NRFX_ERROR_BUSY\r\n" );
         break;
      case NRFX_ERROR_FORBIDDEN :
         printf("NRFX_ERROR_FORBIDDEN\r\n" );
         break;
      case NRFX_ERROR_INVALID_ADDR :
         printf("NRFX_ERROR_INVALID_ADDR\r\n" );
         break;
      case NRFX_ERROR_INTERNAL :
         printf("NRFX_ERROR_INTERNAL\r\n" );
         break;
      default :
         printf("NRF_SUCCESS\r\n" );
   }
}


// initialization of UART
void uart_init(void) {
  g_UarteCfg.pselrxd = UART_RX;
  g_UarteCfg.pseltxd = UART_TX;
  g_UarteCfg.hwfc = NRF_UARTE_HWFC_DISABLED;
  g_UarteCfg.baudrate = NRF_UARTE_BAUDRATE_115200;
  g_UarteCfg.interrupt_priority = 1;

  uint32_t err_code = nrfx_uarte_init(&g_Uarte, &g_UarteCfg, NULL);   // BLOCKING MODE

  if (err_code != NRF_SUCCESS) {
      printf("Error %lx\r\n", err_code);
  } else {
      printf("UARTE INIT NRF_SUCCESS\r\n");
  }
}


void uart_putstring(const char *s)
{
    uint8_t len = strlen((char *) s);
    uint32_t ret_code = nrfx_uarte_tx(&g_Uarte, &s, len);
    if (ret_code== NRF_SUCCESS)
    {
      printf("Message Sent succeed: %s", s);
    } else {
      printf("Message Sent failed: ");
      uart_error_handle(ret_code);
    }
}

void uart_getstring(void){
  uint8_t data = 0;
  while(nrfx_uarte_rx_ready(&g_Uarte)){
     printf("Receive Data from RX...\r\n");
    if (nrfx_uarte_rx(&g_Uarte, &data, 1) == NRF_SUCCESS)
    {
      printf("%c", data);
    } else {
      printf("Error in nrfx_uarte_rx\r\n");
    }
  }
  printf("Data Receive Ends\r\n");
}

void sendAT(char* command){
  uart_putstring(command);
  nrf_delay_ms(1000);
  uart_getstring();
}


int main(void) {
  printf("\r\n\r\n**** Board Start ****\r\n");
  // init uart
  char test[] = "AT\r\n";
  uart_init();
  nrf_delay_ms(500);

  //uart_putstring("AT\r\n");
  uint8_t len = strlen((char *) test);
  uint32_t ret_code = nrfx_uarte_tx(&g_Uarte, &test, len);
  if (ret_code== NRF_SUCCESS)
  {
    printf("Message Sent succeed: %s", test);
  } else {
    printf("Message Sent failed: ");
    uart_error_handle(ret_code);
  }

  nrf_delay_ms(500);
  uart_getstring();
  while (1) {
    nrf_delay_ms(500);
  }
}
