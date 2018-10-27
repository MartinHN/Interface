/*
 * ofPanel.cpp
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#include "ofxPanel.h"
#include "ofGraphics.h"
#include "ofImage.h"

ofImage ofxPanel::loadIcon;
ofImage ofxPanel::saveIcon;

ofxPanel::ofxPanel()
:bGrabbed(false){}

ofxPanel::ofxPanel(const ofParameterGroup & parameters, string filename, float x, float y)
: ofxGuiGroup(parameters, filename, x, y)
, bGrabbed(false){
}

ofxPanel::~ofxPanel(){
	unregisterMouseEvents();
}

ofxPanel * ofxPanel::setup(string collectionName, string filename, float x, float y){
	if(!loadIcon.isAllocated() || !saveIcon.isAllocated()){
		loadIcons();
	}
	registerMouseEvents();
	return (ofxPanel*)ofxGuiGroup::setup(collectionName,filename,x,y);
}

ofxPanel * ofxPanel::setup(const ofParameterGroup & parameters, string filename, float x, float y){
	if(!loadIcon.isAllocated() || !saveIcon.isAllocated()){
		loadIcons();
	}
	registerMouseEvents();
    if(parameters.contains("isPiping")){
        isDynamic = true;
        isPiping = static_cast<ofParameter<bool> *>(&parameters.get("isPiping"));
    }
    
	return (ofxPanel*)ofxGuiGroup::setup(parameters,filename,x,y);
}

void ofxPanel::loadIcons(){
	unsigned char loadIconData[] = {0x38,0x88,0xa,0x6,0x7e,0x60,0x50,0x11,0x1c};
	unsigned char saveIconData[] = {0xff,0x4a,0x95,0xea,0x15,0xa8,0x57,0xa9,0x7f};
	loadIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
	saveIcon.allocate(9, 8, OF_IMAGE_COLOR_ALPHA);
	loadStencilFromHex(loadIcon, loadIconData);
	loadStencilFromHex(saveIcon, saveIconData);
    
	loadIcon.getTextureReference().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
	saveIcon.getTextureReference().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
}

void ofxPanel::generateDraw(){
	border.clear();
	border.setStrokeColor(thisBorderColor);
	border.setStrokeWidth(1);
	border.setFilled(false);
	border.rectangle(b.x,b.y,b.width+1,b.height-spacingNextElement);
    
    
	headerBg.clear();
	headerBg.setFillColor(ofColor(thisHeaderBackgroundColor,180));
	headerBg.setFilled(true);
	headerBg.rectangle(b.x,b.y+1,b.width,header);
    
    
	float iconHeight = header*.5;
	float iconWidth = loadIcon.getWidth()/loadIcon.getHeight()*iconHeight;
	int iconSpacing = iconWidth*.5;
    
    activeBox.set(b.getMaxX()-defaultHeight-2,b.y+2,defaultHeight,defaultHeight);
    pipeBox.set(b.getMaxX()-2*defaultHeight-2,b.y+2,defaultHeight,defaultHeight);
    
	loadBox.x = b.getMaxX() - (iconWidth * 2 + iconSpacing + textPadding);
	loadBox.y = b.y + header / 2. - iconHeight / 2.;
	loadBox.width = iconWidth;
	loadBox.height = iconHeight;
	saveBox.set(loadBox);
	saveBox.x += iconWidth + iconSpacing;
    
	textMesh = getTextMesh(getName(), textPadding + b.x, header / 2 + 4 + b.y);
}

void ofxPanel::render(){
	border.draw();
	headerBg.draw();
    
    
    
	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofColor c = ofGetStyle().color;
	ofSetColor(thisTextColor);
    
	bindFontTexture();
	textMesh.draw();
	unbindFontTexture();
    
	bool texHackEnabled = ofIsTextureEdgeHackEnabled();
	ofDisableTextureEdgeHack();
    //	loadIcon.draw(loadBox);
    //	saveIcon.draw(saveBox);
    if(isDynamic){
        ofSetColor(255,0,0);
        if(*isActive){
            ofFill();
        }
        else ofNoFill();
        
        ofRect(activeBox);
        
        ofSetColor(0,255,0);
        if(*isPiping){
            ofFill();
        }
        else ofNoFill();
        ofRect(pipeBox);
    }
	if(texHackEnabled){
		ofEnableTextureEdgeHack();
	}
    
	for(int i = 0; i < (int)collection.size(); i++){
		collection[i]->draw();
	}
    
	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

bool ofxPanel::mouseReleased(ofMouseEventArgs & args){
    this->bGrabbed = false;
    if(ofxGuiGroup::mouseReleased(args)) return true;
    if(isGuiDrawing() && b.inside(ofPoint(args.x,args.y))){
    	return true;
    }else{
    	return false;
    }
}

bool ofxPanel::setValue(float mx, float my, bool bCheck){
    
	if( !isGuiDrawing() ){
		bGrabbed = false;
		bGuiActive = false;
		return false;
	}
	if( bCheck ){
		if( b.inside(mx, my) ){
			bGuiActive = true;
            
			if( my > b.y && my <= b.y + header ){
				bGrabbed = true;
				grabPt.set(mx-b.x, my-b.y);
			} else{
				bGrabbed = false;
			}
            if(isDynamic&&activeBox.inside(mx,my)){
                *isActive = ! *isActive;
                return true;
            }
            if(isDynamic&&pipeBox.inside(mx,my)){
                *isPiping = ! *isPiping;
                return true;
            }
            //			if(loadBox.inside(mx, my)) {
            //                
            ////				loadFromFile(filename);
            //				ofNotifyEvent(loadPressedE,this);
            //				return true;
            //			}
            //			if(saveBox.inside(mx, my)) {
            //				saveToFile(filename);
            //				ofNotifyEvent(savePressedE,this);
            //				return true;
            //			}
		}
	} else if( bGrabbed ){
		setPosition(mx - grabPt.x,my - grabPt.y);
		return true;
	}
	return false;
}
