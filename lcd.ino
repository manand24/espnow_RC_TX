#include <Wire.h>

#define LCD_LCD03_ADDRESS 0x63
// *********************
// I2C LCD03 primitives
// *********************
void i2c_LCD03_init () 
{
  Wire.begin();
  Wire.setClock(400000);

  Wire.beginTransmission(LCD_LCD03_ADDRESS); // The LCD03 is located on the I2C bus at address 0xC6
  Wire.write(0x00);// Command register
  Wire.write(04);// Hide cursor
  Wire.write(12);// Clear screen
  Wire.write(19);// Backlight on
  Wire.endTransmission();
}

void i2c_LCD03_send_cmd (byte c) 
{
  Wire.beginTransmission(LCD_LCD03_ADDRESS); // The LCD03 is located on the I2C bus at address 0xC6
  Wire.write(0x00);
  Wire.write(c);
  Wire.endTransmission();
}

void i2c_LCD03_send_char (char c) 
{
  Wire.beginTransmission(LCD_LCD03_ADDRESS); // The LCD03 is located on the I2C bus at address 0xC6
  Wire.write(0x00);
  Wire.write(c);
  Wire.endTransmission();
}

void i2c_LCD03_set_cursor (byte col, byte row) 
{
  //row = min(row,1);
  //col = min(col,15);
  i2c_LCD03_send_cmd(03); // set cursor (row, column)
  i2c_LCD03_send_cmd(row+1);
  i2c_LCD03_send_cmd(col+1);
}

