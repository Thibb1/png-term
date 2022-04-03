#include <filesystem>
#include <iostream>
#include <clocale>
#include <cmath>
#include <png.h>
#include <curses.h>

typedef std::string Path;
typedef std::pair<float, float> Position;

class Pixel {
public:
    int r{};
    int g{};
    int b{};
    Pixel() = default;
    virtual ~Pixel() = default;
    void ConvertPixel() {
        r *= .256;
        g *= .256;
        b *= .256;
    }
};

class Converter {
private:
    float xFactor;
    float yFactor;
    uint height;
    uint width;
    FILE *fp;
    png_structp pngPtr;
    png_infop pngInfo;
    png_bytepp rows;
public:
    explicit Converter(const Path &Source) {
        fp = fopen(Source.c_str(), "r");
        pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        pngInfo = png_create_info_struct(pngPtr);
        png_init_io(pngPtr, fp);
        png_read_png(pngPtr, pngInfo, PNG_TRANSFORM_IDENTITY, nullptr);
        rows = png_get_rows(pngPtr, pngInfo);
        height = png_get_image_height(pngPtr, pngInfo);
        width = png_get_image_width(pngPtr, pngInfo);
        fclose(fp);
        xFactor = float(width) / float(COLS);
        yFactor = float(height) / float(LINES);
//        xFactor = 1;
//        yFactor = 1;
    }

    virtual ~Converter() = default;

    void processImage(Position position) {
        for (float yIdx = 0; uint(yIdx) < height; yIdx += 1 * yFactor) {
            for (float xIdx = 0; uint(xIdx) < width; xIdx += 1 * xFactor) {
                int x = int(xIdx);
                int y = int(yIdx);
                int r = rows[y][x * 4 + 0];
                int g = rows[y][x * 4 + 1];
                int b = rows[y][x * 4 + 2];
                int alpha = rows[y][x * 4 + 3];
                if (alpha == 0)
                    continue;
                int Color = 0;
                int SmallestColorDiff = 1000;
                for (int i = 0; i < 256; i++) {
                    Pixel p;
                    extended_color_content(i, &p.r, &p.g, &p.b);
                    p.ConvertPixel();
                    int ColorDiff = abs(p.r - r) + abs(p.g - g) + abs(p.b - b);
                    if (ColorDiff < SmallestColorDiff) {
                        Color = i;
                        SmallestColorDiff = ColorDiff;
                    }
                }
                attron(COLOR_PAIR(Color));
                mvprintw(int(position.second) + int(yIdx / yFactor), int(position.first) + int(xIdx / xFactor), "█");
                attroff(COLOR_PAIR(Color));
            }
        }
    }
};