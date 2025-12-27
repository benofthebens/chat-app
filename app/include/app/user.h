#ifndef USER_H
#define USER_H

#include "network_events.h"
#include <chrono>
#include <random>
#include <sstream>

struct User {
    int id = 0;
    char name[32] = {};

    static User& Get() {
        static User instance;
        return instance;
    }

    void Initialize() {
        auto seed = static_cast<unsigned>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::mt19937 rng(seed);
        std::uniform_int_distribution<uint16_t> dist(1, UINT16_MAX);

        id = dist(rng);
        std::stringstream ss;
        ss << "User" << id;
        strcpy_s(name, ss.str().c_str());
    }
};

#endif