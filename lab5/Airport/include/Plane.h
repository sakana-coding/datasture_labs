#ifndef AIRPORT_PLANE_H
#define AIRPORT_PLANE_H

#include "Types.h"

// Plane 表示一架飞机。
// 它只保存单架飞机自己的编号、进入系统的时刻和状态，
// 并负责输出与这架飞机相关的文字信息。
class Plane {
public:
    // 默认构造：生成一个“空飞机”对象。
    Plane();
    // 带参构造：创建真实飞机，并输出“ready to ...”提示。
    Plane(int flt, int time, PlaneStatus status);

    // 当队列已满时，输出拒绝信息。
    void refuse() const;
    // 输出该飞机降落的信息。
    void land(int time) const;
    // 输出该飞机起飞的信息。
    void fly(int time) const;
    // 返回该飞机进入机场系统的时刻。
    int started() const;

private:
    // 航班编号。
    int flt_num;
    // 进入机场系统的时刻。
    int clock_start;
    // 飞机当前状态：到达/离港。
    PlaneStatus state;
};

#endif
