#include "../flir_ros_cam.hpp"
#include "ros-node-interface/interface.hpp"

int main(int argc, char **argv) {
    ros::init(argc, argv, "tracking_interface");
    ros::NodeHandle nh("~");
    std::unique_ptr<FlirROSInterfaceModule> _flirROSInterfaceModule;
    if (!init_FlirROSInterfaceModule(nh, _flirROSInterfaceModule)){
        return 1;
    };
    ros_node_interface::BaseRosInterface<FlirROSInterfaceModule> tracking_interface(
        std::move(_flirROSInterfaceModule)
    );
    spdlog::info("Main thread exiting (tracking_interface)...");

    return 0;
}