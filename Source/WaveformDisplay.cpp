/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 31 Jan 2024 2:14:44pm
    Author:  arcsl

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse,
                                 AudioThumbnailCache& cacheToUse,
                                 bool isDeck1) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse),
                                 isDeck1(isDeck1),
                                 // "file not loaded" is not being printed,
                                 // thus adding fileLoaded(false)
                                 fileLoaded(false),
                                 position(0)
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(
        juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    if (fileLoaded)
    {
        if (isDeck1)
            g.setColour(Colours::orange);
        else
            g.setColour(Colours::deepskyblue);

        audioThumb.drawChannel(g, getLocalBounds(), 0, 
            audioThumb.getTotalLength(), 0, 1.0f);
    }
    else
    {
        if (isDeck1)
            g.setColour(Colours::orange.withAlpha(0.5f));
        else
            g.setColour(Colours::deepskyblue.withAlpha(0.7f).brighter());

        g.setFont(Font(20.0f).withTypefaceStyle("Bold"));
        
        g.drawText("Load Music Files :3", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(URL audioURL) {
    DBG("WaveformDisplay::loadURL");

    // clear any existing data it holds
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster *source){
    DBG("wfd: change received!");
    repaint();
}

// set position as isnan to prevent activation of breakpoint
void WaveformDisplay::setPositionRelative(double pos) {
    if (pos != position && !isnan(pos)) {
        position = pos;
        repaint();
    }
}