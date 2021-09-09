#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;

const float renderWidth = 800.0;
const float renderHeight = 450.0;

// Output fragment color
out vec4 finalColor;

vec3 Hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


void main()
{
vec3 tc = texture2D(texture0, fragTexCoord).rgb;



gl_FragColor = vec4(Hsv2rgb(tc.r,0.6,1.0), 1.0);
}