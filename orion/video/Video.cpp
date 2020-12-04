//
//  Video.cpp
//  orion
//
//  Created by Артём Оконечников on 11.11.2020.
//

#include <cstdint>
#include <iostream>
#include <iomanip>

#include "Video.hpp"
#include "Memory.hpp"

const BWPalette Video::bwpalette = BWPalette();

bool Video::isChanged() const
{
    if (pixelBuffer != framePixelBuffer)
        return true;
    
    return colorBuffer != frameColorBuffer;
}

void Video::setColorMode  (uint8_t mode)
{
    colorMode = (ColorMode) (mode >> 1);
}

void Video::refreshBuffer ()
{
    std::unique_lock lock(_mutex);
    
    videoRam -> readPixel(pixelBuffer.begin());
    videoRam -> readColor(colorBuffer.begin());
}

void Video::swapBuffers()
{
    framePixelBuffer = pixelBuffer;
    frameColorBuffer = colorBuffer;
}

// Returns one frame
std::array<std::array<Pixel, Video::width>, Video::height> Video::output()
{
    std::shared_lock lock(_mutex);
    swapBuffers();
    
    uint8_t row = 0x00;
    frame frame;

    do
    {
        line line = getLine(row);
        frame[row] = line;
    }
    while (row++ != height - 1);
    return frame;
}

// Returns one line
std::array<Pixel, Video::width> Video::getLine(uint8_t row) const
{   
    line line;
    
    for (uint8_t col = 0x00; col < (width / 8); col++)
    {
        // Each video address in Orion has store column
        // index in high order bits and row index in low
        uint16_t address = ((col << 8) | row);
        
        // One byte video data has 8 points on screen
        // Each set bit match drawed point on screen.
        uint8_t data = framePixelBuffer[address];
        
        switch (colorMode)
        {
            case MONO:
            case COLOR4:
                colorisebw(line, col, data);
                break;
            case COLOR16:
                colorise16(line, col, data, address);
            default:
                break;
        }
    }
    
    return line;
}

void Video::colorise (line & line, size_t size, uint8_t data, const Palette & palette) const
{
    for (uint8_t offset = 7;;offset--)
    {
        // Black pixel by default if no data
        Pixel pixel = palette.getBackground();
        
        if ((data & (1 << offset)))
        {
            // White pixel if bit is set
            pixel = palette.getForeground();
        }
        
        line[(size * 8) + (7 - offset)] = pixel;
        
        if (offset == 0x00)
            break;
    }
}

void Video::colorisebw(line & line, size_t size, uint8_t data) const
{
    colorise(line, size, data, bwpalette);
}

void Video::colorise16(line & line, size_t size, uint8_t data, uint16_t address) const
{
    // Read data by address from second page
    // Second page store data about colors;
    auto color = frameColorBuffer[address];
    auto palette = Color16Palette(color);
    
    colorise(line, size, data, palette);
}
