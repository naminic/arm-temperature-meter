





//void lcd_pin(int port,int ain,int bin,int cin,int din,int gin,int fin);
void lcd_init(void);
void lcd_putchar(char ch);
void lcd_command(char cmd);
void lcd_gotoxy(char x, char y);
void lcd_clear(void);
void lcd_clear_line(char y);
void lcd_shift_left(char n);
void lcd_shift_right(char n);
void lcd_puts(int num);
void lcd_putsf(char* data);