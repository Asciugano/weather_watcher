#include "weather.h"
#include "raylib.h"

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

WeatherData fetch_weather_data(const char *city) {
  CURL *curl;
  CURLcode res;

  Memory chunk = {.res = NULL, .size = 0};
  WeatherData data = {0};

  char url[512];
  snprintf(url, sizeof(url), URL_FORMAT, city, API_KEY);

  curl = curl_easy_init();
  if (!curl) {
    fprintf(stderr, "Errore inizializzazione di CURL\n");
    return data;
  }

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "Errore nella richiesta: %s\n", curl_easy_strerror(res));
    curl_easy_cleanup(curl);
    free(chunk.res);
    return data;
  }

  curl_easy_cleanup(curl);

  // parsing json-lcurl -ljasson
  json_error_t error;
  json_t *root = json_loads(chunk.res, 0, &error);
  free(chunk.res);

  if (!root) {
    fprintf(stderr, "Errore nel parsing del JSON: %s\n", error.text);
    return data;
  }

  json_t *main = json_object_get(root, "main");
  json_t *weather_array = json_object_get(root, "weather");
  json_t *name = json_object_get(root, "name");

  if (main && weather_array && json_is_array(weather_array)) {
    data.temp = json_number_value(json_object_get(main, "temp"));
    data.desc = json_string_value(
        json_object_get(json_array_get(weather_array, 0), "description"));
    data.city = json_string_value(name);

    printf("Citta': %s\n", data.city);
    printf("Temperatura: %.2f°C\n", data.temp);
    printf("Descrizione: %s\n", data.desc);

    return data;
  } else {
    fprintf(stderr, "Dati JSON mancanti o malformati.\n");
    return data;
  }

  json_decref(root);

  return data;
}

#define INFO_WT 280
#define INFO_HG 160

void DrawWeatherInfo(float x, float y, WeatherData data) {

  DrawRectangle(x, y, INFO_WT, INFO_HG, BLACK);
  x += 20;
  y += 20;
  DrawText(TextFormat("Citta: %s", data.city), x + 3, y + 3, 20, WHITE);
  y += 40;
  DrawText(TextFormat("Temperatura: %.2f°C", data.temp), x + 3, y + 3, 20,
           WHITE);
  y += 40;
  DrawText(TextFormat("Descrizione: %s", data.desc), x + 3, y + 3, 20, WHITE);
}
