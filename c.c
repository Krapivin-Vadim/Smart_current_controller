#include "avr/io.h"
#include <stdio.h>
#include "avr/interrupt.h"
#include "util/delay.h"
#include "constants.h"

// 1. Инициализация UART
// 2. Реализация функции передачи символа
// 3. * Реализация передачи строк
// 4. * Реализация приёма символа


void uart_init() {

    // UBRR0 = UBRR_val;
    UBRR0H = (uint8_t)(UBRR_val >> 8);
    UBRR0L = (uint8_t)(UBRR_val);
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

    DDRD |= (1 << 1); // Enable TX

}

void uart_tx (char* data) {

    while (!(UCSR0A & (1 << UDRE0))); // Бесконечный цикл пока в UDRE0 гаходитс 
    UDR0 = *data;

}

void uart_tx_str(char* data, uint8_t len) {
    
    uint8_t i = 0;
    for (i = 0; i < len; ++i) uart_tx(data + i);

}

// void uart_rx_str(char* buffer, uint8_t len){
//     uint8_t i = 0;
//     for(i = 0; i < len; ++i){
//         USART_Receive(buffer[i]);
//     }
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

void INT_init() {

    sei();
    DDRD &= ~(1 << 2); // -----0--
    PORTD |= (1 << 2); // -----1--
    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);
    UCSR0B |= (1 << RXCIE0);

}

volatile uint8_t isADC_conv = 0;

// void USART_Receive(char* buffer) {
//  /* Wait for data to be received */
//  while ( !(UCSR0A & (1<<RXC0)) )
//  ;
//  /* Get and return received data from buffer */
//  buffer = UDR0;
// }

unsigned char USART_Receive( void )
{
 /* Wait for data to be received */
 while ( !(UCSR0A & (1<<RXC0)) )
 ;
 /* Get and return received data from buffer */
 return UDR0;
}


void USART_Transmit( unsigned char data )
{
 /* Wait for empty transmit buffer */
 while ( !( UCSR0A & (1<<UDRE0)) )
 ;
 /* Put data into buffer, sends the data */
 UDR0 = data;
}

uint8_t FROM_UART;

ISR(USART_RX_vect) {
    FROM_UART = USART_Receive();
    USART_Transmit(FROM_UART);
}



void main() {
    INT_init();
    uart_init();
    ADC_init();
    USART_Transmit('a');
    while (1) {
        _delay_ms(100);
    }
}
