#include "testApp.h"
#include "ofxCv/RunningBackground.h"


int thresh = 0;
bool ignore = false;
using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	cam.initGrabber(640, 480);
	background.setLearningTime(900);
	background.setThresholdValue(10);
    //background.setIgnoreForeground(true);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		background.update(cam, thresholded);
		thresholded.update();
	}
}

void testApp::draw() {
    

    
	cam.draw(0, 0);
	//thresholded.draw(640, 0);
    
    
    drawMat(background.testMat2, 640,0);
    cv::Mat& fore = background.getForeground();//get mask
   
    cv::Mat& foreG = fore;
   
   // Canny(fore, fore1, mouseX, mouseY,3);
    drawMat(background.foregroundGray, 0,480);
    cv::Mat& bk = background.getBackground();
    
    drawMat(bk, 640, 480);
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		background.reset();
	}
    if(key == 'd') {
		background.setDifferenceMode(background.ABSDIFF);
	}
    if(key == 'e') {
		background.setDifferenceMode(background.BRIGHTER);
	}
    if(key == 'f') {
		background.setDifferenceMode(background.DARKER);
	}
    if(key == ' ') {
		background.reset();
	}
    
    if(key == '=') {
        if (thresh++ >255) thresh = 255;
		background.setThresholdValue(thresh);
	}
    if(key == '-') {
        if (thresh-- <0) thresh = 0;
		background.setThresholdValue(thresh);
	}
    if(key == 'i') {
        ignore = !ignore;
		background.setIgnoreForeground(ignore);
	}
    
}