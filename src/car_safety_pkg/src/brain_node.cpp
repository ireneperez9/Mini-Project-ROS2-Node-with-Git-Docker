#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/string.hpp"

// 🔹 This node listens to sensors and sends commands
class ControlNode : public rclcpp::Node
{
public:
    ControlNode() : Node("control_node")
    {
        // Subscribe to front distance
        sub_front_ = create_subscription<std_msgs::msg::Float32>(
            "front_distance", 10,
            std::bind(&ControlNode::front_callback, this, std::placeholders::_1)
        );

        // Publisher to send command
        pub_command_ = create_publisher<std_msgs::msg::String>("command", 10);
    }

private:
    // 🔹 This runs when new front distance arrives
    void front_callback(const std_msgs::msg::Float32::SharedPtr msg)
    {
        float distance = msg->data;

        std_msgs::msg::String command;

        if (distance < 50)
        {
            command.data = "STOP";
        }
        else if (distance < 100)
        {
            command.data = "SLOW";
        }
        else
        {
            command.data = "GO";
        }

        pub_command_->publish(command);
    }

    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sub_front_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_command_;
};

// 🔹 Start ROS2 and run node
int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<ControlNode>());

    rclcpp::shutdown();
}
