#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "textInput.h"


class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void getImagesBy();
	void keyPressed(int key);
	void addText(string& s);
    void drawEntries();
    void loadImages();
    void loadImagesAndText(int key);
    void gotMessage(ofMessage &msg);
    void updateLayout();

	ofVideoGrabber cam;
	ofxCv::RunningBackground background;
	ofImage thresholded;
    
    cv::Mat frame, oldframe;
    cv::Mat back;
    cv::Mat fore;

    cv::BackgroundSubtractorMOG2 bg;
   
    std::vector<std::vector<cv::Point> > contours;
    
    ofTrueTypeFont font;
    
    int mode;
    
    string userString;
    string saveImage;
       bool bTakePicture;
    bool bInsertTextMode;
    
    	textInput textBox;
    
    vector<string> textEntries;
    
    vector<ofImage> cartoons;
    vector<ofImage> cartoonSelection;
    float startTime;
    float endTime;
    bool bTimerReached;
    ofImage screenshot;
    ofxCvGrayscaleImage graySS;

    
    
    };
