#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#ifdef QUESYNC_CLIENT
#include <napi.h>
#include <uv.h>
#endif

#include "packets/packet.h"
#include "packets/packet_types.h"

class Utils
{
public:
    static void CopyString(const std::string& input, char *dst);
    static std::vector<std::string> Split(const std::string& s, char delimiter);
    
    static bool isValidUsername(std::string username);
    static bool isValidEmail(std::string email);

    static std::string SHA256(const std::string str);

    static Packet *ParsePacket(std::string packet);
    static PacketType GetPacketType(std::string packet);
    
    #ifdef QUESYNC_CLIENT
    static Napi::Object JsonToObject(Napi::Env env, nlohmann::json& json);
    #endif
};