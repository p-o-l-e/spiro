#include <string>
#include <span>
#include <string_view>

namespace core {
namespace shader {

    namespace vertex 
    {
        constexpr std::string_view passthrough(R"(

            attribute vec4 position;
            attribute vec2 texCoord;
            varying vec2 vTex;
            void main() 
            {
                vTex = texCoord;
                gl_Position = position;
            }

        )");
        
        constexpr std::string_view postprocess(R"(

            attribute vec2 position; 
            varying vec2 vPos; 
            void main() 
            { 
                vPos = position; 
                gl_Position = vec4(position, 0.0, 1.0); 
            }

        )");

        constexpr std::string_view ndc_to_uv(R"(

            attribute vec2 position;
            varying vec2 vTex;

            void main()
            {
                vTex = (position + 1.0) * 0.5;  // NDC [-1,1] → [0,1]
                gl_Position = vec4(position, 0.0, 1.0);
            }

        )");
    }

    namespace fragment 
    {
        constexpr std::string_view solid(R"(

            uniform float color[4];
            varying vec2 vPos;
            void main()
            {
                vec4 vColor = vec4(color[0], color[1], color[2], color[3]);
                gl_FragColor = vColor;
            }

        )");

        constexpr std::string_view threshold(R"(

            uniform sampler2D tex;
            uniform float threshold;
            varying vec2 vTex;

            void main()
            {
                vec4 c = texture2D(tex, vTex);
                float a = c.a;
                gl_FragColor = (a > threshold) ? c : vec4(0.0);
            }

        )");  

        constexpr std::string_view hblur(R"(

            uniform sampler2D tex;
            uniform float texelSize;
            varying vec2 vTex;

            void main()
            {
                vec4 sum = vec4(0.0);
                sum += texture2D(tex, vTex + vec2(-5.0 * texelSize, 0.0)) * 0.020;
                sum += texture2D(tex, vTex + vec2(-4.0 * texelSize, 0.0)) * 0.025;
                sum += texture2D(tex, vTex + vec2(-3.0 * texelSize, 0.0)) * 0.037;
                sum += texture2D(tex, vTex + vec2(-2.0 * texelSize, 0.0)) * 0.056;
                sum += texture2D(tex, vTex + vec2(-1.0 * texelSize, 0.0)) * 0.164;
                sum += texture2D(tex, vTex) * 0.455;
                sum += texture2D(tex, vTex + vec2( 1.0 * texelSize, 0.0)) * 0.164;
                sum += texture2D(tex, vTex + vec2( 2.0 * texelSize, 0.0)) * 0.056;
                sum += texture2D(tex, vTex + vec2( 3.0 * texelSize, 0.0)) * 0.037;
                sum += texture2D(tex, vTex + vec2( 4.0 * texelSize, 0.0)) * 0.025;
                sum += texture2D(tex, vTex + vec2( 5.0 * texelSize, 0.0)) * 0.020;
                gl_FragColor = sum;
            }

        )");

        constexpr std::string_view vblur(R"(

            uniform sampler2D tex;
            uniform float texelSize;
            varying vec2 vTex;

            void main()
            {
                vec4 sum = vec4(0.0);
                sum += texture2D(tex, vTex + vec2( 0.0, -5.0 * texelSize)) * 0.020;
                sum += texture2D(tex, vTex + vec2( 0.0, -4.0 * texelSize)) * 0.025;
                sum += texture2D(tex, vTex + vec2( 0.0, -3.0 * texelSize)) * 0.037;
                sum += texture2D(tex, vTex + vec2( 0.0, -2.0 * texelSize)) * 0.056;
                sum += texture2D(tex, vTex + vec2( 0.0, -1.0 * texelSize)) * 0.164;
                sum += texture2D(tex, vTex) * 0.455;
                sum += texture2D(tex, vTex + vec2( 0.0,  1.0 * texelSize)) * 0.164;
                sum += texture2D(tex, vTex + vec2( 0.0,  2.0 * texelSize)) * 0.056;
                sum += texture2D(tex, vTex + vec2( 0.0,  3.0 * texelSize)) * 0.037;
                sum += texture2D(tex, vTex + vec2( 0.0,  4.0 * texelSize)) * 0.025;
                sum += texture2D(tex, vTex + vec2( 0.0,  5.0 * texelSize)) * 0.020;
                gl_FragColor = sum;
            }

        )");

        constexpr std::string_view combine_add(R"(

            uniform sampler2D uTexA;
            uniform sampler2D uTexB;
            varying vec2 vTex;

            void main()
            {
                vec4 a = texture2D(uTexA, vTex);
                vec4 b = texture2D(uTexB, vTex);
                gl_FragColor = a + b;
            }

        )");

        constexpr std::string_view red_gate(R"(

            uniform sampler2D tex;
            uniform float fgColor[4];
            uniform float bgColor[4];
            varying vec2 vTex;

            void main()
            {
                float alpha = texture2D(tex, vTex).r;

                vec3 fg = vec3(fgColor[0], fgColor[1], fgColor[2]);
                vec3 bg = vec3(bgColor[0], bgColor[1], bgColor[2]);
                vec3 rgb = mix(bg, fg, alpha);
                gl_FragColor = vec4(rgb, alpha);
            }

        )");

        constexpr std::string_view afterglow(R"(

            uniform sampler2D tex;
            uniform float decay;
            varying vec2 vTex;

            void main()
            {
                vec4 c = texture2D(tex, vTex);
                c.r *= decay; 
                c.g *= decay * 0.8;
                c.b *= decay * 0.7;
                c.a *= decay;
                gl_FragColor = c;
            }

        )");

    }
}
}
