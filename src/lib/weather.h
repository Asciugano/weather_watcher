#define WEATHER_H
#ifdef WEATHER_H

#define API_KEY "000dd6ca5191e08135cc4d67aaa175bc"
#define CITY "Milan"
#define URL_FORMAT                                                             \
  "https://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s&units=metric"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <jansson.h>

typedef struct {
  const char *city;
  const char *desc;
  float temp;
} WeatherData;

WeatherData fetch_weather_data(const char *city);
void DrawWeatherInfo(float x, float y, WeatherData data);

#endif // WEATHER_H
