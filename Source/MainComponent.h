#pragma once

// Include juce library
#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
/**
 * The MainComponent class represents the main application component.
 *
 * It acts as the central point for managing audio playback, UI components (DJ decks, playlist),
 * and serves as the entry point for the application. The class also initializes and manages
 * various audio-related objects such as DJAudioPlayer, DeckGUI, and PlaylistComponent.
 */
class MainComponent : public AudioAppComponent,
					  public Slider::Listener
{
public:
	//==============================================================================
	/**
	 * Constructor for the MainComponent.
	 */
	MainComponent();

	/**
	 * Destructor for the MainComponent.
	 */
	~MainComponent();

	//==============================================================================
	/**
	 * Prepares the audio device for playback.
	 * @param samplesPerBlockExpected The expected number of samples per block.
	 * @param sampleRate The sample rate of the audio device.
	 */
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

	/**
	 * Processes the next audio block.
	 * @param bufferToFill Information about the buffer that needs to be filled with audio samples.
	 */
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

	/**
	 * Releases audio resources.
	 */
	void releaseResources() override;

	//==============================================================================
	/**
	 * Paints the graphical user interface of the MainComponent.
	 * @param g The Graphics object used for rendering.
	 */
	void paint(Graphics& g) override;
	/**
	 * Called when the size of the MainComponent is changed.
	 * Adjusts the size and position of the child components to fit the new dimensions.
	 */
	void resized() override;


private:
	//==============================================================================
	/**
	 * AudioFormatManager to handle audio file formats.
	 */
	AudioFormatManager formatManager;

	/**
	 * AudioThumbnailCache to cache waveforms, storing up to 100 waveforms.
	 */
	AudioThumbnailCache thumbCache{ 100 };

	/**
	 * First DJAudioPlayer using the format manager.
	 */
	DJAudioPlayer player1{ formatManager };

	/**
	 * First DeckGUI associated with player1, using format manager and thumbnail cache.
	 */
	DeckGUI deckGUI1{ &player1, formatManager, thumbCache , true};

	/**
	 * Second DJAudioPlayer using the same format manager.
	 */
	DJAudioPlayer player2{ formatManager };

	/**
	 * Second DeckGUI associated with player2, using format manager and thumbnail cache.
	 */
	DeckGUI deckGUI2{ &player2, formatManager, thumbCache , false};

	/**
	 * MixerAudioSource to mix the outputs of player1 and player2.
	 */
	MixerAudioSource mixerSource;

	/**
	 * PlaylistComponent associated with format manager, deckGUI1, and deckGUI2.
	 */
	PlaylistComponent playlistComponent{ formatManager, &deckGUI1, &deckGUI2 };

	/**
	 * Slider for controlling volume balance between deck 1 and deck 2.
	 */
	Slider crossFadeSlider;

	/**
	 * Set up the crossfade slider, configuring its initial parameters.
	 */
	void setupCrossFadeSlider();

	/**
	 * Callback function triggered when the value of the slider is changed.
	 *
	 * @param slider   The Slider object whose value has changed.
	 */
	void sliderValueChanged(Slider* slider) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
