#pragma once

#include <sstream>
#include <string>

#include "../packet.h"
#include "../utils/parser.h"

#define MAX_VOICE_DATA_LEN 4096

namespace quesync {
namespace packets {
class voice_participant_packet {
   public:
    /// Default constructor.
    voice_participant_packet(){};

    /**
     * Packet constructor.
     *
     * @param user_id The id of the user.
     * @param voice_data The encoded opus data.
     * @param voice_data_len The length of the encoded opus data.
     */
    voice_participant_packet(std::string user_id, char *voice_data, unsigned int voice_data_len)
        : _user_id(user_id),
          _voice_data(new char[voice_data_len]),
          _voice_data_len(voice_data_len) {
        memcpy(_voice_data, voice_data, voice_data_len);
    }

    /**
     * Encode the packet.
     *
     * @return The packet encoded.
     */
    std::string encode() {
        std::stringstream encoded_packet;

        encoded_packet << _user_id << PACKET_DELIMETER;
        encoded_packet << std::string(_voice_data, _voice_data_len) << PACKET_DELIMETER;

        return encoded_packet.str();
    }

    /**
     * Decode the packet.
     *
     * @param buf The packet's encoded data.
     * @return True if the packet was decoded successfully or false otherwise.
     */
    bool decode(std::string buf) {
        // Split the packet
        std::vector<std::string> params = utils::parser::split(buf, PACKET_DELIMETER);

        if (params.size() >= 2) {
            // Parse the packet
            _user_id = params[0];

            // Parse voice data
            _voice_data_len = (unsigned int)(buf.length() - (_user_id.length() + 2));
            _voice_data = new char[_voice_data_len];
            memcpy(_voice_data, buf.c_str() + (_user_id.length() + 1), _voice_data_len);
        } else {
            return false;
        }

        return true;
    }

    /**
     * Get the user id.
     *
     * @return The user id.
     */
    std::string user_id() { return _user_id; }

    /**
     * Get the voice data which is the encoded opus data.
     *
     * @return The voice data.
     */
    char *voice_data() { return _voice_data; }

    /**
     * Get the voice data length.
     *
     * @return The voice data length.
     */
    unsigned int voice_data_len() { return _voice_data_len; }

   private:
    std::string _user_id;

    char *_voice_data;
    unsigned int _voice_data_len;
};
};  // namespace packets
};  // namespace quesync