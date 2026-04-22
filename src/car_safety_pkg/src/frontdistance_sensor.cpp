#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include <cstdlib>
#include <ctime>

// Creates a node for the fron distance sensor & after create publisher and timer
class FrontDistanceSensor : public rclcpp::Node
{
public:
    FrontDistanceSensor() : Node("front_distance_sensor")
    {
        publisher_ = create_publisher<std_msgs::msg::Float32>("front_distance", 10);

        timer_ = create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&FrontDistanceSensor::send, this)
        );
    }



private:
    void send()
    {
        std_msgs::msg::Float32 msg;

        msg.data = rand() % 500;  // simulate distance up to 500 cm (trying to make it realistic, it will go from high to low value it should be smoother in real life but it exceds my capacity right now)

        publisher_->publish(msg);
    }

    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

//
int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    srand(time(NULL));  // make randomness different each run

    rclcpp::spin(std::make_shared<FrontDistanceSensor>());

    rclcpp::shutdown();
}
