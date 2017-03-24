#include <BCDUtil.h>
#include <Wire.h>
#include "DS1307.h"

// rate: 0 = 1KHz, 1 = 4.096kHz, 2 = 8.192kHz, 3 = 32.768kHz.
void DS1307::changeControl(boolean SQWE, byte rate)
{
  byte data;
  
  data = (SQWE?16:0) + (rate & 3);
  writeData(DS1307_CONTROL_REG, data); 
}

void DS1307::changeClockHalt(boolean halt)
{
  byte data = readData(DS1307_SECONDS_REG);
  byte upd;
  
  if (halt)
    upd = data | 128;
  else
    upd = data & 0x7F;
    
  if (data != upd)
    writeData(DS1307_SECONDS_REG, upd);
      
  timeData[0] = upd;
}

void DS1307::change24Hour(boolean h24)
{
  byte data = readData(DS1307_HOURS_REG);
  byte upd;
  
  if (!h24)
    upd = data | 0x40; // Set bit to enable 12 hour mode.
  else
    upd = data & ~0x40; // Clear bit to enable 24 hour mode.

  if (data != upd)
    writeData(DS1307_HOURS_REG, upd);
    
  timeData[2] = upd;
}

void DS1307::writeData(byte reg, byte data)
{
  Wire.beginTransmission(DS1307_I2C_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void DS1307::writeData(byte reg, byte *data, byte len)
{
  Wire.beginTransmission(DS1307_I2C_ADDR);
  Wire.write(reg);
  
  while (len > 0)
  {
    Wire.write(*data);
    ++data;
    --len;
  }
  
  Wire.endTransmission();
}

byte DS1307::readData(byte reg)
{
  byte data;
  
  Wire.beginTransmission(DS1307_I2C_ADDR);
  Wire.write(reg);
  Wire.endTransmission();
  
  Wire.requestFrom(DS1307_I2C_ADDR, 1);
  data = Wire.read();

  return data;
}

void DS1307::readData(byte reg, byte *data, byte len)
{
  int i = 0;
  Wire.beginTransmission(DS1307_I2C_ADDR);
  Wire.write(reg);
  Wire.endTransmission();
  
  Wire.requestFrom(DS1307_I2C_ADDR, (int)len);
  while (len > 0)
  {
    data[i] = Wire.read();
    ++i;
    --len;
  }
}

void DS1307::readTime()
{
  byte i;
  
  Wire.beginTransmission(DS1307_I2C_ADDR);
  Wire.write(DS1307_SECONDS_REG);
  Wire.endTransmission();
  
  Wire.requestFrom(DS1307_I2C_ADDR, 7);
  
  for (i = 0; i < 7; i++)
    timeData[i] = Wire.read();
}

void DS1307::writeTime()
{
  writeData(DS1307_SECONDS_REG, timeData, 7);
  
  /*byte i;
  
  Wire.beginTransmission(DS1307_I2C_ADDR);
  Wire.write(DS1307_SECONDS_REG);
  
  for (i = 0; i < 7; i++)
    Wire.write(timeData[i]);
  
  Wire.endTransmission();*/
}

void DS1307::writeTimePart()
{
  writeData(DS1307_SECONDS_REG, timeData, 3);
}

void DS1307::writeDatePart()
{
  writeData(DS1307_SECONDS_REG + 3, timeData + 3, 4);
}

void DS1307::setTime(byte hr, byte mi, byte se, byte wd, byte y, byte m, byte d, boolean conv)
{
  setSeconds(se, conv);
  setMinutes(mi, conv);
  setHours(hr, conv);
  setWeekday(wd);
  setYear(y, conv);
  setMonth(m, conv);
  setDay(d, conv);
}

byte DS1307::getSeconds(boolean conv)
{
  byte d = timeData[0] & 0x7F; // Remove clock halt flag.
  if (conv)
    d = bcdToByte(d);
    
  return d;
}

void DS1307::setSeconds(byte d, boolean conv)
{
  if (conv)
    d = byteToBCD(d);
    
  timeData[0] = (timeData[0] & 0x80) | d;
}

byte DS1307::getMinutes(boolean conv)
{
  byte d = timeData[1];
  if (conv)
    d = bcdToByte(d);

  return d;
}

void DS1307::setMinutes(byte d, boolean conv)
{
  if (conv)
    d = byteToBCD(d);
    
  timeData[1] = d;
}

byte DS1307::getHours(boolean conv)
{
  byte d = timeData[2];
  
  if ((d & 0x40) == 0) // 24 hour
    d = d & 0x3F;
  else
    d = d & 0x1F;
  
  if (conv)
    d = bcdToByte(d);
    
  return d;
}

void DS1307::setHours(byte d, boolean conv)
{
  if (conv)
    d = byteToBCD(d);
    
  if ((timeData[2] & 0x40) == 0) // 24 hour mode
    timeData[2] = (d & 0x3F);
  else
    timeData[2] = (timeData[2] & 0xE0) | (d & 0x1F);
}

byte DS1307::getWeekday()
{
  return timeData[3];
}

void DS1307::setWeekday(byte d)
{
  timeData[3] = d;
}

byte DS1307::getDay(boolean conv)
{
  byte d = timeData[4];
  if (conv)
    d = bcdToByte(d);
    
  return d;
}

void DS1307::setDay(byte d, boolean conv)
{
  if (conv)
    d = byteToBCD(d);
    
  timeData[4] = d;
}

byte DS1307::getMonth(boolean conv)
{
  byte d = timeData[5];
  if (conv)
    d = bcdToByte(d);
    
  return d;
}

void DS1307::setMonth(byte d, boolean conv)
{
  if (conv)
    d = byteToBCD(d);
    
  timeData[5] = d;
}

byte DS1307::getYear(boolean conv)
{
  byte d = timeData[6];
  if (conv)
    d = bcdToByte(d);
    
  return d;
}

void DS1307::setYear(byte d, boolean conv)
{
  if (conv)
    d = byteToBCD(d);
    
  timeData[6] = d;
}

