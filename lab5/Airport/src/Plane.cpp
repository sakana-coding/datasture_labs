#include "Plane.h"

#include <iostream>

// 默认构造出的对象不代表真实飞机，通常用于临时变量。
Plane::Plane() : flt_num(-1), clock_start(-1), state(PlaneStatus::null) {}

// 创建一架飞机时，立即输出其申请动作。
Plane::Plane(int flt, int time, PlaneStatus status)
    : flt_num(flt), clock_start(time), state(status) {
    std::cout << "Plane number " << flt_num << " ready to ";
    if (state == PlaneStatus::arriving) {
        std::cout << "land." << std::endl;
    } else {
        std::cout << "take off." << std::endl;
    }
}

// 当飞机因队列满而不能进入系统时，输出对应提示。
void Plane::refuse() const {
    std::cout << "Plane number " << flt_num;
    if (state == PlaneStatus::arriving) {
        std::cout << " directed to another airport" << std::endl;
    } else {
        std::cout << " told to try to take off again later" << std::endl;
    }
}

// 降落成功时输出等待时间。
void Plane::land(int time) const {
    const int wait = time - clock_start;
    std::cout << time << ": Plane number " << flt_num << " landed after "
              << wait << " time unit" << (wait == 1 ? "" : "s")
              << " in the landing queue." << std::endl;
}

// 起飞成功时输出等待时间。
void Plane::fly(int time) const {
    const int wait = time - clock_start;
    std::cout << time << ": Plane number " << flt_num << " took off after "
              << wait << " time unit" << (wait == 1 ? "" : "s")
              << " in the takeoff queue." << std::endl;
}

// 返回飞机进入机场系统的时刻。
int Plane::started() const {
    return clock_start;
}
