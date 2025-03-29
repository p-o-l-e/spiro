#pragma once

#include "containers.hpp"
namespace cell {
    class logger 
    {
        private:
            const int buffer_size { 1024 };
            static wavering<std::string> _log[buffer_size]            
        public:
            static void push(const std::string& msg) { _log.set(msg); }
            static void print(const std::string& msg, const int& lv)
            {
                std::string o { "" };
                for(int i = 0; i < lv; ++i) o += "-";
                o += " ";
                o += msg;
                o += "\n";
                
                _log.set(o);

                std::cout<<o;
            }
            
            logger();
           ~logger();
    };

}
