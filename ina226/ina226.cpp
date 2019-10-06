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
    if (avgSetup<=3){
        factor= (1<< avgSetup*2);
    }else{
        factor= (1<< avgSetup+3);
    }
    delayTime=((1<<(busTime-4))+(1<<(shuntTime-4)))*factor;
}

float ina226::readBusVoltage()
{
    forceMeasurement();
    Wire.beginTransmission(i2cAddr);
    Wire.write(0x2);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddr, (uint8_t)2);
    unt16_t value = ((Wire.read() << 8) | Wire.read());
    return (float)value * lsbBUS * 1e-3;
};
float ina226::readShuntVoltage()
{
    forceMeasurement();
    Wire.beginTransmission(i2cAddr);
    Wire.write(0x1);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddr, (uint8_t)2);
    int16_t value = ((Wire.read() << 8) | Wire.read());
    return (float)value * lsbShunt * 1e-6;
};

float ina226::readCurrent()
{
    forceMeasurement();
    Wire.beginTransmission(i2cAddr);
    Wire.write(0x4);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddr, (uint8_t)2);
    int16_t value = ((Wire.read() << 8) | Wire.read());
    return (float)value * lsbShuntCurrent;
};

float ina226::readPower()
{
    forceMeasurement();
    Wire.beginTransmission(i2cAddr);
    Wire.write(0x3);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddr, (uint8_t)2);
    int16_t value = ((Wire.read() << 8) | Wire.read());
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

    Wire.beginTransmission(i_i2cAddr);
    Wire.write(0x1);
    Wire.write(config1);
    Wire.write(config2);
    Wire.endTransmission();
}

    void setMode(operatingMode_t mode){
        this.operatingMode=mode;
        calcInternals();
        writeConfig();
    }
    void setAvgMode(avgSetting_t mode){
        this.avgSetup=mode;
        calcInternals();
        writeConfig();
    }
    void setShuntTime(convTime_t time){
        this.shuntTime=time;
        calcInternals();
        writeConfig();
    }
    void setBusTime(convTime_t time){
        this.busTime=time;
        calcInternals();
        writeConfig();
    }