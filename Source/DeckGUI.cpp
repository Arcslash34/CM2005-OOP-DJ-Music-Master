/*
  ==============================================================================

    DeckGUI.cpp
    Created: 31 Jan 2024 1:20:40pm
    Author:  arcsl

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 AudioFormatManager & formatManagerToUse,
                 AudioThumbnailCache & cacheToUse,
                 bool isDeck1) 
    : player(_player),
      waveformDisplay(formatManagerToUse, cacheToUse, isDeck1),
      isDeck1(isDeck1)
{
    // Output the music name and waveform
    initializeComponents();

    // Output the buttons
    initializeButtons();

    // slider
    initializeSliders();

    // configure vol speed position and dj slider
    configureSlider(volSlider, 0.0, 1.0, 0.5, Slider::LinearBarVertical, Slider::NoTextBox, true, 0.5);
    configureSlider(speedSlider, 0.0, 5.0, 1.0, Slider::Rotary, Slider::TextBoxBelow, true, 1.0);
    configureSlider(posSlider, 0.0, 1.0, 0.0, Slider::LinearBar, Slider::NoTextBox, false, 0.0);
    configureSlider(djSlider, 0.0, 1.0, 0.0, Slider::Rotary, Slider::NoTextBox, false, 0.0);

    // Listeners for the buttons
    addAndMakeVisible(loopToggleBtn);
    loopToggleBtn.addListener(this);

    startTimer(100);    
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(
        juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

}

void DeckGUI::resized()
{
    float height = getHeight() * 0.1;
    float width = getWidth() * 0.2;

    musicNameLabel.setBounds(0, height * 0.25, getWidth(), height);

    // set where the waveformdisplay is located
    posSlider.setBounds(0, height * 1.5, getWidth(), height * 2);
    waveformDisplay.setBounds(0, height * 1.5, getWidth(), height * 2);

    if (isDeck1)
    {
        // Set bounds for components in Deck 1
        djSlider.setBounds(width * 2.45, height * 3.9, width * 2.15, width * 2.15);
        loadBtn.setBounds(width * 0.7, height * 7.6, height, height);
        playBtn.setBounds(width * 1.2, height * 7.3, height * 1.5, height * 1.5);
        pauseBtn.setBounds(width * 1.9, height * 7.6, height, height);
        loopToggleBtn.setBounds(width * 1.1, height * 8.9, width, height);
        volSlider.setBounds(width * 0.1, height * 3.75, width * 0.4, height * 6);
        speedSlider.setBounds(width * 0.88, height * 4.5, width * 1.2, width * 0.8);
    }
    else
    {
        // Set bounds for components in Deck 2
        djSlider.setBounds(width * 0.4, height * 3.9, width * 2.15, width * 2.15);
        loadBtn.setBounds(width * 2.7, height * 7.6, height, height);
        playBtn.setBounds(width * 3.2, height * 7.3, height * 1.5, height * 1.5);
        pauseBtn.setBounds(width * 3.9, height * 7.6, height, height);
        loopToggleBtn.setBounds(width * 3.1, height * 8.9, width, height);
        volSlider.setBounds(width * 4.5, height * 3.75, width * 0.4, height * 6);
        speedSlider.setBounds(width * 2.88, height * 4.5, width * 1.2, width * 0.8);
    }
}


void DeckGUI::buttonClicked(Button* button) {
    // Play
    if (button == &playBtn) {
        DBG("Play button was clicked.");
        player->start();
    }
    // Pause
    if (button == &pauseBtn) {
        DBG("Pause button was clicked.");
        player->pause();
    }
    // Load file
    // Load file
    if (button == &loadBtn) {
        DBG("Load button was clicked.");
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
            {
                auto chosenFile = chooser.getResult();
                if (chosenFile.exists()) {
                    DBG("Chosen file exists: " << chosenFile.getFullPathName());

                    // Construct a URL from the chosen file
                    juce::URL chosenUrl(chosenFile);
                    DBG("Constructed URL: " << chosenUrl.toString(true));

                    // Load the URL into the player and waveform display
                    player->loadURL(chosenUrl);
                    waveformDisplay.loadURL(chosenUrl);

                    // Update the musicNameLabel content                    
                    updateLabels(chosenUrl);

                    DBG("Music Name Label Text: " << musicNameLabel.getText());
                }
            });
    }
}

void DeckGUI::sliderValueChanged(Slider* slider) {
    if (slider == &volSlider) {
        DBG("vol slider moved." << volSlider.getValue());
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider) {
        DBG("speed slider moved." << speedSlider.getValue());
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider) {
        DBG("pos slider moved." << posSlider.getValue());
        player->setPositionRelative(slider->getValue());
    }
    if (slider == &djSlider) {
        player->setPositionRelative(slider->getValue());
    }
}


bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    DBG("DeckGUI::filesDropped");
    if (files.size() == 1)
    {
        // Load the URL into the player and waveform display
        juce::URL musicUrl = juce::URL{ juce::File{files[0]} };
        loadMusicFileToApplication(musicUrl);
    }
}

// Added music looping feature
void DeckGUI::timerCallback()
{
    // Update the waveform display position
    double currentPos = player->getPositionRelative();

    if (currentPos > 0.0 && currentPos < 1.0)
    {
        waveformDisplay.setPositionRelative(currentPos);
        posSlider.setValue(currentPos);
        djSlider.setValue(currentPos);

        auto angle = currentPos * 360.0;

        otherLookAndFeel.rotateDisc(angle);

        repaint();
    }
    // Check if the audio playback has reached the end
    if (player->getPositionRelative() >= 1)
    {
        // Set position back to the start
        player->setPositionRelative(0);

        // Check if loop toggle button is enabled
        bool isLoopEnabled = loopToggleBtn.getToggleState();
        if (isLoopEnabled)
        {
            DBG("Loop button is enabled.");
            // Start playing the audio file
            player->start();
        }
        else
        {
            DBG("Loop button is disabled.");
            // Stop the audio file
            player->pause();
        }
    }
    
}

// Load an audio file from a URL as input into the player and waveform display for visualization
void DeckGUI::loadMusicFileToApplication(const juce::URL& musicUrl)
{
    player->loadURL(musicUrl);
    waveformDisplay.loadURL(musicUrl);

    // Update the musicNameLabel content
    updateLabels(musicUrl);
}

void DeckGUI::updateLabels(const juce::URL& musicUrl)
{
    // Update the musicNameLabel content
    juce::File musicFile(musicUrl.getLocalFile());
    String musicName = musicFile.getFileNameWithoutExtension();
    musicNameLabel.setText("< Now Playing >\n  <  " + musicName + "  >", dontSendNotification);
}

void DeckGUI::initializeComponents()
{
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(musicNameLabel);
    musicNameLabel.setJustificationType(Justification::centred);
    musicNameLabel.setFont(Font(16.0f).boldened());
}

void DeckGUI::initializeButtons()
{
    Image playImage = ImageCache::getFromMemory(BinaryData::play_png, BinaryData::play_pngSize);
    Image pauseImage = ImageCache::getFromMemory(BinaryData::pause_png, BinaryData::pause_pngSize);
    Image loadImage = ImageCache::getFromMemory(BinaryData::upload_png, BinaryData::upload_pngSize);

    addAndMakeVisible(playBtn);
    addAndMakeVisible(pauseBtn);
    addAndMakeVisible(loadBtn);

    playBtn.addListener(this);
    pauseBtn.addListener(this);
    loadBtn.addListener(this);

    playBtn.setImages(true, true, true, playImage, 1.0f, isDeck1 ? Colours::orange : Colours::deepskyblue,
        playImage, 1.0f, Colours::yellow,
        playImage, 1.0f, Colours::orange);

    pauseBtn.setImages(true, true, true, pauseImage, 1.0f, isDeck1 ? Colours::orange : Colours::deepskyblue,
        pauseImage, 1.0f, Colours::yellow,
        pauseImage, 1.0f, Colours::orange);

    loadBtn.setImages(true, true, true, loadImage, 1.0f, isDeck1 ? Colours::orange : Colours::deepskyblue,
        loadImage, 1.0f, Colours::yellow,
        loadImage, 1.0f, Colours::orange);

    musicNameLabel.setColour(Label::textColourId, isDeck1 ? Colours::orange : Colours::deepskyblue);
    volSlider.setColour(Slider::trackColourId, isDeck1 ? Colours::orange : Colours::deepskyblue);

    otherLookAndFeel2.setSlider(speedSlider, isDeck1);
    speedLabel.setColour(Label::textColourId, isDeck1 ? Colours::orange : Colours::deepskyblue);
    loopToggleBtn.setColour(ToggleButton::textColourId, isDeck1 ? Colours::orange : Colours::deepskyblue);
}

void DeckGUI::initializeSliders()
{
    // Volume Slider
    addAndMakeVisible(volSlider);
   

    // Speed Slider
    addAndMakeVisible(speedSlider);
    speedSlider.setNumDecimalPlacesToDisplay(2);

    // Position Slider
    addAndMakeVisible(posSlider);
    posSlider.setColour(Slider::trackColourId, Colours::grey.withAlpha(0.3f));

    // DJ Slider
    addAndMakeVisible(djSlider);
    djSlider.setLookAndFeel(&otherLookAndFeel);
}

void DeckGUI::configureSlider(Slider& slider, double minValue, double maxValue, double startValue,
    Slider::SliderStyle style, Slider::TextEntryBoxPosition textBoxPos,
    bool doubleClickReturnValue, double interval)
{
    slider.setRange(minValue, maxValue);
    slider.setValue(startValue);
    slider.setSliderStyle(style);
    slider.setTextBoxStyle(textBoxPos, false, 60, 20);
    slider.setDoubleClickReturnValue(doubleClickReturnValue, interval);
    slider.addListener(this);
}