#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;

const float renderWidth = 1920.0;
const float renderHeight = 1080.0;

// Output fragment color
out vec4 finalColor;

vec3 Hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 colorA = vec3(1.0,1.0,1.0);
vec3 colorB = vec3(1.000,0.0,0.0);

//  Function from Iñigo Quiles
//  https://www.shadertoy.com/view/MsS3Wc
vec3 hsb2rgb( in vec3 c ){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0,
                     0.0,
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

void main()
{
 vec3 color = vec3(0.0);
vec3 tc = texture2D(texture0, fragTexCoord).rgb;

//color = mix(colorA, colorB, tc.r);
 color = hsb2rgb(vec3(tc.x,1.0,tc.y));


    gl_FragColor = vec4(color,1.0);

//gl_FragColor = vec4(Hsv2rgb(tc.g,0.5,0), 1.0);
}