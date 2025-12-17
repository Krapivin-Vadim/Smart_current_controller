#pragma once

typedef struct SPI
{
  char buffer;
  void (*SPI_init)(struct SPI *);
  void (*transmit)(struct SPI *, char data);
} SPI;

SPI *SPI_constructor();