#include <algorithm>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#define _CRT_SECURE_NO_WARNINGS
#include "font.h"

using litehtml::string;

string readfile(string filename) {
  std::stringstream ss;
  std::ifstream(filename) >> ss.rdbuf();
  return ss.str();
}

using namespace std;

string LiteFont::font_dir = "./fonts/"; // ctest is run from litehtml/build
LiteFont::size_name LiteFont::installed_fonts[] = {
    {12, "terminus-ascii-bold-12px.yaff"},
    {14, "terminus-ascii-bold-14px.yaff"},
    {16, "terminus-ascii-bold-16px.yaff"},
    {18, "terminus-ascii-bold-18px.yaff"},
    {20, "terminus-ascii-bold-20px.yaff"},
    {22, "terminus-ascii-bold-22px.yaff"},
    {24, "terminus-ascii-bold-24px.yaff"},
    {28, "terminus-ascii-bold-28px.yaff"},
    {32, "terminus-ascii-bold-32px.yaff"},
    {0, ""}};

LiteFont::LiteFont(int size) {
  // find most suitable font
  int min_diff = 1000;
  int n = 0;
  for (int i = 0; installed_fonts[i].size; i++) {
    int diff = abs(installed_fonts[i].size - size);
    if (diff < min_diff) {
      min_diff = diff;
      n = i;
    }
  }

  load(font_dir + installed_fonts[n].name);
}

Bitmap LiteFont::get_glyph(int ch, litehtml::web_color color) {
  if (ch < 0 || ch >= 128 || glyphs[ch].width == 0) {
    Bitmap bmp(width, height, litehtml::web_color::transparent);
    bmp.draw_rect(1, 1, width - 2, height - 2, color);
    return bmp;
  } else if (color != litehtml::web_color::black) {
    Bitmap bmp = glyphs[ch];
    bmp.replace_color(litehtml::web_color::black, color);
    return bmp;
  } else {
    return glyphs[ch];
  }
}

// load .yaff font file in an ad hoc manner (can't parse arbitrary yaff files)
void LiteFont::load(string filename) {
  string text = readfile(filename);

  litehtml::string_vector lines;
  litehtml::split_string(text, lines, "\n");

  int i;
  // parse header
  for (i = 0; i < (int)lines.size(); i++) {
    string line = lines[i];
    litehtml::trim(line);
    if (line.empty() || line[0] == '#')
      continue; // skip empty lines and comments

    auto sep = line.find(':');
    if (sep == string::npos)
      return; // line without ':' - error

    auto key = line.substr(0, sep);
    litehtml::trim(key);
    auto val = line.substr(sep + 1);
    litehtml::trim(val);
    if (val.empty())
      break; // end of header

    if (key == "cell-size")
      sscanf(val.c_str(), "%d %d", &width, &height);
    else if (key == "ascent")
      ascent = atoi(val.c_str());
    else if (key == "descent")
      descent = atoi(val.c_str());
  }

  // parse glyphs

  // only u+NNNN: label is recognized, all others are skipped
  auto parse_key = [&]() {
    int ch = -1;
    for (; i < (int)lines.size(); i++) {
      string line = lines[i];
      litehtml::trim(line);
      if (line.empty())
        continue;
      if (line.find(':') == string::npos)
        break; // start of glyph data
      if (line.substr(0, 2) == "u+")
        sscanf(line.c_str(), "u+%X:", (unsigned int *)&ch);
    }
    return ch;
  };

  auto parse_glyph = [&](int ch) {
    Bitmap &glyph = glyphs[ch] =
        Bitmap(width, height, litehtml::web_color::transparent);
    for (int y = 0; i < (int)lines.size() && y < height; i++, y++) {
      string line = lines[i];
      litehtml::trim(line);
      for (int x = 0; x < min((int)line.size(), width); x++) {
        if (line[x] == '@')
          glyph.set_pixel(x, y, litehtml::web_color::black);
      }
    }
  };

  while (i < (int)lines.size()) {
    int ch = parse_key();
    if (ch < 0 || ch >= 128)
      break;
    parse_glyph(ch);
  }

  x_height = glyphs['x'].find_picture(litehtml::web_color::transparent).height;
}