// 顶点着色器
/*
 vec4      -> contain x,y,z,w (0,0,0,1)
 attribute -> 顶点属性
*/
//4*4 矩阵
uniform mat4 u_Matrix;

attribute vec4 a_Position;
attribute vec4 a_Color;//r,g,b,a default 1

varying vec4 v_Color;

void main() {

    v_Color = a_Color;

    //must init the gl_Position
    gl_Position = u_Matrix * a_Position;
    gl_PointSize = 10.0;//gl_Position为中心的四边形，四条边的长度等于 gl_PointSize
}