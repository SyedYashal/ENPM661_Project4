from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package="package_122288393",
            executable="pick_and_place",
            output="screen",
        )
    ])
