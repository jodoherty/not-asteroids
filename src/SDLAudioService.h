#ifndef SDL_AUDIO_SERVICE_H__
#define SDL_AUDIO_SERVICE_H__

#include <SDL.h>
#include <SDL_mixer.h>

#include <map>

#include "AudioService.h"

class SDLAudioService : public AudioService {
  // SDLAudioService implements the Singleton pattern, so only
  // one instance is allowed at any one time.
  static SDLAudioService *instance;
  // Reference counting is used to handle resource management.
  static unsigned int refcount;
  // Samples are cached by name using STL maps in this implementation
  std::map<const char *, Mix_Music *> tracks;
  std::map<const char *, Mix_Chunk *> samples;

  // Private constructor and destructor
  SDLAudioService();
  ~SDLAudioService();

  void loadSample(const char *);
  void loadTrack(const char *);

 public:
  void release();

  AudioHandle playSample(const char *, int = 0);
  void stopSample(AudioHandle);
  void playTrack(const char *);

  static SDLAudioService *getInstance();
};

#endif  // SDL_AUDIO_SERVICE_H__
