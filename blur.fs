
#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// NOTE: Add here your custom variables

// NOTE: Render size values must be passed from code
const float renderWidth = 800.0;
const float renderHeight = 450.0;

vec3 offset = vec3(0.0, 1.3846153846, 3.2307692308);
vec3 weight = vec3(0.2270270270, 0.3162162162, 0.0702702703);
const float w = 0.1111111111;

vec3 Hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    // Texel color fetching from texture sampler
    //vec3 tc = texture2D(texture0, fragTexCoord).rgb*weight.x;
    vec3 tc = texture2D(texture0, fragTexCoord).rgb*w;
    //tc += texture2D(texture0, fragTexCoord + vec2(offset.y)/renderWidth, 0.0).rgb*weight.y;
    //tc += texture2D(texture0, fragTexCoord - vec2(offset.y)/renderWidth, 0.0).rgb*weight.y;

   // tc += texture2D(texture0, fragTexCoord + vec2(offset.z)/renderWidth, 0.0).rgb*weight.z;
   // tc += texture2D(texture0, fragTexCoord - vec2(offset.z)/renderWidth, 0.0).rgb*weight.z;
    tc += texture2D(texture0, fragTexCoord + vec2(-1,-1)/renderWidth, 0.0).rgb*w;
    tc += texture2D(texture0, fragTexCoord + vec2(0,-1)/renderWidth, 0.0).rgb*w;
    tc += texture2D(texture0, fragTexCoord + vec2(1,-1)/renderWidth, 0.0).rgb*w;
    tc += texture2D(texture0, fragTexCoord + vec2(-1,0)/renderWidth, 0.0).rgb*w;
    tc += texture2D(texture0, fragTexCoord + vec2(1,0)/renderWidth, 0.0).rgb*w;
    tc += texture2D(texture0, fragTexCoord + vec2(-1,1)/renderWidth, 0.0).rgb*w;
    tc += texture2D(texture0, fragTexCoord + vec2(0,1)/renderWidth, 0.0).rgb*w;
    tc += texture2D(texture0, fragTexCoord + vec2(1,1)/renderWidth, 0.0).rgb*w;
    gl_FragColor = vec4(Hsv2rgb(tc)*0.90, 1.0);
}