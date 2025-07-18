#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <jansson.h>

#include "weather.h"

#define API_KEY "000dd6ca5191e08135cc4d67aaa175bc"
#define CITY "Milan"
#define URL_FORMAT                                                             \
  "https://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s&units=metric"

typedef struct {
  char *res;
  size_t size;
} Memory;

static size_t write_callback(void *data, size_t size, size_t nmemb,
                             void *userp) {
  size_t real_size = size * nmemb;
  Memory *mem = (Memory *)userp;

  mem->res = realloc(mem->res, mem->size + real_size + 1);
  if (mem->res == NULL)
    return 0;

  memcpy(&(mem->res[mem->size]), data, real_size);
  mem->size += real_size;
  mem->res[mem->size] = '\0';

  return real_size;
}

void fetch_weather_data() {
  CURL *curl;
  CURLcode res;

  Memory chunk = {.res = NULL, .size = 0};

  char url[512];
  snprintf(url, sizeof(url), URL_FORMAT, CITY, API_KEY);

  curl = curl_easy_init();
  if (!curl) {
    fprintf(stderr, "Errore inizializzazione di CURL\n");
    return;
  }

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "Errore nella richiesta: %s\n", curl_easy_strerror(res));
    curl_easy_cleanup(curl);
    free(chunk.res);
    return;
  }

  curl_easy_cleanup(curl);

  // parsing json-lcurl -ljasson
  json_error_t error;
  json_t *root = json_loads(chunk.res, 0, &error);
  free(chunk.res);

  if (!root) {
    fprintf(stderr, "Errore nel parsing del JSON: %s\n", error.text);
    return;
  }

  json_t *main = json_object_get(root, "main");
  json_t *weather_array = json_object_get(root, "weather");
  json_t *name = json_object_get(root, "name");

  if (main && weather_array && json_is_array(weather_array)) {
    float temp = json_number_value(json_object_get(main, "temp"));
    const char *desc = json_string_value(
        json_object_get(json_array_get(weather_array, 0), "description"));
    const char *city_name = json_string_value(name);

    printf("Citta': %s\n", city_name);
    printf("Temperatura: %.2f°C\n", temp);
    printf("Descrizione: %s\n", desc);
  } else {
    fprintf(stderr, "Dati JSON mancanti o malformati.\n");
  }

  json_decref(root);
}
