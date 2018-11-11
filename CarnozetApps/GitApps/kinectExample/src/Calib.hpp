//
//  Calib.hpp
//  kinectExample
//
//  Created by Martin Hermant on 06/11/2018.
//
//

#ifndef Calib_hpp
#define Calib_hpp

#include "ofMain.h"

class Calib{
public:
    Calib();
    void drawUI(ofRectangle && r);
    void computeOnTexture(ofTexture & t,bool updatePixels);

    ofParameterGroup settings;
    ofParameter<ofVec2f> p1,p2,p3,p4;
    ofVec2f screenP1,screenP2,screenP3,screenP4;
    ofRectangle drawRect;
    int radius;
    ofFbo mappedTexture;

    void applyQuadVertices(ofVec2f & p);
    ofVec2f textureSize;

    ofParameter<ofVec2f> * selected;
    ofPixels pixels;
    ofParameter<int > alphaBlur;
    void reset();
private:

    ofPoint src[4];
    ofPoint dst[4];
    GLfloat matrix[16];
    
    void mouseDragged(ofMouseEventArgs & me);
    void mousePressed(ofMouseEventArgs & me);
    void mouseReleased(ofMouseEventArgs & me);
    void updateMatrix();

};
#endif /* Calib_hpp */
