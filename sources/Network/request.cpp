#include"request.h"

std::string SizePacket::serialize() {
    std::size_t length = m_serialized_data_object.size();
    std::string lengthStr = std::to_string(length);
    std::ostringstream oss;
    oss << "NEXT_QUERY_SIZE" << "\n"
        << lengthStr;
    return oss.str();
}

size_t SizePacket::deserialize(const std::string& str) {
    std::istringstream iss(str);
    std::string sizeStr;
    std::getline(iss, sizeStr);
    std::size_t value = std::stoul(sizeStr);
    return value;
}

using namespace snd;

std::string GetFriendsStatusesPacket::serialize() {
    std::ostringstream oss;
    oss << "GET_FRIENDS_STATUSES" << "\n";
    for (const auto& elem : m_vec_friends_logins) {
        oss << elem << " ";
    }
    return oss.str();
}

std::string AuthorizationPacket::serialize() {
    std::ostringstream oss;
    oss << "AUTHORIZATION" << "\n" 
        << m_login << '\n'
        << m_password;
    return oss.str();
}

std::string RegistrationPacket::serialize() {
    std::ostringstream oss;
    oss << "REGISTRATION" << "\n"
        << m_login << '\n'
        << m_password << '\n'
        << m_name;
    return oss.str();
}

std::string CreateChatPacket::serialize() {
    std::ostringstream oss;
    oss << "CREATE_CHAT" << "\n"
        << m_my_login << "\n"
        << m_friend_login;
    return oss.str();
}

std::string GetUserInfoPacket::serialize() {
    std::ostringstream oss;
    oss << "GET_USER_INFO" << "\n"
        << m_user_login;
    return oss.str();
}

std::string UpdateUserInfoPacket::serialize() {
    std::ostringstream oss;
    oss << "UPDATE_USER_INFO" << "\n"
        << m_user_login << '\n'
        << m_user_name << '\n'
        << m_user_password << '\n'
        << m_user_photo.serialize();
    return oss.str();
}

using namespace rpl;
std::string MessagesReadPacket::serialize() const {
    std::ostringstream oss;
    oss << "MESSAGES_READ_PACKET" << "\n"
        << m_my_login << '\n'
        << m_friend_login << '\n';

    for (const auto& id : m_read_messages_id_vec) {
        oss << id << ' '; 
    }
    oss << '\n'; 

    return oss.str();
}

MessagesReadPacket MessagesReadPacket::deserialize(const std::string& str) {
    std::istringstream iss(str);
    std::string line;
    MessagesReadPacket packet;


    std::getline(iss, packet.m_my_login);
    std::getline(iss, packet.m_friend_login);

    std::getline(iss, line);
    std::istringstream lineStream(line);
    double messageId;
    while (lineStream >> messageId) {
        packet.m_read_messages_id_vec.push_back(messageId);
    }

    return packet;
}

Message Message::deserialize(const std::string& str) {
    std::istringstream iss(str);
    Message message;
    std::string line;

    // ������ ID
    std::getline(iss, line);
    double id = 0;
    std::from_chars(line.data(), line.data() + line.size(), id);
    message.m_id = id;

    // ������ timestamp
    std::getline(iss, message.m_timeStamp);

    // ������ ��������� � ������������ |MSG_END|
    std::string msg;
    while (std::getline(iss, line)) {
        if (line == "|MSG_END|") {
            break;
        }
        msg += line + "\n";

    }
    message.m_message = msg.substr(0, msg.length() - 1);

    std::ostringstream remainingStream1;
    std::ostringstream remainingStream2;

    bool fl = true;
    while (std::getline(iss, line)) {
        if (line != ":") {
            if (fl) {
                remainingStream1 << line << "\n";
            }
            else {
                remainingStream2 << line << "\n";
            }
        }
        else {
            fl = false;
        }
    }

    std::string remainingPart1 = remainingStream1.str();
    UserInfoPacket packMy = UserInfoPacket::deserialize(remainingPart1);

    std::string remainingPart2 = remainingStream2.str();
    UserInfoPacket packFriend = UserInfoPacket::deserialize(remainingPart2);

    message.setMyInfo(packMy);
    message.setFriendInfo(packFriend);

    return message;
}

std::string Message::serialize() {
    std::ostringstream oss;
    oss << "MESSAGE" << "\n";
    oss << m_id << '\n'
        << m_timeStamp << '\n'
        << m_message << '\n'
        << "|MSG_END|" << '\n'
        << m_my_info.serialize() << '\n'
        << ":" << '\n'
        << m_friend_info.serialize();
    return oss.str();
}


