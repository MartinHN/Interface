#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxOsc.h"


#include "Calib.hpp"
#define CONTOUR 0

#include "ofxGUI.h"
#include "ofxSyphonServer.h"
class ofApp : public ofBaseApp {
public:

    void setup();
    void update();
    void draw();
    void exit();

    

    void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void setTilt( float&  a);
    void openCloseKinnect(bool & b);
    ofxKinect kinect;

    ofxOscReceiver osc;
    ofxOscSender fbOsc;
    ofxCvColorImage colorImg;

    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image

#if CONTOUR
    ofxCvContourFinder contourFinder;
#endif

#if USE_QT

    ofVideoPlayer player;
    ofParameter<bool> useVideo;
    void modeChanged(bool & m);
#endif
    bool drawGUI;
    string settingFile;
    ofParameter<bool> bThreshWithOpenCV;
    ofParameter<bool> isOpen;
    ofParameter<int> nearThreshold;
    ofParameter<int> farThreshold;
    ofParameter<float> angle;

    ofParameterGroup settings;


    ofxPanel kinectCtrl;
    ofxSyphonServer syphonServer;
    Calib calib;

private:
    void updateOSC();
    

};
