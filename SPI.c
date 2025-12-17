#include <avr/io.h>
#include <util/delay.h>
#include "SPI.h"
#include <stdlib.h>

void SPI_init(SPI *spi) {
    // Для чего PB0 ?
    DDRB |= (1 << PB0) | (1 << PB3) | (1 << PB5);
    // Зачем это?
    PORTB |= (1 << PB0);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_send_to_pot(struct SPI *spi, char val) {
    PORTB &= ~(1 << PB0);
    SPDR = 0x11;
    while (!(SPSR & (1 << SPIF)));
    SPDR = val;
    while (!(SPSR & (1 << SPIF)));
    PORTB |= (1 << PB0);
}

void SPI_MasterInit(void)
{
 /* Set MOSI and SCK output, all others input */
 DDRB |= (1 << PB3)|(1 << PB5)|(1 << PB0);
 /* Enable SPI, Master, set clock rate fck/16 */
 SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
void SPI_MasterTransmit(SPI* spi, char cData)
{
 /* Start transmission */
 SPDR = cData;
 /* Wait for transmission complete */
 while(!(SPSR & (1<<SPIF)))
 ;
}

SPI *SPI_constructor(){
    SPI* spi = malloc(sizeof(SPI));
    spi->SPI_init = SPI_init;
    spi->transmit = SPI_send_to_pot;
    return spi;
}