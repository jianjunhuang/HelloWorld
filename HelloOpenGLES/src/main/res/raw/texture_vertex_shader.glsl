uniform mat4 u_Mtrix;

attribute vec4 a_Position;
//S,T
attribute vec2 a_TextureCoordinates;

varying vec2 v_TextureCoordinates;

void main() {
    v_TextureCoordinates = a_TextureCoordinates;
    gl_Position = u_Mtrix * a_Position;
}