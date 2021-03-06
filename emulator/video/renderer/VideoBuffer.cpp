/*
 * This file is part of the Orion-128 distribution (https://github.com/temaweb/orion-128).
 * Copyright (c) 2020 Artem Okonechnikov.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "VideoBuffer.hpp"

vbuffer::iterator VideoBuffer::getPixelsIterator()
{
    return pixels.begin();
}

vbuffer::iterator VideoBuffer::getColorsIterator()
{
    return colors.begin();
}

bool VideoBuffer::operator==(const VideoBuffer & buffer) const
{
    if (pixels != buffer.pixels)
        return false;

    return colors == buffer.colors;
}

bool VideoBuffer::operator!=(const VideoBuffer & buffer) const
{
    return !(*this == buffer);
}

uint8_t VideoBuffer::readFrame(uint16_t address) const
{
    return pixels[address];
}

uint8_t VideoBuffer::readColor(uint16_t address) const
{
    return colors[address];
}
