#ifndef APP_NETWORK_LAYER_H
#define APP_NETWORK_LAYER_H

#include <mutex>
#include <queue>

#include "network_events.h"
#include "engine/layers/layer.h"
#include "network/application_client.h"


class NetworkLayer : public Engine::Layer {
private:
    ApplicationClient<Message> client_;
    std::queue<Message> pending_messages_;
    std::thread network_thread_;
    std::mutex mutex_;
public:
    NetworkLayer() = default;
    ~NetworkLayer() override = default;
    void OnEvent(Event& event) override;
    void OnAttach(Engine::Window& window) override;
    void OnUpdate() override;
};

#endif