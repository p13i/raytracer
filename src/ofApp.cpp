#include "ofApp.h"

#include <ctime>

#include "data/data_scenes.hpp"
#include "rt_math.hpp"
#include "rt_settings.hpp"
#include "rt_str.hpp"

void ofApp::setup() {
  app_setup_time_ = time(nullptr);
  environment_changed_time_ = app_setup_time_;

  ofBackground(0, 0, 0);  // black

  // Frame rate
  ofSetFrameRate(APP_FRAME_RATE);

  ofSetWindowTitle(rt::str::format(
      "Raycaster (depth=%d, spread=%.2f, n=%d)",
      RT_MAX_DEPTH, RT_SPREAD_RADIANS, RT_NUM_RAYS));

  for (auto &scene : data::scenes::SCENES) {
    geometries_list_.push_back(
        scene.environment_.mGeometry);
  }

  raytracer_ = rt::RayTracer{geometries_list_[0]};

#if APP_ENABLE_AUDIO
  // Audio
  mSoundBuffer.assign(APP_AUDIO_RATE, 0.f);

  // Set up openframeworks audio
  ofSoundStreamSettings settings;
  settings.bufferSize = APP_AUDIO_BUFFER_SIZE;
  settings.numBuffers = APP_AUDIO_N_NUM_BUFFERS;
  settings.numInputChannels = APP_AUDIO_NUM_INPUTS;
  settings.numOutputChannels = APP_AUDIO_NUM_CHANNELS;
  settings.sampleRate = APP_AUDIO_SAMPLE_RATE;
  settings.setOutListener(this);
#ifdef _WIN32
  settings.setApi(ofSoundDevice::MS_DS);
#endif  // _WIN32
  ofSoundStreamSetup(settings);
#endif  // APP_ENABLE_AUDIO
}

void ofApp::update() {
#if APP_ENABLE_AUDIO
  // Write to the audio buffer
  size_t numSamplesForUpdate =
      APP_AUDIO_SAMPLE_RATE / APP_FRAME_RATE;
  for (size_t i = 0; i < numSamplesForUpdate; i++) {
    float phase =
        static_cast<float>(mSoundBufferWriteIndex) /
        static_cast<float>(APP_AUDIO_RATE);

    assert(0.f <= phase && phase <= 1.f);

    int nTones = 7;

    float value = 0;
    value += rt::math::sinusodal(659.2551f, phase);  // E4
    value += rt::math::sinusodal(440.0000f, phase);  // A4
    value += rt::math::sinusodal(554.3653f, phase);  // E4
    value += rt::math::sinusodal(391.9954f, phase);  // G3
    value += rt::math::sinusodal(164.8138f, phase);  // E3
    value += rt::math::sinusodal(130.8128f, phase);  // C3
    value += rt::math::sinusodal(055.0000f, phase);  // A1

    // adjust into [0, 1]
    value /= nTones;
    value += 1;
    value /= 2;

    sound_buffer_mutex_.lock();
    {
      mSoundBuffer[mSoundBufferWriteIndex] = value;
      mSoundBufferWriteIndex =
          (mSoundBufferWriteIndex + 1) % APP_AUDIO_RATE;
    }
    sound_buffer_mutex_.unlock();
  }
#endif  // APP_ENABLE_AUDIO
}

