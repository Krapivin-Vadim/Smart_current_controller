#include "constants.h"
#include "avr/io.h"
#include <stdio.h>
#include "avr/interrupt.h"
#include "util/delay.h"
#include "UART.h"
#include <stdlib.h>


// void INT_init() {

//     sei();
//     DDRD &= ~(1 << 2); // -----0--
//     PORTD |= (1 << 2); // -----1--
//     EICRA |= (1 << ISC01);
//     EIMSK |= (1 << INT0);
//     UCSR0B |= (1 << RXCIE0);

// }
UART* UART_ = NULL;
volatile uint8_t isADC_conv = 0;

uint8_t FROM_UART;

ISR(USART_RX_vect) {
    // FROM_UART = USART_Receive();
    // USART_Transmit(FROM_UART);
    UART_->USART_Receive(UART_);
    UART_->USART_Transmit(UART_, UART_->char_buffer);
    UART_->USART_Transmit(UART_, '\n');
    UART_->USART_Transmit(UART_, '\r');
}



int main() {
    UART_ = uart_constructor();
    UART_->uart_init(UART_);
    UART_->INT_init(UART_);
    while (1) {
        UART_->USART_Transmit(UART_, '\n');
        UART_->USART_Transmit(UART_, '\r');
        _delay_ms(100);
    }
    free(UART_);
    return 0;
}
