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
#pragma once
#include <cstdint>
#include "curves.hpp"
#include "shapes.hpp"
#include "primitives.hpp"
#include "canvas.hpp"
#include "constants.hpp"
#include <string>
#include <atomic>

#define SOCKET_IN       1
#define SOCKET_OUT      0
#define DISCONNECTED   -1

namespace core {

    namespace settings {
        enum mb { lmb = 16, mmb = 64, rmb = 32 };
    };


   /***************************************************************************************************************************
    * 
    *  Patchcord
    * 
    **************************************************************************************************************************/
    struct Patchcord
    {
        Point2D<float>* data;               // Rendered spline
        Point2D<float> spline[4];           // Control points
        const int segments = 4;             // # Segments
        const int iterations;               // Precision
        bool focused = false;
        constexpr void process();           // Fill spline data
        Patchcord(int);
       ~Patchcord();
    };

   /***************************************************************************************************************************
    * 
    *  Socket
    * 
    **************************************************************************************************************************/
    struct Socket
    {
        const unsigned* w;
        const unsigned* h;

        Patchcord cord;
        RadialSquare<unsigned> bounds { 0, 0, 0 };
        unsigned id = 0;
        int pos;                                            // Array position
        bool route = 0;                                     // 0: Output - 1: Input
        bool on = false;                                    // Is connected ?
        Socket* to = nullptr;

        std::atomic<float>* data  = &zero;                  // Output
        std::atomic<float>** com  = &data;                  // Pointer to module input pointer
        constexpr void collapse();                          // Collapse to centre
        constexpr void drag(const float&, const float&);
        Socket(int);
       ~Socket();
    };

   /***************************************************************************************************************************
    * 
    *  Patchbay
    * 
    **************************************************************************************************************************/
    class Patchbay
    {
        private:
            Socket* src = nullptr;              // Armed source
            Socket* dst = nullptr;              // Armed destination
            int counter = 0;

        public:
            const int get_index(const uint32_t&) const;
            void  connect(Socket*, Socket*);
            void  disconnect(Socket*, Socket*);

            Canvas<unsigned>    canvas;         // Hit test layer
            Canvas<bool>        matrix;         // Connections matrix
            const int           nodes;          // Number of sockets
            Socket*             io;

            const int inputs;
            const int outputs;
            
            void set_socket(const Point2D<int>*, const int&, const uint32_t&, const bool&, const int&);
            void drag(const float&, const float&);
            void draw();
            int  down_test(const float&, const float&, const int&);
            int  up_test(const float&, const float&, const int&);
            void move_test(const float&, const float&, const int&);
            void deselect();
            void clear();

            Patchbay(const int&, const int&, const int&, const int&);
           ~Patchbay();
    };

};

