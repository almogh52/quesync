#pragma once

namespace quesync {
enum class error {
    success,
    unknown_error,
    invalid_packet,
    cannot_reach_server,
    no_connection,
    user_not_found,
    incorrect_password,
    already_authenticated,
    not_authenticated,
    invalid_username,
    invalid_email,
    password_too_weak,
    username_already_in_use,
    email_already_in_use,
    nickname_full,
    already_friends,
    self_friend_request,
    not_friends,
    self_approve_friend_request,
    channel_not_found,
    already_member,
    not_member_of_channel,
    amount_exceeded_max,
    self_private_channel,
    invalid_session,
    voice_not_connected,
    call_already_started,
    invalid_event,
    file_size_exceeded_max,
    empty_file,
    file_not_found,
    not_file_owner,
    file_session_not_connected,
    file_already_downloading,
    invalid_download_file_path,
    profile_photo_too_big
};
};
