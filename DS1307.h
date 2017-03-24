#include <Wire.h>
#include <Arduino.h>

#ifndef DS1307_h
#define DS1307_h

#define DS1307_I2C_ADDR 0x68

#define DS1307_SECONDS_REG 0
#define DS1307_HOURS_REG 2
#define DS1307_CONTROL_REG 7
#define DS1307_RAM 8

class DS1307
{
  public:
    void changeControl(boolean SQWE, byte rate);
    void changeClockHalt(boolean halt);
    void change24Hour(boolean hour24);
    
    void writeData(byte reg, byte data);
    void writeData(byte reg, byte *data, byte len);
    byte readData(byte reg);
    void readData(byte reg, byte *data, byte len);
    void readTime();
    void writeTime();
    void writeTimePart();
    void writeDatePart();

    byte getSeconds(boolean convFromBCD = true);
    void setSeconds(byte sec, boolean convToBCD = true);
    
    byte getMinutes(boolean convFromBCD = true);
    void setMinutes(byte min, boolean convToBCD = true);
    
    byte getHours(boolean convFromBCD = true);
    void setHours(byte hour, boolean convToBCD = true);
    
    byte getWeekday();
    void setWeekday(byte wd);
    
    byte getDay(boolean convFromBCD = true);
    void setDay(byte d, boolean convToBCD = true);

    byte getMonth(boolean convFromBCD = true);
    void setMonth(byte m, boolean convToBCD = true);
    
    byte getYear(boolean convFromBCD = true);
    void setYear(byte y, boolean convToBCD = true);

    void setTime(byte hr, byte mi, byte se, byte wd, byte y, byte m, byte d, boolean convToBCD = true);
    
  private:
    byte timeData[7];
};

#endif

