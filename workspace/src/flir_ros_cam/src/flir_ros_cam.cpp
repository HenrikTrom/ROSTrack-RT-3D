#include "flir_ros_cam.hpp"

FlirROSInterfaceModule::FlirROSInterfaceModule(
    ros::NodeHandle &nh,
    const flirmulticamera::CameraSettings &cam_settings
) : cam_settings(cam_settings)
{
    this->nh = nh;
    this->type = "FlirROSInterfaceModule";
    this->compression_params = {cv::IMWRITE_JPEG_QUALITY, COMPRESSION_QUALITY};
    for (std::size_t i = 0; i<flirmulticamera::GLOBAL_CONST_NCAMS; i++)
    {
        this->pubs.at(i) = this->nh.advertise<sensor_msgs::CompressedImage>(
            std::string(TOPIC_IMAGES)+"/"+std::string(flirmulticamera::GLOBAL_CONST_CAMERA_SERIAL_NUMBERS.at(i))+"/compressed",
            1
        );
    }
};

bool FlirROSInterfaceModule::start(){
    spdlog::info("----------------------------------------");
    spdlog::info("------------- FLIR-ROS-CAMS ------------");
    spdlog::info("----------------------------------------");

    this->ShouldClose = false;
    this->ThreadHandleCamera.reset( // online tracking
        new std::thread(&FlirROSInterfaceModule::ThreadCamera, this)
    );

    return true;
}

FlirROSInterfaceModule::~FlirROSInterfaceModule(){};

void FlirROSInterfaceModule::Terminate()
{
    this->ShouldClose = true;
    this->ThreadHandleCamera->join();
    // this->stage_publishimages->Terminate();
    if (!this->steps == 0.){
    spdlog::info(
        "Average Camera Cycle: {} milliseconds over {} samples, should be {} ms", 
        static_cast<int>(this->total_t/this->steps), static_cast<int>(this->steps), (int ) (1000./this->cam_settings.fps)
    );
    }
    else{
        spdlog::info("Average Camera Cycle: 0 milliseconds over 0 samples");
    }
};

void FlirROSInterfaceModule::ThreadCamera()
{
    flirmulticamera::FlirCameraHandler fcamerahandler(this->cam_settings);
    if(!fcamerahandler.Configure()){
        throw std::runtime_error("Could not configure camera");
        return;
    };
    fcamerahandler.Start();

    std::array<flirmulticamera::Frame, flirmulticamera::GLOBAL_CONST_NCAMS> frame;
    this->last = std::chrono::steady_clock::now();
    while(!this->ShouldClose)
    {
        if(fcamerahandler.Get(frame))
        {
            for (std::size_t i = 0; i<flirmulticamera::GLOBAL_CONST_NCAMS; i++)
            {
                this->cpuImgs.at(i) = cv::Mat(
                    frame.at(i).frameData->GetHeight(), 
                    frame.at(i).frameData->GetWidth(), CV_8UC3, 
                    frame.at(i).frameData->GetData()
                );
                cv::cvtColor(cpuImgs.at(i), cpuImgs.at(i), cv::COLOR_RGB2BGR);
            }
            for (std::size_t j = 0; j<flirmulticamera::GLOBAL_CONST_NCAMS; j++)
            {
                cv::imencode(".jpg", cpuImgs.at(j), 
                    this->msg_img_c.data, 
                    this->compression_params
                );
                this->pubs.at(j).publish(this->msg_img_c);
            }
            this->seq++;
            this->now = std::chrono::steady_clock::now();
            this->duration = std::chrono::duration_cast<std::chrono::milliseconds>(this->now - this->last);
            this->total_t += (double) this->duration.count();
            this->steps += 1.;
            this->last = this->now;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }

    fcamerahandler.Stop();
};

bool init_FlirROSInterfaceModule(
    ros::NodeHandle &nh,
    std::unique_ptr<FlirROSInterfaceModule> &_flirROSInterfaceModule
){
    flirmulticamera::CameraSettings cam_settings;
    if (!flirmulticamera::load_camera_settings(std::string(CONFIG_DIR)+"/CamSettings1024x768.json", cam_settings)){
        throw std::runtime_error("Could not load Camera settings");
        return false;
    }
    // how to make it const:
    _flirROSInterfaceModule.reset(new FlirROSInterfaceModule(nh, cam_settings));
    return true;
}

bool frames_to_msg(
    std::array<flirmulticamera::Frame, flirmulticamera::GLOBAL_CONST_NCAMS>& spinnakerframes, 
    keiko_msgs::ImgsList& imgs_msg, 
    uint32_t& seq
){
    imgs_msg.images.clear();
    for (int i = 0; i<flirmulticamera::GLOBAL_CONST_NCAMS; i++)
    {
        sensor_msgs::Image img;
        img.data = std::vector<uint8_t>
        {
            (uint8_t*)spinnakerframes[i].frameData->GetData(), 
            (uint8_t*)spinnakerframes[i].frameData->GetData() + 3 * 
            spinnakerframes[i].frameData->GetWidth() * 
            spinnakerframes[i].frameData->GetHeight()
        };
        img.encoding = "rgb8";
        img.header.stamp.sec = spinnakerframes[i].Timestamp.tv_sec;
        img.header.stamp.nsec = spinnakerframes[i].Timestamp.tv_nsec;
        img.header.frame_id = "cam"+std::to_string(i);
        img.height = spinnakerframes[i].frameData->GetHeight();
        img.width = spinnakerframes[i].frameData->GetWidth();
        img.step = img.width*3; // width * byte_depth * num_channels; 
        imgs_msg.images.push_back(img);
    }
    imgs_msg.header.frame_id = "camera";
    imgs_msg.header.stamp = ros::Time::now();
    imgs_msg.header.seq = seq;
    return true;
}

bool frame_to_msg(
    flirmulticamera::Frame& spinnakerframe, 
    sensor_msgs::Image& img, 
    uint32_t& seq
){
        img.data = std::vector<uint8_t>
        {
            (uint8_t*)spinnakerframe.frameData->GetData(), 
            (uint8_t*)spinnakerframe.frameData->GetData() + 3 * 
            spinnakerframe.frameData->GetWidth() * 
            spinnakerframe.frameData->GetHeight()
        };
        img.encoding = "rgb8";
        img.header.stamp.sec = spinnakerframe.Timestamp.tv_sec;
        img.header.stamp.nsec = spinnakerframe.Timestamp.tv_nsec;
        img.header.seq = seq;
        img.height = spinnakerframe.frameData->GetHeight();
        img.width = spinnakerframe.frameData->GetWidth();
        img.step = img.width*3; // width * byte_depth * num_channels; 

    return true;
}