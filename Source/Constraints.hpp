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
#include <map>
#include <sys/types.h>
#include <unordered_map>
#include "module_headers.hpp"


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
/////////////////////////////////////////////////////////////////////////////
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


	{ interface::button_list::env_a, 		{ rectangle<float> {55 + 38 * 0, 434 , 16, 16}, "env_a", "Env A", 0xFE, true, false, button_descriptor::type_t::A } }, 
	{ interface::button_list::env_b, 		{ rectangle<float> {55 + 38 * 1, 434 , 16, 16}, "env_b", "Env B", 0xFE, true, false, button_descriptor::type_t::A } }, 
	{ interface::button_list::env_c, 		{ rectangle<float> {55 + 38 * 2, 434 , 16, 16}, "env_c", "Env C", 0xFE, true, false, button_descriptor::type_t::A } }, 
	{ interface::button_list::env_d, 		{ rectangle<float> {55 + 38 * 3, 434 , 16, 16}, "env_d", "Env D", 0xFE, true, false, button_descriptor::type_t::A } }, 

	{ interface::button_list::envs, 		{ rectangle<float> {88 + 38 * 4, 579 , 12, 12}, "envs", "Envelopes", 0xFF, true, false, button_descriptor::type_t::B } }, 


};


const std::map<interface::parameter_list, parameter_descriptor> parameter_list
{
	{ interface::parameter_list::vco_a_oct, 		{ "vco_a_oct",     "VCO A Octave", 0.0f, 1.0f, 0.0f } }, 
	{ interface::parameter_list::vco_b_oct, 		{ "vco_b_oct",     "VCO B Octave", 0.0f, 1.0f, 0.0f } }, 
	{ interface::parameter_list::vco_c_oct, 		{ "vco_c_oct",     "VCO C Octave", 0.0f, 1.0f, 0.0f } }, 
	{ interface::parameter_list::vco_d_oct, 		{ "vco_d_oct",     "VCO D Octave", 0.0f, 1.0f, 0.0f } }, 

//	{ interface::parameter_list::vco_a_freerun, 	{ "vco_a_freerun", "VCO A Freerun", 0.0f, 1.0f, 0.0f } }, 
//	{ interface::parameter_list::vco_b_freerun, 	{ "vco_b_freerun", "VCO B Freerun", 0.0f, 1.0f, 0.0f } }, 
//	{ interface::parameter_list::vco_c_freerun, 	{ "vco_c_freerun", "VCO C Freerun", 0.0f, 1.0f, 0.0f } }, 
//	{ interface::parameter_list::vco_d_freerun, 	{ "vco_d_freerun", "VCO D Freerun", 0.0f, 1.0f, 0.0f } }, 
	
 //   { interface::parameter_list::vco_a_form,    	{ "vco_a_form",    "VCO A Form", 0.0f, 2.0f, 0.0f } }, 
//	{ interface::parameter_list::vco_b_form,    	{ "vco_b_form",    "VCO B Form", 0.0f, 2.0f, 0.0f } }, 
//	{ interface::parameter_list::vco_c_form,    	{ "vco_c_form",    "VCO C Form", 0.0f, 2.0f, 0.0f } }, 
//	{ interface::parameter_list::vco_d_form,    	{ "vco_d_form",    "VCO D Form", 0.0f, 2.0f, 0.0f } }, 


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
    int sck_offset_x    = 10;
    int sck_offset_y    = 8;
    int sck_gap_x       = 19;
    int sck_gap_y       = 15;
/**********************************************************************************************************************
 * 
 *  Socket positions
 * 
 **********************************************************************************************************************/
const std::map<uint32_t, cell::point2d<int>> socket_pos
{

                                                                             
};
