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
#include <cstring>

namespace core {


template <typename T>
constexpr void draw_square_filled(core::Canvas<T>* canvas, const RadialSquare<T>& sq, const T colour)
{
    int xe = sq.xCentre + sq.radius;
    int ye = sq.yCentre + sq.radius;
    int xo = sq.xCentre - sq.radius;
    int yo = sq.yCentre - sq.radius;

    for(int y = yo; y <= ye; y++)
    {
        for(int x = xo; x <= xe; x++)
        {
            canvas->set(x, y, colour);
        }
    }
}

constexpr void draw_glyph(core::Canvas<float>* canvas, const char* font, int id, int xo, int yo, const float colour)
{
    int pos = id * 7;
    u_int8_t stencil = 0b1;
    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 7; x++)
        {
            if(font[pos + x] & stencil) canvas->set(x + xo, y + yo, colour);
        }
        stencil<<=1;
    }
}

constexpr void draw_text_label(core::Canvas<float>* canvas, const char* font, const char* text, int xo, int yo, const float colour)
{
    int n = strlen(text);
    for(int i = 0; i < n; i++)
    {
        draw_glyph(canvas, font, text[i] - 32, xo + i*8, yo, colour);
    }
}

inline float capsule_sdf(float px, float py, float ax, float ay, float bx, float by, float r) 
{
    float pax = px - ax, pay = py - ay, bax = bx - ax, bay = by - ay;
    float h = fmaxf(fminf((pax * bax + pay * bay) / (bax * bax + bay * bay), 1.0f), 0.0f);
    float dx = pax - bax * h, dy = pay - bay * h;
    return sqrtf(dx * dx + dy * dy) - r;
}

inline void alphablend(core::Canvas<float>* canvas, int x, int y, float alpha) 
{   
    canvas->set(x, y, canvas->get(x, y) * (1.0f - alpha) + alpha);
}

constexpr void lineSDFAABB(core::Canvas<float>* canvas, float ax, float ay, float bx, float by, float radius, float alpha) 
{
    int xo = (int)floor(std::min(ax, bx) - radius);
    int xe = (int) ceil(std::max(ax, bx) + radius);
    int yo = (int)floor(std::min(ay, by) - radius);
    int ye = (int) ceil(std::max(ay, by) + radius);
    for (int y = yo; y <= ye; y++)
    {
        for (int x = xo; x <= xe; x++)
        {
            alphablend(canvas, x, y, std::max(std::min(0.5f - capsule_sdf(x, y, ax, ay, bx, by, radius), 1.0f), alpha));
        }
    }
}

};

