# 🎶 DJ Music Master: Otodecks 🎛️

Otodecks is a feature-rich DJ application developed as an Object-Oriented Programming (OOP) project. This application aims to provide a cutting-edge platform for DJs to mix, control, and enhance music playback creatively. With innovative features, user-friendly controls, and a sleek GUI, Otodecks offers an intuitive and professional DJing experience.

---

## 🚀 Features

### **1. Loading Audio Files into Players**
- **Drag-and-Drop Support**: Import audio files effortlessly by dragging them onto the application.
- **File Chooser**: Select audio files through a file dialog for easy loading.
- **Real-Time Validation**: Ensures files are compatible before loading.

### **2. Dual Deck Audio Playback**
- Supports playing **two tracks simultaneously** with independent controls for each deck.
- Includes individual playback controls (play, pause) and waveform displays.

### **3. Track Mixing**
- Vary the volume of each track to achieve a seamless blend.
- Adjustable **crossfader** for smooth transitions between tracks.

### **4. Speed Control**
- Modify playback speed using a slider (range: 0.1x to 5x).
- Debug messages for real-time feedback on speed changes.

### **5. GUI Enhancements**
- **Custom Sliders**:
  - Volume and speed sliders with rotary and bar styles.
  - Disk-style position sliders for an immersive DJ feel.
- **Waveform Display**:
  - Colour-coded visualizations for Deck 1 and Deck 2.
  - Real-time playback position updates.

### **6. Playlist Management**
- **Music Length Column**: Displays the duration of each track in minutes and seconds.
- **Clear Playlist Button**: Clear all tracks from the playlist with confirmation.
- **Save and Load Playlists**:
  - Automatically saves the current playlist to a text file when the app closes.
  - Reloads the playlist when the app reopens.
- **Import Tracks**:
  - Add multiple tracks at once to the playlist.
  - Load tracks directly to Deck 1 or Deck 2.

### **7. Real-Time Track Display**
- Displays the name of the currently loaded track for each deck.

### **8. Search Functionality**
- Search for tracks in the playlist by name (case-insensitive).
- Highlights matching tracks for quick selection.

### **9. Additional Features**
- **Loop Toggle Button**: Enable or disable looping for tracks.
- **Crossfader**: Smoothly transitions audio between decks.

---

## 🎨 GUI Design
- Inspired by **YouDJ** online DJ software.
- Features:
  - Modern, intuitive layout with easy-to-access controls.
  - Unique rotary sliders for playback speed and DJ effects.
  - Play, pause, and load buttons with custom icons.
  - Colour themes for Deck 1 (orange) and Deck 2 (blue).

---

## 📂 File Structure
- **`MainComponent`**: Manages the main audio mixing logic.
- **`DeckGUI`**: Handles individual deck controls and displays.
- **`PlaylistComponent`**: Manages track imports, playlist display, and search functionality.
- **Assets**:
  - Disk and button images for GUI customization.
- **Data**:
  - Auto-saved playlist file (`CurrentPlaylist.txt`).

---

## 🛠️ Setup Instructions
1. **Prerequisites**:
   - Visual Studio 2022
   - JUCE framework installed on your system
2. **Clone the Repository**:
   ```bash
   git clone https://github.com/Arcslash34/DJ-Music-Master.git
   cd DJ-Music-Master
