//
//  mover.h
//  quarterSystem
//
//  Created by Matthew Herrero on 11/1/16.
//
//

#ifndef mover_h
#define mover_h

#include <stdio.h>
#include <iostream>
#include <random>
#include "ofMain.h"

class mover{
    
public:
    mover(ofFmodSoundPlayer& pop);
    ~mover();
    
    void setup(int x, int y, int rad, ofFmodSoundPlayer& pop);
    void update(int currKeyStates[], bool currNoiseKeyStates[]);
    void draw();
    
    int getX();
    int getY();
    float getXSpeed();
    float setXSpeed(float new_speed);
    float getYSpeed();
    float setYSpeed(float new_speed);
    float getNoiseRatio();
    
private:
    void updateXSpeeds(int currKeyStates[]);
    void updateYSpeeds(int currKeyStates[]);
    void updateNoiseMagnitude(bool zDown, bool xDown);
    float addNoiseVal(float low, float high);
    
    int radius, window_width, window_height;
    float x_position, y_position, x_speed, y_speed, noise_magn;
    ofFmodSoundPlayer& pop_player;
    const float MAX_NOISE = 20;
    const float DAMPING = 0.5;
    int MAX_SPEED = 12;
};

#endif /* mover_h */

