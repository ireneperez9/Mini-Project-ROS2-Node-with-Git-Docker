#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include <cstdlib>
#include <ctime>

//public= what the world can use  & private= internal logic


//This class creates a node that sends speed data, once it is created it creates a publisher and a timer
class SpeedSensor : public rclcpp::Node
{
public:
    SpeedSensor() : Node("speed_sensor")
    {
        publisher_ = create_publisher<std_msgs::msg::Float32>("speed", 10);

        timer_ = create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&SpeedSensor::send, this)
        );
    }

//here it creates a random number and sends it
private:
    void send()
    {
        std_msgs::msg::Float32 msg;

        msg.data = rand() % 60;  // random speed between 0 and 59, to simulate the speed in the carrera

        publisher_->publish(msg);
    }

    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};


//Start ROS2 → create node → keep it running → stop ROS2
int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    srand(time(NULL));  // make randomness different each run

    rclcpp::spin(std::make_shared<SpeedSensor>());

    rclcpp::shutdown();
}
