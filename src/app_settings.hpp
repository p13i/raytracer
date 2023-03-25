#ifndef APP_SETTINGS_HPP
#define APP_SETTINGS_HPP

// Configurations for the app front-end

// Whether audio should be setup and rendered
#define APP_ENABLE_AUDIO false
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
// Whether the OpenFrameworks app should run in the main() function
#define APP_ENABLE false

#endif  // APP_SETTINGS_HPP