#ifndef AUDIO_SERVICE_H__
#define AUDIO_SERVICE_H__

typedef int AudioHandle;

class AudioService
{
public:
    virtual void release() = 0;

    virtual AudioHandle playSample(const char *, int = 0) = 0;
    virtual void stopSample(AudioHandle) = 0;
    virtual void playTrack(const char *) = 0;
};

AudioService *GetAudioServiceInstance();

#endif // AUDIO_SERVICE_H__
