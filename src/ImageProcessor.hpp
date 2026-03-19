#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <cocos2d.h>

struct PixelColor {
    uint8_t r, g, b, a;
    
    PixelColor() : r(0), g(0), b(0), a(255) {}
    PixelColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) 
        : r(red), g(green), b(blue), a(alpha) {}

    bool isTransparent() const { return a < 128; }
    
    bool operator==(const PixelColor& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
    
    bool operator!=(const PixelColor& other) const {
        return !(*this == other);
    }
};

struct ImageData {
    int width;
    int height;
    std::vector<PixelColor> pixels;

    ImageData() : width(0), height(0) {}

    ImageData(int w, int h) : width(w), height(h), pixels(w * h) {}

    const PixelColor& getPixel(int x, int y) const {
        static PixelColor empty;
        if (x < 0 || x >= width || y < 0 || y >= height) return empty;
        return pixels[y * width + x];
    }

    void setPixel(int x, int y, const PixelColor& color) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;
        pixels[y * width + x] = color;
    }
};

class ImageProcessor {
public:
    static ImageData loadImage(const std::string& filePath);
    
    static ImageData scaleDown(const ImageData& source, int targetWidth, int targetHeight);
    
    static std::vector<std::pair<PixelColor, std::vector<CCPoint>>> 
    groupPixelsByColor(const ImageData& image, float blockSize);
};
