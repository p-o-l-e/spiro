#include "node.hpp"
#include "constants.hpp"

namespace core
{
    template<typename T>
    Module<T>::Module(const int id, const Descriptor* d): position(id), descriptor(d)
    {
        ccv = new std::atomic<T>*[*descriptor->cv[map::cv::c]];
        icv = new std::atomic<T>*[*descriptor->cv[map::cv::i]];
        ocv = new std::atomic<T> [*descriptor->cv[map::cv::o]];

        for(int i = 0; i < *descriptor->cv[map::cv::i]; ++i) icv[i] = &zero;
        for(int i = 0; i < *descriptor->cv[map::cv::c]; ++i) ccv[i] = &zero;
        for(int i = 0; i < *descriptor->cv[map::cv::o]; ++i) ocv[i].store(0.0f);
    }

    template<typename T>
    Module<T>::~Module()
    {
        delete[] ocv;
        delete[] icv;
        delete[] ccv;
    }

    template class Module<float>;
}
