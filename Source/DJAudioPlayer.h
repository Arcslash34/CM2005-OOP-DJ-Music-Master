/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 31 Jan 2024 12:05:59pm
    Author:  arcsl

  ==============================================================================
*/

#pragma once
// Include juce library
#include <JuceHeader.h>

/**
 * The DJAudioPlayer class is responsible for audio playback.
 * It implements the AudioSource interface and provides methods for loading
 * audio files, controlling playback parameters, and managing audio resources.
 */
class DJAudioPlayer : public AudioSource
{
public:
    /**
     * Constructor for DJAudioPlayer.
     * @param _formatManager Reference to the AudioFormatManager.
     */
    DJAudioPlayer(AudioFormatManager& _formatManager);

    /**
     * Destructor for DJAudioPlayer.
     */
    ~DJAudioPlayer();

    /**
     * Called to signal that the audio device is ready to start processing audio.
     * @param samplesPerBlockExpected The number of samples in each block of audio.
     * @param sampleRate The sample rate of the audio stream.
     */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    
    /**
     * Override of the getNextAudioBlock method to provide the next audio block.
     * @param bufferToFill The buffer that will be filled with audio data.
     */
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    
    /**
     * Override of the releaseResources method to release audio resources.
     */
    void releaseResources() override;

    // Audio playback control functions
    /**
     * Load a new audio file from the specified URL.
     * @param audioURL The URL of the audio file to load.
     */
    void loadURL(URL audioURL);

    /**
     * Set the gain (volume) of the audio player.
     * @param gain The desired gain value (between 0.0 and 1.0).
     */
    void setGain(double gain);

    /**
     * Set the speed (resampling ratio) of the audio player.
     * @param ratio The desired resampling ratio (positive value, typically between 0 and 5).
     */
    void setSpeed(double ratio);

    /**
     * Set the playback position in seconds.
     * @param posInSec The desired playback position in seconds.
     */
    void setPosition(double posInSec);

    /**
     * Set the playback position based on a relative value.
     * @param pos The relative playback position (between 0 and 1).
     */
    void setPositionRelative(double pos);

    /**
     * Start audio playback.
     */
    void start();

    /**
     * Pause audio playback.
     */
    void pause();

    /**
     * Get the relative position of the playback.
     * @return The relative position of the playback.
     */
    double getPositionRelative();

private:
    // & make it reference
    /**
     * Reference to the AudioFormatManager.
     */
    AudioFormatManager& formatManager;

    /**
     * Unique pointer to manage the AudioFormatReaderSource's lifecycle.
     */
    std::unique_ptr<AudioFormatReaderSource> readerSource;

    /**
     * AudioTransportSource for managing audio playback.
     */
    AudioTransportSource transportSource;

    /**
     * ResamplingAudioSource with transportSource as the input, no interpolation, and a resampling ratio of 2.
     */
    ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
};