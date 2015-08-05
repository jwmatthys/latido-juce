//  This software uses the following open source packages: JUCE (GUI API), libpd (audio dsp), portaudio (Audio API)
//  You are free to use this however you like, all licenses to other packages are as is.
//  The JUCE library is GPL, so any other instances of this code must also be GPL licensed.
//
//  Copyright by Robert Esler on 9/11/14.
//
//


#include "MainComponent.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize (500, 400);
    theAudio = new portAudio;
    theAudio->sendError( theAudio->initPortAudio() );
    
    centerText = "An audio template using JUCE, libpd and portaudio";
    textButton1 = new TextButton("Bang!");
    togButton1 = new ToggleButton("Delay ON/OFF");
    slider1 = new Slider("Slider1");
    label1 = new Label("Label", "Press Button for Sound");
    volume = new Label("Label", "Volume");
    
    slider1->setSliderStyle(sliderStyle);
    
    addAndMakeVisible(textButton1);
    addAndMakeVisible(togButton1);
    addAndMakeVisible(slider1);
    addAndMakeVisible(label1);
    addAndMakeVisible(volume);
    
    textButton1->setBounds(10,10,50,20);
    togButton1->setBounds(80, 80, 100, 40);
    slider1->setBounds (60, 40, 20, 260);
    label1->setBounds (80, 10, 280, 20);
    volume->setBounds(40, 300, 120, 20);
    
    //add listeners so that the main window knows when you are interacting with the GUI elements
    textButton1->addListener(this);
    textButton1->setName("button1");//set button names
    togButton1->setName("togButton");
    togButton1->addListener(this);
    
    label1->addListener(this);
    slider1->addListener(this);
    
    slider1->setRange(0, 100, 1);
 
    
    
}

//destructor for the main window.
//deleteAllChildren() will delete all new GUI objects created in the constructor
MainContentComponent::~MainContentComponent()
{
    delete theAudio;//this will close the audio too
    deleteAllChildren();
}

//add your custom window dressing here
void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xffffeedd));

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
    g.drawText (centerText, getLocalBounds(), Justification::centred, true);
}

//polymorphism: required function if inheriting Button::Listener
void MainContentComponent::buttonClicked(Button *button){
    
    String name = button->getName();
    
    if(name == "button1")
    {
        label1->setText("Sent to Pd", sendNotificationSync);
        theAudio->bangSweep();
    }
    
    if(name == "togButton")
    {
        label1->setText("Toggle Pressed", sendNotificationSync);
        theAudio->delayOn(togButton1->getToggleState());
    }
    
}

//polymorphism: required function if inheriting a Label::Listener
void MainContentComponent::labelTextChanged(Label *label) {
    
    
}

//polymorphism: required function if inherting a Slider::Listener
void MainContentComponent::sliderValueChanged(Slider *slider) {
    
    double value = slider->getValue();
    
    theAudio->updateVolume(value);
    
    
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
