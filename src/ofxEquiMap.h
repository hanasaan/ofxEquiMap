#pragma once

#include "ofMain.h"
#include "ofxCubeMap.h"

namespace ofxEquiMap
{
    class Scene
    {
    public:
        virtual ~Scene() {}
        virtual void drawEquiScene() = 0;
    };
    
    class Renderer
    {
    protected:
        Scene* scene = NULL;
        ofxCubeMap cm;
        ofShader warpShader;
    public:
        void setup(float size, Scene* s);
        void render();
        void draw(float x, float y, float w, float h);
        
        void setPosition(const ofVec3f& p) {
            ofVec3f p2 = p;
            cm.setPosition(p2);
        }
        
        void registerScene(Scene* s) {scene = s;}
        void setPosition(float x, float y, float z) {cm.setPosition(x, y, z);}
        ofxCubeMap& getCubeMap() {return cm;}
        
    };
};