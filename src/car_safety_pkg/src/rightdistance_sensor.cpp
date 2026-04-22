#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include <cstdlib>
#include <ctime>

class RightDistanceSensor : public rclcpp::Node
{
public:
    RightDistanceSensor() : Node("right_distance_sensor")
    {
        publisher_ = create_publisher<std_msgs::msg::Float32>("right_distance", 10);

        timer_ = create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&RightDistanceSensor::send, this)
        );
    }


private:
    void send()
    {
        std_msgs::msg::Float32 msg;

        msg.data = rand() % 200;  // distance between 0 and 199 cm, di not know if is realistic maybe should do it smaller range

        publisher_->publish(msg);
    }

    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};



int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    srand(time(NULL));

    rclcpp::spin(std::make_shared<RightDistanceSensor>());

    rclcpp::shutdown();
}
