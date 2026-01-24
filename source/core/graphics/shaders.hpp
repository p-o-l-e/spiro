#include <string>

namespace shader
{
    namespace vertex 
    {
        constexpr std::string_view passthrough(R"(
            attribute vec4 position;
            attribute vec2 texCoord;
            varying vec2 vTex;
            void main() {
                vTex = texCoord;
                gl_Position = position;
            })"
        );
    }

    namespace fragment 
    {
        constexpr std::string_view brt_extract(R"(
            uniform sampler2D texture;
            uniform float threshold;
            varying vec2 vTexCoord;
            void main() {
                vec4 color = texture2D(texture, vTexCoord);
                float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
                if(brightness > threshold)
                    gl_FragColor = color;
                else
                    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
            })"
        );  

        constexpr std::string_view blur_gauss(R"(
            uniform sampler2D texture;
            uniform vec2 direction;
            uniform vec2 resolution;
            varying vec2 vTexCoord;
            void main() {
                vec2 off = direction / resolution;
                vec4 sum = vec4(0.0);
                sum += texture2D(texture, vTexCoord - off * 4.0) * 0.0162162162;
                sum += texture2D(texture, vTexCoord - off * 3.0) * 0.0540540541;
                sum += texture2D(texture, vTexCoord - off * 2.0) * 0.1216216216;
                sum += texture2D(texture, vTexCoord - off * 1.0) * 0.1945945946;
                sum += texture2D(texture, vTexCoord) * 0.2270270270;
                sum += texture2D(texture, vTexCoord + off * 1.0) * 0.1945945946;
                sum += texture2D(texture, vTexCoord + off * 2.0) * 0.1216216216;
                sum += texture2D(texture, vTexCoord + off * 3.0) * 0.0540540541;
                sum += texture2D(texture, vTexCoord + off * 4.0) * 0.0162162162;
                gl_FragColor = sum;
            })"
        );

        constexpr std::string_view combined(R"(
            uniform sampler2D originalTexture;
            uniform sampler2D bloomTexture;
            uniform float intensity;
            varying vec2 vTexCoord;
            void main() {
                vec4 original = texture2D(originalTexture, vTexCoord);
                vec4 bloom = texture2D(bloomTexture, vTexCoord);
                gl_FragColor = original + bloom * intensity;
            })"
        );

        constexpr std::string_view red_gate(R"(
            uniform sampler2D tex; 
            varying vec2 vTex; 
            void main()
            {
                float b = texture2D(tex, vTex).r;
                gl_FragColor = vec4(b, 0, 0, 1.0);
            })"
        );
    }
   
}

