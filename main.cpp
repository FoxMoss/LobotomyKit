#include "bitmap.h"
#include "container.h"
#include "raylib.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <litehtml/litehtml.h>
#include <litehtml/os_types.h>
#include <litehtml/web_color.h>
#include <math.h>
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

std::string masterCSS;
litehtml::document::ptr doc;
litehtml::position clip(0, 0, 960, 540);
Container *container;
int y = 0;

void UpdateDrawFrame();

extern "C" {
int loadHtml(char *data) {
  doc =
      litehtml::document::createFromString(data, container, masterCSS.c_str());

  doc->render(960);
  return 0;
}

int start() {
  InitWindow(960, 540, "LobotomyKit");
  container = new Container(960, 540, "./");

  std::string bodyText = readfile("./fonts/index.html");
  masterCSS = readfile("./fonts/html.css");

  doc = litehtml::document::createFromString(bodyText.c_str(), container,
                                             masterCSS.c_str());

  doc->render(960);
#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    UpdateDrawFrame();
  }
#endif

  CloseWindow();
  return 0;
}
}
void UpdateDrawFrame() {
  BeginDrawing();

  ClearBackground(WHITE);

  doc->draw((litehtml::uint_ptr)NULL, 0, -y, &clip);
  y -= (int)round(GetMouseWheelMove() * 100);
  y = std::max(y, 0);
  y = std::min(y, std::max(0, doc->content_height() - 540));

#ifndef PLATFORM_WEB
  DrawFPS(10, 10);
#endif

  EndDrawing();
}

int main(int argc, char *argv[]) {
  start();
  return 0;
}
