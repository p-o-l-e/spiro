/*****************************************************************************************************************************
* Copyright (c) 2022-2023 POLE
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Signed distnace field (SDF) optimization with AABB 
* Based on: https://github.com/miloyip/line/blob/master/line_sdfaabb.c
******************************************************************************************************************************/
#pragma once

#include "canvas.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace core {

template <typename T>
constexpr void drawSquareFilled(core::Canvas<T>& canvas, const SquareXYR<T>& square, const T colour)
{
    int xe = square.x + square.r;
    int ye = square.y + square.r;
    int xo = square.x - square.r;
    int yo = square.y - square.r;

    for(int y = yo; y <= ye; y++)
    {
        for(int x = xo; x <= xe; x++)
        {
            canvas.set(x, y, colour);
        }
    }
}

constexpr void drawVLine(core::Canvas<uint8_t>* canvas, unsigned xo, unsigned yo, unsigned ye, uint8_t c) noexcept
{
    for(unsigned i = yo; i <= ye; i++)
    {
        canvas->set(xo, i, c);
    }
}

constexpr void drawHLine(core::Canvas<uint8_t>* canvas, unsigned xo, unsigned yo, unsigned xe, uint8_t c) noexcept
{
    for(unsigned i = xo; i <= xe; i++)
    {
        canvas->set(i, yo, c);
    }
}

constexpr void drawGlyph(core::Canvas<uint8_t>* canvas, const uint8_t* font, unsigned id, size_t xo, size_t yo, uint8_t colour, size_t w = 7, size_t h = 8)
{
    size_t pos = id * w;
    uint8_t stencil = 0b1;
    for(size_t y = 0; y < h; ++y)
    {
        for(size_t x = 0; x < w; ++x)
        {
            if(font[pos + x] & stencil) canvas->set(x + xo, y + yo, colour);
        }
        stencil <<= 1;
    }
}

const inline void drawTextLabel(core::Canvas<uint8_t>* canvas, const uint8_t* font, const char* text, size_t xo, size_t yo, uint8_t opacity, size_t w = 7)
{
    size_t n = (size_t)strlen(text);
    for(size_t i = 0; i < n; ++i)
    {
        drawGlyph(canvas, font, (unsigned)text[i] - 32, xo + i * (w + 1), yo, opacity);
    }
}

inline float capsuleSDF(float px, float py, float ax, float ay, float bx, float by, float r) 
{
    float pax = px - ax, pay = py - ay, bax = bx - ax, bay = by - ay;
    float h = fmaxf(fminf((pax * bax + pay * bay) / (bax * bax + bay * bay), 1.0f), 0.0f);
    float dx = pax - bax * h, dy = pay - bay * h;
    return sqrtf(dx * dx + dy * dy) - r;
}

inline void alphablend(core::Canvas<float>* canvas, size_t x, size_t y, float alpha) 
{   
    canvas->set(x, y, canvas->get(x, y) * (1.0f - alpha) + alpha);
}

const inline void lineSDFAABB(core::Canvas<float>* canvas, float ax, float ay, float bx, float by, float radius, float alpha) 
{
    float xo = (float)floor(std::min(ax, bx) - radius);
    float xe = (float) ceil(std::max(ax, bx) + radius);
    float yo = (float)floor(std::min(ay, by) - radius);
    float ye = (float) ceil(std::max(ay, by) + radius);
    for(size_t y = (size_t)yo; y <= (size_t)ye; y++)
    {
        for(size_t x = (size_t)xo; x <= (size_t)xe; x++)
        {
            alphablend(canvas, x, y, std::max(std::min(0.5f - capsuleSDF((float)x, (float)y, ax, ay, bx, by, radius), 1.0f), alpha));
        }
    }
}

}

