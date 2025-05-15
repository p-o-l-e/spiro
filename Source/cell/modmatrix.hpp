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
#include <vector>
#include "curves.hpp"
#include "containers.hpp"
#include "primitives.hpp"
#include <iostream>
#include <string>
#include <atomic>
#include <map>

#include "interface.h"

using std::vector;

#define SOCKET_IN       1
#define SOCKET_OUT      0
#define DISCONNECTED   -1
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace cell {

    namespace settings {
        enum mb { lmb = 16, mmb = 64, rmb = 32 };
    };


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PatchCord //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
    struct patchcord
    {
        point2d<float>* data;               // Rendered spline
        point2d<float> spline[4];           // Control points
        const int segments = 4;             // # Segments
        const int iterations;               // Precision
        bool focused = false;
        constexpr void process();           // Fill spline data
        patchcord(int);
       ~patchcord();
    };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Socket Data ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    struct socket
    {
        const unsigned* w;
        const unsigned* h;

        patchcord cord;
        square<unsigned> bounds { 0, 0, 0 };
        // ID: |32----------------|24----------------|16----------------|08----------------
        //     |Unique Number     |Empty             |Empty             |Grid Position
        unsigned id = 0;
        int pos;                                            // Array position
        bool route = 0;                                     // 0: Output - 1: Input
        bool on = false;                                    // Is connected ?
        socket* to = nullptr;

        std::atomic<float>* data  = &zero;                  // Output
        std::atomic<float>** com  = &data;                  // Pointer to module input pointer
        constexpr void collapse();                          // Collapse to centre
        constexpr void drag(const float&, const float&);    // Drag
        socket(int);
       ~socket();
    };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Patchbay ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    class patchbay
    {
        private:
            socket* src = nullptr;          // Armed source
            socket* dst = nullptr;          // Armed destination
            
        public:
            void  connect(socket*, socket*);
            void  disconnect(socket*, socket*);

            frame<unsigned> canvas;         // Hit test layer
            frame<bool>     matrix;         // Connections matrix
            const int       nodes;          // Number of sockets
            socket*         io;             // Sockets array

            int inputs, outputs;            // Number of inpunts and outputs
            
            void set_socket(const point2d<int>*, const int&, const unsigned&, const bool&, const int&);
            void drag(const float&, const float&);
            void draw();
            int  down_test(const float&, const float&, const int&);
            int  up_test(const float&, const float&, const int&);
            void move_test(const float&, const float&, const int&);
            void deselect();
            void clear();

            patchbay(const int&, const int&, const int&, const int&);
           ~patchbay();
    };

};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
