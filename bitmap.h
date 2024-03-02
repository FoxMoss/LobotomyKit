#include <litehtml/litehtml.h>

class Bitmap {
public:
  int width = 0;
  int height = 0;
  std::vector<litehtml::web_color> data;

  Bitmap() {}
  Bitmap(int width, int height,
         litehtml::web_color color = litehtml::web_color::white)
      : width(width), height(height) {
    data.resize(width * height, color);
  }
  Bitmap(litehtml::string filename) { load(filename); }
  bool operator==(const Bitmap &bmp) const {
    return width == bmp.width && height == bmp.height && data == bmp.data;
  }
  bool operator!=(const Bitmap &bmp) const { return !(*this == bmp); }

  litehtml::web_color get_pixel(int x, int y) const;
  void set_pixel(int x, int y, litehtml::web_color color);
  void draw_line(int x0, int y0, int x1, int y1, litehtml::web_color color);
  void draw_rect(int x, int y, int width, int height,
                 litehtml::web_color color);
  void fill_rect(litehtml::position rect, litehtml::web_color color);
  void draw_bitmap(int x, int y, const Bitmap &bmp);
  void replace_color(litehtml::web_color original,
                     litehtml::web_color replacement);

  litehtml::position
  find_picture(litehtml::web_color bgcolor = litehtml::web_color::white);
  void resize(int new_width, int new_height);
  void load(litehtml::string filename);
  void save(litehtml::string filename);
};

litehtml::string readfile(litehtml::string filename);
