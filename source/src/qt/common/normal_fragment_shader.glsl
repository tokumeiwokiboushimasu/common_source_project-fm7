#ifdef GL_ES
 precision mediump float;
#endif 

varying vec2 v_texcoord;
uniform vec4 color;

uniform sampler2D a_texture;

// This is from sample of Qt5 , see http://doc.qt.io/qt-5/qtopengl-cube-example.html .
void main()
{
    // Set fragment color from texture
    vec4 pixel_t = texture2D(a_texture, v_texcoord );
    vec4 pixel_r;
    pixel_r = pixel_t * color;
    gl_FragColor = pixel_r;
}
