#ifndef AIRPORT_RUNWAY_H
#define AIRPORT_RUNWAY_H

#include <cstddef>

#include "ExtendedQueue.h"
#include "Plane.h"
#include "Types.h"

// Runway 表示机场唯一的一条跑道。
// 它内部维护两条等待队列：
// 1. landing : 等待降落
// 2. takeoff : 等待起飞
// 同时统计仿真全过程中的各种数据。
class Runway {
public:
    // 构造跑道，并设置队列容量上限。
    explicit Runway(int limit);

    // 新飞机申请降落，能进入降落队列则返回 success，否则返回 overflow。
    ErrorCode can_land(const Plane& current);
    // 新飞机申请起飞，能进入起飞队列则返回 success，否则返回 overflow。
    ErrorCode can_depart(const Plane& current);
    // 决定当前时刻跑道执行什么动作。
    // 规则：降落优先；若无降落，则处理起飞；若两队都空，则 idle。
    RunwayActivity activity(int time, Plane& moving);
    // 仿真结束后输出统计信息。
    void shut_down(int time) const;

private:
    // 等待降落队列。
    ExtendedQueue<Plane> landing;
    // 等待起飞队列。
    ExtendedQueue<Plane> takeoff;
    // 两个队列共同使用的容量上限。
    int queue_limit;

    // 统计量：请求数。
    int num_land_requests;
    int num_takeoff_requests;
    // 统计量：真正完成的降落/起飞架次。
    int num_landings;
    int num_takeoffs;
    // 统计量：成功进入队列的架次。
    int num_land_accepted;
    int num_takeoff_accepted;
    // 统计量：被拒绝的架次。
    int num_land_refused;
    int num_takeoff_refused;
    // 总等待时间。
    int land_wait;
    int takeoff_wait;
    // 跑道空闲总时间。
    int idle_time;
};

#endif
