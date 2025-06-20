#pragma once
#include "flirmulticamera/FlirCamera.h"
#include "flirmulticamera/hardware_constants.h"
#include "ros/ros.h"
#include "ros-node-interface/interface.hpp"
#include "keiko_msgs/ImgsList.h"
#include "config.h"
#include <opencv2/core/mat.hpp>

#include "sys/stat.h"
// #include "stage_publishimages.hpp"

class FlirROSInterfaceModule : public ros_node_interface::BaseRosInterfaceModule
{
public:
    FlirROSInterfaceModule(
        ros::NodeHandle &nh,
        const flirmulticamera::CameraSettings &cam_settings
    );
    ~FlirROSInterfaceModule();
    void Terminate();
    bool start();
    void run();
    bool ShouldClose = false;

    // camera
    void ThreadCamera();
    std::unique_ptr<std::thread> ThreadHandleCamera;
    const flirmulticamera::CameraSettings cam_settings;
    std::array<cv::Mat, flirmulticamera::GLOBAL_CONST_NCAMS> cpuImgs;
    uint32_t seq = 0;

    // modules
    // std::unique_ptr<stages::PublishImages> stage_publishimages;

    std::chrono::steady_clock::time_point now, last; // get publish speed
    std::chrono::milliseconds duration;
    double total_t = 0.;
    double steps = 0.;

    ros::Publisher pub;
    keiko_msgs::ImgsList msg_imgs;
    sensor_msgs::Image msg_img;
};

bool init_FlirROSInterfaceModule(
    ros::NodeHandle &nh,
    std::unique_ptr<FlirROSInterfaceModule> &FlirROSInterfaceModule
);


bool frames_to_msg(
    std::array<flirmulticamera::Frame, flirmulticamera::GLOBAL_CONST_NCAMS>& spinnakerframes, 
    keiko_msgs::ImgsList& imgs_msg, 
    uint32_t& seq
);

bool frame_to_msg(
    flirmulticamera::Frame& spinnakerframe, 
    sensor_msgs::Image& img_msg, 
    uint32_t& seq
);