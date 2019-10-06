#include "ina226.h"
ina226::ina226(uint8_t i2cAddr)
{
    this.avgSetup = AVG1;
    this.busTime = MS11;
    this.shuntTime = MS11;
    this.operatingMode = BothCont;
    this.i2cAddr = i2cAddr;
    this.lsbShuntCurrent = 0;
    this.calReg = 0;
    this.delayTime = 0;
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
    uint16_t factor;
    uint8_t busDelay;
    uint8_t shuntDelay;
    if (avgSetup <= 3)
    {
        factor = (1 << avgSetup * 2);
    }
    else
    {
        factor = (1 << avgSetup + 3);
    }

    if (busTime<MS11){
        busDelay=1;
    }else{
    busDelay= (1 << (busTime - 4)) +1;;
    }
    if (shuntTime<MS11){
        shuntDelay=1;
    }else{
        shuntDelay= (1 << (shuntTime - 4))+1;
    }
    delayTime = (busDelay+shuntDelay)*factor+1;
}

float ina226::readBusVoltage()
{
    forceMeasurement();
    unt16_t value = readRegister(0x2);
    return (float)value * lsbBUS * 1e-3;
};
float ina226::readShuntVoltage()
{
    forceMeasurement();
    int16_t value = readRegister(0x1);
    return (float)value * lsbShunt * 1e-6;
};

float ina226::readCurrent()
{
    forceMeasurement();
    int16_t value = readRegister(0x4);
    return (float)value * lsbShuntCurrent;
};

float ina226::readPower()
{
    forceMeasurement();
    int16_t value = readRegister(0x3);
    return (float)value * lsbShuntCurrent * 25;
};

void ina226::setShunt(float current, float voltage)
{
    float rShunt = (voltage / 1000) / current;
    float lsbTemp = current / 32768;
    calReg = 0.00512 / (lsbTemp * rShunt);
    lsbShuntCurrent = 0.00512 / (calReg * rShunt);
    uint8_t calReg1 = calReg >> 8;
    uint8_t calReg2 = calReg & 0xff;

    Wire.beginTransmission(i2cAddr);
    Wire.write(0x5);
    Wire.write(calReg1);
    Wire.write(calReg2);
    Wire.endTransmission();
}

void ina226::forceMeasurement()
{

    if (operatingMode <= BothTrig)
    {
        writeConfig();
        delay(delayTime);
    }
}

void ina226::writeConfig()
{

    uint16_t config = (avgSetup << 9) | (busTime << 6) | (shuntTime << 3) | (operatingMode);
    uint8_t config1 = config >> 8;
    uint8_t config2 = config & 0xff;

    Wire.beginTransmission(i2cAddr);
    Wire.write(0x0);
    Wire.write(config1);
    Wire.write(config2);
    Wire.endTransmission();
}

void ina226::setMode(operatingMode_t mode)
{
    this.operatingMode = mode;
    calcInternals();
    writeConfig();
}
void ina226::setAvgMode(avgSetting_t mode)
{
    this.avgSetup = mode;
    calcInternals();
    writeConfig();
}
void ina226::setShuntTime(convTime_t time)
{
    this.shuntTime = time;
    calcInternals();
    writeConfig();
}
void ina226::setBusTime(convTime_t time)
{
    this.busTime = time;
    calcInternals();
    writeConfig();
}

uint16_t ina226::readRegister(uint8_t register)
{
    Wire.beginTransmission(i2cAddr);
    Wire.write(register);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddr, (uint8_t)2);
    return ((Wire.read() << 8) | Wire.read());
}