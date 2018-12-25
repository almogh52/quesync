#pragma once
#include <iostream>
#include <string>

#include <mingw.thread.h>
#include <al.h>
#include <alc.h>
#include <opus.h>
#include <bass.h>

#include "socket-manager.hpp"

#define VOICE_CHAT_PORT 61111

#define RECORD_FREQUENCY 48000
#define RECORD_CHANNELS 2
#define FRAMERATE 960
#define MAX_FRAMERATE 4800

#define RECV_BUFFER_SIZE 500

class VoiceChat
{
public:
    VoiceChat(const char *serverIP);

private:
    SOCKET _voiceSocket;

    void receiveVoiceThread() const;
    void sendVoiceThread() const;
};