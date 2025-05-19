#include "rclcpp/rclcpp.hpp"
#include "shop_customer/srv/order.hpp"
#include <map>
#include <string>
#include <memory>

class ShopNode : public rclcpp::Node
{
public:
    ShopNode(std::string name) : Node(name)
    {
        // 初始化商品库存
        inventory_["chips"] = 10;
        inventory_["coke"] = 15;
        inventory_["water"] = 20;
        inventory_["pen"] = 5;

        // 创建服务服务器
        service_ = this->create_service<shop_customer::srv::Order>(
            "shop_service",
            std::bind(&ShopNode::handle_order, this, std::placeholders::_1, std::placeholders::_2));
        
        RCLCPP_INFO(this->get_logger(), "Shop service is ready!");
    }

private:
    void handle_order(
        const std::shared_ptr<shop_customer::srv::Order::Request> request,
        std::shared_ptr<shop_customer::srv::Order::Response> response)
    {
        std::string item = request->item_name;
        RCLCPP_INFO(this->get_logger(), "Received order for: %s", item.c_str());
        
        // 检查库存
        if (inventory_.find(item) == inventory_.end()) {
            response->response = "Sorry, we don't sell " + item;
            response->success = false;
            response->remaining = 0;
        } else if (inventory_[item] <= 0) {
            response->response = "Sorry, " + item + " is out of stock";
            response->success = false;
            response->remaining = 0;
        } else {
            inventory_[item]--;  // 减少库存
            response->response = "Here is your " + item;
            response->success = true;
            response->remaining = inventory_[item];
        }
        
        RCLCPP_INFO(this->get_logger(), "Response: %s", response->response.c_str());
    }

    std::map<std::string, int> inventory_;
    rclcpp::Service<shop_customer::srv::Order>::SharedPtr service_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ShopNode>("shop");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
