#ifndef sonde_temperature_h
#define sonde_temperature_h
#include <OneWire.h>
#include <DallasTemperature.h>

class Sonde
{
  public:
  Sonde();
  ~Sonde();
  void initialiser_sonde_ds18b20();
  int sonde_connecte(); //permet d'indiquer si le capteur est bien connecté
  float get_temperature();

  private:
  const int PIN_DS18B20=2; //pin sur laquelle est branché la sonde onewire
  const int TEMPERATURE_PRECISION=12; //précision de la sonde
  DallasTemperature* sensors;   //pointeur de classe DallasTemperature Pass our oneWire reference to Dallas Temperature.
  OneWire* sonde; //ponteur de classe OneWire
  DeviceAddress addrSonde = { 0x28, 0xFF, 0xED, 0x6A, 0x56, 0x14, 0x03, 0x6A };  // Tableau contenant l'adresse de la sonde OneWire trouvé avec le programme de scan

};

#endif
