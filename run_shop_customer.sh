#!/bin/bash
# 编译项目
colcon build --packages-select shop_customer

# 如果编译成功，则继续执行
if [ $? -eq 0 ]; then
  # 配置环境变量
  source install/setup.bash

  # 运行节点
  ros2 launch shop_customer shop_customer_launch.py
else
  echo "项目编译失败，请检查。"
fi

