//
//  mover.cpp
//  quarterSystem
//
//  Created by Matthew Herrero on 11/1/16.
//
//

#include "ofMain.h"
#include "mover.h"

mover::mover(ofFmodSoundPlayer& pop) : pop_player(pop) {}

mover::~mover(){}

void mover::setup(int x, int y, int rad, ofFmodSoundPlayer& pop){
    y_speed, x_speed, noise_magn = 0;
    x_position = x;
    y_position = y;
    radius = rad;
    pop_player = pop;
}

// this takes in the current state of which arrow keys are being
// held down, and updates the speeds in the x direction accordingly.
// it also protects against a mover getting caught on an edge by bumping
// it back 3 pixels if it gets caught on the wrong side of the fence
void mover::updateXSpeeds(int currKeyStates[]) {
    //0 = left, 1 = up, 2 = down, 3 = right
    if (x_position < ofGetWidth() && x_position > 0) {
        if (currKeyStates[0]) {
            if (x_speed > -MAX_SPEED) x_speed--;
        } else if (x_speed < 0) {
            x_speed += DAMPING;
        }
        //right
        if (currKeyStates[3]) {
            if (x_speed < MAX_SPEED) x_speed++;
        } else if (x_speed > 0) {
            x_speed -= DAMPING;
        }
    } else {
        if (x_position > ofGetWidth()) x_position = ofGetWidth() - 3;
        if (x_position < 0) x_position = 3;
        pop_player.play();
        x_speed = -x_speed;
    }
}

// this takes in the current state of which arrow keys are being
// held down, and updates the speeds in the y direction accordingly.
// it also protects against the mover getting caught on an edge by bumping
// it back 3 pixels if it gets caught on the wrong side of the fence
void mover::updateYSpeeds(int currKeyStates[]) {
    //0 = left, 1 = up, 2 = down, 3 = right
    if (y_position < ofGetHeight() && y_position > 0) {
        if (currKeyStates[1]) {
            if (y_speed > -MAX_SPEED) y_speed--;
        } else if (y_speed < 0) {
            y_speed += DAMPING;
        }
        //down
        if (currKeyStates[2]) {
            if (y_speed < MAX_SPEED) y_speed++;
        } else if (y_speed > 0) {
            y_speed -= DAMPING;
        }
    } else {
        if (y_position > ofGetHeight()) y_position = ofGetHeight() - 3;
        if (y_position < 0) y_position = 3;
        pop_player.play();
        y_speed = -y_speed;
    }
}

void mover::updateNoiseMagnitude(bool zDown, bool xDown) {
    if(zDown && !xDown){
        if(noise_magn > 0) noise_magn -= 0.5;
    }
    if (xDown && !zDown){
        if(noise_magn < MAX_NOISE) noise_magn += 0.5;
    }
}

// this just generates a random real number between low and high
float mover::addNoiseVal(float low, float high) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_real_distribution<> distr(low, high); // define the range
    return distr(eng);
}


void mover::update(int currKeyStates[],  bool currNoiseKeyStates[]) {
    updateXSpeeds(currKeyStates);
    updateYSpeeds(currKeyStates);
    updateNoiseMagnitude(currNoiseKeyStates[0], currNoiseKeyStates[1]);
    x_position += x_speed + addNoiseVal(-noise_magn, noise_magn);
    y_position += y_speed + addNoiseVal(-noise_magn, noise_magn);
}

void mover::draw() {
    ofDrawCircle(x_position, y_position, radius);
}

int mover::getX() {
    return x_position;
}

int mover::getY() {
    return y_position;
}

float mover::getXSpeed(){
    return x_speed;
}

float mover::setXSpeed(float new_speed){
    if (new_speed < MAX_SPEED && new_speed > -MAX_SPEED) {
        x_speed = new_speed;
    } else {
        x_speed = MAX_SPEED;
    }
}

float mover::getYSpeed(){
    return y_speed;
}

float mover::setYSpeed(float new_speed){
    if (new_speed < MAX_SPEED && new_speed > -MAX_SPEED) {
        y_speed = new_speed;
    } else {
        y_speed = MAX_SPEED;
    }
}

float mover::getNoiseRatio() {
    return noise_magn/MAX_NOISE;
}
