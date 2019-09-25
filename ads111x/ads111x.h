#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

typedef enum
{
    FSR6144 = 0b000,
    FSR4096 = 0b001,
    FSR2048 = 0b010,
    FSR1024 = 0b011,
    FSR0512 = 0b100,
    FSR0256 = 0b101
} adsGain_t;

typedef enum
{
    A0GND = 0b100,
    A1GND = 0b101,
    A2GND = 0b110,
    A3GND = 0b111,
    A0A1 = 0b000,
    A0A3 = 0b001,
    A1A3 = 0b010,
    A2A3 = 0b011
} adsChan_t;
typedef enum
{
    SPS8 = 0b000,
    SPS16 = 0b001,
    SPS32 = 0b010,
    SPS64 = 0b011,
    SPS128 = 0b100,
    SPS250 = 0b101,
    SPS475 = 0b110,
    SPS860 = 0b111
} adsSPS_t;

/**
 * @brief 
 * 
 */
class ADS1115
{
public:
    /**
     * @brief Construct a new ads object
     * 
     */
    ADS1115(uint8_t i2cAddr);
    /**
     * @brief 
     * 
     */
    ~ADS1115();
    /**
     * @brief 
     * 
     */
    void begin();
    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    bool conneted();
    /**
     * @brief Set the Gain object
     * 
     * @param gain 
     */
    void setGain(adsGain_t gain);
    /**
     * @brief Set the Data Rate object
     * 
     * @param dataRate 
     */
    void setDataRate(adsSPS_t dataRate);
    /**
     * @brief 
     * 
     * @param channel 
     * @return float 
     */
    float readVoltage(adsChan_t channel);
    /**
     * @brief Set the Voltage Divider object
     * 
     * @param r1 wiederstand zwischen zi Messender Spannung und messpin
     * @param r2 Widerstand zwischen Messpin und Ground
     */
    void setVoltageDivider(float r1,float r2);

protected:
    uint8_t confOS;
    uint8_t confMUX;
    uint8_t confPGA;
    uint8_t confMODE;
    uint8_t confDR;
    uint8_t confCompMode;
    uint8_t confCompPol;
    uint8_t confCompLat;
    uint8_t confCompQue;
    uint8_t i_i2cAddr;
    uint8_t delayTime;
    float i_lsb;
    void calcInternals();
    float voltageDivisonFactor;
};