# HW1_Shop_Customer
Homework1 of robot class
# ROS2 商店-顾客通信

基于ROS2的通信系统，实现了商店和顾客之间的交易功能。项目包含两个版本：
1. 基于Topic的通信实现
2. 基于Service的通信实现

## 项目结构

```
HW1_Shop_Customer/
├── shop_customer/           # Topic版本实现
│   ├── src/                # 源代码
│   ├── launch/             # launch文件
│   ├── CMakeLists.txt      # 编译配置
│   └── package.xml         # 包信息
├── Service_Client/         # Service版本实现
│   └── shop_customer/      # Service实现代码
└── run_shop_customer.sh    # build&run脚本
```


## 使用方法

### 运行Topic版本

使用提供的脚本运行：
```bash
./run_shop_customer.sh
```

或者手动运行：
```bash
source install/setup.bash
ros2 launch shop_customer shop_customer_launch.py
```

### 运行Service版本

进入Service_Client目录并按照相同步骤编译运行。

## 通信机制说明

### Topic版本
- 使用ROS2的发布/订阅模式
- 商店作为发布者，发布商品信息
- 顾客作为订阅者，接收商品信息并发送购买请求

### Service版本
- 使用ROS2的服务/客户端模式
- 商店作为服务提供者
- 顾客作为服务请求者
- 实现同步通信，更适合交易场景

