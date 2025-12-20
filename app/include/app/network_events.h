#ifndef APP_NETWORK_EVENTS_H
#define APP_NETWORK_EVENTS_H

#include "engine/events/event.h"

using namespace Engine;

struct Message {
	char data[1024];
};

class MessageReceiveEvent final : public Engine::Event {
private:
	Message msg_;
public:
	MessageReceiveEvent(const Message& msg)
		: msg_(msg) {}
	~MessageReceiveEvent() override = default;
	const Message& GetMsg() const { return msg_; }
	EVENT_CLASS_TYPE(kMessageReceive)
};

class MessageSendEvent final : public Engine::Event {
private:
	Message msg_;
public:
	MessageSendEvent(const Message& msg)
	    :msg_(msg) {}
	~MessageSendEvent() override = default;
	const Message& GetMsg() const { return msg_; }
	EVENT_CLASS_TYPE(kMessageSend)
};

#endif