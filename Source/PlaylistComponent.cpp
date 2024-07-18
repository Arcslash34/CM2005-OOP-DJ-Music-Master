/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 1 Feb 2024 8:01:17pm
    Author:  arcsl

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"


//==============================================================================
PlaylistComponent::PlaylistComponent(AudioFormatManager& _formatManager, 
                                     DeckGUI* _deckGUI1, 
                                     DeckGUI* _deckGUI2)
    : formatManager(_formatManager), 
    deckGUI1(_deckGUI1), 
    deckGUI2(_deckGUI2)
{
    formatManager.registerBasicFormats();

    readExistingPlaylistData();

    // Add buttons and editable text box to make them visible.
    addAndMakeVisible(importTrackToLib);
    addAndMakeVisible(loadBtn1);
    addAndMakeVisible(loadBtn2);
    addAndMakeVisible(searchBox);
    addAndMakeVisible(clearPlaylistBtn);

    // Add listeners for click events.
    importTrackToLib.addListener(this);
    loadBtn1.addListener(this);
    loadBtn2.addListener(this);
    searchBox.addListener(this);
    clearPlaylistBtn.addListener(this);

    loadBtn1.setColour(TextButton::textColourOffId, Colours::orange);
    importTrackToLib.setColour(TextButton::textColourOffId, Colours::orange);
    loadBtn2.setColour(TextButton::textColourOffId, Colours::deepskyblue);
    clearPlaylistBtn.setColour(TextButton::textColourOffId, Colours::deepskyblue);

    // Configure columns for the table component
    tableComponent.getHeader().addColumn("Track Title", 1, 320);
    tableComponent.getHeader().addColumn("Music Length (MM/SS)", 2, 150);
    tableComponent.getHeader().addColumn("Remove", 3, 150);

    // Set the model for the table component
    tableComponent.setModel(this);

    // Make the table component visible
    addAndMakeVisible(tableComponent);

    // Display placeholder text in the search box
    searchBox.setTextToShowWhenEmpty("Search here...", juce::Colours::navajowhite);

    // Center the placeholder text vertically
    searchBox.setJustification(juce::Justification::centred);

    // Set the font size for the placeholder text
    searchBox.setFont(16.0f);

    // Enable keyboard focus for the TextEditor
    searchBox.setWantsKeyboardFocus(true);

    // Ensure that "Enter" key press is not treated as a newline
    searchBox.setReturnKeyStartsNewLine(false);
}

PlaylistComponent::~PlaylistComponent()
{
    // Save the current playlist state before closing the application
    savePlaylistToFile();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void PlaylistComponent::resized()
{
    // Calculate width and height based on the component's size
    int width = getWidth() / 10;
    int height = getHeight() / 12;

    // Set the bounds for the respective buttons
    searchBox.setBounds(0, 0, getWidth(), height);
    loadBtn1.setBounds(0, height, width, height * 4);
    loadBtn2.setBounds(width * 9, height, width, height * 4);
    tableComponent.setBounds(width, height, width * 8, height * 4);
    clearPlaylistBtn.setBounds(getWidth() / 2, height * 5, getWidth() / 2, height);
    importTrackToLib.setBounds(0, height * 5, getWidth() / 2, height);

    // Adjust column widths in the table header based on the component's width
    tableComponent.getHeader().setColumnWidth(1, getWidth() * 0.4);
    tableComponent.getHeader().setColumnWidth(2, getWidth() * 0.31);
    tableComponent.getHeader().setColumnWidth(3, getWidth() * 0.09);
}


int PlaylistComponent::getNumRows()
{
    // return size of sound track
    return soundTrack.size();
}

void PlaylistComponent::paintRowBackground(Graphics & g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(Colours::orange);
    }
    else
    {
        g.fillAll(Colours::transparentBlack);
    }
}

