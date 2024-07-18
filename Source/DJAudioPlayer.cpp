/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 31 Jan 2024 12:05:59pm
    Author:  arcsl

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager) : 
    formatManager(_formatManager)
{
}

DJAudioPlayer::~DJAudioPlayer() {}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) 
{
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources() 
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0) 
    {
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    }
    else
    {
        transportSource.setGain(gain);
    }
}

// changed the ratio to be <= 0 to prevent breakpoint activation
void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio <= 0 || ratio > 5.0) {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 5");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSec)
{
    transportSource.setPosition(posInSec);
}

void DJAudioPlayer::start()
{
    transportSource.start();
}

void DJAudioPlayer::pause()
{
    transportSource.stop();
}

// Get the relative position of the playback
double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

// Set the playback position based on a relative value
void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        // Print a warning if the relative position is out of bounds
        std::cout << "DJAudioPlayer::setPositionRelative: Relative pos should be between 0 and 1." << std::endl;
    }
    else
    {
        // Convert relative position to absolute seconds and set the position
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}