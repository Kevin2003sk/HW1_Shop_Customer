
# 作业1： ros商店-顾客通信
项目包含两个版本：
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
或者手动编译运行

### 运行Service版本
进入Service_Client目录并按照相同步骤编译运行。

## 通信机制说明
### 以Topic版本为例
- 商店作为发布者，发布商品信息
- 顾客作为订阅者，接收商品信息并发送购买请求
- 商店收到请求后查看库存，如果库存充足则发送成交请求。
  
## 实现效果
