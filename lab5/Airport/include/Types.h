#ifndef AIRPORT_TYPES_H
#define AIRPORT_TYPES_H

// 队列或跑道操作后的状态码。
enum class ErrorCode {
    success,
    fail,
    overflow,
    underflow
};

// 飞机当前的业务状态：
// null      : 默认空对象
// arriving  : 申请降落
// departing : 申请起飞
enum class PlaneStatus {
    null,
    arriving,
    departing
};

// 某一时间单位内跑道实际执行的动作。
enum class RunwayActivity {
    idle,
    land,
    takeoff
};

#endif
