#include "shaders.hpp"
#pragma once


namespace core {
namespace shader {

    struct Descriptor 
    {
        enum class Type { Vertex, Fragment };
        Type type;

        std::string_view shader;

        std::span<const std::string_view> attribute;
        std::span<const std::string_view> uniform;
        std::span<const std::string_view> varying;
    };
 
    namespace passthrough
    {
        struct attribute { enum { position, tex_coord }; };
        struct varying   { enum { vTex }; };

        static constexpr std::array<std::string_view, 2> a { "position", "texCoord" };
        static constexpr std::array<std::string_view, 0> u { };
        static constexpr std::array<std::string_view, 1> v { "vTex" };

        static constexpr Descriptor descriptor {
            Descriptor::Type::Vertex,
            shader::vertex::passthrough,
            a,
            u,
            v
        };
    }

    namespace postprocess
    {
        struct attribute { enum { position }; };
        struct varying   { enum { vPos }; };

        static constexpr std::array<std::string_view, 1> a { "position" };
        static constexpr std::array<std::string_view, 0> u { };
        static constexpr std::array<std::string_view, 1> v { "vPos" };

        static constexpr Descriptor descriptor {
            Descriptor::Type::Vertex,
            shader::vertex::postprocess,
            a,
            u,
            v
        };
    }

    namespace ndc_to_uv
    {
        struct attribute { enum { position }; };
        struct varying   { enum { vTex }; };

        static constexpr std::array<std::string_view, 1> a { "position" };
        static constexpr std::array<std::string_view, 0> u { };
        static constexpr std::array<std::string_view, 1> v { "vTex" };

        static constexpr Descriptor descriptor {
            Descriptor::Type::Vertex,
            shader::vertex::ndc_to_uv,
            a,
            u,
            v
        };
    }

    namespace solid
    {
        struct uniform { enum { color }; };
        struct varying { enum { vPos }; };

        static constexpr std::array<std::string_view, 0> a { };
        static constexpr std::array<std::string_view, 1> u { "color" };
        static constexpr std::array<std::string_view, 1> v { "vPos" };

        static constexpr Descriptor descriptor {
            Descriptor::Type::Fragment,
            shader::fragment::solid,
            a,
            u,
            v
        };
    }

    namespace threshold
    {
        struct uniform { enum { tex, threshold }; };
        struct varying { enum { vTex }; };

        static constexpr std::array<std::string_view, 0> a { };
        static constexpr std::array<std::string_view, 2> u { "tex", "threshold" };
        static constexpr std::array<std::string_view, 1> v { "vTex" };

        static constexpr Descriptor descriptor {
            Descriptor::Type::Fragment,
            shader::fragment::threshold,
            a,
            u,
            v
        };
    }

    namespace cauchy {
        namespace H5
        {
            struct uniform { enum { tex, texelSize }; };
            struct varying { enum { vTex }; };

            static constexpr std::array<std::string_view, 0> a { };
            static constexpr std::array<std::string_view, 2> u { "tex", "texelSize" };
            static constexpr std::array<std::string_view, 1> v { "vTex" };

            static constexpr Descriptor descriptor {
                Descriptor::Type::Fragment,
                shader::fragment::hblur,
                a,
                u,
                v
            };
        }

        namespace V5
        {
            struct uniform { enum { tex, texelSize }; };
            struct varying { enum { vTex }; };

            static constexpr std::array<std::string_view, 0> a { };
            static constexpr std::array<std::string_view, 2> u { "tex", "texelSize" };
            static constexpr std::array<std::string_view, 1> v { "vTex" };

            static constexpr Descriptor descriptor {
                Descriptor::Type::Fragment,
                shader::fragment::vblur,
                a,
                u,
                v
            };
        }
    }

    namespace combine {
        namespace add {
        
            struct uniform { enum { uTexA, uTexB }; };
            struct varying { enum { vTex }; };

            static constexpr std::array<std::string_view, 0> a { };
            static constexpr std::array<std::string_view, 2> u { "uTexA", "uTexB" };
            static constexpr std::array<std::string_view, 1> v { "vTex" };

            static constexpr Descriptor descriptor {
                Descriptor::Type::Fragment,
                shader::fragment::combine_add,
                a,
                u,
                v
            };
        }
    }

    namespace gate {
        namespace red {

            struct uniform { enum { tex, fgColor, bgColor }; };
            struct varying { enum { vTex }; };

            static constexpr std::array<std::string_view, 0> a { };
            static constexpr std::array<std::string_view, 3> u { "tex", "fgColor", "bgColor" };
            static constexpr std::array<std::string_view, 1> v { "vTex" };

            static constexpr Descriptor descriptor {
                Descriptor::Type::Fragment,
                shader::fragment::red_gate,
                a,
                u,
                v
            };
        }
    }

    namespace afterglow
    {
        struct uniform { enum { tex, decay }; };
        struct varying { enum { vTex }; };

        static constexpr std::array<std::string_view, 0> a { };
        static constexpr std::array<std::string_view, 2> u { "tex", "decay" };
        static constexpr std::array<std::string_view, 1> v { "vTex" };

        static constexpr Descriptor descriptor {
            Descriptor::Type::Fragment,
            shader::fragment::afterglow,
            a,
            u,
            v
        };
    }

} // namespace shader
} // namespace core

