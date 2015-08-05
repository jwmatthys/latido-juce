//
//  portAudioRender.cpp
//  This software uses the following open source packages: JUCE (GUI API), libpd (audio dsp), portaudio (Audio API)
//  You are free to use this however you like, all licenses to other packages are as is.
//  The JUCE library is GPL, so any other instances of this code must also be GPL licensed.
//
//  Copyright by Robert Esler on 9/11/14.
//
//

#include "portAudioRender.h"
#include "z_libpd.h"
#include <errno.h>
#include <unistd.h>

portAudio::portAudio() {
    
}

portAudio::~portAudio() {
    
    err = Pa_StopStream( stream );
    if( err != paNoError )
        this->sendError(err);
    err = Pa_CloseStream( stream );
    if( err != paNoError )
        this->sendError(err);
    Pa_Terminate();
    if(handle != NULL)
        libpd_closefile(handle);
   
}

void pdprint(const char *s) {
    printf("libpd print: %s", s);
}

//This is the portaudio callback function, the audio is processed
//in the for loop.
int portAudio::paCallback( const void *inputBuffer, void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags,
                                 void *userData ) {
    
    /* Cast data passed through stream to our structure. */
    Buffer *theBuffer = (Buffer*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;
    
    (void) inputBuffer; /* Prevent unused variable warning. */
    
    //This is the audio data buffered from Pd.  We get it in chunks of 64,
    //since we are using stereo, the outbuf gets two chucks, or 128 numbers
    libpd_process_double(1, theBuffer->inbuf, theBuffer->outbuf);
    
    //dsp perform routine, channels are interlaced (e.g out[0] = left, out[1] = right, etc.)
    for( i=0; i<framesPerBuffer*2; i++ )
    {
        if(i % 2)
            *out++ = theBuffer->outbuf[i]; // right channel
        else
            *out++ = theBuffer->outbuf[i]; // left channel
    }
    
    return 0;

}

int portAudio::initPortAudio() {
   
    //the *hook functions deal with receiving messages from Pd, see https://github.com/libpd/libpd/wiki/libpd
    libpd_set_printhook(pdprint);
    //libpd_set_noteonhook(pdnoteon);
    
    //libpd_set_floathook(pdfloat);
    libpd_init();


    libpd_init_audio(2, 2, this->getSampleRate()); //one channel in, one channel out
    
    // compute audio    [; pd dsp 1(
    libpd_start_message(1); // one entry in list
    libpd_add_float(1.0f);
    libpd_finish_message("pd", "dsp");
    
    // open patch       [; pd open file folder(, the handle is a void*
    handle = libpd_openfile("latido.pd","../../Source");
    
    //pass a number to pd, myMessage is a pd receive
    libpd_float("myMessage", 10011);
    
    for (int i = 0; i < this->getBlockSize(); i++) {
        //intialize buffer, this is probably unnecessary
        theBuffer.outbuf[i] = 0;
    }
   
    /* Initialize portaudio library before making any other calls. */
    err = Pa_Initialize();
    if( err != paNoError )
        this->sendError(err);
    
    /* Open an portaudio I/O stream. */
    err = Pa_OpenDefaultStream( &stream,
                               1,          /* input channels */
                               2,          /* output channels */
                               paFloat32,  /* 32 bit floating point output */
                               portAudio::getSampleRate(),
                               portAudio::getBlockSize(),        /* frames per buffer */
                               portAudio::paCallback,
                               &theBuffer );
    if( err != paNoError )
        this->sendError(err);
    
    err = Pa_StartStream( stream );
    if( err != paNoError )
        this->sendError(err);
    
    return err;
    
}

void portAudio::sendError(int err) {
    if(err)
    {
        Pa_Terminate();
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    }
}

void portAudio::updateVolume(double v) {
    
    libpd_float("volume", v);
    
}

void portAudio::bangSweep() {
    
    libpd_bang("sweep");
}

void portAudio::delayOn(bool on) {
    
    float delay = (float)on;
    
    libpd_float("delayOn", delay);
}


void portAudio::setSampleRate(unsigned long sr) {
    samplerate = sr;
}

unsigned long portAudio::getSampleRate() {
    return samplerate;
}

int portAudio::getBlockSize() {
    return blocksize;
}
