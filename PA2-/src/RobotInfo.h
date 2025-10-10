// RobotInfo.h
#pragma once
#include <cstring>

struct RobotInfo {
    int customer_id;
    int order_number;
    int robot_type;
    int engineer_id;
    int expert_id;

    void serialize(char *buf) const {
        memcpy(buf, this, sizeof(RobotInfo));
    }
    void deserialize(const char *buf) {
        memcpy(this, buf, sizeof(RobotInfo));
    }
};