void PlaylistComponent::paintCell(juce::Graphics & g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    g.setColour(Colours::white);
    g.drawText(soundTrack[rowNumber].MusicName,
        2,
        0,
        width - 4,
        height,
        Justification::centredLeft,
        true);
}

juce::Component* PlaylistComponent::refreshComponentForCell(int row,
    int columnId,
    bool isRowSelected,
    juce::Component* componentToUpdate)
{
    if (columnId == 2)
    {
        // Check if the rowNumber is within the valid range
        if (row >= 0 && row < soundTrack.size())
        {
            // Get the music URL for the current row
            juce::URL musicURL = soundTrack[row].MusicUrl;

            // Use the modified getMusicLength function to get the minutes and seconds
            std::pair<int, int> musicLength = getMusicLength(musicURL);

            int minutes = musicLength.first;
            int seconds = musicLength.second;

            // Attempt to cast the existing componentToUpdate to a Label
            juce::Label* durationLabel = dynamic_cast<juce::Label*>(componentToUpdate);

            if (durationLabel == nullptr)
            {
                // If componentToUpdate is not a Label, create a new Label
                durationLabel = new juce::Label{};
                durationLabel->setJustificationType(juce::Justification::centred);
                durationLabel->setColour(juce::Label::textColourId, juce::Colours::white);
                durationLabel->setColour(juce::Label::backgroundColourId, juce::Colours::transparentWhite);

                // Set the label as the componentToUpdate
                componentToUpdate = durationLabel;
            }

            // Set the label's text to the minutes and seconds
            durationLabel->setText(juce::String::formatted("%02d:%02d", minutes, seconds),
                juce::dontSendNotification); // tell the system not to trigger any listeners or observers associated with the change
        }
    }
    else if (columnId == 3)
    {
        if (componentToUpdate == nullptr)
        {
            // If componentToUpdate is null, create a new TextButton
            juce::TextButton* deleteButton = new juce::TextButton{ "Del" };

            // Convert the row index to a string and set it as the component ID
            juce::String id{ std::to_string(row) };
            deleteButton->setComponentID(id);

            // Add a listener for the delete button
            deleteButton->addListener(this);
            deleteButton->setColour(TextButton::textColourOffId, Colours::gold);

            // Set the TextButton as the componentToUpdate
            componentToUpdate = deleteButton;
        }
    }

    // Return the updated or newly created component
    return componentToUpdate;
}

// When a button is clicked
void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &importTrackToLib)
    {
        DBG("PlaylistComponent::buttonClicked - Import track button was clicked");

        // Launch the file chooser and able to select multiple files
        auto fileChooser = juce::FileBrowserComponent::canSelectMultipleItems;
        fChooser.launchAsync(fileChooser, [this](const juce::FileChooser& chooser)
            {
                // Iterate through all the selected files
                for (const auto& file : chooser.getResults())
                {
                    // Add the selected file to the playlist
                    juce::File musicFile{ file };
                    addSoundTrack(musicFile);
                }
            });
    }
    else if (button == &loadBtn1)
    {
        DBG("PlaylistComponent::buttonClicked - Load to Deck1 button was clicked");

        // Load the selected music into deck1
        loadToSpecifiedPlayer(deckGUI1);
    }
    else if (button == &loadBtn2)
    {
        DBG("PlaylistComponent::buttonClicked - Load to Deck2 button was clicked");

        // Load the selected music into deck2
        loadToSpecifiedPlayer(deckGUI2);
    }
    else if (button == &clearPlaylistBtn)
    {
        DBG("PlaylistComponent::buttonClicked - Clear Playlist button was clicked");

        // Display a confirmation dialog before clearing the playlist
        juce::AlertWindow confirmDelete("Clear Playlist", "Are you sure you want to clear the playlist?", juce::AlertWindow::QuestionIcon);
        confirmDelete.setUsingNativeTitleBar(true);
        confirmDelete.addButton("Clear", 1); // If the "Clear" button is clicked, returns 1
        confirmDelete.addButton("Cancel", 0); // If the "Cancel" button is clicked, returns 0

        // define JUCE_MODAL_LOOPS_PERMITTED=1 at "defined at preprocessor definations"
        // For runModalLoop()
        // Run the modal loop to wait for user input in the confirmation dialog
        int result = confirmDelete.runModalLoop();

        if (result == 1) // "Clear" button clicked
        {
            // Clear the playlist if confirmed
            clearPlaylist();
        }
    }
    else
    {
        // Remove a track based on the button's component ID
        int id = std::stoi(button->getComponentID().toStdString());
        deleteMusic(id);
    }
}

