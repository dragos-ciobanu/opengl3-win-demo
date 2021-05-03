#shader vertex
#version 120

varying vec2 outtexture;
attribute vec2 v_pos;
attribute vec2 v_texCoord;
uniform mat4 u_MVP;

void main() {
    gl_Position = u_MVP * vec4(v_pos, 0.0, 1.0);
    outtexture = v_texCoord;
}
#shader fragment
#version 120

varying vec2 outtexture;
uniform sampler2D u_Texture;

void main(void) {
    vec4 color = texture2D(u_Texture, outtexture);
    gl_FragColor=  color;

}