#include "avr/io.h"
#include <stdio.h>
#include "avr/interrupt.h"
#include "util/delay.h"
#include "constants.h"
#include "UART.h"


void INT_init() {

    sei();
    DDRD &= ~(1 << 2); // -----0--
    PORTD |= (1 << 2); // -----1--
    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);
    UCSR0B |= (1 << RXCIE0);

}

volatile uint8_t isADC_conv = 0;

uint8_t FROM_UART;

// ISR(USART_RX_vect) {
//     FROM_UART = USART_Receive();
//     USART_Transmit(FROM_UART);
// }



void main() {
    UART uart = malloc(sizeof(UART));
    uart->constructor = uart_constructor;
    uart->constructor();
    while (1) {
        uart->USART_Transmit('>');
        uart->USART_Transmit('\n');
        _delay_ms(100);
    }
    free(uart);
}
