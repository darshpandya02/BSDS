// Order.h
#pragma once
#include <cstring>

struct Order {
    int customer_id;
    int order_number;
    int robot_type;

    void serialize(char *buf) const {
        memcpy(buf, this, sizeof(Order));
    }
    void deserialize(const char *buf) {
        memcpy(this, buf, sizeof(Order));
    }
};
