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
    
    void Renderer::setup(int size, Scene* s, int internalformat)
    {
        int type = ofGetGlTypeFromInternal(internalformat);
        int format = ofGetGLFormatFromInternal(internalformat);
        cm.initEmptyTextures(size, format, type);
        warpShader.setupShaderFromSource(GL_FRAGMENT_SHADER, warp_frag_shader_str);
        warpShader.linkProgram();
        registerScene(s);
    }

    void Renderer::render() {
        if (!scene) {
            return;
        }
        for (int i = 0; i < 6; i++) {
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
    
    void CustomFboRenderer::setup(int size, Scene* s, int internalformat, int numSamples)
    {
        Renderer::setup(size, s, internalformat);
        fbo.allocate(cm.getWidth(), cm.getHeight(), internalformat, numSamples);
    }
    
    void CustomFboRenderer::setup(int size, Scene* s, ofFbo::Settings fbo_settings)
    {
        Renderer::setup(size, s, fbo_settings.internalformat);
        fbo.allocate(fbo_settings);
    }

    void CustomFboRenderer::render() {
        for (int i = 0; i < 6; ++i) {
            fbo.begin();
            ofClear(0);
            ofPushView();
            
            glMatrixMode( GL_PROJECTION );
            glLoadIdentity();
            
            glLoadMatrixf( cm.getProjectionMatrix().getPtr() );
            
            glMatrixMode( GL_MODELVIEW );
            glLoadMatrixf( cm.getLookAtMatrixForFace( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i).getPtr() );
            
            scene->drawEquiScene();
            
            ofPopView();
            fbo.end();
            
            cm.beginDrawingInto2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
            ofClear(0);
            fbo.draw(0, 0);
            cm.endDrawingInto2D();
        }
    }
}