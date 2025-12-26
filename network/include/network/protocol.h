#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <vector>

class IProtocol {
public:
	virtual ~IProtocol() = default;
	virtual std::vector<uint8_t> Serialise(const void* data) = 0;
	virtual void Deserialise(const std::vector<uint8_t>& data, void* buffer) = 0;
	virtual size_t MessageSize() const = 0;  
};

template <typename TProtocol>
class PodProtocol : public IProtocol {
public:
    std::vector<uint8_t> Serialise(const void* data) override {
		auto* t_data = static_cast<const TProtocol*>(data);
		const auto* ptr = reinterpret_cast<const uint8_t*>(t_data);
		return std::vector<uint8_t>(ptr, ptr + sizeof(TProtocol));
    }
    void Deserialise(const std::vector<uint8_t>& data, void* buffer) override {
		std::memcpy(buffer, data.data(), MessageSize());
    }
    size_t MessageSize() const override { return sizeof(TProtocol); }
};

#endif