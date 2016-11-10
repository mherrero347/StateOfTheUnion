#pragma once

#include "ofMain.h"
#include "mover.h"

#include "Ambience.h"
#include "ofSoundBuffer.h"
#include "ofSoundUtils.h"
#include "ofLog.h"
#include <limits>

class ofApp : public ofBaseApp{

	public:
        ofApp();
        ~ofApp();
    
        void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void audioOut(float * input, int bufferSize, int nChannels);
    
        void playPop();
    
    private:
        /* UI STUFF */
        vector<mover> mover_arr;
        int currKeyState [4];
        bool currNoiseKeyState [2];
    
        /* SOUND PROCESSING STUFF */
        ofSoundStream soundStream;
        int sampleRate, nInputChans;
        float volume;
        const float MIN_VOLUME = 0.2f;
        const float MAX_VOLUME = 0.8f;
        vector <float> lAudio;
        vector <float> rAudio;
        ofMutex mutex;
    
        /*VOLUME TRACKING STUFF */
        int mid_counter, low_counter, upper_counter, screen_counter;
        int mid_frame_cycle, low_frame_cycle, upper_frame_cycle, screen_counter_lim;
    
        /* FAUST STUFF */
        float **audioBuffer;
        Ambience drone;
        MapUI droneControl;
    
        /* POP PLAYING STUFF */
        ofFmodSoundPlayer pop_player;
    
        /* CONSTANTS */
        const int NUM_OF_MOVERS = 300;
        const int MOVER_RADIUS = 5;
    
};
