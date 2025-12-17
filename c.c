#include "constants.h"
#include "avr/io.h"
#include <stdio.h>
#include "avr/interrupt.h"
#include "UART.h"
#include "SPI.h"
#include <stdlib.h>
#include <math.h>

void ADC_init() {
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);

}

uint16_t ADC_conversion(uint8_t channel) {

    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1 << ADSC);
    while (!(ADCSRA & (1 << ADIF)));
    ADCSRA |= (1 << ADIF);
    return ADC;

}

UART* UART_ = NULL;
unsigned char current_level = 15;
volatile uint8_t isADC_conv = 0;

ISR(USART_RX_vect) {
    UART_->USART_Receive(UART_);
    uint8_t h = UART_->char_buffer - 48;
    UART_->USART_Receive(UART_);
    uint8_t d = UART_->char_buffer - 48;
    UART_->USART_Receive(UART_);
    uint8_t o = UART_->char_buffer - 48;;
    current_level = h * 100 + d * 10 + o;
}


int main() {
    SPI* spi = SPI_constructor();
    spi->SPI_init(spi);
    UART_ = uart_constructor();
    UART_->uart_init(UART_);
    UART_->INT_init(UART_);
    UART_->char_buffer = 15;
    unsigned char resistance = 1;
    ADC_init();
    char data[LEN_SIZE];
    while (1) {
        
        char* output = "CURRENT: %d\n\r";
        uint32_t ADC_meas = (uint32_t)ADC_conversion(6) * 5000 / 1024  / 100;
        uint8_t len = snprintf(data, LEN_SIZE, output, ADC_meas, (uint8_t)current_level);
        if(ADC_meas > current_level){
            unsigned char delta = 1; //pow(ADC_meas - current_level, 2);
            unsigned char new_resistance = resistance + delta;
            if (resistance > (255 - delta)){
                new_resistance = 255;
            }
            resistance = new_resistance;
        }
        else if(current_level > ADC_meas){
            unsigned char delta = 1; //pow(current_level - ADC_meas, 2);
            unsigned char new_resistance = resistance - delta;
            if(delta > resistance){
                new_resistance = 0;
            }
            resistance = new_resistance;
        }
        spi->transmit(spi, resistance);
        UART_->USART_Transmit_str(UART_, data, len);
    }
    free(UART_);
    free(spi);
    return 0;
}
