#include <stdlib.h>
#include "ofApp.h"
#include "mover.h"

//--------------------------------------------------------------

ofApp::ofApp() {}
ofApp::~ofApp() {}

void ofApp::setup(){
    ofBackground(0, 0, 0);
    ofSetWindowShape(500, 500);
    
    int bufferSize = 512;
    sampleRate = 44100;
    nInputChans = 2;
    low_counter = 0;
    mid_counter = 0;
    upper_counter = 0;
    screen_counter = 0;
    screen_counter_lim = 15;
    mid_frame_cycle = 1000;
    low_frame_cycle = 6000;
    upper_frame_cycle = 8000;
    
    pop_player.initializeFmod();
    ofFmodSetBuffersize(bufferSize);
    pop_player.load("pop.wav", true);
    pop_player.setVolume(1);
    pop_player.setMultiPlay(true);
    
    for (int i = 0; i < sizeof(currKeyState)/sizeof(int); i++) {
        currKeyState[i] = 0;
    }
    for (int i = 0; i < NUM_OF_MOVERS; i++) {
        mover new_mover(pop_player);
        new_mover.setup((rand()%(ofGetWidth()-10)+5), (rand()%(ofGetHeight()-10)+5), MOVER_RADIUS, pop_player);
        mover_arr.push_back(new_mover);
    }
    
    lAudio.assign(bufferSize, 0.0);
    rAudio.assign(bufferSize, 0.0);
    audioBuffer = new float*[2];
    audioBuffer[0] = &lAudio[0];
    audioBuffer[1] = &rAudio[0];
    
    soundStream.setup(this, nInputChans, 0, sampleRate, bufferSize, 4);
    
    ofSetFrameRate(60);
    
    // Setup our Faust reverb
    drone.init(sampleRate); // initializing the Faust module
    drone.buildUserInterface(&droneControl); // linking the Faust module to the controler
    
    droneControl.setParamValue("/ambience/lower_level", 0);
    droneControl.setParamValue("/ambience/mid_level", 0);
    droneControl.setParamValue("/ambience/upper_level", 0);
}

//--------------------------------------------------------------
void ofApp::update(){
    for (int i = 0; i < mover_arr.size(); i++) {
        mover_arr[i].update(currKeyState, currNoiseKeyState);
    }
    volume = MIN_VOLUME + (mover_arr[0].getNoiseRatio() * MAX_VOLUME);
    
    if (screen_counter >= (screen_counter_lim * (1-mover_arr[0].getNoiseRatio()))+5) {
        ofSetWindowShape(ofGetWidth()-1, ofGetHeight()-1);
        screen_counter = 0;
    } else {
        screen_counter++;
    }
    
    if (mid_counter >= mid_frame_cycle){
        ofSetWindowShape(ofGetWidth()-1, ofGetHeight()-1);
        mid_counter = 0;
    } else {
        mid_counter++;
    }
    
    if (low_counter >= low_frame_cycle){
        low_counter = 0;
    } else {
        low_counter++;
    }
    
    if (upper_counter >= upper_frame_cycle){
        upper_counter = 0;
    } else {
        upper_counter++;
    }
    
    float low_angle = ((2*M_PI)/low_frame_cycle) * low_counter;
    float next_low_volume = (1 + sin(low_angle)) / 2;
    droneControl.setParamValue("/ambience/lower_level", next_low_volume);
    
    float mid_angle = ((2*M_PI)/mid_frame_cycle) * mid_counter;
    float next_mid_volume = (1 + sin(mid_angle)) / 2;
    droneControl.setParamValue("/ambience/mid_level", next_mid_volume);
    
    float upper_angle = ((2*M_PI)/mid_frame_cycle) * mid_counter;
    float next_upper_volume = (1 + sin(upper_angle)) / 2;
    droneControl.setParamValue("/ambience/upper_level", next_upper_volume);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(225);
    ofDrawBitmapString("The State of the Union", 10, 20);
    ofDrawBitmapString("'x' to increase fear, 'z' to decrease", 10, 40);
    ofDrawBitmapString("arrow keys to try and escape", 10, 60);
    ofDrawBitmapString("by the way, the whole application is collapsing in on itself..", 10, 80);
    ofDrawBitmapString("but you can drag it out if you want (bottom right-hand corner)", 10, 100);
    for (int i = 0; i < mover_arr.size(); i++) {
        mover_arr[i].draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key > 355 && key < 360) {
        //left
        if (key == 356) currKeyState[0] = 1;
        //up
        if (key == 357) currKeyState[1] = 1;
        //right
        if (key == 358) currKeyState[3] = 1;
        //down
        if (key == 359) currKeyState[2] = 1;
    }
    if (key == 122) {
        currNoiseKeyState[0] = true;
    }
    if (key == 120) {
        currNoiseKeyState[1] = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key > 355 && key < 360) {
        //left
        if (key == 356) currKeyState[0] = 0;
        //up
        if (key == 357) currKeyState[1] = 0;
        //right
        if (key == 358) currKeyState[3] = 0;
        //down
        if (key == 359) currKeyState[2] = 0;
    }
    if (key == 122) {
        currNoiseKeyState[0] = false;
    }
    if (key == 120) {
        currNoiseKeyState[1] = false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::audioOut(float *output, int bufferSize, int nChannels){
    mutex.lock();
    drone.compute(bufferSize, audioBuffer, audioBuffer);
    mutex.unlock();
    
    // Interleave the output buffer
    for (int i = 0; i < bufferSize; i++)
    {
        output[2*i] = output[2*i+1] = audioBuffer[0][i] * volume;
    }
}