std::string UserInfoPacket::serialize() {
    std::ostringstream oss;
    oss << "USER_INFO_FOUND" << "\n"
        << m_user_login << '\n'
        << m_user_name << '\n'
        << m_last_seen << '\n';

    std::string photo_serialized_str = m_user_photo.serialize();
    oss << photo_serialized_str << '\n';

    oss << (m_isOnline ? "true" : "false") << '\n'
        << (m_isHasPhoto ? "true" : "false");

    return oss.str();
}

UserInfoPacket UserInfoPacket::deserialize(const std::string& str) {
    std::istringstream iss(str);
    std::string line;
    UserInfoPacket packet;

    std::string lineType; // reads USER_INFO_FOUND
    std::getline(iss, lineType);
    std::getline(iss, packet.m_user_login);
    std::getline(iss, packet.m_user_name);
    std::getline(iss, packet.m_last_seen);

    std::string photo_serialized_str;
    std::getline(iss, photo_serialized_str);
    packet.m_user_photo = Photo::deserialize(photo_serialized_str);

    std::getline(iss, line);
    packet.m_isOnline = (line == "true");

    std::getline(iss, line);
    packet.m_isHasPhoto = (line == "true");

    return packet;
}


using namespace rcv;
ChatSuccessPacket ChatSuccessPacket::deserialize(const std::string& str) {
    ChatSuccessPacket pack;
    pack.setUserInfoPacket(UserInfoPacket::deserialize(str));
    return pack;
}

std::pair<Response, std::string> rcv::parseResponse(const std::string& response) {
    std::istringstream iss(response);

    std::string responseTypeStr;
    std::getline(iss, responseTypeStr);
    StatusPacket packet = StatusPacket::deserialize(responseTypeStr);

    std::ostringstream remainingStream;
    std::string line;
    while (std::getline(iss, line)) {
        remainingStream << line;
        remainingStream << "\n";
    }

    std::string remainingPart = remainingStream.str();
    if (!remainingPart.empty() && remainingPart.back() == '\n') {
        remainingPart.pop_back();
    }

    return std::make_pair(packet.getStatus(), remainingPart);

}



StatusPacket StatusPacket::deserialize(const std::string& str) {
    Response response = Response::EMPTY_RESPONSE;
    if (str == "EMPTY_RESPONSE") {
        response = Response::EMPTY_RESPONSE;
    }
    else if (str == "MESSAGE") {
        response = Response::MESSAGE;
    }
    else if (str == "MESSAGES_READ_PACKET") {
        response = Response::MESSAGES_READ_PACKET;
    }
    else if (str == "AUTHORIZATION_SUCCESS") {
        response = Response::AUTHORIZATION_SUCCESS;
    }
    else if (str == "REGISTRATION_SUCCESS") {
        response = Response::REGISTRATION_SUCCESS;
    }
    else if (str == "AUTHORIZATION_FAIL") {
        response = Response::AUTHORIZATION_FAIL;
    }
    else if (str == "REGISTRATION_FAIL") {
        response = Response::REGISTRATION_FAIL;
    }
    else if (str == "CHAT_CREATE_SUCCESS") {
        response = Response::CHAT_CREATE_SUCCESS;
    }
    else if (str == "CHAT_CREATE_FAIL") {
        response = Response::CHAT_CREATE_FAIL;
    }
    else if (str == "USER_INFO_FOUND") {
        response = Response::USER_INFO_FOUND;
    }
    else if (str == "USER_INFO_NOT_FOUND") {
        response = Response::USER_INFO_NOT_FOUND;
    }
    else if (str == "USER_INFO_UPDATED") {
        response = Response::USER_INFO_UPDATED;
    }
    else if (str == "USER_INFO_NOT_UPDATED") {
        response = Response::USER_INFO_NOT_UPDATED;
    }
    else if (str == "FRIEND_STATE_CHANGED") {
        response = Response::FRIEND_STATE_CHANGED;
    }
    else if (str == "ALL_FRIENDS_STATES") {
        response = Response::ALL_FRIENDS_STATES;
    }
    else if (str == "NEXT_QUERY_SIZE") {
        response = Response::NEXT_QUERY_SIZE;
    }
    else {
        printf("receiving error");
    }
    StatusPacket packet;
    packet.setStatus(response); 
    return packet;
}

FriendStatePacket FriendStatePacket::deserialize(const std::string& str) {
    std::istringstream iss(str);
    std::string line;
    FriendStatePacket packet;
    std::getline(iss, packet.m_friend_login);
    std::getline(iss, packet.m_last_seen);
    return packet;
}

FriendsStatusesPacket FriendsStatusesPacket::deserialize(const std::string& str) {
    FriendsStatusesPacket pack;
    std::istringstream iss(str);
    std::string token;

    while (std::getline(iss, token, ',')) {
        std::string first, second;
        std::istringstream pairStream(token);

        if (std::getline(pairStream, first, ':') && std::getline(pairStream, second)) {
            pack.getVecStatuses().emplace_back(first, second);
        }
    }

    return pack;
}

