#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/string.hpp"

class ControlNode : public rclcpp::Node
{
public:
    ControlNode() : Node("control_node")
    {
        sub_front_ = create_subscription<std_msgs::msg::Float32>(
            "front_distance", 10,
            std::bind(&ControlNode::front_callback, this, std::placeholders::_1) //Connects a function to arguments so it can be called later
        );

        sub_left_ = create_subscription<std_msgs::msg::Float32>(
            "left_distance", 10,
            std::bind(&ControlNode::left_callback, this, std::placeholders::_1)
        );

        sub_right_ = create_subscription<std_msgs::msg::Float32>(
            "right_distance", 10,
            std::bind(&ControlNode::right_callback, this, std::placeholders::_1)
        );

        sub_speed_ = create_subscription<std_msgs::msg::Float32>(
            "speed", 10,
            std::bind(&ControlNode::speed_callback, this, std::placeholders::_1)
        );

        pub_command_ = create_publisher<std_msgs::msg::String>("command", 10);
    }

private:
    // kepp last values so we do not lose them with the others sensor 
    float front_ = 500;
    float left_ = 500;
    float right_ = 500;
    float speed_ = 0;


//DIFFICULT PART Sensor publishes data->ROS2 receives message->Callback function is triggered->You process the data
//"A read-only pointer to a message that contains a float number"
    void front_callback(const std_msgs::msg::Float32::SharedPtr msg) //A smart pointer (a safe reference to the message) 
    {
        front_ = msg->data;
        decide();
    }

    void left_callback(const std_msgs::msg::Float32::SharedPtr msg)
    {
        left_ = msg->data;
        decide();
    }

    void right_callback(const std_msgs::msg::Float32::SharedPtr msg)
    {
        right_ = msg->data;
        decide();
    }

    void speed_callback(const std_msgs::msg::Float32::SharedPtr msg)
    {
        speed_ = msg->data;
    }

    //DECISION
    void decide()
    {
        std_msgs::msg::String command;

        if (front_ < 50)
        {
            command.data = "STOP";
        }
        else if (front_ < 100)
        {
            command.data = "SLOW";
        }
        else if (left_ < 30 || right_ < 30)
        {
            command.data = "SLOW";
        }
        else
        {
            command.data = "GO";
        }

//DIFFICULT PART Take the decision (STOP/SLOW/GO)->Add speed information->Send it
        command.data += " | speed: " + std::to_string(speed_); //converts a number into text

        pub_command_->publish(command);
    }


    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sub_front_;
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sub_left_;
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sub_right_;
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sub_speed_;

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_command_;
};



int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ControlNode>());
    rclcpp::shutdown();
}
