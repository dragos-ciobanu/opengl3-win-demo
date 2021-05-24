#shader vertex
#version 450 core

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float a_texIndex;


out vec2 v_TexCoord;
out float v_texIndex;
uniform mat4 u_MVP;

void main() {
    gl_Position = u_MVP * vec4(v_pos, 1.0);
    v_TexCoord = texCoord;
    v_texIndex = a_texIndex;
}
#shader fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_texIndex;

uniform sampler2D u_Textures[8];

void main(void) {
    int texIndex = int(v_texIndex);
    vec4 texColor = texture(u_Textures[texIndex], v_TexCoord);
    color = texColor;
}