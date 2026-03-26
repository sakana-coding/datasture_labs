#ifndef AIRPORT_UTILITY_H
#define AIRPORT_UTILITY_H

// 读取并检查用户输入的仿真参数。
void initialize(int& end_time, int& queue_limit, double& arrival_rate, double& departure_rate);
// 输出某一时刻跑道空闲。
void run_idle(int time);

#endif
