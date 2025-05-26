/*****************************************************************************************************************************
* Copyright (c) 2022-2025 POLE
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
******************************************************************************************************************************/
#include "Socket.h"
#include "Constraints.hpp"
#include "descriptor.hxx"
#include "modmatrix.hpp"
#include "primitives.hpp"
#include "uid.hpp"
#include <iostream>
#include <iomanip>
#define SR core::constraints::SR
#define XG core::constraints::gap_x

Sockets::Sockets(const core::Rectangle<int>& bounds, const core::Grid& o)
{
    std::cout  <<"-- Initializing sockets...\n";
    auto inputs = o.count(core::Control::input);
    auto outputs = o.count(core::Control::output);

    bay = new core::Patchbay(bounds.w, bounds.h, inputs, outputs);

    unsigned position = 0; // 255 sockets max
    std::cout  <<"---- Outputs...\n";
    for(int i = 0; i < inputs; ++i)
    {   
        auto hash = o.getHash(i, core::Control::input);
        auto f = o.getBounds(o.getUID(i, core::Control::input));
        core::Point2D<int> offset 
        {
            static_cast<int>(f.x - bounds.x + XG + SR),
            static_cast<int>(f.y - bounds.y + SR)
        };
        std::cout<<o.name(core::decode_uid(hash), false)<<"\n";
        std::cout<<"X: "<<std::setw(3)<<offset.x<<" - Y: "<<offset.y<<" - Hash: "<<std::setw(10)<<std::hex<<hash<<"\n";
        bay->set_socket(&offset, SR, hash, SOCKET_IN, position);
        ++position;
    }
    std::cout  <<"---- Outputs...\n";
    for(int i = 0; i < outputs; ++i)
    {
        auto hash = o.getHash(i, core::Control::output);
        auto f = o.getBounds(o.getUID(i, core::Control::output));
        core::Point2D<int> offset 
        {
            static_cast<int>(f.x - bounds.x + XG + SR),
            static_cast<int>(f.y - bounds.y + SR)
        };
        std::cout<<o.name(core::decode_uid(hash), false)<<"\n";
        std::cout<<"X: "<<std::setw(3)<<offset.x<<" - Y: "<<offset.y<<" - Hash: "<<std::setw(10)<<std::hex<<hash<<"\n";
        bay->set_socket(&offset, SR, hash, SOCKET_OUT, position);
        ++position;
    }
    bay->draw();
    std::cout  <<"-- Sockets initialized!\n";
};

Sockets::~Sockets()
{
    delete bay;
}

void Sockets::drawMask(juce::Graphics& g, juce::Colour colour)
{
    auto h  = area.getHeight();
    auto w  = area.getWidth();

    static auto layer = juce::Image(juce::Image::PixelFormat::ARGB, w, h, true);
    static juce::Image::BitmapData bmp(layer, juce::Image::BitmapData::ReadWriteMode::readWrite);

    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            auto c = bay->canvas.get(x, y);
            float alpha = c ? 1.0f : 0.0f;
            bmp.setPixelColour(x, y, colour_set[c&0xFF].withAlpha(alpha));
        }
    }
    g.setOpacity(1.0f);
    g.drawRect(area);
    g.drawImageAt(layer, area.getX(), area.getY());
}

void Sockets::drawCords(juce::Graphics& g, float alpha)
{
    for(int j = 0; j < bay->nodes; j++)
    {
        core::Point2D<float> prior, current = bay->io[j].cord.data[0];
        bay->io[j].cord.focused ? g.setColour (colour_highlighted.withAlpha(alpha)) : g.setColour (colour_normal.withAlpha(alpha));
        for(int i = 0; i < bay->io[j].cord.iterations; i++)
        {
            prior = current;
            current = bay->io[j].cord.data[i];
            g.drawLine(prior.x, prior.y, current.x, current.y, 2.0f);
        }
        if(bay->io[j].on) g.fillEllipse(bay->io[j].cord.data[0].x - 3, bay->io[j].cord.data[0].y - 3, 6, 6);
    }
}

core::Socket* Sockets::from_grid(int position, bool route)
{
    if(route == SOCKET_IN)
    {
        for(int i = 0; i < bay->nodes; ++i)
        {
            if(bay->io[i].route == SOCKET_IN)
            {
                if(bay->io[i].pos == position) return &bay->io[i];
            }
        }
    }
    else
    {
        for(int i = 0; i < bay->nodes; ++i)
        {
            if(bay->io[i].route == SOCKET_OUT)
            {
                if(bay->io[i].pos == position) return &bay->io[i];
            }
        }
    }
    return nullptr;
}

void Sockets::load()
{
    bay->clear();
    for(int x = 0; x < bay->inputs; ++x)
    {
        for(int y = 0; y < bay->outputs; ++y)
        {
            bool on = bay->matrix.get(x, y);
            if(on)
            {
                bay->connect(from_grid(x, SOCKET_IN), from_grid(y, SOCKET_OUT));
            }
        }
    }
    bay->deselect();
}

void Sockets::paint(juce::Graphics& g)
{
    drawCords(g, 1.0f);
    // drawMask(g, colour_set[0]);
}

void Sockets::resized ()
{
    area = getLocalBounds();
}

void Sockets::mouseDown(const juce::MouseEvent& event)
{
    int mb = 0;
    if     (event.mods.isLeftButtonDown())  mb = core::settings::mb::lmb;
    else if(event.mods.isRightButtonDown()) mb = core::settings::mb::rmb;

    auto t = bay->down_test(event.x, event.y, mb);
    if(t == 1)
    {
        i_armed = true;
        setMouseCursor(juce::MouseCursor::CrosshairCursor);
    }
    else if(t == -1)
    {
        o_armed = true;
        setMouseCursor(juce::MouseCursor::CrosshairCursor);
    }
    repaint();
}

void Sockets::mouseDrag(const juce::MouseEvent& event)
{
    bay->drag(event.x, event.y);
    repaint();
}

void Sockets::mouseUp(const juce::MouseEvent& event)
{
    bay->up_test(event.x, event.y, 0);
    i_armed = false;
    o_armed = false;
    setMouseCursor(juce::MouseCursor::NormalCursor);
    repaint();
}

void Sockets::mouseMove(const juce::MouseEvent& event)
{
    bay->move_test(event.x, event.y, 0);
    repaint();
}

