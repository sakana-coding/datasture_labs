#include "Plane.h"
#include "Random.h"
#include "Runway.h"
#include "Utility.h"
/*author 徐翔
  由于配置.json文件太烦选择了使用cmake
*/
int main() {
    // 仿真参数。
    int end_time = 0;
    int queue_limit = 0;
    // 用于给每架飞机分配唯一编号。
    int flight_number = 0;
    double arrival_rate = 0.0;
    double departure_rate = 0.0;

    // 读取用户输入。
    initialize(end_time, queue_limit, arrival_rate, departure_rate);

    // 创建随机数对象和机场跑道对象。
    Random variable;
    Runway small_airport(queue_limit);

    // 主循环：每次循环代表 1 个时间单位。
    for (int current_time = 0; current_time < end_time; ++current_time) {
        // 先生成当前时刻的降落请求。
        const int number_arrivals = variable.poisson(arrival_rate);
        for (int i = 0; i < number_arrivals; ++i) {
            Plane current_plane(flight_number++, current_time, PlaneStatus::arriving);
            if (small_airport.can_land(current_plane) != ErrorCode::success) {
                current_plane.refuse();
            }
        }

        // 再生成当前时刻的起飞请求。
        const int number_departures = variable.poisson(departure_rate);
        for (int i = 0; i < number_departures; ++i) {
            Plane current_plane(flight_number++, current_time, PlaneStatus::departing);
            if (small_airport.can_depart(current_plane) != ErrorCode::success) {
                current_plane.refuse();
            }
        }

        // 当前时间单位内，最多只允许一架飞机使用跑道。
        Plane moving_plane;
        switch (small_airport.activity(current_time, moving_plane)) {
            case RunwayActivity::land:
                moving_plane.land(current_time);
                break;
            case RunwayActivity::takeoff:
                moving_plane.fly(current_time);
                break;
            case RunwayActivity::idle:
                run_idle(current_time);
                break;
        }
    }

    // 输出仿真总结。
    small_airport.shut_down(end_time);
    return 0;
}