// Convert the music file into music name and url and add into the SoundTrack
void PlaylistComponent::addSoundTrack(const juce::File& musicFile)
{
    DBG("PlaylistComponent::addSoundTrack");

    // Get the music name and URL of the of the file.
    juce::String musicName = musicFile.getFileNameWithoutExtension();
    juce::String musicUrl = juce::URL{ musicFile }.toString(false);

    // Check if the musicName is already in the playlist.
    if (matchingMusicTitle(musicName))
    {
        DBG(musicName << " is already loaded");
        return;  // Exit early if the track is already in the playlist.
    }
    // Create a new SoundTrack object and add it to the playlist.
    SoundTrack newTrack{ musicName, musicUrl };
    soundTrack.push_back(newTrack);

    // Update the table content.
    tableComponent.updateContent();
}

// Delete music from the playlist
void PlaylistComponent::deleteMusic(int id)
{
    DBG("PlaylistComponent::deleteMusic");
    // remove the music at the requested index within the playlist
    soundTrack.erase(soundTrack.begin() + id);
    tableComponent.updateContent();
}

// Load track into a specified player
void PlaylistComponent::loadToSpecifiedPlayer(DeckGUI* deckGUI)
{
    DBG("PlaylistComponent::loadInPlayer");

    try
    {
        // Get the selected row from the table component.
        std::optional<int> selectedRow = tableComponent.getSelectedRow();

        // Check if no row is selected
        if (!selectedRow.has_value())
        {
            throw std::runtime_error("Error: No row selected.");
        }

        // Check if the selected row is within the valid range of soundTrack.
        if (selectedRow.value() < 0 || selectedRow.value() >= soundTrack.size())
        {
            throw std::out_of_range("Error: Selected row is out of bounds.");
        }
        // Load the selected track into the deck.
        deckGUI->loadMusicFileToApplication(soundTrack[selectedRow.value()].MusicUrl);
    }
    catch (const std::exception& e)
    {
        // Handle exceptions and print error messages.
        DBG("Exception caught: " << e.what());
    }
}

bool PlaylistComponent::matchingMusicTitle(const juce::String& MusicName) const
{
    // Store the titles of imported audio tracks in a set
    std::unordered_set<juce::String> musicTitles;

    // Populate with existing track titles
    for (const auto& music : soundTrack)
    {
        musicTitles.insert(music.MusicName);
    }

    // Check if the imported music title exists
    return musicTitles.find(MusicName) != musicTitles.end();
}

// It updates the search results in the searhbox based on the search input
void PlaylistComponent::textEditorReturnKeyPressed(juce::TextEditor& editor)
{
    // Handle the "Enter" key press
    searchBoxInput = editor.getText().toLowerCase();  // Convert search input to lowercase
    int selectedRow = -1;  // Initialize selected row index

    // Check if the search bar contains any input
    if (!searchBoxInput.isEmpty())
    {
        // Iterate through the tracks in the playlist to find a matching track.
        for (int i = 0; i < soundTrack.size(); ++i)
        {
            // Convert track name to lowercase for case-insensitive comparison
            juce::String trackNameLowerCase = soundTrack[i].MusicName.toLowerCase();

            // Check if the track name contains the lowercase search input
            if (trackNameLowerCase.contains(searchBoxInput))
            {
                selectedRow = i;  // Record the index of the matching track
                break;  // Exit the loop when a match is found.
            }
        }
    }

    // Select the row if a match is found, otherwise deselect all rows.
    if (selectedRow != -1)
    {
        tableComponent.selectRow(selectedRow);  // Select the matching row in the table
    }
    else
    {
        tableComponent.deselectAllRows();  // Deselect all rows if no match is found
    }

    // Update the table to display the search results.
    tableComponent.updateContent();  // Refresh the table to reflect the selected/deselected rows
}

