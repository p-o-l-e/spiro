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
******************************************************************************************************************************/


#include "modmatrix.hpp"
namespace cell {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PatchCord //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr void patchcord::process()
{
    const float inc = 1.04 / (float)iterations;
    float t = 0.0;
    for(int i = 0; i < iterations; i++)
    {
        point<float> car { interpolate_bezier(spline[0], spline[1], spline[2], spline[3], t) };
        data[i].x = car.x;
        data[i].y = car.y;
        t += inc;
    }
}

patchcord::patchcord(int j = 8): iterations(j)
{
    data = new point<float>[iterations];
}

patchcord::~patchcord()
{
    delete[] data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Socket /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr void socket::collapse()
{
    for(int i = 0; i < cord.segments; i++)
    {
        cord.spline[i].x = bounds.xCentre;
        cord.spline[i].y = bounds.yCentre;
    }
    cord.process();
}


constexpr void socket::drag(const float& x, const float& y)
{          
    cord.focused = true;
    auto xe = x, ye = y;

    if(xe > *w) xe = *w;
    if(xe <  0) xe = 0;
    if(ye > *h) ye = *h;
    if(ye <  0) ye = 0;

    
    float xo = cord.spline[0].x;
    float yo = cord.spline[0].y;

    cord.spline[1].y = (ye - yo) * (2.0f/3.0f) + yo;
    cord.spline[2].y = (ye - yo) * (1.0f/3.0f) + yo;

    if(xe < xo)
    {
        cord.spline[1].x = abs(xo - xe) * 0.999999999f + xe;
        cord.spline[2].x = abs(xo - xe) * 0.000000001f + xe;
    }

    if(xe > xo)
    {
        cord.spline[1].x = abs(xo - xe) * 0.000000001f + xo;
        cord.spline[2].x = abs(xo - xe) * 0.999999999f + xo;
    }

    cord.spline[3].x = xe;
    cord.spline[3].y = ye;

    cord.process();
}

socket::socket(int n = 32): cord(n)
{
    collapse();
}

socket::~socket() 
{

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Patchbay ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Connect cords //////////////////////////////////////////////////////////////////////////////////////////////////////////////
void patchbay::connect(socket* a, socket* b)
{
    a->drag(b->bounds.xCentre, b->bounds.yCentre);
    a->on = true;
    b->on = true;
    a->to = b;
    b->to = a;

    if(a->route == SOCKET_IN)
    {
        a->data = b->data;
        matrix.set(a->pos, b->pos, true);
    }
    else
    {
        b->data = a->data;
        matrix.set(b->pos, a->pos, true);
    }
    *a->com = a->data;
    *b->com = b->data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Disconnect cords ///////////////////////////////////////////////////////////////////////////////////////////////////////////
void patchbay::disconnect(socket* a, socket* b)
{
    a->on = false;
    b->on = false;

    if(a->route == SOCKET_IN)
    {
        a->data = &zero;
        *a->com = a->data;
        matrix.set(a->pos, b->pos, false);
    }
    else
    {
        b->data = &zero;
        *b->com = b->data;
        matrix.set(b->pos, a->pos, false);
    }

    a->to = nullptr;
    b->to = nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Disconnect cords ///////////////////////////////////////////////////////////////////////////////////////////////////////////
void patchbay::clear()
{
    for(int i = 0; i < nodes; i++)
    {
        if(io[i].route == SOCKET_IN) io[i].data = &zero;
        io[i].collapse();
        io[i].on = false;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Draw ID on canvas //////////////////////////////////////////////////////////////////////////////////////////////////////////
void patchbay::draw()
{
    for(int i = 0; i < nodes; i++)
    {
        draw_square_filled<uint>(&canvas, io[i].bounds, io[i].id);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HitTest DOWN ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int patchbay::down_test(const float& x, const float& y, const int& mb)
{
    if(mb == cell::settings::mb::lmb)
    {
        auto stamp = canvas.get(x, y); 

        if(stamp)
        {
            // Socket is hitted ///////////////////////////////////////////////////////////////////////////////////////////////////
            int i = (stamp>>24) - 1;
            {
                // Already connected? /////////////////////////////////////////////////////////////////////////////////////////////
                if(io[i].on)
                {
                    src = io[i].to;
                    src->drag(io[i].bounds.xCentre, io[i].bounds.yCentre);
                    io[i].collapse();
                    disconnect(&io[i], io[i].to);
                    return src->route? 1 : -1;
                }
                // Test successful ////////////////////////////////////////////////////////////////////////////////////////////////
                else
                {
                    src = &io[i];
                    src->collapse();
                    return src->route? 1 : -1;
                }
            }
        }
        // Void hitted ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(src != nullptr) 
        {
            src->collapse();
        }
        src = nullptr;
    }

    else if(mb == cell::settings::mb::rmb)
    {
        auto stamp = canvas.get(x, y); 

        if(stamp)
        {
            // Socket is hitted ///////////////////////////////////////////////////////////////////////////////////////////////////
            int i = (stamp>>24) - 1;
            {
                // Already connected? /////////////////////////////////////////////////////////////////////////////////////////////
                if(io[i].on)
                {
                    io[i].collapse();
                    io[i].to->collapse();
                    disconnect(&io[i], io[i].to);
                    return 0;
                }
            }
        }
        // Void hitted ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(src != nullptr) 
        {
            src->collapse();
        }
        src = nullptr;

    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HitTest UP /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int patchbay::up_test(const float& x, const float& y, const int& mb)
{
    auto stamp = canvas.get(x, y); 

    if(stamp && src)
    {
        // Socket is hitted ///////////////////////////////////////////////////////////////////////////////////////////////////
        int i = (stamp>>24) - 1;
        {
            // Self hit ///////////////////////////////////////////////////////////////////////////////////////////////////////
            if(src == &io[i])
            {
                src->collapse();
                src = nullptr;
                return 0;
            }
            // Socket of same type is hitted //////////////////////////////////////////////////////////////////////////////////
            if(src->route == io[i].route)
            {
                src->collapse();
                src = nullptr;
                return 0;
            }
            // Other socket is hitted /////////////////////////////////////////////////////////////////////////////////////////
            else
            {
                // Temprary source is free ////////////////////////////////////////////////////////////////////////////////////
                if(src == nullptr) 
                {
                    return 0;
                }
                else
                {
                    // Already connected? /////////////////////////////////////////////////////////////////////////////////////
                    if(io[i].on)
                    {
                        if(src->route != io[i].route)
                        {
                            io[i].collapse();
                            io[i].to->collapse();
                            disconnect(&io[i], io[i].to);
                            connect(src, &io[i]);
                            src = nullptr;
                            return 1;
                        }
                        src = nullptr;
                        return 0;
                    }
                    // Connecting /////////////////////////////////////////////////////////////////////////////////////////////
                    else
                    {
                        if(src->route != io[i].route)
                        {
                            connect(src, &io[i]);
                            src = nullptr;
                            return 1;
                        }
                        src = nullptr;
                        return 0;
                    }
                }
            }
        }
    }

    // Void hitted ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(src!=nullptr) 
    {
        src->collapse();
    }
    
    src = nullptr;
    
    return 0;
}

void patchbay::deselect()
{
    for(int i = 0; i < nodes; i++) io[i].cord.focused = false;
}

void patchbay::move_test(const float& x, const float& y, const int& mb)
{
    auto stamp = canvas.get(x, y);
    if(stamp)
    {
        int p = (stamp>>24) - 1;
        if(io[p].on)
        {
            io[p].cord.focused = true;
            io[p].to->cord.focused = true;

        }
    }
    else deselect();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Drag cords /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void patchbay::drag(const float& x, const float& y)
{
    if(src != nullptr)
    {
        src->drag(x, y);
    }
}


patchbay::patchbay(const int& w, const int& h, const int& ins, const int& outs): canvas(w, h), matrix(w, h), nodes(ins + outs), inputs(ins), outputs(outs)
{
    io = new socket[nodes];

    for(int i = 0; i < nodes; i++)
    {
        io[i].w = &canvas.width;
        io[i].h = &canvas.height;
    }
    canvas.clr(0);
    matrix.clr(false);
}

patchbay::~patchbay()
{
    src = nullptr;
    delete[] io;
}

void patchbay::set_socket(const point<int>* o, const int& radius, const unsigned& id, const bool& route, const int& p)
{
    int pos = (id>>24) - 1;
    if(pos >= 0)
    {
        io[pos].bounds.xCentre = o->x;
        io[pos].bounds.yCentre = o->y;
        io[pos].bounds.radius  = radius;
        io[pos].id = id;
        io[pos].pos = p;
        io[pos].route = route;
        io[pos].data = &zero;
        io[pos].collapse();
    }
}



};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
