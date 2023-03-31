#ifndef APP_SETTINGS_HPP
#define APP_SETTINGS_HPP

// Configurations for the app front-end

// Whether the OpenFrameworks app should run in the main() function
#define APP_ENABLE true
// Whether audio should be setup and rendered
#define APP_ENABLE_AUDIO true
// The app's re-rendering frame rate in frames per second
#define APP_FRAME_RATE 24
// The app's output audio frequency in Hz
#define APP_AUDIO_RATE 44100
// Number of output audio channels (left and right)
#define APP_AUDIO_NUM_CHANNELS 2
// Number of microphone inputs
#define APP_AUDIO_NUM_INPUTS 0
// The sample rate frequencyin Hz
#define APP_AUDIO_SAMPLE_RATE APP_AUDIO_RATE
// Audio buffer size in bytes
#define APP_AUDIO_BUFFER_SIZE 256
// The number of audio buffers
#define APP_AUDIO_N_NUM_BUFFERS 4
// Controls the GUI size
#define APP_DRAW_WINDOW_WIDTH 800
// Height is same as width -> square GUI window
#define APP_DRAW_WINDOW_HEIGHT APP_DRAW_WINDOW_WIDTH
// When grid lines should be drawn
#define APP_DRAW_GRID_SPACING_PX 50

#endif  // APP_SETTINGS_HPP