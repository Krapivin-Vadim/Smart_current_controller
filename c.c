#include "constants.h"
#include "avr/io.h"
#include <stdio.h>
#include "avr/interrupt.h"
#include "util/delay.h"
#include "UART.h"
#include "SPI.h"
#include <stdlib.h>


// void INT_init() {

//     sei();
//     DDRD &= ~(1 << 2); // -----0--
//     PORTD |= (1 << 2); // -----1--
//     EICRA |= (1 << ISC01);
//     EIMSK |= (1 << INT0);
//     UCSR0B |= (1 << RXCIE0);

// }

void ADC_init() {
    // ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);

}

uint16_t ADC_conversion(uint8_t channel) {

    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1 << ADSC);
    while (!(ADCSRA & (1 << ADIF))); // пока в ADIF 0 (или 1, я хз) ждём прерывание
    ADCSRA |= (1 << ADIF);
    return ADC;

}

UART* UART_ = NULL;
volatile uint8_t isADC_conv = 0;

uint8_t FROM_UART;

ISR(USART_RX_vect) {
    // FROM_UART = USART_Receive();
    // USART_Transmit(FROM_UART);
    UART_->USART_Receive(UART_);
    UART_->USART_Transmit(UART_, '>');
    UART_->USART_Transmit(UART_, UART_->char_buffer);
    UART_->USART_Transmit(UART_, '\n');
    UART_->USART_Transmit(UART_, '\r');
}



int main() {
    SPI* spi = SPI_constructor();
    spi->SPI_init(spi);
    UART_ = uart_constructor();
    UART_->uart_init(UART_);
    UART_->INT_init(UART_);
    UART_->char_buffer = (char) 0;
    ADC_init();
    char data[LEN_SIZE];
    uint8_t i = 0;
    while (1) {
        spi->transmit(spi, (char)UART_->char_buffer);
        char* output = "CURRENT: %d.%03d\n\r";
        uint32_t ADC_meas = (uint32_t)ADC_conversion(6) * 5000 / 1024  / 100;
        // ADC_meas = ADC_meas / 750;
        uint16_t ADC_calc = ADC_meas / 1000;
        uint16_t ADC_dec = ADC_meas % 1000;
        uint8_t len = snprintf(data, LEN_SIZE, output, ADC_meas, 0);
        UART_->USART_Transmit_str(UART_, data, len);
        // uint8_t len = snprintf(data, LEN_SIZE, output, ADC_calc, ADC_dec);
        // UART_->USART_Transmit_str(UART_, data, len);
        ++i;
        _delay_ms(100);
        _delay_ms(100);
    }
    free(UART_);
    free(spi);
    return 0;
}
