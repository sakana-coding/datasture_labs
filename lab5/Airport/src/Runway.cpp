#include "Runway.h"

#include <iostream>

// 初始化两条队列和所有统计量。
Runway::Runway(int limit)
    : landing(static_cast<std::size_t>(limit > 0 ? limit : 1)),
      takeoff(static_cast<std::size_t>(limit > 0 ? limit : 1)),
      queue_limit(limit),
      num_land_requests(0),
      num_takeoff_requests(0),
      num_landings(0),
      num_takeoffs(0),
      num_land_accepted(0),
      num_takeoff_accepted(0),
      num_land_refused(0),
      num_takeoff_refused(0),
      land_wait(0),
      takeoff_wait(0),
      idle_time(0) {}

// 一架到达飞机申请进入降落队列。
ErrorCode Runway::can_land(const Plane& current) {
    ++num_land_requests;
    if (landing.size() < static_cast<std::size_t>(queue_limit)) {
        const ErrorCode result = landing.append(current);
        if (result == ErrorCode::success) {
            ++num_land_accepted;
        } else {
            ++num_land_refused;
        }
        return result;
    }
    ++num_land_refused;
    return ErrorCode::overflow;
}

// 一架离港飞机申请进入起飞队列。
ErrorCode Runway::can_depart(const Plane& current) {
    ++num_takeoff_requests;
    if (takeoff.size() < static_cast<std::size_t>(queue_limit)) {
        const ErrorCode result = takeoff.append(current);
        if (result == ErrorCode::success) {
            ++num_takeoff_accepted;
        } else {
            ++num_takeoff_refused;
        }
        return result;
    }
    ++num_takeoff_refused;
    return ErrorCode::overflow;
}

// 决定本时间单位谁使用跑道。
// 规则：
// 1. 先看降落队列，只要非空就优先降落。
// 2. 否则看起飞队列。
// 3. 两者都空，则跑道空闲。
RunwayActivity Runway::activity(int time, Plane& moving) {
    if (!landing.empty()) {
        // 取出队头飞机，累计等待时间，并从队列删除。
        landing.retrieve(moving);
        landing.serve();
        land_wait += time - moving.started();
        ++num_landings;
        return RunwayActivity::land;
    }

    if (!takeoff.empty()) {
        // 若没有降落飞机，再处理起飞队头飞机。
        takeoff.retrieve(moving);
        takeoff.serve();
        takeoff_wait += time - moving.started();
        ++num_takeoffs;
        return RunwayActivity::takeoff;
    }

    ++idle_time;
    return RunwayActivity::idle;
}

// 输出整个仿真的统计结果。
void Runway::shut_down(int time) const {
    std::cout << "Simulation has concluded after " << time << " time units." << std::endl
              << "Total number of planes processed "
              << (num_land_requests + num_takeoff_requests) << std::endl
              << "Total number of planes asking to land " << num_land_requests << std::endl
              << "Total number of planes asking to take off " << num_takeoff_requests << std::endl
              << "Total number of planes accepted for landing " << num_land_accepted << std::endl
              << "Total number of planes accepted for takeoff " << num_takeoff_accepted << std::endl
              << "Total number of planes refused for landing " << num_land_refused << std::endl
              << "Total number of planes refused for takeoff " << num_takeoff_refused << std::endl
              << "Total number of planes that landed " << num_landings << std::endl
              << "Total number of planes that took off " << num_takeoffs << std::endl
              << "Total number of planes left in landing queue " << landing.size() << std::endl
              << "Total number of planes left in takeoff queue " << takeoff.size() << std::endl;

    // 为避免除零，这里先判断分母是否为 0。
    const double idle_percentage = (time > 0) ? (100.0 * static_cast<double>(idle_time) / static_cast<double>(time)) : 0.0;
    const double average_land_wait = (num_landings > 0)
                                         ? (static_cast<double>(land_wait) / static_cast<double>(num_landings))
                                         : 0.0;
    const double average_takeoff_wait = (num_takeoffs > 0)
                                            ? (static_cast<double>(takeoff_wait) / static_cast<double>(num_takeoffs))
                                            : 0.0;
    const double observed_land_rate = (time > 0)
                                          ? (static_cast<double>(num_land_requests) / static_cast<double>(time))
                                          : 0.0;
    const double observed_takeoff_rate = (time > 0)
                                             ? (static_cast<double>(num_takeoff_requests) / static_cast<double>(time))
                                             : 0.0;

    std::cout << "Percentage of time runway idle " << idle_percentage << "%" << std::endl
              << "Average wait in landing queue " << average_land_wait << " time units" << std::endl
              << "Average wait in takeoff queue " << average_takeoff_wait << " time units" << std::endl
              << "Average observed rate of planes wanting to land " << observed_land_rate << " per time unit" << std::endl
              << "Average observed rate of planes wanting to take off " << observed_takeoff_rate << " per time unit" << std::endl;
}
