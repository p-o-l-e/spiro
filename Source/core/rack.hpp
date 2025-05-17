#pragma once
#ifdef DEBUG_MODE
    #include <iostream>
    #define LOG(x) std::cout << "[DEBUG] " << x << std::endl;
#else
    #define LOG(x)
#endif

#include "node.hpp"



// #include "bus.hpp"

// #include "module_headers.hpp"

namespace core
{

    class Rack
    {
        private:
            std::unique_ptr<Module*[]>      node;
            // std::unique_ptr<uid_t[]>        pin;
            size_t _size = 0;
            void build() noexcept;

        public:


            void process(const int& p) { node[p]->process(); }
            void allocate(const int& n) noexcept { node = std::make_unique<Module*[]>(n); _size = n; }
            void bind(Module* m, const unsigned& pos) { if(pos < _size) node[pos] = m; }
            size_t size() const { return _size; }
            Rack() {};
           ~Rack() {};
    };
    // class Rack
    // {
    //     private:
    //         std::unique_ptr<Module*[]> node;
    //         Module* create_node(const map::module::type&);
    //         void allocate() noexcept;
    //         void build() noexcept;

    //     public:
    //         const interface::bus_connector bus;
    //         void connect_pin_i(const uint32_t&, std::atomic<float>**);
    //         void connect_pin_o(const uint32_t&, std::atomic<float>*);
    //         void connect_pin_c(const uint32_t&, std::atomic<float>*);
            
    //         const int ccv(const map::cv::index& i) const { return bus.blueprint.ccv[i]; };              // Get CV[type] count

    //         uint32_t get_uid(const map::module::type&, const int&, const map::cv::index&, const int&);
    //         Module* at(const int&);
    //         Module* at(const map::module::type&, const int&);
    //         void process(const int&);
    //         void bind(Module*, const unsigned&);
    //         Rack(const Descriptor**);
    //        ~Rack();
    // };


}
