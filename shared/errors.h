#pragma once

typedef enum QuesyncError
{
    SUCCESS,
    UNKNOWN_ERROR,
    INVALID_PACKET,
    CANNOT_REACH_SERVER,
    NO_CONNECTION,
    USER_NOT_FOUND,
    INCORRECT_PASSWORD,
    ALREADY_AUTHENTICATED,
    NOT_AUTHENTICATED,
    INVALID_USERNAME,
    INVALID_EMAIL,
    USERNAME_ALREADY_IN_USE,
    EMAIL_ALREADY_IN_USE,
    NICKNAME_FULL,
    ALREADY_FRIENDS,
    SELF_FRIEND_REQUEST,
    NOT_FRIENDS,
    SELF_APPROVE_FRIEND_REQUEST,
    CHANNEL_NOT_FOUND,
    ALREADY_PARTICIPANT
} QuesyncError;