//--------------------------------------------------------------
void ofApp::draw() {
  if (ofGetMousePressed()) {
    ofVec2f mousePosition =
        ofVec2f(static_cast<float>(ofGetMouseX()),
                static_cast<float>(ofGetMouseY()));
    player_pointing_at_ = {
        mousePosition.x - APP_DRAW_WINDOW_WIDTH / 2,
        -1 * (mousePosition.y -
              APP_DRAW_WINDOW_HEIGHT / 2.f)};
    environment_changed_ = true;
    environment_changed_time_ = time(nullptr);
  }

  rt::Ray startingRay(
      {player_location_.x, player_location_.y},
      {player_pointing_at_.x, player_pointing_at_.y});

  if (environment_changed_) {
    cached_paths_ =
        raytracer_.cast(startingRay, RT_SPREAD_RADIANS,
                        RT_NUM_RAYS, RT_MAX_DEPTH);
  }

  ofPushMatrix();
  {
    // Center on (0, 0) with positive x-axis to the right
    // and positive
    ofTranslate((APP_DRAW_WINDOW_WIDTH / 2),
                (APP_DRAW_WINDOW_HEIGHT / 2));
    ofRotateXDeg(180);

    // *****
    // DRAW ENVIRONMENT
    // *****

    // DRAW GRID LINES

    ofSetColor(ofFloatColor(0.5, 0.5, 0.5));  // gray

    // Draw rows
    for (int rowY = (APP_DRAW_WINDOW_HEIGHT / 2);
         rowY > (-1 * APP_DRAW_WINDOW_HEIGHT / 2);
         rowY -= APP_DRAW_GRID_SPACING_PX) {
      ofSetLineWidth(1);
      ofDrawLine((-1 * APP_DRAW_WINDOW_WIDTH / 2), (rowY),
                 (+1 * APP_DRAW_WINDOW_WIDTH / 2), (rowY));
    }

    // Draw columns
    for (int colX = (APP_DRAW_WINDOW_WIDTH / 2);
         colX > (-1 * APP_DRAW_WINDOW_WIDTH / 2);
         colX -= APP_DRAW_GRID_SPACING_PX) {
      ofSetLineWidth(1);
      ofDrawLine((colX), (-1 * APP_DRAW_WINDOW_HEIGHT / 2),
                 (colX), (+1 * APP_DRAW_WINDOW_HEIGHT / 2));
    }

    // Draw x-axis
    ofSetLineWidth(2);
    ofSetColor(ofFloatColor(1, 0, 0));
    ofDrawLine((0), (0), (APP_DRAW_GRID_SPACING_PX / 8),
               (0));

    // Draw y-axis
    ofSetLineWidth(2);
    ofSetColor(ofFloatColor(0, 1, 0));
    ofDrawLine((0), (0), (0),
               (APP_DRAW_GRID_SPACING_PX / 8));

    // Draw mGeometry's edges

    rtDraw(raytracer_.mEnvironment);

    // Draw ray traces

    rtDrawTraceOfVectors(cached_paths_);

    vector<rt::Trace<rt::Beam *> *> beams =
        raytracer_.beamCast(startingRay, RT_SPREAD_RADIANS,
                            2);

    // Draw beams

    rtDrawTracesOfBeams(beams);

    // Draw source ray

    rtDraw(startingRay, player_pointing_at_);

    // Draw metadata
    rtDrawMetadata();
  }
  ofPopMatrix();

  environment_changed_ = false;

  // Update time variables
  frame_num_ = (frame_num_ + 1) % APP_FRAME_RATE;
  total_frame_num_++;

  fps_counter_.newFrame();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
  if (key == OF_KEY_LEFT || key == OF_KEY_RIGHT ||
      key == OF_KEY_UP || key == OF_KEY_DOWN) {
    ofVec2f oldSourcePoint = player_location_;
    constexpr int step = 10;
    if (key == OF_KEY_LEFT) {
      player_location_.x -= step;
    } else if (key == OF_KEY_RIGHT) {
      player_location_.x += step;
    } else if (key == OF_KEY_UP) {
      player_location_.y += step;
    } else if (key == OF_KEY_DOWN) {
      player_location_.y -= step;
    }

    ofVec2f newSourcePoint = player_location_;
    player_pointing_at_ +=
        (newSourcePoint - oldSourcePoint);

    environment_changed_ = true;

  } else if ('1' <= key && key <= '9') {
    size_t index = key - '1';

    if (index < 0 ||
        (index > (geometries_list_.size() - 1))) {
      return;
    }

    rt::Geometry geo = geometries_list_[index];
    raytracer_ = rt::RayTracer(rt::Environment{geo});

    environment_changed_ = true;
  }
}

