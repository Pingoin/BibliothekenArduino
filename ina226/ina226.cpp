#include "ina226.h"
ina226::ina226(uint8_t i2cAddr)
{
    avgSetup = AVG1;
    busTime = MS11;
    shuntTime = MS11;
    operatingMode = BothCont;
    i2cAddr = i2cAddr;
    calcInternals();
}
ina226::~ina226()
{
}
void ina226::begin()
{
    Wire.begin();
};
bool ina226::conneted()
{
    Wire.beginTransmission(i2cAddr);
    uint8_t error = Wire.endTransmission();
    if (error == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ina226::calcInternals()
{
}

float ina226::readBusVoltage(){
    Wire.beginTransmission(i2cAddr);
    Wire.write(0x2);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddr, (uint8_t) 2);
    unt16_t value = ((Wire.read() << 8) | Wire.read());
    return (float) value * lsbBUS*1e-3;
};
float ina226::readShuntVoltage(){
    Wire.beginTransmission(i2cAddr);
    Wire.write(0x1);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddr, (uint8_t) 2);
    int16_t value = ((Wire.read() << 8) | Wire.read());
    return (float) value * lsbBUS*1e-6;
};