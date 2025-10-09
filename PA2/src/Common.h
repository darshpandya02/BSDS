#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <cstring>
#include <stdexcept>

// Order structure
struct Order {
    int customer_id;
    int order_number;
    int robot_type; // 0 - regular, 1 - special
    
    Order() : customer_id(0), order_number(0), robot_type(0) {}
    Order(int cid, int onum, int rtype) 
        : customer_id(cid), order_number(onum), robot_type(rtype) {}
};

// Robot information structure
struct RobotInfo {
    int customer_id;
    int order_number;
    int robot_type;
    int engineer_id;
    int expert_id; // -1 if no special module
    
    RobotInfo() : customer_id(0), order_number(0), robot_type(0), 
                  engineer_id(0), expert_id(-1) {}
    
    RobotInfo(int cid, int onum, int rtype, int engid, int expid)
        : customer_id(cid), order_number(onum), robot_type(rtype),
          engineer_id(engid), expert_id(expid) {}
};

// Message types for protocol
enum MessageType {
    MSG_ORDER = 1,
    MSG_ROBOT = 2
};

#endif // COMMON_H