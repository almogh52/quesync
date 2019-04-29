#include "client.h"

#include "socket-manager.h"
#include "executer.h"

#include "../../../shared/utils.h"
#include "../../../shared/packets/login_packet.h"
#include "../../../shared/packets/register_packet.h"
#include "../../../shared/packets/error_packet.h"
#include "../../../shared/packets/search_packet.h"
#include "../../../shared/packets/friend_request_packet.h"
#include "../../../shared/packets/ping_packet.h"
#include "../../../shared/packets/profile_packet.h"

Napi::FunctionReference Client::constructor;

Client::Client(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Client>(info),
                                                 _user(nullptr)
{
    Napi::Env env = info.Env();
}

Napi::Value Client::connect(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);

    // Convert first parameter to string
    std::string ip = info[0].As<Napi::String>();

    // Create a new executer worker that will make the connection to the server
    Executer *e = new Executer([this, ip]() {       
        try {
            // Try to connect to the server
            _communicator.connect(ip);
        } catch (QuesyncException &ex) {
            return nlohmann::json{{"error", ex.getErrorCode()}};
        } catch (...) {
            return nlohmann::json{{"error", UNKNOWN_ERROR}};
        }

        return nlohmann::json{{"error", SUCCESS}};
    },
                               deferred);

    // Queue the executer worker
    e->Queue();

    // Return the promise
    return deferred.Promise();
}

Napi::Value Client::login(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);

    // Convert parameters to satring
    std::string username = info[0].As<Napi::String>(), password = info[1].As<Napi::String>();

    Executer *e = new Executer([this, username, password]() {
        QuesyncError error = SUCCESS;

        ResponsePacket *response_packet;
        nlohmann::json res;

        // Create a login packet from the username and password
        LoginPacket login_packet(username, password);

        // If already authenticated, return error
        if (_user)
        {
            res["error"] = ALREADY_AUTHENTICATED;

            return res;
        }

        // Send the login packet
        try {
            response_packet = _communicator.send(&login_packet);
        } catch (QuesyncException &ex) {
            res["error"] = ex.getErrorCode();

            return res;
        } catch (...) {
            res["error"] = UNKNOWN_ERROR;

            return res;
        }

        // If the response is an error, handle the error
        if (response_packet && response_packet->type() == ERROR_PACKET)
        {
            // Set the error code from the response packet
            res["error"] = ((ErrorPacket *)response_packet)->error();
        }
        else if (response_packet && response_packet->type() == AUTHENTICATED_PACKET) // If the response packet is a valid authentication response, get the user from it
        {
            // Create a new user
            _user = new User();

            // Deserialize the user data from the response data
            _user->deserialize(response_packet->data());

            // Set success error code
            res["error"] = SUCCESS;

            // Return the user serialized
            res["user"] = _user->json();
        }
        else if (error)
        {
            // If an error occurred, return it
            res["error"] = error;
        }
        else
        {
            // We shouldn't get here
            res["error"] = UNKNOWN_ERROR;
        }

        // Free the response packet
        delete response_packet;

        return res;
    },
                               deferred);

    // Queue the executer worker
    e->Queue();

    return deferred.Promise();
}

Napi::Value Client::signup(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);

    // Convert parameters to string
    std::string username = info[0].As<Napi::String>(), password = info[1].As<Napi::String>(),
                email = info[2].As<Napi::String>();

    Executer *e = new Executer([this, username, password, email]() {
        QuesyncError error = SUCCESS;
        ResponsePacket *response_packet;
        nlohmann::json res;

        // Create a login packet from the username and password
        RegisterPacket register_packet(username, password, email, username);

        // If already authenticated, return error
        if (_user)
        {
            res["error"] = ALREADY_AUTHENTICATED;

            return res;
        }

        // Send the register packet
        try {
            response_packet = _communicator.send(&register_packet);
        } catch (QuesyncException &ex) {
            res["error"] = ex.getErrorCode();

            return res;
        } catch (...) {
            res["error"] = UNKNOWN_ERROR;

            return res;
        }

        // If the response is an error, handle the error
        if (response_packet && response_packet->type() == ERROR_PACKET)
        {
            // Set the error code from the response packet
            res["error"] = ((ErrorPacket *)response_packet)->error();
        }
        else if (response_packet && response_packet->type() == AUTHENTICATED_PACKET) // If the response packet is a valid authentication response, get the user from it
        {
            // Create a new user
            _user = new User();

            // Deserialize the user data from the response data
            _user->deserialize(response_packet->data());

            // Set success error code
            res["error"] = SUCCESS;

            // Return the user serialized
            res["user"] = _user->json();
        }
        else if (error)
        {
            // If an error occurred, return it
            res["error"] = error;
        }
        else
        {
            // We shouldn't get here
            res["error"] = UNKNOWN_ERROR;
        }

        // Free the response packet
        delete response_packet;

        return res;
    },
                               deferred);

    // Queue the executer worker
    e->Queue();

    return deferred.Promise();
}

