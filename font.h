#include "bitmap.h"

class LiteFont {
public:
  int width = 0;
  int height = 0;
  int ascent = 0;
  int descent = 0;
  int x_height = 0;
  Bitmap glyphs[128];

  static litehtml::string font_dir;
  static struct size_name {
    int size;
    litehtml::string name;
  } installed_fonts[];

  LiteFont(int size);
  Bitmap get_glyph(int ch, litehtml::web_color color);
  void load(litehtml::string filename);
};
