#include "blueprint.hpp"
namespace core
{
namespace interface
{
    const std::unique_ptr<int[]> blueprint_t::set_relatives(const descriptor_t* d)
    {
        int n = settings::n_modules();
        auto r = std::make_unique<int[]>(n);

        for(int s = 0; s < n; ++s)
        {
            int pos = 0;
            module_type car = d[s].mtype;
            r[s] = pos;

            for(int i = s + 1; i < n; ++i)
            {
                if(car == d[i].mtype) 
                {
                    r[i] = ++pos;
                    ++s;
                }
            }
        }
        return r;
    }


    const int blueprint_t::count(const parameter_type& p, const descriptor_t* d)
    {
        int c { 0 };
        int m = settings::n_modules();
        switch(p)
        {
            case parameter_type::ic: for(int i = 0; i < m; ++i) c += *d[i].ic; break;
            case parameter_type::oc: for(int i = 0; i < m; ++i) c += *d[i].oc; break;
            case parameter_type::cc: for(int i = 0; i < m; ++i) c += *d[i].cc; break;
            default: break;
        }
        return c;
    }
}
}
