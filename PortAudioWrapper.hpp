#pragma once
#include <iostream>
#include <portaudio.h>

class PortAudioWrapper {
    bool initialized{false};
public:
    PortAudioWrapper() {
        auto err = Pa_Initialize();
        initialized = err == paNoError;
        if(!initialized) {
            std::cout << "PortAudio error during initialization: "
                      << Pa_GetErrorText(err) << std::endl;
        }
    }

    ~PortAudioWrapper() {
        if(!initialized)
            return;
        auto err = Pa_Terminate();
        if(err != paNoError)
            std::cout <<  "PortAudio error during termination: "
                      << Pa_GetErrorText(err) << std::endl;
    }

    bool is_initialized() {
        return initialized;
    }
};
