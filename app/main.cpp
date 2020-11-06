/**
 * @file    main.cpp
 * @author  Sneha Nayak, Sukoon Sarin
 * @copyright Copyright (c) 2020 Sneha Nayak, Sukoon Sarin
 * @version 0.1
 * @date 2020-10-19
 * @brief Main file to run detetction and tracking
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/DataLoader.h"

/**
 * @fn main
 * @brief Main function
 * @detail Finds detection and tracks human, return coordinates in UAV's camera frame
 * @return Program execution status
 */
int main(int argc, char **argv) {
    DataLoader data;
    // keys It is used for showing parsing examples.
    const char *keys =
        "{help h usage ? | | Usage examples: \n\t\t."
        "/object_detection_yolo.out --image=dog.jpg \n\t\t."
        "/object_detection_yolo.out --video=run_sm.mp4}"
        "{image i        |<none>| input image   }"
        "{video v       |<none>| input video   }";
    // takes as input commandline arguments
    cv::CommandLineParser parser(argc, argv, keys);
    parser.about("Use this script to run Human detection"
    "using YOLOv4 in OpenCV.");
    if (parser.has("help")) {
        parser.printMessage();
        return 0;
    }
    int input_type = data.checkParser(parser);
    // If no command line argument provided, takes default values of Constructor
    if (input_type == -1) {
        data.setInputStreamMethod(std::string("Image"));
        std::cout << "Data input method is : " <<
        data.getInputStreamMethod() << std::endl;
    }
    data.processInput(parser);

    return 0;
}
