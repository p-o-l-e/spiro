#include "control.hpp"
#include "modules/interface/descriptor.hxx"

namespace core {
    namespace svg {

    const Control parse_control(const tinyxml2::XMLElement* rect)
    {
        Rectangle<float> constrain 
        {
            rect->FloatAttribute("x"),
            rect->FloatAttribute("y"),
            rect->FloatAttribute("width"),
            rect->FloatAttribute("height")
        };

        const char* postfix = rect->Attribute("data-name");

        static const std::unordered_map<std::string, Control::type> typeMap = 
        {
            {"slider",    Control::slider},
            {"button",    Control::button},
            {"parameter", Control::parameter},
            {"input",     Control::input},
            {"output",    Control::output},
            {"pin",       Control::pin}
        };

        Control::type parameter = Control::parameter;
        if (const char* parameter_name = rect->Attribute("data-type")) 
        {
            auto it = typeMap.find(parameter_name);
            if (it != typeMap.end()) parameter = it->second;
        }

        const float min  = rect->FloatAttribute("data-min", 0.0f);
        const float max  = rect->FloatAttribute("data-max", 1.0f);
        const float def  = rect->FloatAttribute("data-def", 0.0f);
        const float skew = rect->FloatAttribute("data-skew", 0.5f);
        const float step = rect->FloatAttribute("data-step", 0.0f);

        const unsigned int flag = rect->UnsignedAttribute("data-flag", 0);
        const int          rad  = rect->IntAttribute("data-radio", 0);
        const bool         sym  = rect->BoolAttribute("data-symmetric", false);

        return 
        { 
            parameter,
            { 
                constrain.x, constrain.y, constrain.w, constrain.h 
            },
            postfix ? postfix : "",
            min, max, def, skew, step,
            rad, sym, flag 
        };
    }


    std::vector<std::string> split(const char* s) 
    {
        std::vector<std::string> tokens;
        if (!s) return tokens;
        std::string str(s);
        size_t start = 0, end;
        
        while ((end = str.find(',', start)) != std::string::npos) 
        {
            tokens.push_back(str.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(str.substr(start));
        return tokens;
    }


    const Descriptor parse_descriptor(const std::string& data)
    {
        tinyxml2::XMLDocument doc;
        if (doc.Parse(data.c_str()) != tinyxml2::XML_SUCCESS)
        {
            throw std::runtime_error("Failed to parse SVG string");
        }

        const auto* svg  = doc.FirstChildElement("svg");
        if (!svg) throw std::runtime_error("No <svg> root element");
        
        const auto* meta = svg->FirstChildElement("metadata");
        if (!meta) throw std::runtime_error("No <metadata> element");

        auto safeAttr = [&](const tinyxml2::XMLElement* elem, const char* key) -> std::string 
        {
            const char* v = elem->Attribute(key);
            return v ? std::string(v) : std::string();
        };

        map::module::type moduleType = map::module::off;

        if (const char* moduleAttr = meta->Attribute("data-module")) 
        {
            auto it = module_type_map.find(moduleAttr);
            if (it != module_type_map.end()) moduleType = it->second;
        }

        static std::string prefix = safeAttr(meta, "data-prefix");

        static int ic = meta->IntAttribute("data-ic", 0);
        static int oc = meta->IntAttribute("data-oc", 0);
        static int cc = meta->IntAttribute("data-cc", 0);

        static Rectangle<float> constrain 
        {
            0.0f, 0.0f,
            svg->FloatAttribute("width", 0.0f),
            svg->FloatAttribute("height", 0.0f)
        };

        static std::vector<Control> controls;
        controls.clear();

        for (auto* rect = svg->FirstChildElement("rect"); rect; rect = rect->NextSiblingElement("rect")) 
        {
            Control ctl = parse_control(rect);
            if (ctl.postfix.empty())
            {
                throw std::runtime_error("Missing data-name on <rect>");
            }
            controls.push_back(std::move(ctl));
        }

        auto ctlNames = split(safeAttr(meta, "data-ctl").c_str());
        auto cviNames = split(safeAttr(meta, "data-cvi").c_str());
        auto cvoNames = split(safeAttr(meta, "data-cvo").c_str());

        if ((int)ctlNames.size() != cc ||
            (int)cviNames.size() != ic ||
            (int)cvoNames.size() != oc)
        {
            throw std::runtime_error("Metadata counts do not match enum name lists");
        }

        auto lookup = [&](const std::string& name) -> const Control* 
        {
            for (auto& c : controls) 
            {
                if (c.postfix == name) return &c;
            }
            throw std::runtime_error("Missing control: " + name);
        };

        const Control* set_i = lookup(cviNames[0]);
        const Control* set_o = lookup(cvoNames[0]);
        const Control* set_c = cc > 0 ? lookup(ctlNames[0]) : nullptr;

        Descriptor descriptor 
        {
            moduleType,
            { &ic, &oc, &cc },
            &prefix,
            { set_i, set_o, set_c },
            &constrain
        };
        dump_descriptor(descriptor, "[parse]");
        return descriptor;
    }

}
}




