#pragma once
#include "../event.h"

namespace quesync {
namespace events {
struct friendship_status_event : public event {
    /// Default constructor.
    friendship_status_event() : event(event_type::friendship_status_event) {}

    /**
     * Event constructor.
     * 
     * @param friend_id The id of the friend.
     * @param status The new friendship status.
     */
    friendship_status_event(std::string friend_id, bool status)
        : event(event_type::friendship_status_event) {
        this->friend_id = friend_id;
        this->status = status;
    }

    virtual nlohmann::json encode() const {
        return {{"eventType", type}, {"friendId", friend_id}, {"status", status}};
    }
    virtual void decode(nlohmann::json j) {
        type = j["eventType"];
        friend_id = j["friendId"];
        status = j["status"];
    }

    /// The id of the friend.
    std::string friend_id;

    /// The new friendship status.
    bool status;
};
};  // namespace events
};  // namespace quesync