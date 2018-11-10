#include "ofApp.h"
#include "ofxXmlSettings.h"
#include "ParamHelpers.h"
//--------------------------------------------------------------
void ofApp::setup() {
    settingFile = "settingsKinect.xml";
    ofSetLogLevel(OF_LOG_VERBOSE);

    // enable depth->video image calibration
//    kinect.setRegistration(true);

    kinect.init(false,false);
    //kinect.init(true); // shows infrared instead of RGB video image
    //kinect.init(false, false); // disable video image (faster fps)

    kinect.open();		// opens first available kinect
                        //kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
                        //kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #

    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }



    colorImg.allocate(kinect.width, kinect.height);
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);

    nearThreshold = 230;
    farThreshold = 70;
    bThreshWithOpenCV = true;

    ofSetFrameRate(30);

    // zero the tilt on startup
    angle = 0;
    kinect.setCameraTiltAngle(angle);



    settings.setName("settings");
    P(nearThreshold,230,0,255);
    P(farThreshold,70,0,255);
    PB(bThreshWithOpenCV,true);
    P(angle,0,-30,30);
    angle.addListener(this, &ofApp::setTilt);
    PB(isOpen,true);
    P(calib.alphaBlur,255,0,255);
    isOpen.setSerializable(false);
    isOpen.addListener(this,&ofApp::openCloseKinnect);
    settings.add(calib.settings);
    kinectCtrl.setup(settings,"",10,20);
    drawGUI = true;

    kinectCtrl.loadFromFile(settingFile);

    syphonServer.setName("");
    

    osc.setup(11002);
}



//--------------------------------------------------------------
void ofApp::update() {
    updateOSC();
    ofBackground(100, 100, 100);

    kinect.update();


    // there is a new frame and we are connected
    if(kinect.isFrameNewDepth()) {
        calib.computeOnTexture(kinect.getDepthTextureReference(),true);
        
        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(calib.pixels);
//        grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);

        // we do two thresholds - one for the far plane and one for the near plane
        // we then do a cvAnd to get the pixels which are a union of the two thresholds
        if(bThreshWithOpenCV) {
            grayThreshNear = grayImage;
            grayThreshFar = grayImage;
            grayThreshNear.threshold(nearThreshold, true);
            grayThreshFar.threshold(farThreshold);
            cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        } else {

            // or we do it ourselves - show people how they can work with the pixels
//            unsigned char * pix = grayImage.getPixels();
//
//            int numPixels = grayImage.getWidth() * grayImage.getHeight();
//            for(int i = 0; i < numPixels; i++) {
//                if(pix[i] < nearThreshold && pix[i] > farThreshold) {
//                    pix[i] = 255;
//                } else {
//                    pix[i] = 0;
//                }
//            }
        }

        // update the cv images
        grayImage.flagImageChanged();
        grayImage.updateTexture();
        syphonServer.publishTexture(&grayImage.getTextureReference());

#if CONTOUR
        //        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        //        // also, find holes is set to true so we will get interior contours as well....
        contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);
#endif
    }


}

//--------------------------------------------------------------
void ofApp::draw() {

    ofSetColor(255, 255, 255);
if(drawGUI) {
    kinectCtrl.draw();

    kinect.draw(0, ofGetHeight()/2, 400, 300);
    
    // draw from the live kinect
    int left = ofGetWidth()/2;
    int height = 250;
    int cH = 0;
    kinect.drawDepth(left, cH, 400, height);
    calib.drawUI(ofRectangle(left, cH, 400, height));
    cH+=height;
    grayImage.draw(left, cH, 400, height);
    cH+=height;

    calib.mappedTexture.draw(left,cH,400,height);
    cH+=height;
#if CONTOUR
    contourFinder.draw(10, 320, 400, height);
#endif
}
 if(!drawGUI && ofGetWidth()!=100){
    ofSetWindowShape(100, 30);
 }
    if(drawGUI && ofGetWidth()==100){
        ofSetWindowShape(800, 600);
    }
    ofRectangle fpsR(0,0,200,20);
    ofSetColor(0);
    ofRect(fpsR);
    ofSetColor(255);
    ofDrawBitmapString(ofToString(ofGetFrameRate()),fpsR.getLeft(),fpsR.getBottom());

}



//--------------------------------------------------------------
void ofApp::exit() {
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();


}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
    switch (key) {

        case 'w':
            kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
            break;

        case 'o':
            kinect.setCameraTiltAngle(angle); // go back to prev tilt
            kinect.open();
            break;

        case 'c':
            kinect.setCameraTiltAngle(0); // zero the tilt
            kinect.close();
            break;
        case 'g':
            drawGUI = !drawGUI;
            break;

        case 's':
            kinectCtrl.saveToFile(settingFile);

            break;

    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{}

void ofApp::setTilt(float & a){
    kinect.setCameraTiltAngle(a);
}
void ofApp::openCloseKinnect(bool & b){
    if(b){
        kinect.setCameraTiltAngle(angle); // go back to prev tilt
        kinect.open();
    }
    else{
        kinect.setCameraTiltAngle(0);
        kinect.close();
    }
}

#define SENDVEC(add,v) {  \
auto fb = ofxOscMessage();\
fb.setAddress(add);     \
fb.addFloatArg(v.get().x); \
fb.addFloatArg(v.get().y);\
fbOsc.sendMessage(fb);\
}
#define SENDFLOAT(add,f) {  \
auto fb = ofxOscMessage();\
fb.setAddress(add);     \
fb.addFloatArg(f); \
fbOsc.sendMessage(fb);\
}

void ofApp::updateOSC(){
    if(osc.hasWaitingMessages()){
        ofxOscMessage m;
        while(osc.getNextMessage(&m)){
            if(m.getAddress()=="/p1"){calib.p1 = ofVec2f(m.getArgAsFloat(0),m.getArgAsFloat(1));}
            else if(m.getAddress()=="/p2"){calib.p2 = ofVec2f(m.getArgAsFloat(0),m.getArgAsFloat(1));}
            else if(m.getAddress()=="/p3"){calib.p3 = ofVec2f(m.getArgAsFloat(0),m.getArgAsFloat(1));}
            else if(m.getAddress()=="/p4"){calib.p4 = ofVec2f(m.getArgAsFloat(0),m.getArgAsFloat(1));}
            else if(m.getAddress()=="/near"){nearThreshold = m.getArgAsFloat(0);}
            else if(m.getAddress()=="/far"){farThreshold = m.getArgAsFloat(0);}
            else if(m.getAddress()=="/p4"){calib.p4 = ofVec2f(m.getArgAsFloat(0),m.getArgAsFloat(1));}
            else if(m.getAddress()=="/tilt"){angle = m.getArgAsFloat(0);}
            else if(m.getAddress()=="/save"){kinectCtrl.saveToFile(settingFile);}
            else if(m.getAddress()=="/get"){
                
                fbOsc.setup(m.getAddress(),m.getRemotePort());
                SENDVEC("/p1", calib.p1)
                SENDVEC("/p2", calib.p2)
                SENDVEC("/p3", calib.p3)
                SENDVEC("/p4", calib.p4)
                SENDFLOAT("/tilt",angle )
                SENDFLOAT("/near",nearThreshold )
                SENDFLOAT("/far",farThreshold )
                
            }

        }
    }
}
