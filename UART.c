#include "UART.h"
#include "constants.h"
#include "avr/io.h"
#include "avr/interrupt.h"

void uart_init(struct UART *uart_item){
    UBRR0 = UBRR_val;
    UBRR0H = (uint8_t)(UBRR_val >> 8);
    UBRR0L = (uint8_t)(UBRR_val);
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    DDRD |= (1 << 1); // Enable TX
}

void INT_init(struct UART *uart_item){
    sei();
    UCSR0B |= (1 << RXCIE0);
}

void USART_Receive(struct UART *uart_item){
    while ( !(UCSR0A & (1<<RXC0)) )
    ;
    uart_item->char_buffer = UDR0;
}

void USART_Transmit(struct UART *uart_item, unsigned char data ){
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) )
    ;
    /* Put data into buffer, sends the data */
    UDR0 = uart_item->char_buffer;
}

void uart_constructor(struct UART *uart_item){
  uart_item->uart_init = uart_init;
  uart_item->INT_init = INT_init;
  uart_item = USART_Receive;
  uart_item = USART_Transmit;
}
