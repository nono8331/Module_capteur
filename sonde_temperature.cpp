#include "sonde_temperature.h"

/*
 *
 * CONSTRUCTEUR
 *
 */
Sonde::Sonde()
{
}

/*
 *
 * DESTRUCTEUR
 *
 */

Sonde::~Sonde()
{
  delete sensors; // vide le pointeur sensors
  delete sonde; // vide le pointeur oneWire
}

/*
 *
 * INITIALISE LA SONDE ONEWIRE
 *
 */
void Sonde::initialiser_sonde_ds18b20()
{
  sonde = new OneWire(PIN_DS18B20); //instancie la classe OneWire
  sensors = new DallasTemperature(sonde); // instancie la classe DallasTemperature
  sensors->begin();  // Start up the library
  sensors->setResolution(addrSonde, TEMPERATURE_PRECISION); //initialiser la resolution de la sonde
}

/*
 *
 * INDIQUE SI LA SONDE EST BIEN BRANCHÉ
 *
 */
int Sonde::sonde_connecte()
{
  if(sensors->getAddress(addrSonde, 0)) //si la sonde est connecter alors on renvoie 1
  {
    return 1;
  }
  return 0; // si la sonde n'est pas connecter on renvoie 0;

}

/*
 *
 * RENVOIE LA TEMPERATAURE
 *
 */
float Sonde::get_temperature()
{
  sensors->requestTemperatures(); //permet de faire une requette de temperature
  return (sensors->getTempC(addrSonde)); //renvoie la temperature
}
