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
#include "blueprint.hpp"
#include "uid.hpp"
#include <iostream>

namespace core
{
namespace interface
{
    const uint32_t Blueprint::get_hash(const map::cv::index& i, const int& p) const
    {
        return hash_table[i][p];
    }


    const int Blueprint::get_index(const uint32_t& hash) const
    {
        auto j = static_cast<core::map::cv::index>(core::extract_byte(hash, uid_t::shift::m_index));
        for(int i = 0; i < ccv[j]; ++i) if(hash_table[j][i] == hash) return i;    
        return error::invalid_index;
    }

    const std::unique_ptr<int[]> Blueprint::set_relatives(const Descriptor** d) const
    {
        LOG("Blueprint::set_relatives() :");
        int  n = settings::sectors;
        auto r = std::make_unique<int[]>(n);

        for(int s = 0; s < n; ++s)
        {
            int pos = 0;
            core::map::module::type car = d[s]->type;
            r[s] = pos;

            for(int i = s + 1; i < n; ++i)
            {
                if(car == d[i]->type) 
                {
                    r[i] = ++pos;
                    ++s;
                }
            }
        }
        LOG("---- Relatives set...");
        return r;
    }

    void Blueprint::calculate_hash()
    {
        LOG("Blueprint::calculate_hash() : ");
        for(int index = 0; index < map::cv::count; ++index)
        {
            hash_table[index] = std::make_unique<uint32_t[]>(ccv[index]);
            for(int m = 0, h = 0; m < mc; ++m)
            {
                for(int i = 0; i < *descriptor[m]->cv[index]; ++i)
                {
                    hash_table[index][h] = core::uid_t::encode_uid(descriptor[m]->type, static_cast<map::cv::index>(index), relative[m], i);
                    ++h;
                }
            }
        }
        LOG("---- Hash calculated...");
    }


    const int Blueprint::count(const core::map::cv::index& p, const Descriptor** d) const
    {
        int c { 0 };
        for(int i = 0; i < settings::sectors; ++i) c += *d[i]->cv[p];
        return c;
    }

    // Blueprint::Blueprint(): 
    //     descriptor(nullptr), 
    //     relative(set_relatives(d)),
    //     mc(settings::modules_n), 
    //     ccv[map::cv::i](count(map::cv::i, d)),
    //     ccv[map::cv::o](count(map::cv::o, d)),
    //     ccv[map::cv::c](count(map::cv::c, d)) {}


    Blueprint::Blueprint(const Descriptor** d): 
        descriptor { d }, 
        relative { set_relatives(d) },
        mc { settings::sectors }, 
        ccv { count(map::cv::i, d), count(map::cv::o, d), count(map::cv::c, d) }
    { 
        LOG("Blueprint : ");
        calculate_hash();
        LOG("-- Blueprint created...");
    };
}
}
