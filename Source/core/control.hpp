#pragma once
#include "modules/interface/descriptor.hxx"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdlib>

#include "tools/tinyxml2.h"

namespace core {
    namespace svg {

     static const std::unordered_map<core::map::module::type, const char*> module_names {
        { core::map::module::type::off, "off" },
        { core::map::module::type::lfo, "lfo" },
        { core::map::module::type::vco, "vco" },
        // ... add other types
    };

    static inline const char* mod_name(core::map::module::type t) {
        if (auto it = module_names.find(t); it != module_names.end()) return it->second;
        return "unknown";
    }


    static void dump_control(const Control* c, const char* label, const std::string &px ) {
        std::cout << px << label << ":\n";
        std::cout << px << "  type: " << static_cast<int>(c->is) << "\n";
        std::cout << px << "  id: "   << (c->postfix.empty() ? "<empty>" : c->postfix) << "\n";
        std::cout << px << "  bounds: x=" << c->constrain.x << " y=" << c->constrain.y
                  << px << " w=" << c->constrain.w << " h=" << c->constrain.h << "\n";
        std::cout << px << "  range: min=" << c->min << " max=" << c->max
                  << px << " def=" << c->def << " skew=" << c->skew << " step=" << c->step << "\n";
        std::cout << px << "  radio=" << c->radio_id << " symmetric=" << (c->symmetric ? "true" : "false")
                  << px << " flag=0x" << std::hex << c->flag << std::dec << "\n";
    }



    static void dump_descriptor(const core::Descriptor& d, const std::string& px)
    {
        using core::map::cv;

        std::cout << px << "Descriptor:\n";
        std::cout << px << "- module: " << mod_name(d.type) << " : " << (int)d.type << "\n";
        std::cout << px << "- counts: ic=" << *d.cv[cv::i] << " oc=" << *d.cv[cv::o] << " cc=" << *d.cv[cv::c] << "\n";
        std::cout << px << "- prefix: " << *d.prefix << "\n";
        std::cout << px << "- constrain: x=" << d.constrain->x << " y=" << d.constrain->y
                  << px << " w=" << d.constrain->w << " h=" << d.constrain->h << "\n";

        dump_control(d.set[cv::i], "first CV-in", px);
        dump_control(d.set[cv::o], "first CV-out", px);
        dump_control(d.set[cv::c], "first Control", px);

        std::cout << px << "- CV-in set (" << *d.cv[cv::i] << "):\n";
        for (int i = 0; i < *d.cv[cv::i]; ++i) dump_control(&d.set[cv::i][i], ("i[" + std::to_string(i) + "]").c_str(), px);

        std::cout << px << "- CV-out set (" << *d.cv[cv::o] << "):\n";
        for (int i = 0; i < *d.cv[cv::o]; ++i) dump_control(&d.set[cv::o][i], ("o[" + std::to_string(i) + "]").c_str(), px);

        std::cout << px << "- Control set (" << d.cv[cv::c] << "):\n";
        for (int i = 0; i < *d.cv[cv::c]; ++i) dump_control(&d.set[cv::c][i], ("c[" + std::to_string(i) + "]").c_str(), px);
    }

    static const std::unordered_map<std::string, map::module::type> module_type_map = {
        {"off", map::module::off},
        {"vco", map::module::vco},
        {"lfo", map::module::lfo},
        {"cso", map::module::cso},
        {"vca", map::module::vca},
        {"vcd", map::module::vcd},
        {"vcf", map::module::vcf},
        {"snh", map::module::snh},
        {"sum", map::module::sum},
        {"pdt", map::module::pdt},
        {"rtr", map::module::rtr},
        {"mix", map::module::mix},
        {"env", map::module::env},
        {"mod", map::module::mod},
        {"com", map::module::com},
        {"cro", map::module::cro}
    };

    const Control parse_control(const tinyxml2::XMLElement*);
    std::vector<std::string> split(const char*); 
    const Descriptor parse_descriptor(const std::string&);
    
}
}
