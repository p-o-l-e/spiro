
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

#include "grid.hpp"
#include "modules/interface/descriptor.hxx"
#include "uid.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

namespace core
{
    Grid::Grid(const Sector* s, const int size): 
    sector(s), sectors(size), relative(setRelatives(s)), elements(countElements(s)), modules(countModules(s)),
    controlMap(calculateControlMap(s)), idMap(calculateIdMap(s))
    {
        for(int i = 0; i < Control::count; ++i) indices[i] = std::make_unique<uint32_t[]>(elements[i]);
        calculateUIDs();
    };

    void Grid::calculateUIDs()
    {
        int count[Control::count];
        for(int i = 0; i < Control::count; ++i) count[i] = 0;

        for(int i = 0; i < sectors; ++i)
        {
            auto mp = relative[i];
            for(int pt = 0; pt < map::cv::count; ++pt)
            {
                for(int pp = 0; pp < *sector[i].descriptor->cv[pt]; ++pp)
                {
                    auto t = sector[i].descriptor->set[pt][pp].is;
                    indices[t][count[t]] = encode_uid(sector[i].descriptor->type, mp, static_cast<map::cv::index>(pt), pp);
                    ++count[t];
                }
            }
        }
    }

    const int Grid::getIndex(const uid_t& uid) const
    {
        uint32_t hash = encode_uid(uid);
        auto t = control(uid)->is;
        for(int i = 0; i < elements[t]; ++i)
        {
            if(indices[t][i] == hash) return i;
        }
        return -1;
    }

    const int Grid::getIndex(const uint32_t hash) const
    {
        auto uid = decode_uid(hash);
        auto t = control(uid)->is;
        for(int i = 0; i < elements[t]; ++i)
        {
            if(indices[t][i] == hash) return i;
        }
        return -1;
    }

    const Control* Grid::control(const uid_t& uid) const
    {
        return controlMap->find(encode_uid(uid))->second;
    }

    const std::unique_ptr<std::unordered_map<uint32_t, const Control*>> Grid::calculateControlMap(const Sector* d) const
    {
        auto r = std::make_unique<std::unordered_map<uint32_t, const Control*>>();
        for(int i = 0; i < settings::sectors; ++i)
        {
            for(int pt = 0; pt < core::map::cv::count; ++pt)
            {
                for(int pp = 0; pp < *d[i].descriptor->cv[pt]; ++pp)
                {   
                    uid_t uid { d[i].descriptor->type, relative[i], pt, pp };
                    r->emplace(encode_uid(uid), &d[i].descriptor->set[pt][pp]);
                }

            }
        }
        return r;
    }
    
    const std::unique_ptr<std::unordered_map<uint32_t, std::pair<std::string, std::string>>> Grid::calculateIdMap(const Sector* d) const
    {
        auto r = std::make_unique<std::unordered_map<uint32_t, std::pair<std::string, std::string>>>();

        auto caps = [](const std::string& s) { return std::string(1, std::toupper(s[0])) + s.substr(1); };

        for(int i = 0; i < settings::sectors; ++i)
        {
            for(int pt = 0; pt < core::map::cv::count; ++pt)
            {
                for(int pp = 0; pp < *d[i].descriptor->cv[pt]; ++pp)
                {
                    core::uid_t uid { d[i].descriptor->type, relative[i], pt, pp };

                    std::string id {};
                    std::string name {};

                    id += *d[i].descriptor->prefix + "_";
                    id += std::to_string(relative[i]) + "_";
                    id += d[i].descriptor->set[pt][pp].postfix;

                    name += caps(*d[i].descriptor->prefix) + " ";
                    name += std::string(1, relative[i] + 'A') + " ";
                    name += caps(d[i].descriptor->set[pt][pp].postfix);

                    std::pair<std::string, std::string> idName { id, name };

                    r->emplace(encode_uid(uid), idName);
                }
            }
        }
        return r;
 
    }


    const Sector* Grid::getSector(const map::module::type& mt, const int mp) const
    {
        for(int i = 0; i < sectors; ++i)
        {
            if(sector[i].descriptor->type == mt)
            {
                if(relative[i] == mp) return &sector[i];
            }
        }
        return nullptr;
    }

    const uid_t Grid::getUID(const int index, const Control::type& type) const
    {
        return decode_uid(indices[type][index]);
    }

    const uint32_t Grid::getHash(const int index, const Control::type& type) const
    {
        return indices[type][index];
    }

    const Rectangle<float> Grid::getBounds(const uid_t& uid) const
    {
        Rectangle<float> bounds { 0, 0, 0, 0 };
        for(int i = 0; i < sectors; ++i)
        {
            if(sector[i].descriptor->type == static_cast<map::module::type>(uid.mt))
            {
                if(relative[i] == uid.mp) 
                {
                    bounds.x = sector[i].offset.x + sector[i].descriptor->set[uid.pt][uid.pp].constrain.x;
                    bounds.y = sector[i].offset.y + sector[i].descriptor->set[uid.pt][uid.pp].constrain.y;
                    bounds.w = sector[i].descriptor->set[uid.pt][uid.pp].constrain.w;
                    bounds.h = sector[i].descriptor->set[uid.pt][uid.pp].constrain.h;
                    return bounds;
                }
            }
        }
        return bounds;
    }

    const std::string Grid::name(const uid_t& uid, const bool snake) const
    {
        return snake ? idMap->find(encode_uid(uid))->second.first : idMap->find(encode_uid(uid))->second.second;
    }


    const std::unique_ptr<int[]> Grid::setRelatives(const Sector* d) const
    {
        int  n = settings::sectors;
        auto r = std::make_unique<int[]>(n);
        bool check[n];
        for(int i = 0; i < n; ++i) check[i] = false;

        for(int s = 0; s < n; ++s)
        {
            int pos = 0;
            auto carry = d[s].descriptor->type;

            for(int i = s; i < n; ++i)
            {
                if(!check[i])
                {
                    if(carry == d[i].descriptor->type) 
                    {
                        r[i] = pos;
                        check[i] = true;
                        ++pos;
                    }
                }
            }
        }
        return r;
    }
    
    const std::unique_ptr<int[]> Grid::countElements(const Sector* d) const
    {
        int  length = settings::sectors;
        auto r = std::make_unique<int[]>(Control::count);
        for(int i = 0; i < Control::count; ++i) r[i] = 0;
        for(int i = 0; i < length; ++i)
        {
            for(int t = 0; t < map::cv::count; ++t)
            {
                for(int e = 0; e < *d[i].descriptor->cv[t]; ++e)
                {   
                    ++r[d[i].descriptor->set[t][e].is];
                }
            }
        }
        return r;
    }

    const std::unique_ptr<int[]> Grid::countModules(const Sector* d) const
    {
        auto r = std::make_unique<int[]>(map::module::count);
        for(int i = 0; i < map::module::count; ++i) r[i] = 0;
        int length = settings::sectors;

        for(int i = 0; i < length; ++i)
        {
            ++r[d[i].descriptor->type];
        }
        return r;
    }

    const Grid grid(settings::sector_map, settings::sectors);   
}


