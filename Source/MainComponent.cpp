#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);

    //formatManager.registerBasicFormats();

    // Add and initialize the slider
    setupCrossFadeSlider();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{

    // ************
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    // ************
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
    // Clear memory when stopped
    mixerSource.removeAllInputs();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    float height = getHeight() * 0.5;
    float width = getWidth() * 0.5;
    float height2 = getHeight() * 0.1;
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    deckGUI1.setBounds(0, 0, width, height);
    deckGUI2.setBounds(width, 0, width, height);
    playlistComponent.setBounds(0, height2 * 5.5, getWidth(), height2 * 9);

    // Set the bounds of the crossFadeSlider
    crossFadeSlider.setBounds(getWidth() * 0.1, height2 * 5, getWidth() * 0.8, height2*0.5);
}

// Setting the crossfader
void MainComponent::setupCrossFadeSlider()
{
    addAndMakeVisible(crossFadeSlider);
    crossFadeSlider.setSliderStyle(Slider::LinearHorizontal);
    crossFadeSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    crossFadeSlider.setRange(0.0, 1.0, 0.01); // Range from 0 to 1 with a step of 0.01
    crossFadeSlider.setValue(0.5); // Set initial value to center position
    crossFadeSlider.setDoubleClickReturnValue(true, 0.5);
    crossFadeSlider.setColour(Slider::trackColourId, juce::Colours::orange);
    crossFadeSlider.setColour(Slider::backgroundColourId, juce::Colours::deepskyblue);
    crossFadeSlider.addListener(this);
}

// Handling slider value changes
void MainComponent::sliderValueChanged(Slider* slider)
{
    DBG("Slider Value: " << slider->getValue());

    // Adjust volume levels based on slider position
    float sliderValue = slider->getValue();
    double gain1 = 1.0 - sliderValue; // Gain for deck 1
    double gain2 = sliderValue;       // Gain for deck 2

    player1.setGain(gain1);
    player2.setGain(gain2);
}