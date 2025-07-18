#define WEATHER_H
#ifdef WEATHER_H

typedef struct {
  const char *city;
  const char *desc;
  float temp;
} WeatherData;

WeatherData fetch_weather_data();

#endif // WEATHER_H
