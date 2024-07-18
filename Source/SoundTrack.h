/*
  ==============================================================================

    SoundTrack.h
    Created: 16 Feb 2024 11:58:46am
    Author:  arcsl

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SoundTrack
{
public:
    /**
     * Constructor to initialize the SoundTrack with a name and URL.
     *
     * @param musicName The name of the music track.
     * @param musicUrl The URL of the music track.
     */
    SoundTrack(juce::String _MusicName, juce::String _MusicUrl);

    /**
     * Destructor.
     */
    ~SoundTrack();

    /**
     * Public member variables to store the music name and URL
     */
    juce::String MusicName;
    juce::String MusicUrl;
};