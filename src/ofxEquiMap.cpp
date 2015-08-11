#include "ofxEquiMap.h"

#ifndef STRINGIFY
#define STRINGIFY(A) #A
#endif

namespace ofxEquiMap {
    static string warp_frag_shader_str = STRINGIFY(
       uniform samplerCube envMap;
       
       void main() {
           
           vec2 tc = gl_TexCoord[0].st / vec2(2.0) + 0.5;  //only line modified from the shader toy example
           vec2 thetaphi = ((tc * 2.0) - vec2(1.0)) * vec2(3.1415926535897932384626433832795, 1.5707963267948966192313216916398);
           vec3 rayDirection = vec3(cos(thetaphi.y) * cos(thetaphi.x), sin(thetaphi.y), cos(thetaphi.y) * sin(thetaphi.x));
           
           gl_FragColor = textureCube(envMap, rayDirection);
       }
       );
    
    void Renderer::setup(float size, Scene* s)
    {
        cm.initEmptyTextures(size);
        warpShader.setupShaderFromSource(GL_FRAGMENT_SHADER, warp_frag_shader_str);
        warpShader.linkProgram();
        registerScene(s);
    }

    void Renderer::render() {
        if (!scene) {
            return;
        }
        for( int i = 0; i < 6; i++ )
        {
            cm.beginDrawingInto3D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i );
            ofClear(0);
            scene->drawEquiScene();
            cm.endDrawingInto3D();
        }
    }
    
    void Renderer::draw(float x, float y, float w, float h) {
        warpShader.begin();
        warpShader.setUniform1i("envMap", 0);
        cm.drawFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, x, y, w, h);
        warpShader.end();
    }

}