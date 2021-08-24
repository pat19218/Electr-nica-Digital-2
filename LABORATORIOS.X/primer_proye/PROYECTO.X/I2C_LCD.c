/*******************************************************************************
* I2C_LCD.c                                                                      
* MPLAB XC8 compiler LCD driver for I2C LCDs with HD44780 compliant controllers. 
* https://simple-circuit.com/                                                    
*                                                                               
*******************************************************************************/
#pragma warning disable 520
#include <stdint.h>
#include "I2C_LCD.h"

#ifndef LCD_TYPE
   #define LCD_TYPE 2           // 0=5x7, 1=5x10, 2=2 lines
#endif

__bit RS;
uint8_t i2c_addr, backlight_val = LCD_BACKLIGHT;

/********************** I2C functions **************************/

void I2C_Init(uint32_t i2c_clk_freq)
{
  SSPCON  = 0x28;  // configure MSSP module to work in I2C mode
  SSPADD  = (_XTAL_FREQ /(4 * i2c_clk_freq)) - 1;  // set I2C clock frequency
  SSPSTAT = 0;
}

void I2C_Start()
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));  // wait for MSSP module to be free (not busy)
  SEN = 1;  // initiate start condition
}

void I2C_Stop()
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));  // wait for MSSP module to be free (not busy)
  PEN = 1;  // initiate stop condition
}

void I2C_Write(uint8_t i2c_data)
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));  // wait for MSSP module to be free (not busy)
  SSPBUF = i2c_data;  // update buffer
}

void I2C_Repeated_Start()
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));  // wait for MSSP module to be free (not busy)
  RSEN = 1;  // initiate repeated start condition
}

uint8_t I2C_Read(uint8_t ack)
{
  uint8_t _data;
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));  // wait for MSSP module to be free (not busy)
  RCEN = 1;
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));  // wait for MSSP module to be free (not busy)
  _data = SSPBUF;  // read data from buffer
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));  // wait for MSSP module to be free (not busy)
  // send acknowledge pulse ? (depends on ack, if 1 send, otherwise don't send)
  ACKDT = !ack;
  ACKEN = 1;
  return _data;  // return data read
}

/********************** end I2C functions **********************/

void Expander_Write(uint8_t value)
{
  I2C_Start();
  I2C_Write(i2c_addr);
  I2C_Write(value | backlight_val);
  I2C_Stop();
}

void LCD_Write_Nibble(uint8_t n)
{
  n |= RS;
  Expander_Write(n & 0xFB);
  __delay_us(1);
  Expander_Write(n | 0x04);
  __delay_us(1);
  Expander_Write(n & 0xFB);
  __delay_us(100);
}

void LCD_Cmd(uint8_t Command)
{
  RS = 0;
  LCD_Write_Nibble(Command & 0xF0);
  LCD_Write_Nibble((Command << 4) & 0xF0);
  if((Command == LCD_CLEAR) || (Command == LCD_RETURN_HOME))
    __delay_ms(2);
}

void LCD_Goto(uint8_t col, uint8_t row)
{
  switch(row)
  {
    case 2:
      LCD_Cmd(LCD_SECOND_ROW + col - 1);
      break;
    case 3:
      LCD_Cmd(LCD_THIRD_ROW  + col - 1);
      break;
    case 4:
      LCD_Cmd(LCD_FOURTH_ROW + col - 1);
    break;
    default:      // case 1:
      LCD_Cmd(LCD_FIRST_ROW  + col - 1);
  }

}

void LCD_PutC(char LCD_Char)
{
  RS = 1;
  LCD_Write_Nibble(LCD_Char & 0xF0);
  LCD_Write_Nibble((LCD_Char << 4) & 0xF0);
}

void LCD_Print(char* LCD_Str)
{
  uint8_t i = 0;
  RS = 1;
  while(LCD_Str[i] != '\0')
  {
    LCD_Write_Nibble(LCD_Str[i] & 0xF0);
    LCD_Write_Nibble( (LCD_Str[i++] << 4) & 0xF0 );
  }
}

void LCD_Begin(uint8_t *_i2c_addr)
{
  i2c_addr = _i2c_addr;
  Expander_Write(0);

  __delay_ms(40);
  LCD_Cmd(3);
  __delay_ms(5);
  LCD_Cmd(3);
  __delay_ms(5);
  LCD_Cmd(3);
  __delay_ms(5);
  LCD_Cmd(LCD_RETURN_HOME);
  __delay_ms(5);
  LCD_Cmd(0x20 | (LCD_TYPE << 2));
  __delay_ms(50);
  LCD_Cmd(LCD_TURN_ON);
  __delay_ms(50);
  LCD_Cmd(LCD_CLEAR);
  __delay_ms(50);
  LCD_Cmd(LCD_ENTRY_MODE_SET | LCD_RETURN_HOME);
  __delay_ms(50);
}

void Backlight() {
  backlight_val = LCD_BACKLIGHT;
  Expander_Write(0);
}

void noBacklight() {
  backlight_val = LCD_NOBACKLIGHT;
  Expander_Write(0);
}
