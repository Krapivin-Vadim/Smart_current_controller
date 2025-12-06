typedef struct Bad_string
{
  /* data */
};

typedef struct UART
{
  Bad_string str_buffer;
  unsigned char char_buffer;
  void (*uart_init)(struct UART *); // "Метод" инициализации UART
  void (*INT_init)(struct UART *);  // "Метод" разрешения прерывания по UART
  void (*USART_Receive)(struct UART *);
  void (*USART_Transmit)(struct UART *, unsigned char data);
  void (*constructor)(struct UART *);
};

void uart_constructor(struct UART *uart_item);