#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_at24c02.h"
#include "bsp_i2c.h"
#include <string.h>
#include <stdlib.h>

extern u8 USART_RX_DATA[];
extern u8 USART_TI;
extern u8 i;

int j;
int main(void)
{
        char *p;
        int address=0;
        int data=0;
        char *cmd=NULL;
        uint8_t writebuff[256];
        uint8_t readbuff[256];
        systick_init();
        usart_init();
        at24c02_init();
        led_init();
        while (1)
        {      
                if(USART_TI)
                {
                        delay_ms(100);
                        p = strtok((char*)USART_RX_DATA, " ");
                        if(p)
                        {
                                cmd = p;
                                printf("[%s][%d] cmd = %s \r\n", __FUNCTION__, __LINE__, cmd);
                        }
                        p = strtok(NULL, " ");
                        if(p)
                        {
                                address = atoi(p);
                                printf("[%s][%d] address = %d \r\n", __FUNCTION__, __LINE__, address);
                        }
                        p = strtok(NULL, " ");
                        if(p)
                        {
                                data = atoi(p);
                                printf("[%s][%d] data = %d \r\n", __FUNCTION__, __LINE__, data);
                        }
                        if(!strcmp("write", cmd)) 
                        {
                               writebuff[0]=data;
                               at24c02_byte_write(address,writebuff);
                               printf("write  succeed");
                        }
                        if(!strcmp("read", cmd))
                        {
                                at24c02_random_read(address,readbuff );
                                printf("the %d address's data =  %d \r\n", address, readbuff[0]);
                        }   
                        delay_ms(10);
                        i=0;
                        USART_TI=0;
                        for(j=0;j<256;j++)
                        {
                                USART_RX_DATA[j]=' ';
                        }
                }
        }
}
