#version 450

layout(binding = 2) uniform sampler2D textureSampler;

layout(location = 0) in vec3 in_fragmentColor;
layout(location = 1) in vec2 in_textureCoordinate;

layout(location = 0) out vec4 out_fragmentColor;

void main() {
//    out_fragmentColor = vec4(in_fragmentColor, 1.0);
    out_fragmentColor = vec4(
        texture(textureSampler, in_textureCoordinate).rgb, 1.0
    );
//    out_fragmentColor = vec4(
//        in_fragmentColor * texture(textureSampler, in_textureCoordinate).rgb, 1.0
//    );
}
