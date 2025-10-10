// ExpertRequest.h
#pragma once
#include "RobotInfo.h"
#include <future>
#include <memory>

struct ExpertRequest {
    RobotInfo robot;
    std::promise<RobotInfo> prom; // Regular engineer waits using future from this promise
};
using ExpertRequestPtr = std::shared_ptr<ExpertRequest>;
