#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <map>
#include <string>

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

        // 订阅顾客的请求
        order_subscriber_ = this->create_subscription<std_msgs::msg::String>(
            "customer_order", 10, std::bind(&ShopNode::order_callback, this, std::placeholders::_1));
        // 发布商品信息
        item_publisher_ = this->create_publisher<std_msgs::msg::String>("shop_item", 10);
    }

private:
    void order_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        std::string item = msg->data;
        RCLCPP_INFO(this->get_logger(), "Received order: %s", item.c_str());
        
        std_msgs::msg::String item_msg;
        
        // 检查库存
        if (inventory_.find(item) == inventory_.end()) {
            item_msg.data = "Sorry, we don't sell " + item;
        } else if (inventory_[item] <= 0) {
            item_msg.data = "Sorry, " + item + " is out of stock";
        } else {
            inventory_[item]--;  // 减少库存
            item_msg.data = "Here is your " + item + ". Remaining stock: " + std::to_string(inventory_[item]);
        }
        
        item_publisher_->publish(item_msg);
    }

    std::map<std::string, int> inventory_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr order_subscriber_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr item_publisher_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ShopNode>("shop");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
