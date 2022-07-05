#include "SDLAudioService.h"

#include <iostream>

using namespace std;

SDLAudioService *SDLAudioService::instance = nullptr;
unsigned int SDLAudioService::refcount = 0;

SDLAudioService::SDLAudioService() {
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
    cerr << "Couldn't initialize audio service." << endl;
    cerr << "Mix_OpenAudio: " << Mix_GetError() << endl;
  }
  Mix_AllocateChannels(32);
}

SDLAudioService::~SDLAudioService() {
  for (auto track : tracks) Mix_FreeMusic(track.second);
  for (auto sample : samples) Mix_FreeChunk(sample.second);
  Mix_CloseAudio();
}

AudioService *GetAudioServiceInstance() {
  return SDLAudioService::getInstance();
}

SDLAudioService *SDLAudioService::getInstance() {
  if (!instance) instance = new SDLAudioService();
  refcount++;
  return instance;
}

void SDLAudioService::release() {
  if (refcount <= 0 && instance) {
    delete instance;
    instance = nullptr;
    refcount = 0;
  } else {
    refcount--;
  }
}

void SDLAudioService::loadTrack(const char *filename) {
  if (tracks.find(filename) == tracks.end()) {
    tracks[filename] = Mix_LoadMUS(filename);
    if (tracks[filename] == nullptr) {
      // Loading failed, print an error and erase the track
      cerr << "Mix_LoadMUS(\"" << filename << "\"): " << Mix_GetError() << endl;
      tracks.erase(filename);
    }
  }
}

void SDLAudioService::loadSample(const char *filename) {
  if (samples.find(filename) == samples.end()) {
    samples[filename] = Mix_LoadWAV(filename);
    if (samples[filename] == nullptr) {
      cerr << "Mix_LoadWAV(\"" << filename << "\"): " << Mix_GetError() << endl;
      samples.erase(filename);
    }
  }
}

AudioHandle SDLAudioService::playSample(const char *filename, int loops) {
  loadSample(filename);
  return Mix_PlayChannel(-1, samples[filename], loops);
}

void SDLAudioService::stopSample(AudioHandle handle) {
  Mix_HaltChannel(handle);
}

void SDLAudioService::playTrack(const char *filename) {
  loadTrack(filename);
  Mix_PlayMusic(tracks[filename], -1);
}
