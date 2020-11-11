#ifndef __SHADER_FILE_H__
#define __SHADER_FILE_H__

#include <stdio.h>
#include <string>

char* todevVS = R"(attribute vec2 position;
attribute vec2 texcoord;
attribute vec2 texcoord2;
attribute vec4 color;
uniform vec4 g_DevInfo;
uniform mat4 g_WorldMat;
varying vec2 texCoord;
varying vec2 texCoord2;
varying vec4 oColor;
void main()
{
    vec4 tPos = vec4(position.x, position.y, 0, 1);
    tPos = g_WorldMat * tPos;
    float cx = (tPos.x*2.0 / g_DevInfo.x) - 1.0;
    float cy = -(tPos.y*2.0 / g_DevInfo.y) + 1.0;
    gl_Position = vec4(cx, cy, 0.5, 1.0);
    texCoord.xy = texcoord;
    texCoord2 = texcoord2;
    oColor = color;
})";
char* fillColorPS = R"(precision mediump float;
uniform float g_Alpha;
varying vec4 oColor;
void main()
{
    gl_FragColor = vec4(oColor.xyz, g_Alpha);
#ifdef TRANSPARENT_MODE
   gl_FragColor.w*=0.5;
#endif
})";
char* drawImagePS = R"(precision mediump float;
uniform sampler2D g_Tex0;
uniform float g_Alpha;
varying vec2 texCoord;
void main()
{
    gl_FragColor = texture2D(g_Tex0, texCoord.xy);
    gl_FragColor.w *= g_Alpha;
#ifdef TRANSPARENT_MODE
   gl_FragColor.w*=0.5;
#endif
})";
char* fillImagePS = R"(precision mediump float;
uniform float g_Alpha;
uniform sampler2D g_Tex0;
uniform vec4 g_TexRange;
varying vec2 texCoord;
void main()
{
    vec2 newTexCoord;
    newTexCoord.x = mod((texCoord.x - g_TexRange.x), g_TexRange.y) + g_TexRange.x;
    newTexCoord.y = mod((texCoord.y - g_TexRange.z), g_TexRange.w) + g_TexRange.z;
    gl_FragColor = texture2D(g_Tex0, newTexCoord);
    gl_FragColor.a = gl_FragColor.a * g_Alpha;
#ifdef TRANSPARENT_MODE
   gl_FragColor.w*=0.5;
#endif
})";
char* drawFilterImage = R"(precision mediump float;
uniform sampler2D g_Tex0;
uniform vec4 g_Filter;
uniform vec4 g_TexRange;
uniform float g_Alpha;
varying vec2 texCoord;
void main()
{
    vec4 color = texture2D(g_Tex0, texCoord.xy);
    float avg = dot(color.xyz, g_Filter.zzz);
    color.rgb = (avg + g_TexRange.xyz*color.rgb*g_Filter.y);
    color.a *= g_Filter.x;
    gl_FragColor = color;
    gl_FragColor.w *= g_Alpha;
#ifdef TRANSPARENT_MODE
   gl_FragColor.w*=0.5;
#endif
})";
char* blurImg = R"(precision mediump float;
uniform sampler2D g_Tex0;
uniform vec4 u_blurInfo;
uniform vec4 u_blur_off;
uniform float g_Alpha;
varying vec2 texCoord;
#define PI 3.141593
float sigma=u_blurInfo.x/3.0;
float sig2 = sigma*sigma;
float _2sig2 = 2.0*sig2;
float gauss1 = 1.0/(2.0*PI*sig2);
float getGaussian(float x, float y){
    return gauss1*exp(-(x*x+y*y)/_2sig2);
}
vec4 blur(){
    const float blurw = 9.0;
    vec4 vec4Color = vec4(0.0,0.0,0.0,0.0);
    vec2 halfsz=vec2(blurw,blurw)/2.0/u_blurInfo.zw;
    vec2 startpos=texCoord-halfsz-u_blur_off.zw;
    vec2 ctexcoord = startpos;
    vec2 step = 1.0/u_blurInfo.zw;
    for(float y = 0.0;y<=blurw; ++y){
        ctexcoord.x=startpos.x;
        for(float x = 0.0;x<=blurw; ++x){
            vec4Color += texture2D(g_Tex0, ctexcoord)*getGaussian(x-blurw/2.0,y-blurw/2.0);
            ctexcoord.x+=step.x;
        }
        ctexcoord.y+=step.y;
    }
    return vec4Color;
}
void main()
{
    gl_FragColor = blur();
    gl_FragColor.w*=g_Alpha;
#ifdef TRANSPARENT_MODE
    gl_FragColor.w*=0.5;
#endif
})";
char* drawMatFilterImagePS = R"(
precision mediump float;
uniform sampler2D g_Tex0;
uniform mat4 g_colorMat;
uniform vec4 g_colorAlpha;
uniform float g_Alpha;
varying vec2 texCoord;
void main(){
   vec4 color = texture2D(g_Tex0, texCoord.xy);
   gl_FragColor =  color * g_colorMat + g_colorAlpha/255.0;
   gl_FragColor.w *= g_Alpha;
#ifdef TRANSPARENT_MODE
   gl_FragColor.w*=0.5;
#endif
})";
char* glowImg = R"(
precision mediump float;
uniform sampler2D g_Tex0;
uniform vec4 u_color;
uniform vec4 u_blurInfo;
uniform vec4 u_blur_off;
uniform float g_Alpha;
varying vec2 texCoord;
vec4 blur(){
    const float blurw = 10.0;
    const float floatIterationTotalTime = 100.0;
    vec4 vec4Color = vec4(0.);
    vec2 halfsz=u_blurInfo.xy/u_blurInfo.zw;
    vec2 startpos=texCoord-halfsz-u_blur_off.zw/u_blurInfo.zw;
    vec2 ctexcoord = startpos;
    vec2 step = halfsz*2./blurw;
    for(float i = 0.0;i<=blurw; ++i){
        ctexcoord.x=startpos.x;
        for(float j = 0.0;j<=blurw; ++j){
            vec4Color += texture2D(g_Tex0, ctexcoord);
            ctexcoord.x+=step.x;
        }
        ctexcoord.y+=step.y;
    }
    return vec4Color/floatIterationTotalTime;
}
void main(){
   vec4 col = blur();
   gl_FragColor=vec4(u_color.rgb,col.a*u_color.w);
   gl_FragColor.w*=g_Alpha;
#ifdef TRANSPARENT_MODE
   gl_FragColor.w*=0.5;
#endif
})";

#endif
