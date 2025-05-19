from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='shop_customer',
            executable='shop',
            name='shop'
        ),
        Node(
            package='shop_customer',
            executable='customer',
            name='customer'
        )
    ])
