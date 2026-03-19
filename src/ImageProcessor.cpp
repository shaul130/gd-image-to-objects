#include "ImageProcessor.hpp"
#include <Geode/Geode.hpp>
#include <cmath>
#include <algorithm>
#include <map>
#include <cocos2d/cocos2d.h>

using namespace geode::prelude;

ImageData ImageProcessor::loadImage(const std::string& filePath) {
    ImageData result;
    
    auto image = new CCImage();
    if (!image->initWithImageFile(filePath.c_str())) {
        delete image;
        return result;
    }
    
    int width = image->getWidth();
    int height = image->getHeight();
    unsigned char* data = image->getData();
    bool hasAlpha = image->hasAlpha();
    
    result = ImageData(width, height);
    
    int bytesPerPixel = hasAlpha ? 4 : 3;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixelIndex = (y * width + x) * bytesPerPixel;
            int r = data[pixelIndex];
            int g = data[pixelIndex + 1];
            int b = data[pixelIndex + 2];
            int a = hasAlpha ? data[pixelIndex + 3] : 255;
            
            result.setPixel(x, y, PixelColor(r, g, b, a));
        }
    }
    
    delete image;
    return result;
}

ImageData ImageProcessor::scaleDown(const ImageData& source, int targetWidth, int targetHeight) {
    if (targetWidth <= 0 || targetHeight <= 0) {
        return source;
    }
    
    ImageData result(targetWidth, targetHeight);
    
    float scaleX = static_cast<float>(source.width) / targetWidth;
    float scaleY = static_cast<float>(source.height) / targetHeight;
    
    for (int y = 0; y < targetHeight; y++) {
        for (int x = 0; x < targetWidth; x++) {
            int srcX = static_cast<int>(x * scaleX);
            int srcY = static_cast<int>(y * scaleY);
            
            srcX = std::min(srcX, source.width - 1);
            srcY = std::min(srcY, source.height - 1);
            
            result.setPixel(x, y, source.getPixel(srcX, srcY));
        }
    }
    
    return result;
}

std::vector<std::pair<PixelColor, std::vector<CCPoint>>> 
ImageProcessor::groupPixelsByColor(const ImageData& image, float blockSize) {
    std::map<PixelColor, std::vector<CCPoint>, bool(*)(const PixelColor&, const PixelColor&)> 
        colorGroups([](const PixelColor& a, const PixelColor& b) {
            if (a.r != b.r) return a.r < b.r;
            if (a.g != b.g) return a.g < b.g;
            if (a.b != b.b) return a.b < b.b;
            return a.a < b.a;
        });
    
    for (int y = 0; y < image.height; y++) {
        for (int x = 0; x < image.width; x++) {
            PixelColor color = image.getPixel(x, y);
            if (color.isTransparent()) continue;
            
            CCPoint pos = CCPoint(x * blockSize, y * blockSize);
            colorGroups[color].push_back(pos);
        }
    }
    
    std::vector<std::pair<PixelColor, std::vector<CCPoint>>> result;
    for (auto& pair : colorGroups) {
        result.push_back(pair);
    }
    
    return result;
}
