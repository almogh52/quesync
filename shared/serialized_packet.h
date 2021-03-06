#pragma once
#include "packet.h"

#include <iomanip>
#include <nlohmann/json.hpp>
#include <sstream>

#include "utils/parser.h"

namespace quesync {
class serialized_packet : public packet {
   public:
    /**
     * Serialized packet constructor.
     *
     * @param type The type of the packet.
     */
    serialized_packet(packet_type type) : packet(type){};

    virtual std::string encode() {
#ifdef __APPLE__
        // Format the serialized packet by it's data
        return (std::stringstream() << PACKET_IDENTIFIER << PACKET_DELIMETER
                                    << std::setw(PACKET_TYPE_LEN) << std::setfill('0') << (int)_type
                                    << PACKET_DELIMETER << _data.dump() << PACKET_DELIMETER)
            .str();
#else
        // Format the serialized packet by it's data
        return (static_cast<std::stringstream&>(std::stringstream()
                                                << PACKET_IDENTIFIER << PACKET_DELIMETER
                                                << std::setw(PACKET_TYPE_LEN) << std::setfill('0')
                                                << (int)_type << PACKET_DELIMETER << _data.dump()
                                                << PACKET_DELIMETER))
            .str();
#endif
    };

    virtual bool decode(std::string packet) {
        // Split the packet
        std::vector<std::string> params = utils::parser::split(packet, PACKET_DELIMETER);

        // Try to parse the data as a json
        try {
            _data = nlohmann::json::parse(params[0]);

            // Check if a valid data has entered
            if (!this->verify()) {
                throw "";
            }
        } catch (...) {
            return false;
        }

        return true;
    };

    /**
     * Verifies that the json has all required fields.
     * 
     * @return True if the json has all required fields or false otherwise.
     */
    virtual bool verify() const = 0;

   protected:
    nlohmann::json _data;

    virtual bool exists(std::string key) const {
        // Try to get the iterator of the key's value
        return _data.find(key) != _data.end();
    };
};
};  // namespace quesync