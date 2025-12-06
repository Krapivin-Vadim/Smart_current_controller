#pragma once
#define BAUD_rate 19200
#define UBRR_val F_CPU / 16 / BAUD_rate - 1
#define LEN_SIZE 255
#define F_CPU 8000000UL