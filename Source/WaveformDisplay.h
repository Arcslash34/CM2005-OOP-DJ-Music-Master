/*
  ==============================================================================

    WaveformDisplay.h
    Created: 31 Jan 2024 2:14:44pm
    Author:  arcsl

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * The WaveformDisplay class represents a graphical display of an audio waveform.
 * It utilizes the JUCE library's AudioThumbnail to visualize the waveform.
 */
class WaveformDisplay  : public juce::Component,
                         public ChangeListener
{
public:
    /**
    * Constructor for WaveformDisplay.
    *
    * @param formatManagerToUse Reference to the AudioFormatManager.
    * @param cacheToUse Reference to the AudioThumbnailCache.
    */
    WaveformDisplay(AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse,
        bool isDeck1);

    /**
     * Destructor for WaveformDisplay.
     */
    ~WaveformDisplay() override;

    /**
     * Paints the graphical user interface of the WaveformDisplay.
     *
     * @param g The Graphics object used for rendering.
     */
    void paint (juce::Graphics&) override;

    /**
     * Called when the size of the WaveformDisplay is changed.
     * Adjusts the size and position of the components to fit the new dimensions.
     */
    void resized() override;

    /**
     * Callback function for change events in the audio thumbnail.
     *
     * @param source Pointer to the ChangeBroadcaster that triggered the change.
     */
    void changeListenerCallback(ChangeBroadcaster* source) override;

    /**
     * Load an audio file from a URL and update the waveform display.
     *
     * @param audioURL The URL of the audio file to be loaded.
     */
    void loadURL(URL audioURL);

    /**
     * Set the playhead position relative to the audio waveform.
     *
     * @param pos The relative position of the playhead (0.0 to 1.0).
     */
    void setPositionRelative(double pos);

private:
    /**
     * The AudioThumbnail object used to visualize the audio waveform in the WaveformDisplay.
     */
    AudioThumbnail audioThumb;

    /**
     * A flag indicating whether an audio file has been successfully loaded into the WaveformDisplay.
     */
    bool fileLoaded;

    /**
     * The relative position of the playhead within the loaded audio file.
     */
    double position;

    /**
     * A boolean flag indicating the type of deck.
     * If true, it represents Deck 1; if false, it represents Deck 2.
     */
    bool isDeck1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
