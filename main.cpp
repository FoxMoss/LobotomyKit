#include "bitmap.h"
#include "container.h"
#include "raylib.h"
#include <cstring>
#include <iostream>
#include <litehtml/litehtml.h>
#include <litehtml/os_types.h>
#include <litehtml/web_color.h>

int main(int argc, char *argv[]) {
  InitWindow(960, 540, "LobotomyKit");

  std::string bodyText = readfile("index.html");
  Container container(960, 540, "./");

  auto doc = litehtml::document::createFromString(bodyText.c_str(), &container);

  litehtml::position clip(0, 0, 960, 540);
  int y = 0;
  doc->render(960);

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    doc->draw((litehtml::uint_ptr)NULL, 0, -y, &clip);
    if (IsKeyDown(KEY_DOWN)) {
      y++;
    }
    if (IsKeyDown(KEY_UP)) {
      y--;
    }

    DrawFPS(10, 10);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
