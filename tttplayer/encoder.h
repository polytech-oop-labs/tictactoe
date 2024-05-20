#pragma once
#include "events.h"
#include "other.h"


enum EncodingStatus {
	OK = 0,
	NOT_ENOUGH_SPACE = 1,
	ERROR_TYPE = 2,
	MARK_IS_EMPTY = 3,
};
class MessageEncoder {
public:
	size_t determineSize(const Field& field);
	EncodingStatus encodeGameField(char* dst_buf, size_t buf_size, const Field& field);
	size_t determineSize(const GameSettings& settings);
	EncodingStatus encodeGameSetting(char* dst_buf, size_t buf_size, const GameSettings& state);
	size_t determineSize(const Event& event);
	EncodingStatus encodeGameEvent(char* dst_buf, size_t buf_size, const Event& event);
};



enum MessageType {
	FIELD = 0,
	SETTINGS = 1,
	EVENT = 2,
	ERROR = -1,
};
class MessageDecoder {
public:

	MessageType determineMessageType(const char* msg, size_t msg_size);
	Event decodeEvent(const char* msg, size_t msg_size);
	void decodeField(const char* msg, size_t msg_size, Field& field);
	GameSettings decodeSettings(const char* msg, size_t msg_size);
};

