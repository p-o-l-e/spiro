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

#pragma once

#include "utility.hpp"
#include "node.hpp"

namespace core 
{

    #define n_forms_chaotic 4
    inline const char* wforms_chaotic[] = { "SPROTT", "HELMHOLZ", "HALVORSEN", "TSUCS" };

    class cso_t: public Module
    { 
        private:
            static int idc;
            float f[9];
            Limiter limiter;

            void sprott_reset();
            void helmholz_reset();
            void halvorsen_reset();
            void tsucs_reset();

            bool _reset = true; // Reset flag

            void (cso_t::*reset[4])() = 
            { 
                &cso_t::sprott_reset,
                &cso_t::helmholz_reset,
                &cso_t::halvorsen_reset,
                &cso_t::tsucs_reset
            };
            
            void sprott();
            void helmholz();
            void halvorsen();
            void tsucs();

            void (cso_t::*form[4])() = 
            { 
                &cso_t::sprott,
                &cso_t::helmholz,
                &cso_t::halvorsen,
                &cso_t::tsucs
            };


        public:
            const int id = 0;
            void process() override;
            void switch_wave(const int&);

            cso_t();
           ~cso_t() {};
    }; 
} // Namespace core