// make use of url to fet the minutes and seconds of each files
std::pair<int, int> PlaylistComponent::getMusicLength(const juce::URL& musicURL)
{
    // Create an AudioFormatReader for each audio track URL
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(musicURL.createInputStream(false)));

    if (reader != nullptr) {
        // Get the length of the audio track in seconds
        double audioLengthInSeconds = reader->lengthInSamples / static_cast<double>(reader->sampleRate);

        // Convert to minutes and seconds
        int minutes = static_cast<int>(audioLengthInSeconds / 60);
        int seconds = static_cast<int>(audioLengthInSeconds - minutes * 60);

        // Return a pair of minutes and seconds
        return std::make_pair(minutes, seconds);
    }
    else {
        // Return default value if an error occurs
        return std::make_pair(0, 0);
    }
}

bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files)
{
    DBG("PlaylistComponent::isInterestedInFileDrag");
    return true; // Always interested in file drag events
}

void PlaylistComponent::filesDropped(const StringArray& files, int x, int y)
{
    DBG("PlaylistComponent::filesDropped");

    if (files.size() > 0)
    {
        // Iterate through all the dropped files
        for (const auto& file : files)
        {
            // Add each file to the playlist
            juce::File musicFile{ file };
            addSoundTrack(musicFile);
        }   
    }
}

void PlaylistComponent::clearPlaylist()
{
    // Clear the playlist by removing all sound tracks
    soundTrack.clear();

    // Clear the associated text file
    std::ofstream playlistFile("CurrentPlaylist.txt", std::ios::trunc);

    // Update the table after clearing the playlist
    tableComponent.updateContent();
}

void PlaylistComponent::savePlaylistToFile()
{
    try {
        // Open the text file CurrentPlaylist.txt for writing
        std::ofstream playlistFile("CurrentPlaylist.txt");

        // Check if the file is open for writing
        if (!playlistFile.is_open()) {
            DBG("Error: Could not open playlist file for writing.");
            return;
        }

        // Iterate through soundTrack and write each track's information to the file
        for (const auto& track : soundTrack) {
            playlistFile << track.MusicName << "," << track.MusicUrl << std::endl;
        }

        // Close the file
        playlistFile.close();
    }
    catch (const std::exception& e) {
        DBG("Exception caught while saving playlist: " << e.what());
    }
}

void PlaylistComponent::readExistingPlaylistData()
{
    try {
        // Open the text file CurrentPlaylist.txt for reading
        std::ifstream file("CurrentPlaylist.txt");
        std::string str;

        // Check if the file is open for reading
        if (!file.is_open()) {
            DBG("Error: Could not open playlist file for reading.");
            return;
        }

        // Read each line and add it to soundTrack
        while (std::getline(file, str)) {
            std::istringstream iss(str);
            std::string musicName, musicUrl;

            // Extract MusicName and MusicUrl from each line
            if (std::getline(std::getline(iss, musicName, ','), musicUrl)) {
                // Debug print for verification
                DBG(musicName);
                DBG(musicUrl);
                // Add a new SoundTrack to soundTrack
                soundTrack.emplace_back(SoundTrack{ musicName, musicUrl });
                tableComponent.updateContent();
            }
        }

        // Close the file
        file.close();
    }
    catch (const std::exception& e) {
        DBG("Exception caught while reading playlist: " << e.what());
    }
}