#include "rclcpp/rclcpp.hpp"
#include "shop_customer/srv/order.hpp"
#include <vector>
#include <random>
#include <chrono>
#include <memory>

class CustomerNode : public rclcpp::Node
{
public:
    CustomerNode(std::string name) : Node(name)
    {
        // 初始化商品列表
        items_ = {"chips", "coke", "water", "pen"};
        
        // 创建服务客户端
        client_ = this->create_client<shop_customer::srv::Order>("shop_service");

        // 定时发送订单
        timer_ = this->create_wall_timer(
            std::chrono::seconds(5),
            std::bind(&CustomerNode::send_order, this));
    }

private:
    void send_order()
    {
        // 等待服务可用
        while (!client_->wait_for_service(std::chrono::seconds(1))) {
            if (!rclcpp::ok()) {
                RCLCPP_ERROR(this->get_logger(), "Interrupted while waiting for the service.");
                return;
            }
            RCLCPP_INFO(this->get_logger(), "Waiting for shop service...");
        }

        // 使用当前时间作为随机数种子
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed);
        std::uniform_int_distribution<int> distribution(0, items_.size() - 1);
        
        // 创建请求
        auto request = std::make_shared<shop_customer::srv::Order::Request>();
        request->item_name = items_[distribution(generator)];
        
        RCLCPP_INFO(this->get_logger(), "Sending order for: %s", request->item_name.c_str());

        // 发送请求
        auto result = client_->async_send_request(
            request,
            std::bind(&CustomerNode::response_callback, this, std::placeholders::_1));
    }

    void response_callback(rclcpp::Client<shop_customer::srv::Order>::SharedFuture future)
    {
        try {
            auto result = future.get();
            RCLCPP_INFO(this->get_logger(), "Received response: %s", result->response.c_str());
            if (result->success) {
                RCLCPP_INFO(this->get_logger(), "Remaining stock: %d", result->remaining);
            }
        } catch (const std::exception & e) {
            RCLCPP_ERROR(this->get_logger(), "Service call failed: %s", e.what());
        }
    }

    std::vector<std::string> items_;
    rclcpp::Client<shop_customer::srv::Order>::SharedPtr client_;
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
