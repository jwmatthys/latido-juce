//
//  portAudioRender.h
//  This software uses the following open source packages: JUCE (GUI API), libpd (audio dsp), portaudio (Audio API)
//  You are free to use this however you like, all licenses to other packages are as is.
//  The JUCE library is GPL, so any other instances of this code must also be GPL licensed.
//
//  Copyright by Robert Esler on 9/11/14.
//
//


#ifndef portAudioRender
#define portAudioRender
#define BLOCKSIZE 64

#include <iostream>
#include "portaudio.h" /*if you are using portaudio on another platform, other than Mac OSX,
 you may need to compile it and import it into this package*/

typedef struct  {
    
    double inbuf[BLOCKSIZE];
    double outbuf[BLOCKSIZE*2]; //stereo output
} Buffer;

class portAudio {
    
private:
    unsigned long samplerate = 4410;
    int blocksize = BLOCKSIZE;
    Buffer theBuffer;
    void* handle = NULL;
    PaStream *stream; //opens the audio stream
    PaError err;

public:
    
    
    portAudio();
    ~portAudio();

    static int paCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData );
    
    int initPortAudio();
    
    void sendError(int );
    
    //send data to pd with these type of routines
    void updateVolume(double);
    void bangSweep();
    void delayOn(bool);
    
    void setSampleRate(unsigned long);
    unsigned long getSampleRate();
    int getBlockSize();
    
    
};



#endif /* defined(ortAudioRender) */
