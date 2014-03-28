/*  Comic Strip selfie - or Flat New World
    author Cindy S. Bishop
    utilizing opencv, oF ofxCv, and elliot wood's textInput
 */
#include "testApp.h"
#include "ofxCv/RunningBackground.h"
#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>

int thresh = 0;
bool ignore = false;
int print = 0;
bool init = true;

int randX;
int randY;
int randW ;
int newX;
int newY;

int oldalpha = 255;
int newalpha = 255;
vector<int> xCoords;
vector<int> yCoords;
vector<int> widthC;

using namespace ofxCv;
using namespace cv;

void testApp::loadImages(){
	
	
	// read from directory for now... next, load from node/Couch/JSON
    ofDirectory dir;
   
	
    int nFiles = dir.listDir("captures");//"plops");
	int savedFileCount = dir.numFiles();
    if(nFiles) {
        
        for(int i=0; i<dir.numFiles(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            cartoons.push_back(ofImage());
            cartoons.back().loadImage(filePath);
        }
    }
    else printf("Could not find folder\n");
	
		
	
}

//totally random right now... will be more dependent on context later...
void testApp::updateLayout(){
    int numpics = ofRandom(2, 10);
    if (cartoons.size() < numpics)
        numpics = cartoons.size();
  
    
    xCoords.clear();
    yCoords.clear();
    widthC.clear();
        cartoonSelection.clear();
    newalpha = 255;
    randX = ofRandom(50,50);
    randY = ofRandom(480,960);
    randW = ofRandom(450, 550);
    newX = ofRandom(20,200);
    newY = ofRandom(20,300);

    
   
    
    for(int i=0; i<numpics; i++) {
        int j = ofRandom(0,cartoons.size());
        cartoonSelection.push_back(cartoons[j]);
                
        if (i ==0){
            xCoords.push_back(randX);
            yCoords.push_back(randY);
            widthC.push_back(randW);
        
        }


        float tmp = randX + randW+ 12;
        xCoords.push_back(tmp);  //next placement algorithm
        randX = tmp;
       
        float tmp2 = randY/2;//ofRandom(0,randY/2);
        yCoords.push_back(tmp2);
        randY = tmp;
        
    
        float tmp3 = ofRandom(200,400);
        widthC.push_back(tmp3);
        randW = tmp3;
        

    }
        
    
    //eg maybe the cartoons would be updated by querying for the latest tags..
    
}
//--------------------------------------------------------------



void testApp::gotMessage(ofMessage &msg){
	cout<<"gotmessage:"<<msg.message<<endl;
    
	if (msg.message == "UpdateView"){
		
		updateLayout();
        startTime = ofGetElapsedTimeMillis();  //starttimer over
        bTimerReached = false;
		
	}
}

//--------------------------------------------------------------


void testApp::setup() {
	cam.initGrabber(640, 480);
	//background.setLearningTime(900);
	background.setThresholdValue(10);
    //background.setIgnoreForeground(true);
    
    bg.nmixtures = 7;
    bg.bShadowDetection = false;
    
    mode = 0;
    font.loadFont("verdana.ttf", 20);
    
    frame.setTo(0);
    oldframe.setTo(0);
    textBox.init();
	
	ofAddListener(textBox.evtEnter, this, &testApp::addText);
    
    bInsertTextMode = true;
    
    loadImages();
    
    ofRegisterGetMessages(this);
    bTimerReached = false;
    startTime = ofGetElapsedTimeMillis();  // display start time
    endTime = 5000; // probably should be longer for people to read the random comic 'strip' but keeping this as is for now
    
    graySS.allocate(640,480);
    
}
//--------------------------------------------------------------

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		background.update(cam, thresholded);
       
	}
}
//--------------------------------------------------------------

