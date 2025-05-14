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
        switch(static_cast<core::map::cv::index>(core::extract_byte(hash, uid_t::shift::m_index)))
        {
        case core::map::cv::index::c:
                for(int i = 0; i < cc; ++i) if(hash_table[map::cv::c][i] == hash) return i;    
                break;
        case core::map::cv::index::i:
                for(int i = 0; i < ic; ++i) if(hash_table[map::cv::i][i] == hash) return i;    
                break;
        case core::map::cv::index::o:
                for(int i = 0; i < oc; ++i) if(hash_table[map::cv::o][i] == hash) return i;    
                break;
            default:
                break;
        }
        return error::invalid_index;
    }

    const std::unique_ptr<int[]> Blueprint::set_relatives(const Descriptor* d) const
    {
        int n = settings::modules_n;
        auto r = std::make_unique<int[]>(n);

        for(int s = 0; s < n; ++s)
        {
            int pos = 0;
            core::map::module::type car = d[s].type;
            r[s] = pos;

            for(int i = s + 1; i < n; ++i)
            {
                if(car == d[i].type) 
                {
                    r[i] = ++pos;
                    ++s;
                }
            }
        }
        #ifdef DEBUG 
            std::cout<<"---- Relatives set...\n";
        #endif
        return r;
    }

    void Blueprint::calculate_hash()
    {
        hash_table[map::cv::c] = std::make_unique<uint32_t[]>(cc);
        hash_table[map::cv::i] = std::make_unique<uint32_t[]>(ic);
        hash_table[map::cv::o] = std::make_unique<uint32_t[]>(oc);
        for(int j = 0; j < map::cv::count; ++j)
        {
            for(int m = 0, h = 0; m < mc; ++m)
            {
                for(int i = 0; i < *descriptor[m].cv[j]; ++i)
                {
                    hash_table[j][h] = core::uid_t::encode_uid(descriptor[m].type, static_cast<map::cv::index>(j), relative[m], i);
                    ++h;
                }
            }
        }
        #ifdef DEBUG 
            std::cout<<"---- Hash calculated...\n";
        #endif
    }


    const int Blueprint::count(const core::map::cv::index& p, const Descriptor* d) const
    {
        int c { 0 };
        for(int i = 0; i < settings::modules_n; ++i) c += *d[i].cv[p];
        return c;
    }

    Blueprint::Blueprint(const Descriptor* d): 
        descriptor(d), 
        relative(set_relatives(d)),
        mc(settings::modules_n), 
        ic(count(map::cv::index::i, d)),
        oc(count(map::cv::index::o, d)),
        cc(count(map::cv::index::c, d))
    { 
        calculate_hash();
        #ifdef DEBUG 
            std::cout<<"-- Blueprint created...\n";
        #endif
    };
}
}
