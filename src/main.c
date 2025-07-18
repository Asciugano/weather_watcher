#include <raylib.h>
#include <stdio.h>

#include "./lib/weather.h"

int main(void) {

  InitWindow(800, 600, "Weather Watcher");
  SetTargetFPS(60);

  WeatherData weather;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(CLITERAL(Color){0x18, 0x18, 0x18, 0xFF});
    weather = fetch_weather_data();

    DrawText(TextFormat("Citta': %s", weather.city), 20, 20, 20, WHITE);
    DrawText(TextFormat("Temperatura: %.2f°C", weather.temp), 20, 50, 20,
             WHITE);
    DrawText(TextFormat("Descrizione: %s", weather.desc), 20, 80, 20, WHITE);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