void testApp::draw() {
	
    if (init){
        frame = toCv(cam.getPixelsRef());
        oldframe = frame.clone();
        init = false;
}
 

    if (print++ > 2){  //swallow some frames
        frame = toCv(cam.getPixelsRef());
        oldframe = frame.clone();
        print = 0;
    }
    else;
        frame = oldframe.clone();
    
    background.getBackground();
    
    cv::Mat foreMask = background.getForeground();  //running aggregate background
  
    ofxCv::imitate(fore, foreMask, CV_8UC1);
  
    Mat gray;
    cv::cvtColor(frame, gray, CV_BGR2GRAY);
    const int MEDIAN_BLUR_FILTER_SIZE = 7;
    medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);
    Mat edges;
    const int LAPLACIAN_FILTER_SIZE = 5;
    cv::Laplacian(gray, edges, CV_8U, LAPLACIAN_FILTER_SIZE);
    
    Mat mask;
    const int EDGES_THRESHOLD =140;
    threshold(edges, mask, EDGES_THRESHOLD, 255, THRESH_BINARY_INV);
    //playing around for different effects
    // cv::erode(fore,fore,cv::Mat());
    //cv::dilate(fore,fore,cv::Mat());
    // cv::findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
    // cv::drawContours(frame,contours,-1,cv::Scalar(0,0,255),2);
     //threshold(gray, gray, 50, 255, THRESH_OTSU);
    //drawMat(gray, 0,0);
    //rcv::erode(edges,edges, cv::Mat());
    cv::dilate(edges,edges, cv::Mat());
    //cv::bitwise_not(edges, edges);
    //  drawMat(edges, 640,0);
    
    
    cv::Size size = frame.size();
    cv::Size smallSize;
    smallSize.width = size.width/2;
    smallSize.height = size.height/2;
    Mat smallImg = Mat(smallSize, CV_8UC3);
    resize(gray, smallImg, smallSize, 0,0, INTER_LINEAR);
    
    Mat tmp = Mat(smallSize,CV_8UC3);
    int repetitions = 2;
    for (int i = 0; i< repetitions; i++){
        int ksize = 9;
        double sigmaColor =9;
        double sigmaSpace = 7;
        bilateralFilter(smallImg, tmp, ksize,sigmaColor, sigmaSpace);
        bilateralFilter(tmp, smallImg, ksize, sigmaColor, sigmaSpace);

    }
   
    Mat bigImg = gray.clone();

    cv::multiply(bigImg, bigImg, bigImg);
    Mat dst = Mat(frame.size(), CV_8UC3);
    resize(smallImg, bigImg, size, 0,0, INTER_LINEAR);
    //dst.setTo(0);  //additive tracing if not cleared out... //same as memset((char*)dst.data, 0, dst.step * dst.rows);
    bigImg.convertTo(bigImg, -1,2,2);
    bigImg.copyTo(dst, mask);
    //drawMat(bigImg, 640,0);
    
    Mat contrastImg = bigImg.clone();
    
    dst.convertTo(contrastImg, -1,1.2,1);
    
    ofSetColor(255);
    ofRect(0,0,670,510);
    ofSetColor(0);
    ofRect(10,10,646,482);
    ofSetColor(255);
    int r = ofRandom(100);  //how I create believable flicker.  could still tweak
    
    
    if (r<20)
        drawMat(contrastImg,12,12);
    else
        drawMat(dst,12,12);

    ofPushMatrix();
    ofTranslate(50,130);
    textBox.draw();
    ofPopMatrix();
    
    //drawEntries();
    
    
    ///////////////
    /* simulate rolling screen of images.  break this out into it's own exe when ready */
    
    //get a few random images and load them
    //white background
    ofSetColor(0);
    ofRect(670, 0, 640,510);
    ofSetColor(0);
    ofRect(0, 510, 1280,480);
    ofSetColor(255);
    
    float timer = ofGetElapsedTimeMillis() - startTime;
    
    if(timer >= endTime && !bTimerReached) {
        bTimerReached = true;
        ofMessage msg("UpdateView");
        ofSendMessage(msg);
    }
    
    newalpha = newalpha -5;
    if (newalpha < 0) newalpha = 0;
    
    ofEnableAlphaBlending();
    ofSetColor(255,255,255,newalpha);
    
    if (cartoonSelection.size() > 0 ){
        for (int k = 0;k<cartoonSelection.size();k++){
            ofPushMatrix();
            ofLog() << xCoords[k] << " " << yCoords[k] << " " << widthC[k] << endl;
            ofTranslate(xCoords[k], yCoords[k]);
            ofSetColor(255,255,255,newalpha);
            ofRect(-10,-10,widthC[k]+20,(widthC[k]*2/3)+20);
            
            ofSetColor(0,0,0,newalpha);
            ofRect(-2,-2,widthC[k] +4,(widthC[k]*2/3)+4);
            ofSetColor(255,255,255,newalpha);
            cartoonSelection[k].draw(0,0,widthC[k],(widthC[k]*2/3));
            ofPopMatrix();

        }
        ofDisableAlphaBlending();
        ofSetColor(255,255,255,newalpha);
        
    }


    
    
    
}




//--------------------------------------------------------------
/*
 void testApp::keyPressed(int key){
 //if we don't setup the event using
 //textInput::init(), then we can
 //send key events ourselves here
 //manually. like so:
 
 textBox.keyPressed(key);
 }*/

//--------------------------------------------------------------
void testApp::addText(string &s) {
	textEntries.push_back(s);
	textBox.clear();
}
//--------------------------------------------------------------
void testApp::drawEntries() {
	for (int i=0; i<textEntries.size(); ++i) {
		ofDrawBitmapString(textEntries[i], 50, 100+10*i);
	}
}

//toDO
void testApp::getImagesBy(){
    
    ofHttpResponse resp = ofLoadURL("localhost:3333/fetch");
    cout << resp.data << endl;
    //JSON handling here
    
}

void testApp::keyPressed(int key) {
    
    if (!bInsertTextMode){
        
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
        if(key == 'm') {
            mode++;
            if (mode > 2) mode = 0;
        }
        
        if (key=='s'){
        
            saveImage =  ofToDataPath("captures") + "/storyfaceme" + ofGetTimestampString() + ".png";
            screenshot.grabScreen(0,0,640,480);
            screenshot.crop(0,0,640,480);
           
            
            
            screenshot.saveImage(saveImage);
            cartoons.push_back(ofImage());
            cartoons.back().loadImage(saveImage);

        
        }
    }
   
    if (key ==OF_KEY_CONTROL){
        userString =="";
       
        if (bInsertTextMode)
            ofRemoveListener(textBox.evtEnter, this, &testApp::addText);
        else
            ofAddListener(textBox.evtEnter, this, &testApp::addText);
         bInsertTextMode = !bInsertTextMode;
    
    }
       
}