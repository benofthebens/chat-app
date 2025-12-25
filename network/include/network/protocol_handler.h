#ifndef PROTOCOL_HANDLER_H
#define PROTOCOL_HANDLER_H

#include <vector>

template <typename TProtocol>
class ProtocolHandler {
public:
	std::vector<uint8_t> Serialise(const TProtocol& data) {
		const auto* ptr = reinterpret_cast<const uint8_t*>(&data);
		return std::vector(ptr, ptr + sizeof(TProtocol));
	}
	TProtocol Deserialise(const char* data) {
		TProtocol obj{};
		std::memcpy(&obj, data, sizeof(TProtocol));
		return obj;
	}
};

#endif