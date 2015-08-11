#include "ofMain.h"
#include "ofxEquiMap.h"

class ofApp : public ofBaseApp, public ofxEquiMap::Scene{
    ofxEquiMap::Renderer em;
    
    ofVboMesh m;
public:
    void setup()
    {
        ofSetVerticalSync(true);
        ofSetFrameRate(60);
        em.setup(1024, this);
        ofSpherePrimitive p(1000, 24);
        m = p.getMesh();
    }
    
    void drawEquiScene()
    {
        ofPushStyle();
        ofSetLineWidth(3);
        ofSetColor(192);
        m.drawWireframe();
        ofPopStyle();
    }
    
    void update() {
        em.render();
    }
    
    void draw()
    {
        ofClear(0);
        em.draw(0, 0, ofGetWidth(), ofGetHeight());
        
        ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
    }
};

//========================================================================
int main( ){
    ofSetupOpenGL(1280,640,OF_WINDOW);            // <-------- setup the GL context
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
    
}
