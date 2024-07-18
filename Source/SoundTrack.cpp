/*
  ==============================================================================

    SoundTrack.cpp
    Created: 16 Feb 2024 11:58:46am
    Author:  arcsl

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SoundTrack.h"

// Constructor to initialize the SoundTrack with a name and URL
SoundTrack::SoundTrack(juce::String _MusicName, juce::String _MusicUrl)
    : MusicName(_MusicName), MusicUrl(_MusicUrl)
{
}
// Destructor
SoundTrack::~SoundTrack()
{
}