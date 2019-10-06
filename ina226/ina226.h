/*************************************************************/
/**  
 *  @file ina226.h
 *    
 *  @author Pingoin
 *  @date 06.10.2019 – erste Implementierung  
 *  
 *  @version 1   
 *************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//Lowest Significant Bit für die BUS-Spannung in mV
#define lsbBUS 1.25

//Lowest Significant Bit für die Shunt-Spannung in myV
#define lsbShunt 2.5

#include <Wire.h>


/**
 * @brief Durchschnittsberechnung
 * 
 * Die Anzahl der Messungen die vorgenommen werden und aus denen ein Durchschnitt ermittelt wird.
 */
typedef enum
{
    //1
    AVG1 = 0b000,
    //4
    AVG4 = 0b001,
    //16
    AVG16 = 0b010,
    //64
    AVG64 = 0b011,
    //128
    AVG128 = 0b100,
    //256
    AVG256 = 0b101,
    //512
    AVG512 = 0b110,
    //1024
    AVG1024 = 0b111

} avgSetting_t;

/**
 * @brief Messdauer
 * 
 * Dauer der Einzelmessung
 */
typedef enum
{
    //140 mys
    MYS140 = 0b000,
    //204 mys
    MYS204 = 0b001,
    //332 mys
    MYS332 =0b010,
    //588 mys
    MYS588 = 0b011,
    //1.1 ms
    MS11 = 0b100,
    //2.116 ms
    MS21 = 0b101,
    //4.156 ms
    MS42 = 0b110,
    //8.244 ms
    MS82 = 0b111

} convTime_t;

/**
 * @brief Modus
 * 
 * Betriebsmodus
 */
typedef enum
{
    //Power-Down (or Shutdown)
    PowDown = 0b000,
    //Shunt Voltage, Triggered
    ShuntTrig = 0b001,
    //Bus Voltage, Triggered
    BusTrig =0b010,
    //Shunt and Bus, Triggered
    BothTrig = 0b011,
    //Power-Down (or Shutdown)
    PowDown2 = 0b100,
    //Shunt Voltage, Continuous
    ShuntCont = 0b101,
    //Bus Voltage, Continuous
    BusCont = 0b110,
    //Shunt and Bus, Continuous
    BothCont = 0b111

} operatingMode_t;

/**
 * @brief Gesamtklasse des ina226 Sensors
 * 
 */
class ina226
{
public:
    /**
     * @brief Erstellt ein neues ina226 Objekt
     * 
     * Als Standardwerte werden die Werte aus dem datenblatt übernommen.
     * 
     * @param i2cAddr die per Schaltung einestellte I2C Adresse.
     */
    ina226(uint8_t i2cAddr);
    /**
     * @brief Leere Destroyer-Funktion
     * 
     */
    ~ina226();
    /**
     * @brief Startet die I2C Verbindung.
     * 
     * Wenn bereits ein Wire.begin() ausgeführt wurde nicht weiter nötig
     * 
     */
    void begin();
    /**
     * @brief Liefert zurück, on der Definierte Sensor vorhanden ist
     * 
     * @return true Sensor ist vorhanden
     * @return false Sensor ist NICHT vorhanden
     */
    bool conneted();

    float readBusVoltage();
    float readShuntVoltage();
    /**
     * @brief Enstellungen für den Shunt
     * 
     * @param current Nennstrom des Shunts in A
     * @param voltage Nennspannungsabfall in mV
     */
    void setShunt(float current,float voltage);
protected:
    /**
     * @brief Einstellung der Durchschnittsberechnung
     * 
     */
    uint8_t avgSetup;

    /**
     * @brief Messdauer der Busspannung
     *  
     */
    uint8_t busTime;

    /**
     * @brief Messdauer der Shuntspannung
     * 
     */
    uint8_t shuntTime;

    /**
     * @brief Arbeitsmodus
     * 
     */
    uint8_t operatingMode;

    /**
     * @brief Bus-Adresse
     * 
     */
    uint8_t i2cAddr;
    /**
     * @brief Wartezeit nach Beauftragung der Messung
     * 
     */
    uint8_t delayTime;
    /**
     * @brief Interne Funktion für die neuberechnung von Variablen nach anpassung anderer Variablen
     * 
     */
    void calcInternals();
};