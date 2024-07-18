/*
  ==============================================================================

    DeckGUI.h
    Created: 31 Jan 2024 1:20:40pm
    Author:  arcsl

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "OtherLookAndFeel.h"
#include "OtherLookAndFeel2.h"

//==============================================================================
/**
 * The DeckGUI class represents the graphical user interface for a DJ deck.
 * 
 * It handles user interactions, such as button clicks and slider adjustments,
 * and communicates with the DJAudioPlayer for audio playback. The class also
 * incorporates a waveform display for visualizing the audio waveform.
 */
class DeckGUI  : public juce::Component,
                 public Button::Listener,
                 public Slider::Listener,
                 public FileDragAndDropTarget,
                 public Timer
{
public:
    /**
     * Constructor for DeckGUI.
     *
     * @param _player Pointer to the associated DJAudioPlayer.
     * @param formatManagerToUse Reference to the AudioFormatManager.
     * @param cacheToUse Reference to the AudioThumbnailCache.
     */
    DeckGUI(DJAudioPlayer* player,
        AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse,
        bool isDeck1);

    /**
     * Destructor for DeckGUI.
     */
    ~DeckGUI() override;

    /**
     * Paints the graphical user interface of the DeckGUI.
     * @param g The Graphics object used for rendering.
     */
    void paint (juce::Graphics&) override;

    /**
     * Called when the size of the DeckGUI is changed.
     * Adjusts the size and position of the components to fit the new dimensions.
     */
    void resized() override;

    /**
     * Handles button clicks on the DeckGUI.
     * @param button Pointer to the button that was clicked.
     */
    void buttonClicked(Button*) override;

    /**
     * Handles slider value changes on the DeckGUI.
     * @param slider Pointer to the slider that had its value changed.
     */
    void sliderValueChanged(Slider* slider) override;

    /**
     * Checks whether the DeckGUI is interested in file drag events.
     *
     * @param files An array of file paths that are being dragged.
     * @return True if the DeckGUI is interested in the file drag events, false otherwise.
     */
    bool isInterestedInFileDrag(const StringArray& file) override;

    /**
     * Handles the event when files are dropped onto the DeckGUI.
     *
     * @param files An array of file paths that have been dropped.
     * @param x The x-coordinate of the drop location.
     * @param y The y-coordinate of the drop location.
     */
    void filesDropped(const StringArray& files, int x, int y) override;

    /**
     * Timer callback function.
     * This function is called at regular intervals to check and update the audio playback status,
     * and manage features such as looping.
     */
    void timerCallback() override;

    /**
     * Load an audio file from a URL as input into the DJAudioPlayer and WaveformDisplay
     * for visualization and playback within the DeckGUI.
     *
     * @param musicUrl The URL of the audio file to be loaded.
     */
    void loadMusicFileToApplication(const juce::URL& musicUrl);

    /**
     * Update music name label
     *
     * @param musicUrl The URL of the audio file name to be displayed.
     */
    void updateLabels(const juce::URL& musicUrl);

private:

    /**
     *  Define ImageButton for play, pause, load, and a ToggleButton for loop
     */
    ImageButton playBtn{ "PLAY" }, pauseBtn{ "PAUSE" }, loadBtn{ "LOAD" };
    ToggleButton loopToggleBtn{ "LOOP" };

    /**
     *  Define sliders for volume, speed, and position
     */
    Slider volSlider, speedSlider, posSlider, djSlider;

    /**
     *  FileChooser for selecting a file
     */
    FileChooser fChooser{ "Select a file..." };

    /**
     *  Declare pointers for DJAudioPlayer, waveform display, and AudioTransportSource
     */
    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;
    AudioTransportSource transportSource;

    /**
     *  Label for displaying music name and speed
     */
    Label musicNameLabel, speedLabel;

    /**
     * Custom look-and-feel class for defining the appearance of UI components.
     * Used for general customization.
     */
    OtherLookAndFeel otherLookAndFeel;

    /**
     * 2nd custom look-and-feel class for defining the appearance of rotary sliders.
     * Used to set specific colors for sliders based on the deck they belong to.
     */
    OtherLookAndFeel2 otherLookAndFeel2;

    /**
 * Boolean flag indicating whether the component belongs to Deck 1.
 */
    bool isDeck1;

    /**
     * Initialize various components used in the DJ application.
     */
    void initializeComponents();

    /**
     * Initialize buttons used in the DJ application, such as play, pause, and load buttons.
     */
    void initializeButtons();

    /**
     * Initialize sliders used in the DJ application, such as volume, speed, and position sliders.
     */
    void initializeSliders();

    /**
     * Configure a slider with specified parameters.
     *
     * @param slider                The Slider object to configure.
     * @param minValue              The minimum value of the slider.
     * @param maxValue              The maximum value of the slider.
     * @param startValue            The initial value of the slider.
     * @param style                 The style of the slider.
     * @param textBoxPos            The position of the text box associated with the slider.
     * @param doubleClickReturnValue   Whether double-clicking the slider returns to a default value.
     * @param interval              The interval for the slider's double-click return value.
     */
    void configureSlider(Slider& slider, double minValue, double maxValue, double startValue,
        Slider::SliderStyle style, Slider::TextEntryBoxPosition textBoxPos,
        bool doubleClickReturnValue, double interval);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
