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
#include <map>
#include <unordered_map>
#include "cell/oscillator.hpp"
#include "cell/containers.hpp"
#include "cell/interface.h"
#include "cell/envelopes.hpp"

#define SOCKET_RADIUS 5
using namespace cell;

struct slider_descriptor
{
	enum type_t { A, B, Encoder };
	rectangle<float> pos;
	std::string id;
	std::string name;
	float min;
	float max;
    float interval;
    float skew;
    bool  symmetric;
	float def;
	type_t type;
};

struct button_descriptor
{
	enum type_t { A, B };
	rectangle<float> pos;
	std::string id;
	std::string name;
	int  radio_id;
	bool toggle;
	bool def;
	type_t type;
};

struct parameter_descriptor
{
	std::string id;
	std::string name;
	float min;
	float max;
	float def;
};

struct main_window_constraints
{
	int width       = 1060;
    int height      = 596;
    rectangle<float> lcd_display  	{ 820, 36, 196, 176 };
    rectangle<float> patchbay 		{ 35, 238, 926, 198 };
    rectangle<float> env_display 	{ 40, 460, 200, 100 };
};

struct window
{
    cell::point2d<float>     pot_a { 48, 48 };
    cell::point2d<float>     pot_b { 32, 32 };

    cell::rectangle<float> vco_a { 26,  6, 152, 304 };
    cell::rectangle<float> vco_b { 20, 20, 100, 200 };
    cell::rectangle<float> vco_c { 20, 20, 100, 200 };
    cell::rectangle<float> vco_d { 20, 20, 100, 200 };
    
    int sck_offset_x    = 10;
    int sck_offset_y    = 8;
    int sck_gap_x       = 19;
    int sck_gap_y       = 15;

};

const window mw;    // Main window

// inline rectangle<float> grid_at(cell::point<float> ratio, cell::rectangle<float> frame, cell::point<float> size)
// {
//     return rectangle<float>
//     {
//         roundf(frame.x + frame.w * ratio.x - size.x * 0.5f),
//         roundf(frame.y + frame.h * ratio.y - size.y * 0.5f),
//         roundf(size.x),
//         roundf(size.y)
//     };
// }



#define d_w 200.0f - 11.0f
#define d_h 100.0f - 11.0f