#if APP_ENABLE_AUDIO
//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &soundBuffer) {
  sound_buffer_mutex_.lock();
  {
    size_t bufferSize = soundBuffer.getNumFrames();
    size_t nChannels = soundBuffer.getNumChannels();

    for (size_t i = 0; i < bufferSize * nChannels;
         i += nChannels) {
      for (size_t c = 0; c < nChannels; c++) {
        soundBuffer[i + c] =
            mSoundBuffer[mSoundBufferReadIndex];
      }
      mSoundBufferReadIndex =
          (mSoundBufferReadIndex + nChannels) %
          APP_AUDIO_RATE;
    }
  }
  sound_buffer_mutex_.unlock();
}
#endif  // APP_ENABLE_AUDIO

void ofApp::rtDrawTraceOfVectors(
    std::vector<rt::Trace<rt::Vector> *> traces) {
  ofSetColor(ofFloatColor(1, 1, 1, 0.5));  // clear white
  for (rt::Trace<rt::Vector> *trace : traces) {
    // Draw out rays with diminishiong power (i.e.
    // lineWidth)
    float lineWidth = 8;
    while (trace != nullptr && lineWidth > 0.) {
      ofSetLineWidth(lineWidth);
      ofDrawLine(trace->data.origin.x, trace->data.origin.y,
                 trace->data.dest.x, trace->data.dest.y);
      // Round caps on ray ends
      ofDrawCircle((trace->data.origin.x),
                   (trace->data.origin.y), 4);
      ofDrawCircle((trace->data.dest.x),
                   (trace->data.dest.y), 4);

      // Move forward
      lineWidth *= lineWidth > 0.000001 ? 0.9 : 0.;
      trace = trace->next;
    }
  }
}

void ofApp::rtDraw(rt::Ray startingRay, ofVec2f lookingAt) {
  ofSetColor(ofColor(128, 0, 128));  // purple

  ofDrawCircle((startingRay.origin.x),
               (startingRay.origin.y), 4);

  ofSetLineWidth(2);
  ofDrawLine((startingRay.origin.x), (startingRay.origin.y),
             (startingRay(20).x), (startingRay(20).y));

  ofDrawCircle((startingRay(20).x), (startingRay(20).y), 2);

  ofDrawCircle(lookingAt.x, lookingAt.y, 2);
}

void ofApp::rtDrawTracesOfBeams(
    vector<rt::Trace<rt::Beam *> *> traces) {
  srand(42);

  for (rt::Trace<rt::Beam *> *beamTrace : traces) {
    rt::Vector a = beamTrace->data->mBoundA,
               b = beamTrace->data->mBoundB;

    ofSetLineWidth(1);
    ofPath path = ofPath();
    path.moveTo(a.origin.x, a.origin.y);
    path.lineTo(a.dest.x, a.dest.y);
    path.lineTo(b.dest.x, b.dest.y);
    path.lineTo(b.origin.x, b.origin.y);
    ofColor color = ofFloatColor(
        rt::random::randF(), rt::random::randF(),
        rt::random::randF(), 0.5);  // random
    path.setFillColor(color);
    path.draw();
  }
}

void ofApp::rtDraw(rt::Environment env) const {
  for (const rt::LineSegment &edge : env.mGeometry.edges) {
    ofSetColor(ofFloatColor(1, 0, 0));

    ofSetLineWidth(4);

    // The edge from a to b
    ofDrawLine((edge.a.x), (edge.a.y), (edge.b.x),
               (edge.b.y));

    // Round caps on wall ends
    ofDrawCircle((edge.a.x), (edge.a.y), 2);
    ofDrawCircle((edge.b.x), (edge.b.y), 2);
  }
}

void ofApp::rtDrawMetadata() const {
  float fps = fps_counter_.fps();
  string metaFormatString =
      rt::str::format("Frame: %02d/%02d\nFPS:   %.3f\n",
                      frame_num_, APP_FRAME_RATE, fps);

  if (!environment_changed_) {
    string frameString = rt::str::format(
        "Last draw: %d (sec)",
        time(nullptr) - environment_changed_time_);
    metaFormatString += frameString;
  } else {
    metaFormatString += "Calculating...";
  }

  ofDrawBitmapString(metaFormatString,
                     APP_DRAW_WINDOW_WIDTH / 2 - 200,
                     -APP_DRAW_WINDOW_HEIGHT / 2 + 50);
}