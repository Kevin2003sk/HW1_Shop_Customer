#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <vector>
#include <random>
#include <chrono>

class CustomerNode : public rclcpp::Node
{
public:
    CustomerNode(std::string name) : Node(name)
    {
        // 初始化商品列表
        items_ = {"chips", "coke", "water", "pen"};
        
        // 发布订单请求
        order_publisher_ = this->create_publisher<std_msgs::msg::String>("customer_order", 10);
        // 订阅小卖部的商品信息
        item_subscriber_ = this->create_subscription<std_msgs::msg::String>(
            "shop_item", 10, std::bind(&CustomerNode::item_callback, this, std::placeholders::_1));

        // 定时发布订单
        timer_ = this->create_wall_timer(std::chrono::seconds(5), std::bind(&CustomerNode::send_order, this));
    }

private:
    void send_order()
    {
        // 使用当前时间作为随机数种子
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed);
        std::uniform_int_distribution<int> distribution(0, items_.size() - 1);
        
        std_msgs::msg::String order_msg;
        order_msg.data = items_[distribution(generator)];
        order_publisher_->publish(order_msg);
        RCLCPP_INFO(this->get_logger(), "Sent order: %s", order_msg.data.c_str());
    }

    void item_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Received item: %s", msg->data.c_str());
    }

    std::vector<std::string> items_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr order_publisher_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr item_subscriber_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CustomerNode>("customer");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