const std::map<interface::potentiometer_list, slider_descriptor> slider_list
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// VCOs ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ interface::potentiometer_list::vco_a_fine,  	{ rectangle<float> {41 + 76 * 0, 32, 48, 48 }, "vco_a_fine", "VCO A Detune", 0.0f, 1.0f, 0.001f, 1.0f, true, 0.5f, slider_descriptor::type_t::A } }, 

	{ interface::potentiometer_list::vco_b_fine, 	{ rectangle<float> {41 + 76 * 2, 32, 48, 48 }, "vco_b_fine", "VCO B Detune", 0.0f, 1.0f, 0.001f, 1.0f, true, 0.5f, slider_descriptor::type_t::A } }, // 1
	{ interface::potentiometer_list::vco_c_fine, 	{ rectangle<float> {41 + 76 * 4, 32, 48, 48 }, "vco_c_fine", "VCO C Detune", 0.0f, 1.0f, 0.001f, 1.0f, true, 0.5f, slider_descriptor::type_t::A } }, // 2
	{ interface::potentiometer_list::vco_d_fine, 	{ rectangle<float> {41 + 76 * 6, 32, 48, 48 }, "vco_d_fine", "VCO D Detune", 0.0f, 1.0f, 0.001f, 1.0f, true, 0.5f, slider_descriptor::type_t::A } }, // 3

    { interface::potentiometer_list::vco_a_warp, 	{ rectangle<float> {41 + 76 * 1, 32, 48, 48 }, "vco_a_warp", "VCO A Warp",   0.0f, 1.0f, 0.001f, 0.2f, false, 0.0f, slider_descriptor::type_t::A } }, // 4
	{ interface::potentiometer_list::vco_b_warp, 	{ rectangle<float> {41 + 76 * 3, 32, 48, 48 }, "vco_b_warp", "VCO B Warp",   0.0f, 1.0f, 0.001f, 0.2f, false, 0.0f, slider_descriptor::type_t::A } }, // 5
	{ interface::potentiometer_list::vco_c_warp, 	{ rectangle<float> {41 + 76 * 5, 32, 48, 48 }, "vco_c_warp", "VCO C Warp",   0.0f, 1.0f, 0.001f, 0.2f, false, 0.0f, slider_descriptor::type_t::A } }, // 6
	{ interface::potentiometer_list::vco_d_warp, 	{ rectangle<float> {41 + 76 * 7, 32, 48, 48 }, "vco_d_warp", "VCO D Warp",   0.0f, 1.0f, 0.001f, 0.2f, false, 0.0f, slider_descriptor::type_t::A } }, // 7

	{ interface::potentiometer_list::vco_a_pw, 		{ rectangle<float> {41 + 76 * 0, 108, 48, 48 }, "vco_a_pw", "VCO A PW",      0.0f, 1.0f, 0.0001f, 1.3f, true,  0.5f, slider_descriptor::type_t::A } }, // 8
	{ interface::potentiometer_list::vco_b_pw, 		{ rectangle<float> {41 + 76 * 2, 108, 48, 48 }, "vco_b_pw", "VCO B PW",      0.0f, 1.0f, 0.0001f, 1.3f, true,  0.5f, slider_descriptor::type_t::A } }, // 9
	{ interface::potentiometer_list::vco_c_pw, 		{ rectangle<float> {41 + 76 * 4, 108, 48, 48 }, "vco_c_pw", "VCO C PW",      0.0f, 1.0f, 0.0001f, 1.3f, true,  0.5f, slider_descriptor::type_t::A } }, // 10
	{ interface::potentiometer_list::vco_d_pw, 		{ rectangle<float> {41 + 76 * 6, 108, 48, 48 }, "vco_d_pw", "VCO D PW",      0.0f, 1.0f, 0.0001f, 1.3f, true,  0.5f, slider_descriptor::type_t::A } }, // 11

	{ interface::potentiometer_list::vco_a_fm, 		{ rectangle<float> {41 + 76 * 1, 108, 48, 48 }, "vco_a_fm", "VCO A FM",      0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::A } }, // 12
	{ interface::potentiometer_list::vco_b_fm, 		{ rectangle<float> {41 + 76 * 3, 108, 48, 48 }, "vco_b_fm", "VCO B FM",      0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::A } }, // 13
	{ interface::potentiometer_list::vco_c_fm, 		{ rectangle<float> {41 + 76 * 5, 108, 48, 48 }, "vco_c_fm", "VCO C FM",      0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::A } }, // 14
	{ interface::potentiometer_list::vco_d_fm, 		{ rectangle<float> {41 + 76 * 7, 108, 48, 48 }, "vco_d_fm", "VCO D FM",      0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::A } }, // 15

	{ interface::potentiometer_list::vco_a_am, 		{ rectangle<float> {49 + 76 * 0, 177, 32, 32 }, "vco_a_am", "VCO A AM",      0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, 
	{ interface::potentiometer_list::vco_b_am, 		{ rectangle<float> {49 + 76 * 2, 177, 32, 32 }, "vco_b_am", "VCO B AM",      0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 17
	{ interface::potentiometer_list::vco_c_am, 		{ rectangle<float> {49 + 76 * 4, 177, 32, 32 }, "vco_c_am", "VCO C AM",      0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 18
	{ interface::potentiometer_list::vco_d_am, 		{ rectangle<float> {49 + 76 * 6, 177, 32, 32 }, "vco_d_am", "VCO D AM",      0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 19

	{ interface::potentiometer_list::vco_a_amp, 	{ rectangle<float> {49 + 76 * 1, 177, 32, 32 }, "vco_a_amp", "VCO A Amp",    0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 20
	{ interface::potentiometer_list::vco_b_amp, 	{ rectangle<float> {49 + 76 * 3, 177, 32, 32 }, "vco_b_amp", "VCO B Amp",    0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 21
	{ interface::potentiometer_list::vco_c_amp, 	{ rectangle<float> {49 + 76 * 5, 177, 32, 32 }, "vco_c_amp", "VCO C Amp",    0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 22
	{ interface::potentiometer_list::vco_d_amp, 	{ rectangle<float> {49 + 76 * 7, 177, 32, 32 }, "vco_d_amp", "VCO D Amp",    0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 23
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DSystems ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ interface::potentiometer_list::chs_a_warp, 	{ rectangle<float> {41 + 76 * 8, 108, 48, 48 }, "chs_a_warp", "CHS A Warp",  0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::A } }, // 15
	{ interface::potentiometer_list::chs_b_warp, 	{ rectangle<float> {41 + 76 * 9, 108, 48, 48 }, "chs_b_warp", "CHS B Warp",  0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::A } }, // 15

	{ interface::potentiometer_list::chs_a_tune,  	{ rectangle<float> {41 + 76 * 8,  32, 48, 48 }, "chs_a_tune", "CHS A Tune",  0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::A } }, // 3
	{ interface::potentiometer_list::chs_b_tune,  	{ rectangle<float> {41 + 76 * 9,  32, 48, 48 }, "chs_b_tune", "CHS B Tune",  0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::A } }, // 3

	{ interface::potentiometer_list::chs_a_amp, 	{ rectangle<float> {49 + 76 * 8, 177, 32, 32 }, "chs_a_amp", "CHS A Amp",    0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 23
	{ interface::potentiometer_list::chs_b_amp, 	{ rectangle<float> {49 + 76 * 9, 177, 32, 32 }, "chs_b_amp", "CHS B Amp",    0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 23
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// VCFs ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ interface::potentiometer_list::vcf_a_cut, 	{ rectangle<float> {49 + 76 * 5, 458, 32, 32 }, "vcf_a_cut", "VCF A Cut",    0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 24
	{ interface::potentiometer_list::vcf_a_res, 	{ rectangle<float> {49 + 76 * 5, 518, 32, 32 }, "vcf_a_res", "VCF A Res",    0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 25

	{ interface::potentiometer_list::vcf_b_cut, 	{ rectangle<float> {49 + 76 * 6, 458, 32, 32 }, "vcf_b_cut", "VCF B Cut",    0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 26
	{ interface::potentiometer_list::vcf_b_res, 	{ rectangle<float> {49 + 76 * 6, 518, 32, 32 }, "vcf_b_res", "VCF B Res",    0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 27
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// VCAs ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ interface::potentiometer_list::vca_a, 		{ rectangle<float> {49 + 76 * 3, 458, 32, 32 }, "vca_a_amp", "VCA A Amp",    0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 28 256
	{ interface::potentiometer_list::vca_b, 		{ rectangle<float> {49 + 76 * 3, 518, 32, 32 }, "vca_b_amp", "VCA B Amp",    0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 29
	{ interface::potentiometer_list::vcd_time, 		{ rectangle<float> {49 + 76 * 4, 458, 32, 32 }, "vcd_time", "VCD Time",	     0.01f,1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 30
	{ interface::potentiometer_list::vcd_feed, 		{ rectangle<float> {49 + 76 * 4, 518, 32, 32 }, "vcd_feed", "VCD Feed",      0.0f, 1.0f, 0.001f, 2.0f, false,  0.0f, slider_descriptor::type_t::B } }, // 31
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Rotor //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ interface::potentiometer_list::angle_x, 		{ rectangle<float> {794 + 38 * 0, 458, 32, 32 }, "angle_x", "Angle X",       0.0f, tao, 0.001f, 1.0f, false,  0.0f, slider_descriptor::type_t::Encoder } }, // 32
	{ interface::potentiometer_list::angle_y, 		{ rectangle<float> {794 + 38 * 1, 458, 32, 32 }, "angle_y", "Angle Y",       0.0f, tao, 0.001f, 1.0f, false,  0.0f, slider_descriptor::type_t::Encoder } }, // 33
	{ interface::potentiometer_list::angle_z, 		{ rectangle<float> {794 + 38 * 2, 458, 32, 32 }, "angle_z", "Angle Z",       0.0f, tao, 0.001f, 1.0f, false,  0.0f, slider_descriptor::type_t::Encoder  } }, // 34
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 3 to 2 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ interface::potentiometer_list::lc_to_l, 		{ rectangle<float> {981, 288, 32, 32 },          "lc_to_l", "LC to L",       0.0f, 1.0f, 0.001f, 1.0f, false,  0.0f, slider_descriptor::type_t::Encoder } }, 		// 35
	{ interface::potentiometer_list::cr_to_r, 		{ rectangle<float> {981, 323, 32, 32 },          "cr_to_r", "CR to R",       0.0f, 1.0f, 0.001f, 1.0f, false,  0.0f, slider_descriptor::type_t::Encoder } }, 		// 36
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Main Volume ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ interface::potentiometer_list::volume, 		{ rectangle<float> {972, 450, 48, 48 },          "volume", "Volume",         0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::A } }, 		// 37
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SNH ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ interface::potentiometer_list::snh_a, 		{ rectangle<float> {49 + 76 * 7, 458, 32, 32 },  "snh_a", "SNH A Time",  	 0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 28 256
	{ interface::potentiometer_list::snh_b, 		{ rectangle<float> {49 + 76 * 7, 518, 32, 32 },  "snh_b", "SNH B Time",  	 0.0f, 1.0f, 0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 29
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LFOs ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ interface::potentiometer_list::lfo_a_freq, 	{ rectangle<float> {49 + 76 * 8, 458, 32, 32 },  "lfo_a_freq", "LFO A Freq", 0.001f, 1.0f, 0.001f, 0.2f, false,  0.01f, slider_descriptor::type_t::B } }, // 28 256
	{ interface::potentiometer_list::lfo_a_am, 		{ rectangle<float> {49 + 76 * 8, 518, 32, 32 },  "lfo_a_am", "LFO A Am",  	 0.0f, 1.0f,   0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 29

	{ interface::potentiometer_list::lfo_b_freq, 	{ rectangle<float> {49 + 76 * 9, 458, 32, 32 },  "lfo_b_freq", "LFO B Freq", 0.001f, 1.0f, 0.001f, 0.2f, false,  0.01f, slider_descriptor::type_t::B } }, // 28 256
	{ interface::potentiometer_list::lfo_b_am, 		{ rectangle<float> {49 + 76 * 9, 518, 32, 32 },  "lfo_b_am", "LFO B Am",  	 0.0f, 1.0f,   0.001f, 0.2f, false,  0.0f, slider_descriptor::type_t::B } }, // 29
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Buttons ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::map<interface::button_list, button_descriptor> button_list
{
	{ interface::button_list::soft_e, 		{ rectangle<float> {796,  77, 16, 16}, 			"soft_e", "Soft E", 0x00, false, false, button_descriptor::type_t::A } }, 

	{ interface::button_list::soft_f, 		{ rectangle<float> {796, 107, 16, 16}, 			"soft_f", "Soft F", 0x00, false, false, button_descriptor::type_t::A } }, 
	{ interface::button_list::soft_g, 		{ rectangle<float> {796, 137, 16, 16}, 			"soft_g", "Soft G", 0x00, false, false, button_descriptor::type_t::A } }, 
	{ interface::button_list::soft_h, 		{ rectangle<float> {796, 167, 16, 16}, 			"soft_h", "Soft H", 0x00, false, false, button_descriptor::type_t::A } }, 

	{ interface::button_list::vco_a, 		{ rectangle<float> {164      ,  5, 12, 12}, 	"vco_a", "VCO A", 0xFF, true, false, button_descriptor::type_t::B } }, 
	{ interface::button_list::vco_b, 		{ rectangle<float> {164 + 152,  5, 12, 12}, 	"vco_b", "VCO B", 0xFF, true, false, button_descriptor::type_t::B } }, 
	{ interface::button_list::vco_c, 		{ rectangle<float> {164 + 304,  5, 12, 12}, 	"vco_c", "VCO C", 0xFF, true, false, button_descriptor::type_t::B } }, 
	{ interface::button_list::vco_d, 		{ rectangle<float> {164 + 456,  5, 12, 12}, 	"vco_d", "VCO D", 0xFF, true, false, button_descriptor::type_t::B } }, 

	{ interface::button_list::chs_a, 		{ rectangle<float> {164 + 588,  5, 12, 12}, 	"chs_a", "Chaos A", 0xFF, true, false, button_descriptor::type_t::B } }, 
	{ interface::button_list::chs_b, 		{ rectangle<float> {164 + 608,  5, 12, 12}, 	"chs_b", "Chaos B", 0xFF, true, false, button_descriptor::type_t::B } }, 

	{ interface::button_list::lfo_a, 		{ rectangle<float> {155+ 541,  579, 12, 12}, 	"lfo_a", "LFO A", 0xFF, true, false, button_descriptor::type_t::B } }, 
	{ interface::button_list::lfo_b, 		{ rectangle<float> {155+ 617,  579, 12, 12}, 	"lfo_b", "LFO B", 0xFF, true, false, button_descriptor::type_t::B } }, 

	{ interface::button_list::soft_a, 		{ rectangle<float> {865,  223, 16, 16}, 		"soft_a", "Soft A", 0x00, false, false, button_descriptor::type_t::A } }, 
	{ interface::button_list::soft_b, 		{ rectangle<float> {895,  223, 16, 16}, 		"soft_b", "Soft B", 0x00, false, false, button_descriptor::type_t::A } }, 
	{ interface::button_list::soft_c, 		{ rectangle<float> {925,  223, 16, 16}, 		"soft_c", "Soft C", 0x00, false, false, button_descriptor::type_t::A } }, 
	{ interface::button_list::soft_d, 		{ rectangle<float> {955,  223, 16, 16}, 		"soft_d", "Soft D", 0x00, false, false, button_descriptor::type_t::A } }, 

	{ interface::button_list::env_a, 		{ rectangle<float> {55 + 38 * 0, 434 , 16, 16}, "env_a", "Env A", 0xFE, true, false, button_descriptor::type_t::A } }, 
	{ interface::button_list::env_b, 		{ rectangle<float> {55 + 38 * 1, 434 , 16, 16}, "env_b", "Env B", 0xFE, true, false, button_descriptor::type_t::A } }, 
	{ interface::button_list::env_c, 		{ rectangle<float> {55 + 38 * 2, 434 , 16, 16}, "env_c", "Env C", 0xFE, true, false, button_descriptor::type_t::A } }, 
	{ interface::button_list::env_d, 		{ rectangle<float> {55 + 38 * 3, 434 , 16, 16}, "env_d", "Env D", 0xFE, true, false, button_descriptor::type_t::A } }, 

	{ interface::button_list::envs, 		{ rectangle<float> {88 + 38 * 4, 579 , 12, 12}, "envs", "Envelopes", 0xFF, true, false, button_descriptor::type_t::B } }, 

	{ interface::button_list::menu, 		{ rectangle<float> {795,  224, 16, 16},         "menu", "Menu", 0xFF, true, false, button_descriptor::type_t::B } }, 
	{ interface::button_list::scope, 		{ rectangle<float> {989,  224, 16, 16},         "scope", "Scope", 0xFF, true, false, button_descriptor::type_t::B } }, 
};


const std::map<interface::parameter_list, parameter_descriptor> parameter_list
{
	{ interface::parameter_list::vco_a_oct, 		{ "vco_a_oct",     "VCO A Octave", 0.0f, 1.0f, 0.0f } }, 
	{ interface::parameter_list::vco_b_oct, 		{ "vco_b_oct",     "VCO B Octave", 0.0f, 1.0f, 0.0f } }, 
	{ interface::parameter_list::vco_c_oct, 		{ "vco_c_oct",     "VCO C Octave", 0.0f, 1.0f, 0.0f } }, 
	{ interface::parameter_list::vco_d_oct, 		{ "vco_d_oct",     "VCO D Octave", 0.0f, 1.0f, 0.0f } }, 

	{ interface::parameter_list::vco_a_freerun, 	{ "vco_a_freerun", "VCO A Freerun", 0.0f, 1.0f, 0.0f } }, 
	{ interface::parameter_list::vco_b_freerun, 	{ "vco_b_freerun", "VCO B Freerun", 0.0f, 1.0f, 0.0f } }, 
	{ interface::parameter_list::vco_c_freerun, 	{ "vco_c_freerun", "VCO C Freerun", 0.0f, 1.0f, 0.0f } }, 
	{ interface::parameter_list::vco_d_freerun, 	{ "vco_d_freerun", "VCO D Freerun", 0.0f, 1.0f, 0.0f } }, 
	
    { interface::parameter_list::vco_a_form,    	{ "vco_a_form",    "VCO A Form", 0.0f, 2.0f, 0.0f } }, 
	{ interface::parameter_list::vco_b_form,    	{ "vco_b_form",    "VCO B Form", 0.0f, 2.0f, 0.0f } }, 
	{ interface::parameter_list::vco_c_form,    	{ "vco_c_form",    "VCO C Form", 0.0f, 2.0f, 0.0f } }, 
	{ interface::parameter_list::vco_d_form,    	{ "vco_d_form",    "VCO D Form", 0.0f, 2.0f, 0.0f } }, 


	{ interface::parameter_list::chs_a_form, 		{ "chs_a_form",    "Chaos A Form", 0.0f, 3.0f, 0.0f } }, 
	{ interface::parameter_list::chs_b_form, 		{ "chs_b_form",    "Chaos B Form", 0.0f, 3.0f, 0.0f } }, 

	{ interface::parameter_list::lfo_a_form, 		{ "lfo_a_form",    "LFO A Form", 0.0f, 4.0f, 0.0f } }, 
	{ interface::parameter_list::lfo_b_form, 		{ "lfo_b_form",    "LFO B Form", 0.0f, 4.0f, 0.0f } }, 

	{ interface::parameter_list::env_a_scale, 		{ "env_a_scale",   "Envelope A scale", 0.0f, 1.0f, 0.2f } }, 
	{ interface::parameter_list::env_b_scale, 		{ "env_b_scale",   "Envelope B scale", 0.0f, 1.0f, 0.2f } }, 
	{ interface::parameter_list::env_c_scale, 		{ "env_c_scale",   "Envelope C scale", 0.0f, 1.0f, 0.2f } }, 
	{ interface::parameter_list::env_d_scale, 		{ "env_d_scale",   "Envelope D scale", 0.0f, 1.0f, 0.2f } }, 

	{ interface::parameter_list::env_a_form_a, 		{ "env_a_form_a",  "Envelope A form A", 0.0f, 3.0f, 1.0f } }, 
	{ interface::parameter_list::env_a_form_h, 		{ "env_a_form_h",  "Envelope A form H", 0.0f, 3.0f, 2.0f } }, 
	{ interface::parameter_list::env_a_form_d, 		{ "env_a_form_d",  "Envelope A form D", 0.0f, 3.0f, 1.0f } }, 
	{ interface::parameter_list::env_a_form_s, 		{ "env_a_form_s",  "Envelope A form S", 0.0f, 3.0f, 2.0f } }, 
	{ interface::parameter_list::env_a_form_r, 		{ "env_a_form_r",  "Envelope A form R", 0.0f, 3.0f, 1.0f } }, 

	{ interface::parameter_list::env_b_form_a, 		{ "env_b_form_a",  "Envelope B form A", 0.0f, 3.0f, 1.0f } }, 
	{ interface::parameter_list::env_b_form_h, 		{ "env_b_form_h",  "Envelope B form H", 0.0f, 3.0f, 2.0f } }, 
	{ interface::parameter_list::env_b_form_d, 		{ "env_b_form_d",  "Envelope B form D", 0.0f, 3.0f, 1.0f } }, 
	{ interface::parameter_list::env_b_form_s, 		{ "env_b_form_s",  "Envelope B form S", 0.0f, 3.0f, 2.0f } }, 
	{ interface::parameter_list::env_b_form_r, 		{ "env_b_form_r",  "Envelope B form R", 0.0f, 3.0f, 1.0f } }, 

	{ interface::parameter_list::env_c_form_a, 		{ "env_c_form_a",  "Envelope C form A", 0.0f, 3.0f, 1.0f } }, 
	{ interface::parameter_list::env_c_form_h, 		{ "env_c_form_h",  "Envelope C form H", 0.0f, 3.0f, 2.0f } }, 
	{ interface::parameter_list::env_c_form_d, 		{ "env_c_form_d",  "Envelope C form D", 0.0f, 3.0f, 1.0f } }, 
	{ interface::parameter_list::env_c_form_s, 		{ "env_c_form_s",  "Envelope C form S", 0.0f, 3.0f, 2.0f } }, 
	{ interface::parameter_list::env_c_form_r, 		{ "env_c_form_r",  "Envelope C form R", 0.0f, 3.0f, 1.0f } }, 

	{ interface::parameter_list::env_d_form_a, 		{ "env_d_form_a",  "Envelope D form A", 0.0f, 3.0f, 1.0f } }, 
	{ interface::parameter_list::env_d_form_h, 		{ "env_d_form_h",  "Envelope D form H", 0.0f, 3.0f, 2.0f } }, 
	{ interface::parameter_list::env_d_form_d, 		{ "env_d_form_d",  "Envelope D form D", 0.0f, 3.0f, 1.0f } }, 
	{ interface::parameter_list::env_d_form_s, 		{ "env_d_form_s",  "Envelope D form S", 0.0f, 3.0f, 2.0f } }, 
	{ interface::parameter_list::env_d_form_r, 		{ "env_d_form_r",  "Envelope D form R", 0.0f, 3.0f, 1.0f } }, 

	{ interface::parameter_list::env_a_time_a, 		{ "env_a_time_a",  "Envelope A time A", 0.0f, d_w, 30.0f } }, 
	{ interface::parameter_list::env_a_time_h, 		{ "env_a_time_h",  "Envelope A time H", 0.0f, d_w, 60.0f } }, 
	{ interface::parameter_list::env_a_time_d, 		{ "env_a_time_d",  "Envelope A time D", 0.0f, d_w, 90.0f } }, 
	{ interface::parameter_list::env_a_time_s, 		{ "env_a_time_s",  "Envelope A time S", 0.0f, d_w,120.0f } }, 
	{ interface::parameter_list::env_a_time_r, 		{ "env_a_time_r",  "Envelope A time R", 0.0f, d_w,150.0f } }, 
                                                                                                                 //
	{ interface::parameter_list::env_b_time_a, 		{ "env_b_time_a",  "Envelope B time A", 0.0f, d_w, 30.0f } }, 
	{ interface::parameter_list::env_b_time_h, 		{ "env_b_time_h",  "Envelope B time H", 0.0f, d_w, 60.0f } }, 
	{ interface::parameter_list::env_b_time_d, 		{ "env_b_time_d",  "Envelope B time D", 0.0f, d_w, 90.0f } }, 
	{ interface::parameter_list::env_b_time_s, 		{ "env_b_time_s",  "Envelope B time S", 0.0f, d_w,120.0f } }, 
	{ interface::parameter_list::env_b_time_r, 		{ "env_b_time_r",  "Envelope B time R", 0.0f, d_w,150.0f } }, 

	{ interface::parameter_list::env_c_time_a, 		{ "env_c_time_a",  "Envelope C time A", 0.0f, d_w, 30.0f } }, 
	{ interface::parameter_list::env_c_time_h, 		{ "env_c_time_h",  "Envelope C time H", 0.0f, d_w, 60.0f } }, 
	{ interface::parameter_list::env_c_time_d, 		{ "env_c_time_d",  "Envelope C time D", 0.0f, d_w, 90.0f } }, 
	{ interface::parameter_list::env_c_time_s, 		{ "env_c_time_s",  "Envelope C time S", 0.0f, d_w,120.0f } }, 
	{ interface::parameter_list::env_c_time_r, 		{ "env_c_time_r",  "Envelope C time R", 0.0f, d_w,150.0f } }, 

	{ interface::parameter_list::env_d_time_a, 		{ "env_d_time_a",  "Envelope D time A", 0.0f, d_w, 30.0f } }, 
	{ interface::parameter_list::env_d_time_h, 		{ "env_d_time_h",  "Envelope D time H", 0.0f, d_w, 60.0f } }, 
	{ interface::parameter_list::env_d_time_d, 		{ "env_d_time_d",  "Envelope D time D", 0.0f, d_w, 90.0f } }, 
	{ interface::parameter_list::env_d_time_s, 		{ "env_d_time_s",  "Envelope D time S", 0.0f, d_w,120.0f } }, 
	{ interface::parameter_list::env_d_time_r, 		{ "env_d_time_r",  "Envelope D time R", 0.0f, d_w,150.0f } }, 

	{ interface::parameter_list::env_a_value_a, 	{ "env_a_value_a", "Envelope A value A", 0.0f, d_h, 90.0f } }, 
	{ interface::parameter_list::env_a_value_h, 	{ "env_a_value_h", "Envelope A value H", 0.0f, d_h, 70.0f } }, 
	{ interface::parameter_list::env_a_value_d, 	{ "env_a_value_d", "Envelope A value D", 0.0f, d_h, 40.0f } }, 
	{ interface::parameter_list::env_a_value_s, 	{ "env_a_value_s", "Envelope A value S", 0.0f, d_h, 20.0f } }, 
    { interface::parameter_list::env_a_value_r, 	{ "env_a_value_r", "Envelope A value R", 0.0f, d_h, 20.0f } }, 

	{ interface::parameter_list::env_b_value_a, 	{ "env_b_value_a", "Envelope B value A", 0.0f, d_h, 90.0f } }, 
	{ interface::parameter_list::env_b_value_h, 	{ "env_b_value_h", "Envelope B value H", 0.0f, d_h, 70.0f } }, 
	{ interface::parameter_list::env_b_value_d, 	{ "env_b_value_d", "Envelope B value D", 0.0f, d_h, 40.0f } }, 
	{ interface::parameter_list::env_b_value_s, 	{ "env_b_value_s", "Envelope B value S", 0.0f, d_h, 20.0f } }, 
    { interface::parameter_list::env_b_value_r, 	{ "env_b_value_r", "Envelope B value R", 0.0f, d_h, 20.0f } }, 
                                                                                                            
	{ interface::parameter_list::env_c_value_a, 	{ "env_c_value_a", "Envelope C value A", 0.0f, d_h, 90.0f } }, 
	{ interface::parameter_list::env_c_value_h, 	{ "env_c_value_h", "Envelope C value H", 0.0f, d_h, 70.0f } }, 
	{ interface::parameter_list::env_c_value_d, 	{ "env_c_value_d", "Envelope C value D", 0.0f, d_h, 40.0f } }, 
	{ interface::parameter_list::env_c_value_s, 	{ "env_c_value_s", "Envelope C value S", 0.0f, d_h, 20.0f } }, 
    { interface::parameter_list::env_c_value_r, 	{ "env_c_value_r", "Envelope C value R", 0.0f, d_h, 20.0f } }, 

	{ interface::parameter_list::env_d_value_a, 	{ "env_d_value_a", "Envelope D value A", 0.0f, d_h, 90.0f } }, 
	{ interface::parameter_list::env_d_value_h, 	{ "env_d_value_h", "Envelope D value H", 0.0f, d_h, 70.0f } }, 
	{ interface::parameter_list::env_d_value_d, 	{ "env_d_value_d", "Envelope D value D", 0.0f, d_h, 40.0f } }, 
	{ interface::parameter_list::env_d_value_s, 	{ "env_d_value_s", "Envelope D value S", 0.0f, d_h, 20.0f } }, 
    { interface::parameter_list::env_d_value_r, 	{ "env_d_value_r", "Envelope D value R", 0.0f, d_h, 20.0f } }, 

	{ interface::parameter_list::scope_type, 		{ "scope_type",    "Scope Type",         0.0f, 1.0f, 1.0f } }, 
	{ interface::parameter_list::scope_scale, 		{ "scope_scale",   "Scope Scale",        0.01f, 1.0f, 0.5f } }, 
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Socket positions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

const int col[7] = { 8, 38, 68, 98, 128, 158, 188 };

inline cell::point2d<int> place_socket(const window* w, int x, int y)
{
    return cell::point2d<int>
    {
        w->sck_offset_x + w->sck_gap_x * x,
        w->sck_offset_y + w->sck_gap_y * y
    };
}


const std::map<interface::socket_list, cell::point2d<int>> socket_pos
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Inputs /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ interface::socket_list::vco_a_in_pw, place_socket(&mw,  0,  0) },  

	{ interface::socket_list::vco_a_in_ec, place_socket(&mw,  2,  0) }, 
	{ interface::socket_list::vco_a_in_sh, place_socket(&mw,  4,  0) },
	{ interface::socket_list::vco_a_in_fm, place_socket(&mw,  6,  0) },

	{ interface::socket_list::vco_b_in_pw, place_socket(&mw,  8,  0) },
	{ interface::socket_list::vco_b_in_ec, place_socket(&mw, 10,  0) },
	{ interface::socket_list::vco_b_in_sh, place_socket(&mw, 12,  0) },
	{ interface::socket_list::vco_b_in_fm, place_socket(&mw, 14,  0) },

	{ interface::socket_list::vco_c_in_pw, place_socket(&mw, 16,  0) },
	{ interface::socket_list::vco_c_in_ec, place_socket(&mw, 18,  0) },
	{ interface::socket_list::vco_c_in_sh, place_socket(&mw, 20,  0) },
	{ interface::socket_list::vco_c_in_fm, place_socket(&mw, 22,  0) },

	{ interface::socket_list::vco_d_in_pw, place_socket(&mw, 24,  0) },
	{ interface::socket_list::vco_d_in_ec, place_socket(&mw, 26,  0) },
	{ interface::socket_list::vco_d_in_sh, place_socket(&mw, 28,  0) },
	{ interface::socket_list::vco_d_in_fm, place_socket(&mw, 30,  0) },

	{ interface::socket_list::chs_a_in_ec, place_socket(&mw, 32,  0) },
	{ interface::socket_list::chs_a_in_wm, place_socket(&mw, 34,  0) },
	{ interface::socket_list::chs_b_in_ec, place_socket(&mw, 36,  0) },
	{ interface::socket_list::chs_b_in_wm, place_socket(&mw, 38,  0) },
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LCR ins ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ interface::socket_list::lcr_in_l_a, 	cell::point2d<int> 	{14 + 38 * 20, col[3] }},  
	{ interface::socket_list::lcr_in_c_a, 	cell::point2d<int> 	{14 + 38 * 21, col[3] }},  
	{ interface::socket_list::lcr_in_r_a, 	cell::point2d<int> 	{14 + 38 * 22, col[3] }}, 

	{ interface::socket_list::lcr_in_l_b, 	cell::point2d<int> 	{14 + 38 * 20, col[2] }},  
	{ interface::socket_list::lcr_in_c_b, 	cell::point2d<int> 	{14 + 38 * 21, col[2] }},
	{ interface::socket_list::lcr_in_r_b, 	cell::point2d<int> 	{14 + 38 * 22, col[2] }},

	{ interface::socket_list::lcr_out_l_a, 	cell::point2d<int> 	{33 + 38 * 20, col[5] }},  
	{ interface::socket_list::lcr_out_c_a, 	cell::point2d<int> 	{33 + 38 * 21, col[5] }},
	{ interface::socket_list::lcr_out_r_a, 	cell::point2d<int> 	{33 + 38 * 22, col[5] }},

	{ interface::socket_list::lcr_out_l_b, 	cell::point2d<int> 	{33 + 38 * 20, col[4] }},  
	{ interface::socket_list::lcr_out_c_b, 	cell::point2d<int> 	{33 + 38 * 21, col[4] }},
	{ interface::socket_list::lcr_out_r_b, 	cell::point2d<int> 	{33 + 38 * 22, col[4] }},

	{ interface::socket_list::lcr_cv_l, 	cell::point2d<int> 	{14 + 38 * 20, col[6] }},  
	{ interface::socket_list::lcr_cv_c, 	cell::point2d<int> 	{14 + 38 * 21, col[6] }},
	{ interface::socket_list::lcr_cv_r, 	cell::point2d<int> 	{14 + 38 * 22, col[6] }},

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Mix ins ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ interface::socket_list::mix_in_l, 	cell::point2d<int> 	{918,  53}},  
	{ interface::socket_list::mix_in_c, 	cell::point2d<int> 	{918,  83}},  
	{ interface::socket_list::mix_in_r, 	cell::point2d<int> 	{918, 113}}, 

	{ interface::socket_list::mix_lc_cv, 	cell::point2d<int> 	{888,  col[2] }},
	{ interface::socket_list::mix_cr_cv, 	cell::point2d<int> 	{888,  col[3] }},

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// VCFs ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ interface::socket_list::vcf_a_in_a,   place_socket(&mw, 20, 10) },  
	{ interface::socket_list::vcf_a_in_b,  	place_socket(&mw, 20,  8) },  
	{ interface::socket_list::vcf_a_in_c,  	place_socket(&mw, 20,  6) },  

	{ interface::socket_list::vcf_a_out_lp, place_socket(&mw, 22, 10) },  
	{ interface::socket_list::vcf_a_out_bp, place_socket(&mw, 22,  8) },  
	{ interface::socket_list::vcf_a_out_hp, place_socket(&mw, 22,  6) },  

	{ interface::socket_list::vcf_b_in_a,  	place_socket(&mw, 24, 10) },  
	{ interface::socket_list::vcf_b_in_b,  	place_socket(&mw, 24,  8) },  
	{ interface::socket_list::vcf_b_in_c,  	place_socket(&mw, 24,  6) },  

	{ interface::socket_list::vcf_b_out_lp, place_socket(&mw, 26, 10) },  
	{ interface::socket_list::vcf_b_out_bs, place_socket(&mw, 26,  8) },  
	{ interface::socket_list::vcf_b_out_hp, place_socket(&mw, 26,  6) },  

	{ interface::socket_list::vcf_a_cut_in, place_socket(&mw, 20, 12) },  
	{ interface::socket_list::vcf_a_q_in, 	place_socket(&mw, 22, 12) },  
	{ interface::socket_list::vcf_b_cut_in, place_socket(&mw, 24, 12) },  
	{ interface::socket_list::vcf_b_q_in, 	place_socket(&mw, 26, 12) },  


	{ interface::socket_list::vco_a_in_am, 	place_socket(&mw,  1,  2) },  
	{ interface::socket_list::vco_a_out, 	place_socket(&mw,  5,  2) }, 
	{ interface::socket_list::vco_b_in_am, 	place_socket(&mw,  9,  2) },
	{ interface::socket_list::vco_b_out,   	place_socket(&mw, 13,  2) },
	{ interface::socket_list::vco_c_in_am, 	place_socket(&mw, 17,  2) },
	{ interface::socket_list::vco_c_out,   	place_socket(&mw, 21,  2) },
	{ interface::socket_list::vco_d_in_am, 	place_socket(&mw, 25,  2) },
	{ interface::socket_list::vco_d_out,   	place_socket(&mw, 29,  2) },

	{ interface::socket_list::chs_a_out_x, 	place_socket(&mw, 33,  2) },
    { interface::socket_list::chs_a_out_y, 	place_socket(&mw, 33,  4) },
	{ interface::socket_list::chs_a_out_z, 	place_socket(&mw, 33,  6) },
                                                                                         
	{ interface::socket_list::chs_b_out_x, 	place_socket(&mw, 37,  2) },
	{ interface::socket_list::chs_b_out_y, 	place_socket(&mw, 37,  4) },
	{ interface::socket_list::chs_b_out_z, 	place_socket(&mw, 37,  6) },
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Envelopes //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ interface::socket_list::env_a_out, 	place_socket(&mw,  2, 10) },
	{ interface::socket_list::env_b_out, 	place_socket(&mw,  4, 10) },
	{ interface::socket_list::env_c_out, 	place_socket(&mw,  6, 10) },
	{ interface::socket_list::env_d_out, 	place_socket(&mw,  8, 10) },
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// VCAs ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ interface::socket_list::vca_a_in_a, 	place_socket(&mw, 12,  6) },
	{ interface::socket_list::vca_b_in_a, 	place_socket(&mw, 14,  6) },
	{ interface::socket_list::vcd_in_a, 	place_socket(&mw, 16,  6) },
	{ interface::socket_list::vcd_in_b, 	place_socket(&mw, 18,  6) },

	{ interface::socket_list::vca_a_in_b, 	place_socket(&mw, 12,  4) },
	{ interface::socket_list::vca_b_in_b, 	place_socket(&mw, 14,  4) },
	{ interface::socket_list::vcd_in_c, 	place_socket(&mw, 16,  4) },
	{ interface::socket_list::vcd_in_d, 	place_socket(&mw, 18,  4) },

	{ interface::socket_list::vca_a_out_a, 	place_socket(&mw, 12,  8) },
	{ interface::socket_list::vca_b_out_a, 	place_socket(&mw, 14,  8) },
	{ interface::socket_list::vcd_out_a, 	place_socket(&mw, 16,  8) },
	{ interface::socket_list::vcd_out_b,    place_socket(&mw, 18,  8) },

	{ interface::socket_list::vca_a_out_b, 	place_socket(&mw, 12, 10) },
	{ interface::socket_list::vca_b_out_b, 	place_socket(&mw, 14, 10) },
	{ interface::socket_list::vcd_out_c, 	place_socket(&mw, 16, 10) },
	{ interface::socket_list::vcd_out_d, 	place_socket(&mw, 18, 10) },

	{ interface::socket_list::vca_a_in_cv, 	place_socket(&mw, 12, 12) },
	{ interface::socket_list::vca_b_in_cv, 	place_socket(&mw, 14, 12) },
	{ interface::socket_list::vcd_in_time, 	place_socket(&mw, 16, 12) },
	{ interface::socket_list::vcd_in_feed, 	place_socket(&mw, 18, 12) },
	/// SNH
	{ interface::socket_list::snh_a_time, 	place_socket(&mw, 28, 12) },
	{ interface::socket_list::snh_a_out,  	place_socket(&mw, 28, 10) },
	{ interface::socket_list::snh_a_in_b,  	place_socket(&mw, 28,  8) },
	{ interface::socket_list::snh_a_in_a,  	place_socket(&mw, 28,  6) },

	{ interface::socket_list::snh_b_time, 	place_socket(&mw, 30, 12) },
	{ interface::socket_list::snh_b_out, 	place_socket(&mw, 30, 10) },
	{ interface::socket_list::snh_b_in_b, 	place_socket(&mw, 30,  8) },
	{ interface::socket_list::snh_b_in_a, 	place_socket(&mw, 30,  6) },
	/// LFOs
	{ interface::socket_list::lfo_a_in_freq,place_socket(&mw, 32, 12) },
	{ interface::socket_list::lfo_a_in_am,  place_socket(&mw, 34, 12) },

	{ interface::socket_list::lfo_a_out_a, 	place_socket(&mw, 33,  8) }, 
	{ interface::socket_list::lfo_a_out_b, 	place_socket(&mw, 33, 10) },

	{ interface::socket_list::lfo_b_in_freq,place_socket(&mw, 36, 12) },
	{ interface::socket_list::lfo_b_in_am, 	place_socket(&mw, 38, 12) },

	{ interface::socket_list::lfo_b_out_a,  place_socket(&mw, 37,  8) },
	{ interface::socket_list::lfo_b_out_b,  place_socket(&mw, 37, 10) },

	{ interface::socket_list::p_wheel, 	    place_socket(&mw,  1,  4) },
	{ interface::socket_list::m_wheel, 	    place_socket(&mw,  1,  6) }, 
                                                                          
    { interface::socket_list::sum_a_in_a,   place_socket(&mw,  4,  4) },
    { interface::socket_list::sum_a_in_b,   place_socket(&mw,  4,  6) },
    { interface::socket_list::sum_a_out_a,  place_socket(&mw,  6,  4) },
    { interface::socket_list::sum_a_out_b,  place_socket(&mw,  6,  6) },

    { interface::socket_list::sum_b_in_a,   place_socket(&mw,  8,  4) },
    { interface::socket_list::sum_b_in_b,   place_socket(&mw,  8,  6) },
    { interface::socket_list::sum_b_out_a,  place_socket(&mw, 10,  4) },
    { interface::socket_list::sum_b_out_b,  place_socket(&mw, 10,  6) },

    { interface::socket_list::pct_in_a,     place_socket(&mw,  4,  8) },
    { interface::socket_list::pct_in_b,     place_socket(&mw,  6,  8) },
    { interface::socket_list::pct_in_c,     place_socket(&mw,  8,  8) },
    { interface::socket_list::pct_out_a,    place_socket(&mw, 10,  8) },


};
