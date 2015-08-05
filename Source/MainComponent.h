//  This software uses the following open source packages: JUCE (GUI API), libpd (audio dsp), portaudio (Audio API)
//  You are free to use this however you like, all licenses to other packages are as is.
//  The JUCE library is GPL, so any other instances of this code must also be GPL licensed.
//
//  Copyright by Robert Esler on 9/11/14.
//
//


#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "portAudioRender.h"
#include "PdObject.h"
#include "PdBase.hpp"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component, public Button::Listener, public Label::Listener,
                            public Slider::Listener 

{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();
    void buttonClicked(Button *);
    void labelTextChanged(Label *);
    void sliderValueChanged(Slider *);
    
    String centerText;
    portAudio *theAudio;


private:
    //==============================================================================
    ToggleButton *togButton1;
    TextButton *textButton1;
    Slider  *slider1;
    Label *label1;
    Label *volume;
    Slider::SliderStyle sliderStyle = Slider::SliderStyle::LinearVertical;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
