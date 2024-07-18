/*
  ==============================================================================

    PlaylistComponent.h
    Created: 1 Feb 2024 8:01:17pm
    Author:  arcsl

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DeckGUI.h"
#include "SoundTrack.h"
#include "WaveformDisplay.h"
#include <fstream>

//==============================================================================

/**
* The PlaylistComponent class is a custom graphical user interface component 
* designed to handle a playlist of soundtracks.
* 
* It inherits from several JUCE classes and interfaces to facilitate audio playback control,
* including juce::Component, juce::TableListBoxModel, juce::Button::Listener,
* juce::TextEditor::Listener, and juce::FileDragAndDropTarget.
*/
class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public juce::Button::Listener,
                          public juce::TextEditor::Listener,
                          public juce::FileDragAndDropTarget
{
public:
    /**
     * Constructor for the PlaylistComponent class.
     *
     * @param _formatManager    Reference to the AudioFormatManager.
     * @param _deckGUI1         Pointer to the first DeckGUI instance.
     * @param _deckGUI2         Pointer to the second DeckGUI instance.
     */
    PlaylistComponent(juce::AudioFormatManager& _formatManager, 
        DeckGUI* _deckGUI1, DeckGUI* _deckGUI2);

    /**
     * Destructor for the PlaylistComponent class.
     */
    ~PlaylistComponent() override;

    /**
     * Override of the paint method for custom rendering.
     *
     * @param g Graphics context for performing drawing operations.
     */
    void paint(juce::Graphics&) override;

    /**
     * Override of the resized method to manage component size changes.
     */
    void resized() override;

    /**
     * Override of the getNumRows method to determine the number of rows in the playlist.
     *
     * @return The number of rows in the playlist.
     */
    int getNumRows() override;

    /**
     * Override of the paintRowBackground method to handle the rendering of background
     * for a specific row in the playlist.
     *
     * @param g             Graphics context for performing drawing operations.
     * @param rowNumber     The index of the row to be painted.
     * @param width         The width of the row.
     * @param height        The height of the row.
     * @param rowIsSelected Indicates whether the row is currently selected.
     */
    void paintRowBackground(Graphics& g,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override;

    /**
     * Override of the paintCell method to handle the rendering of content
     * within a specific cell of the playlist.
     *
     * @param g             Graphics context for performing drawing operations.
     * @param rowNumber     The index of the row containing the cell to be painted.
     * @param columnId      The ID of the column containing the cell to be painted.
     * @param width         The width of the cell.
     * @param height        The height of the cell.
     * @param rowIsSelected Indicates whether the row containing the cell is currently selected.
     */
    void paintCell(Graphics& g,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;

    /**
     * Override of the refreshComponentForCell method to handle the creation or update
     * of components within a specific cell of the playlist.
     *
     * @param rowNumber                 The index of the row containing the cell to be refreshed.
     * @param columnId                  The ID of the column containing the cell to be refreshed.
     * @param isRowSelected             Indicates whether the row containing the cell is currently selected.
     * @param existingComponentToUpdate Pointer to the existing component that needs to be updated.
     * @return                          A pointer to the updated or newly created component.
     */
    juce::Component* refreshComponentForCell(int rowNumber,
        int columnId,
        bool isRowSelected,
        Component* existingComponentToUpdate) override;

private:
    
    /**
     * Function to add a soundtrack from a music file to the playlist.
     *
     * @param musicFile The File object representing the music file to be added.
     */
    void addSoundTrack(const juce::File& musicFile);

    /**
     * Function to delete a soundtrack from the playlist by its ID.
     *
     * @param id The ID of the soundtrack to be deleted.
     */
    void deleteMusic(int id);

    /**
     * Function to load a selected track into the specified DeckGUI.
     *
     * @param deckGUI Pointer to the DeckGUI instance where the track will be loaded.
     */
    void loadToSpecifiedPlayer(DeckGUI* deckGUI);

    /**
     * Function to check if a music title matches any in the playlist.
     *
     * @param musicName The music title to check.
     * @return True if a match is found, false otherwise.
     */
    bool matchingMusicTitle(const juce::String& MusicName) const;

    /**
     * Callback function triggered when the user presses the "Enter" key in the search box.
     *
     * @param editor Reference to the TextEditor component.
     */
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;

    /**
     * Retrieve the length of a music file in minutes and seconds using its URL.
     *
     * @param audioURL The URL of the audio file.
     * @return A pair representing the length in minutes and seconds.
     */
    std::pair<int, int> getMusicLength(const juce::URL& audioURL);

    /**
     * Determines whether the PlaylistComponent is interested in file drag events.
     *
     * @param files An array of file paths being dragged.
     * @return true if interested in file drag events, false otherwise.
     */
    bool isInterestedInFileDrag(const StringArray& file) override;

    /**
     * Handles the event when files are dropped onto the PlaylistComponent.
     *
     * @param files An array of file paths that have been dropped.
     * @param x The x-coordinate where the files were dropped.
     * @param y The y-coordinate where the files were dropped.
     */
    void filesDropped(const StringArray& files, int x, int y) override;

    /**
     * Override of the buttonClicked method to handle button click events.
     *
     * @param button Pointer to the clicked button.
     */
    void buttonClicked(Button* button) override;

    /**
     * Function to clear all songs in the playlist component.
     */
    void clearPlaylist();

    /**
     * Function to save the current playlist data to a file.
     */
    void savePlaylistToFile();

    /**
     * Reads existing playlist data from a file and populates the PlaylistComponent.
     * This method is called during the initialization of the PlaylistComponent.
     */
    void readExistingPlaylistData();

    /**
     * Vector to store soundtrack information
     */
    std::vector<SoundTrack> soundTrack;

    /**
     * AudioFormatManager reference
     */
    AudioFormatManager& formatManager;

    /**
     * TableListBox for displaying the playlist
     */
    juce::TableListBox tableComponent;

    /**
     * Buttons for importing and loading tracks to Deck 1 and 2
     */
    juce::TextButton importTrackToLib{ "Import To Track Library" };
    juce::TextButton loadBtn1{ "Load\nDeck1" },
                     loadBtn2{ "Load\nDeck2" },
                     clearPlaylistBtn{ "Clear" };

    /**
     * Text editor for searching tracks in the playlist
     */
    juce::TextEditor searchBox{ "Input Song Name: " };
    juce::String searchBoxInput;

    /**
     * FileChooser for importing tracks
     */
    juce::FileChooser fChooser{ "Select the track..." };

    /** 
     * Pointer to the first DeckGUI instance
     */
    DeckGUI* deckGUI1;
    /**
     * Pointer to the second DeckGUI instance
     */
    DeckGUI* deckGUI2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};