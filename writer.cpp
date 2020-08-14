#include "opencv2/opencv.hpp"
#include <iostream>
#include "withrobot_camera.hpp"

using namespace std;
using namespace cv;

int main(){

  // // If no input was specified
    const char* devPath = "/dev/video2";
    Withrobot::Camera camera(devPath);

    /* bayer RBG 640 x 480 80 fps */
    camera.set_format(640, 480, Withrobot::fourcc_to_pixformat('G','B','G','R'), 1, 80);

    /*
     * get current camera format (image size and frame rate)
     */
    Withrobot::camera_format camFormat;
    camera.get_current_format(camFormat);

    /*
     * Print infomations
     */
    std::string camName = camera.get_dev_name();
    std::string camSerialNumber = camera.get_serial_number();

    printf("dev: %s, serial number: %s\n", camName.c_str(), camSerialNumber.c_str());
    printf("----------------- Current format informations -----------------\n");
    camFormat.print();
    printf("---------------------------------------------------------------\n");

    int brightness = camera.get_control("Gain");
    int exposure = camera.get_control("Exposure (Absolute)");

    //camera.set_control("Gain", 70);
    camera.set_control("Gain", 66);
    //camera.set_control("Exposure (Absolute)", 15);
    camera.set_control("Exposure (Absolute)", 170);

    std::string windowName = camName + " " + camSerialNumber;
    cv::Mat srcImg(cv::Size(camFormat.width, camFormat.height), CV_8UC1);
    cv::Mat frame(cv::Size(camFormat.width, camFormat.height), CV_8UC3);
  
  // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file. 
  VideoWriter video("outcpp.avi",CV_FOURCC('M','J','P','G'),80, Size(camFormat.width,camFormat.height)); 
  
  while(1)
  { 
    
    int size = camera.get_frame(srcImg.data, camFormat.image_size, 1);
    if (size == -1) {
            printf("error number: %d\n", errno);
            perror("Cannot get image from camera");
            camera.stop();
            camera.start();
            continue;
        }
    cv::cvtColor(srcImg, frame, cv::COLOR_BayerGB2BGR);

    // Write the frame into the file 'outcpp.avi'
    video.write(frame);
   
    // Display the resulting frame    
    // imshow( "Frame", frame );
 
    // Press  ESC on keyboard to  exit
    char c = (char)waitKey(1);
    if( c == 27 ) 
      break;
  }

  // When everything done, release the video capture and write object
  video.release();

  // Closes all the windows
  destroyAllWindows();
  return 0;
}
