#pragma once

#include "constants.hpp"
#include "../interface_headers.hpp"

namespace core 
{
namespace settings 
{
    inline const descriptor_t descriptor_map[]
    {
        { interface::vco::descriptor    },
        { interface::vco::descriptor    },
        { interface::vco::descriptor    },
        { interface::vco::descriptor    },

        { interface::env::descriptor[0] },
        { interface::env::descriptor[0] },
        { interface::env::descriptor[0] },
        { interface::env::descriptor[0] },

        { interface::lfo::descriptor    },
        { interface::lfo::descriptor    },

        { interface::map::descriptor    },
        { interface::map::descriptor    },

        { interface::mix::descriptor    },

        { interface::pdt::descriptor    },
        { interface::sum::descriptor    },

        { interface::rtr::descriptor    },

        { interface::snh::descriptor[0] },
        { interface::snh::descriptor[0] },

        { interface::vca::descriptor[0] },
        { interface::vca::descriptor[0] },

        { interface::vcd::descriptor    },

        { interface::vcf::descriptor    },
        { interface::vcf::descriptor    },
        
        { interface::cro::descriptor    }
    };

    inline constexpr int n_modules()
    {
        return sizeof(descriptor_map)/sizeof(descriptor_t);
    }
}

namespace interface
{

	class blueprint_t
    {
        private:
            const int count(const parameter_type&, const descriptor_t*);
            const std::unique_ptr<int[]> set_relatives(const descriptor_t*);

        public:
            const descriptor_t* descriptor;
            const std::unique_ptr<int[]> relative; 
            const int ic = 0;
            const int oc = 0;
            const int cc = 0;
            const int mc = 0;

            blueprint_t(const descriptor_t* d): 
                descriptor(d), 
                relative(set_relatives(d)),
                mc(settings::n_modules()), 
                ic(count(parameter_type::ic, d)),
                oc(count(parameter_type::oc, d)),
                cc(count(parameter_type::cc, d))
            { };
           ~blueprint_t() {};
    };
}
    

} // Namespace core
