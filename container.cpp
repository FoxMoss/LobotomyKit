#include "container.h"
#include "font.h"
#include <litehtml/litehtml.h>
#include <raylib.h>

#define TO_RAYLIB(color)                                                       \
  { color.red, color.green, color.blue, color.alpha }

litehtml::string readfile(litehtml::string filename);

// note: font is selected only by size, name and style are not used
litehtml::uint_ptr Container::create_font(const char * /*faceName*/, int size,
                                          int /*weight*/,
                                          litehtml::font_style /*italic*/,
                                          unsigned int /*decoration*/,
                                          litehtml::font_metrics *fm) {
  LiteFont *font = new LiteFont(size);

  if (fm) {
    fm->ascent = font->ascent;
    fm->descent = font->descent;
    fm->height = font->height;
    fm->x_height = font->x_height;
  }
  return (litehtml::uint_ptr)font;
}

int Container::text_width(const char *text, litehtml::uint_ptr hFont) {
  LiteFont *font = (LiteFont *)hFont;
  return (int)strlen(text) * font->width;
}

void Container::draw_text(litehtml::uint_ptr hdc, const char *text,
                          litehtml::uint_ptr hFont, litehtml::web_color color,
                          const litehtml::position &pos) {
  // auto bmp = (Bitmap *)hdc;
  auto font = (LiteFont *)hFont;
  //
  // int x = pos.x;
  // for (auto p = text; *p; p++) {
  // Bitmap glyph = font->get_glyph(*p, color);
  // bmp->draw_bitmap(x, pos.y, glyph);
  //   x += glyph.width;
  // }
  Color out = {color.red, color.green, color.blue, color.alpha};
  // DrawText(text, pos.x, pos.y, font->height, out);
  DrawTextEx(rayFont, text, {(float)pos.x, (float)pos.y}, font->height, 0, out);
}

int Container::pt_to_px(int pt) const { return pt * 96 / 72; }
int Container::get_default_font_size() const { return 16; }
const char *Container::get_default_font_name() const { return ""; }

void Container::draw_background(
    litehtml::uint_ptr hdc, const std::vector<litehtml::background_paint> &bg) {
  // auto bmp = (Bitmap *)hdc;
  // bmp->fill_rect(bg.back().border_box, bg.back().color);
  auto rect = bg.back().border_box;
  Color out = {bg.back().color.red, bg.back().color.green, bg.back().color.blue,
               bg.back().color.alpha};
  DrawRectangle(rect.x, rect.y, rect.width, rect.height, out);
}

void Container::draw_borders(litehtml::uint_ptr hdc,
                             const litehtml::borders &borders,
                             const litehtml::position &pos, bool /*root*/) {
  // auto bmp = (Bitmap *)hdc;
  //
  // // left border
  // for (int x = 0; x < borders.left.width; x++)
  //   bmp->draw_line(pos.left() + x, pos.top(), pos.left() + x, pos.bottom(),
  //                  borders.left.color);
  //
  // // right border
  // for (int x = 0; x < borders.right.width; x++)
  //   bmp->draw_line(pos.right() - x - 1, pos.top(), pos.right() - x - 1,
  //                  pos.bottom(), borders.right.color);
  //
  // // top border
  // for (int y = 0; y < borders.top.width; y++)
  //   bmp->draw_line(pos.left(), pos.top() + y, pos.right(), pos.top() + y,
  //                  borders.top.color);
  //
  // // bottom border
  // for (int y = 0; y < borders.bottom.width; y++)
  //   bmp->draw_line(pos.left(), pos.bottom() - y - 1, pos.right(),
  //                  pos.bottom() - y - 1, borders.bottom.color);
}

void Container::draw_list_marker(litehtml::uint_ptr hdc,
                                 const litehtml::list_marker &marker) {
  // auto bmp = (Bitmap *)hdc;
  // bmp->fill_rect(marker.pos, marker.color);
  DrawRectangle(marker.pos.x, marker.pos.y, marker.pos.width, marker.pos.height,
                TO_RAYLIB(marker.color));
}

void Container::import_css(litehtml::string &text, const litehtml::string &url,
                           litehtml::string &baseurl) {
  baseurl = basedir + "/" + url;
  text = readfile(baseurl);
}

void Container::get_client_rect(litehtml::position &client) const {
  client = litehtml::position(0, 0, width, height);
}
