#pragma once

typedef enum PacketType
{
    // Requests
    LOGIN_PACKET = 1,

    // Respones
    AUTHENTICATED_PACKET = 200,

    // When an error occurred
    ERROR_PACKET = 400
} PacketType;