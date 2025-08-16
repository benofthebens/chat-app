#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>

enum Sender : uint8_t { kUser = 0, kServer = 1 };
constexpr uint32_t kMaxDataSize = 1024;

struct Message {
	uint8_t sender = 0;
	bool is_sender = false;
	char data[kMaxDataSize - 2];
};

#endif