Napi::Value Client::getUserProfile(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);

    // Convert parameters to satring
    std::string user_id = info[0].As<Napi::String>();

    Executer *e = new Executer([this, user_id]() {
        QuesyncError error = SUCCESS;

        ResponsePacket *response_packet;
        nlohmann::json res;

        // Create a profile packet from the user id
        ProfilePacket profile_packet(user_id);

        // If not authenticated, return error
        if (!_user)
        {
            res["error"] = NOT_AUTHENTICATED;

            return res;
        }

        // Send the profile packet
        try {
            response_packet = _communicator.send(&profile_packet);
        } catch (QuesyncException &ex) {
            res["error"] = ex.getErrorCode();

            return res;
        } catch (...) {
            res["error"] = UNKNOWN_ERROR;

            return res;
        }

        // If the response is an error, handle the error
        if (response_packet && response_packet->type() == ERROR_PACKET)
        {
            // Set the error code from the response packet
            res["error"] = ((ErrorPacket *)response_packet)->error();
        }
        else if (response_packet && response_packet->type() == PROFILE_PACKET) // If the response packet is a valid user's profile packet
        {
            // Set success error code
            res["error"] = SUCCESS;

            // Return the profile serialized
            res["profile"] = nlohmann::json::parse(response_packet->data());
        }
        else if (error)
        {
            // If an error occurred, return it
            res["error"] = error;
        }
        else
        {
            // We shouldn't get here
            res["error"] = UNKNOWN_ERROR;
        }

        // Free the response packet
        delete response_packet;

        return res;
    },
                               deferred);

    // Queue the executer worker
    e->Queue();

    return deferred.Promise();
}

Napi::Value Client::search(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);

    // Convert parameters to string
    std::string nickname = info[0].As<Napi::String>();
    int tag = info[1].IsUndefined() ? -1 : info[0].As<Napi::Number>();

    Executer *e = new Executer([this, nickname, tag]() {
        QuesyncError error = SUCCESS;
        ResponsePacket *response_packet;
        nlohmann::json res;

        SearchPacket search_packet(nickname, tag);

        // If not authenticated, return error
        if (!_user)
        {
            res["error"] = NOT_AUTHENTICATED;

            return res;
        }

        // Send the search packet
        try {
            response_packet = _communicator.send(&search_packet);
        } catch (QuesyncException &ex) {
            res["error"] = ex.getErrorCode();

            return res;
        } catch (...) {
            res["error"] = UNKNOWN_ERROR;

            return res;
        }

        // If the response is an error, handle the error
        if (response_packet && response_packet->type() == ERROR_PACKET)
        {
            // Set the error code from the response packet
            res["error"] = ((ErrorPacket *)response_packet)->error();
        }
        // If the response packet is a valid search results response, get the user from it
        else if (response_packet && response_packet->type() == SEARCH_RESULTS_PACKET)
        {
            // Set success error code
            res["error"] = SUCCESS;

            // Return the search results in an object
            res["search_results"] = nlohmann::json::parse(response_packet->data());
        }
        else if (error)
        {
            // If an error occurred, return it
            res["error"] = error;
        }
        else
        {
            // We shouldn't get here
            res["error"] = UNKNOWN_ERROR;
        }

        // Free the response packet
        delete response_packet;

        return res;
    },
                               deferred);

    // Queue the executer worker
    e->Queue();

    return deferred.Promise();
}

Napi::Value Client::sendFriendRequest(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);

    // Convert parameters to string
    std::string friend_id = info[0].As<Napi::String>();

    Executer *e = new Executer([this, friend_id]() {
        QuesyncError error = SUCCESS;
        ResponsePacket *response_packet;
        nlohmann::json res;

        FriendRequestPacket friend_request_packet(friend_id);

        // If not authenticated, return error
        if (!_user)
        {
            res["error"] = NOT_AUTHENTICATED;

            return res;
        }

        // Send the friend request packet
        try {
            response_packet = _communicator.send(&friend_request_packet);
        } catch (QuesyncException &ex) {
            res["error"] = ex.getErrorCode();

            return res;
        } catch (...) {
            res["error"] = UNKNOWN_ERROR;

            return res;
        }

        // If the response is an error, handle the error
        if (response_packet && response_packet->type() == ERROR_PACKET)
        {
            // Set the error code from the response packet
            res["error"] = ((ErrorPacket *)response_packet)->error();
        }
        // If the response packet is a friend request confirmation response, get the user from it
        else if (response_packet && response_packet->type() == FRIEND_REQUEST_SENT_PACKET)
        {
            // Set success error code
            res["error"] = SUCCESS;
        }
        else if (error)
        {
            // If an error occurred, return it
            res["error"] = error;
        }
        else
        {
            // We shouldn't get here
            res["error"] = UNKNOWN_ERROR;
        }

        // Free the response packet
        delete response_packet;

        return res;
    },
                               deferred);

    // Queue the executer worker
    e->Queue();

    return deferred.Promise();
}

Napi::Object Client::Init(Napi::Env env, Napi::Object exports)
{
    // Create scope for Client object
    Napi::HandleScope scope(env);

    // This method is used to hook the accessor and method callbacks
    Napi::Function func = DefineClass(env, "Client", {InstanceMethod("connect", &Client::connect), InstanceMethod("login", &Client::login), InstanceMethod("register", &Client::signup), InstanceMethod("getUserProfile", &Client::getUserProfile), InstanceMethod("search", &Client::search), InstanceMethod("sendFriendRequest", &Client::sendFriendRequest)});

    // Create a peristent reference to the class constructor. This will allow
    // a function called on a class prototype and a function
    // called on instance of a class to be distinguished from each other.
    constructor = Napi::Persistent(func);

    // Call the SuppressDestruct() method on the static data prevent the calling
    // to this destructor to reset the reference when the environment is no longer
    // available.
    constructor.SuppressDestruct();

    exports.Set("Client", func);
    return exports;
}