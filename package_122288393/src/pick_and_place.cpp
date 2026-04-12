#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <thread>
#include <chrono>

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("pick_and_place",
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true));

  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node);
  std::thread spinner([&executor]() { executor.spin(); });

  auto logger = node->get_logger();
  std::this_thread::sleep_for(std::chrono::seconds(2));

  moveit::planning_interface::MoveGroupInterface arm(node, "panda_arm");
  moveit::planning_interface::MoveGroupInterface hand(node, "hand");

  arm.setMaxVelocityScalingFactor(0.1);
  arm.setMaxAccelerationScalingFactor(0.1);
  arm.setPlanningTime(10.0);
  hand.setMaxVelocityScalingFactor(0.1);
  hand.setMaxAccelerationScalingFactor(0.1);

  // Joint values in radians
  // Home - natural upright position
  std::vector<double> home  = {0.000, -0.785,  0.000, -2.356,  0.000,  1.571,  0.785};
  // Goal 1 - reach to the RIGHT side (pick)
  std::vector<double> goal1 = {1.571,  0.500,  0.000, -1.800,  0.000,  2.300,  0.785};
  // Goal 2 - reach to the LEFT side (place)
  std::vector<double> goal2 = {-1.571,  0.500,  0.000, -1.800,  0.000,  2.300,  0.785};

  auto move_arm = [&](std::vector<double>& joints, const std::string& label) {
    arm.setStartStateToCurrentState();
    arm.setJointValueTarget(joints);
    RCLCPP_INFO(logger, "Moving arm to: %s", label.c_str());
    auto result = arm.move();
    if (result == moveit::core::MoveItErrorCode::SUCCESS)
      RCLCPP_INFO(logger, "Arm reached: %s", label.c_str());
    else
      RCLCPP_ERROR(logger, "Arm FAILED for: %s (code: %d)", label.c_str(), result.val);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  };

  auto move_gripper = [&](const std::string& named_pose) {
    hand.setStartStateToCurrentState();
    hand.setNamedTarget(named_pose);
    RCLCPP_INFO(logger, "Moving gripper to: %s", named_pose.c_str());
    auto result = hand.move();
    if (result == moveit::core::MoveItErrorCode::SUCCESS)
      RCLCPP_INFO(logger, "Gripper reached: %s", named_pose.c_str());
    else
      RCLCPP_ERROR(logger, "Gripper FAILED for: %s (code: %d)", named_pose.c_str(), result.val);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  };

  RCLCPP_INFO(logger, "Step 1: Moving to HOME...");
  move_arm(home, "home");

  RCLCPP_INFO(logger, "Step 2: Opening gripper...");
  move_gripper("open");

  RCLCPP_INFO(logger, "Step 3: Moving to GOAL 1 (pick from RIGHT)...");
  move_arm(goal1, "goal1");

  RCLCPP_INFO(logger, "Step 4: Closing gripper (picking)...");
  move_gripper("close");

  RCLCPP_INFO(logger, "Step 5: Moving to GOAL 2 (place on LEFT)...");
  move_arm(goal2, "goal2");

  RCLCPP_INFO(logger, "Step 6: Opening gripper (placing)...");
  move_gripper("open");

  RCLCPP_INFO(logger, "Step 7: Returning to HOME...");
  move_arm(home, "home");

  RCLCPP_INFO(logger, "Pick and place complete!");

  executor.cancel();
  spinner.join();
  rclcpp::shutdown();
  return 0;
}
