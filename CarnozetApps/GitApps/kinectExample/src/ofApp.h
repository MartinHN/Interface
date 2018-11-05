#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

// Windows users:
// You MUST install the libfreenect kinect drivers in order to be able to use
// ofxKinect. Plug in the kinect and point your Windows Device Manager to the
// driver folder in:
//
//     ofxKinect/libs/libfreenect/platform/windows/inf
//
// This should install the Kinect camera, motor, & audio drivers.
//
// You CANNOT use this driver and the OpenNI driver with the same device. You
// will have to manually update the kinect device to use the libfreenect drivers
// and/or uninstall/reinstall it in Device Manager.
//
// No way around the Windows driver dance, sorry.

// uncomment this to read from two kinects simultaneously
//#define USE_TWO_KINECTS
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


    ofxCvColorImage colorImg;

    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image

#if CONTOUR
    ofxCvContourFinder contourFinder;
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

};
