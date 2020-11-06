/**
 * Copyright 2020 Sneha Nayak, Sukoon Sarin
 * @file test.cpp
 * @author Sneha Nayak (snehanyk@umd.edu)
 * @author Sukoon Sarin (sukoon@umd.edu)
 * @brief Solitary Test source file for testing the Detection, DataLoader and Track
 *        class methods. Includes all the required headers
 * @version 0.1
 * @date 2020-10-19
 * 
 * @copyright Copyright (c) 2020 Sneha Nayak, Sukoon Sarin
 * 
 */
#include <gtest/gtest.h>
#include <iostream>
#include <map>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../include/DataLoader.h"
#include "../include/Detection.h"
#include "../include/Track.h"


// keys It is used for showing parsing examples.
const char *keys =
    "{help h usage ? | | Usage examples: \n\t\t."
    "/object_detection_yolo.out --image=dog.jpg \n\t\t."
    "/object_detection_yolo.out --video=run_sm.mp4}"
    "{image i        |<none>| input image   }"
    "{video v       |<none>| input video   }";

DataLoader dummydataloader;
Detection detection1;
Track dummytrack;

std::vector<float> vec1 = {0.7, 0.9};
std::vector<std::vector<float>> vec2 =
{{34.0, 23.1, 64.2, 53.4}, {64.3, 53.3, 94.3, 83.3}};

//*****************************************************************************************************

/**
 * @brief Test case for checkParser method of DataLoader class.
 */
TEST(DataLoaderTest, checkParser) {
    int argc = 0;
    const char *argv = "image";
    cv::CommandLineParser parser(argc, &argv, keys);
    EXPECT_EQ(dummydataloader.checkParser(parser), -1);
}

/**
 * @brief Test case for getInputStreamMethod and setInputStreamMethod method of DataLoader class. 
 */
TEST(DataLoaderTest, SetandgetPath) {
    std::string test_method = "video";
    dummydataloader.setInputStreamMethod(test_method);
    EXPECT_EQ(test_method, dummydataloader.getInputStreamMethod());
}

/**
 * @brief Test case for processInput method of DatALoader class, in case of input method as image. The
 * test verifies that processInputo doesnt throw an exception.
 */
TEST(DataLoaderTest, checkProcessImage) {
    int argc = 0;
    const char *argv = "image";
    cv::CommandLineParser parser(argc, &argv, keys);
    EXPECT_NO_THROW({
        dummydataloader.processInput(parser);
    });
    EXPECT_NO_FATAL_FAILURE({
        dummydataloader.processInput(parser);
    });
}
/**
 * @brief Test case for processInput method of DataLoader class in case of input method as video. The
 * test verifies that processInput doesnt throw an exception.
 */
TEST(DataLoaderTest, checkProcessVideo) {
    int argc = 0;
    const char *argv = "video";
    cv::CommandLineParser parser(argc, &argv, keys);
    DataLoader dummydataloader1("../run.mp4", "video");
    EXPECT_NO_THROW({
        dummydataloader.processInput(parser);
    });
    EXPECT_NO_FATAL_FAILURE({
        dummydataloader.processInput(parser);
    });
}
/**
 * @brief Test case for loadModelandLabelClasses method of Detection class. Loads model class, config and weight files.
 */
TEST(DetectionTest, checkModelFiles) {
    std::string dummyclassfile = "../coco.names";
    std::string dummyweightfile = "../yolov4.weights";
    std::string dummyconfigfile = "../yolov4.cfg";
    EXPECT_NO_FATAL_FAILURE({
        detection1.loadModelandLabelClasses(dummyclassfile,
        dummyweightfile, dummyconfigfile);
    });
}
/**
 * @brief Test Case for Initializing Confidence and non maximum suppression threshold along with width and height of the image
 */
TEST(DetectionTest, checkInitialParams) {
    EXPECT_NO_FATAL_FAILURE({
        detection1.initializeParams(0.4, 0.5, 416, 416);
    });
}
/**
 * @brief Test case for settFrame method of DataLoader class. 
 */
TEST(DetectionTest, TestFrame) {
    cv::Mat test_frame = cv::imread("../person.jpg");
    EXPECT_NO_FATAL_FAILURE({
        detection1.setFrame(test_frame);
    });
}

/**
 * @brief Test case for processFrameforHuman method. Also checks for Detections return from this method.
 */
TEST(DetectionTest, ProcessHumandetections) {
    cv::VideoCapture capture;
    capture.open("../person.jpg");
    cv::Mat frame;
    capture >> frame;
    Detection detection2;
    detection2.setFrame(frame);
    std::vector<cv::Rect> testdetections = detection2.processFrameforHuman();

    std::vector<cv::Rect> getdetect = detection2.getDetections();
    std::vector<int> testdetect = {testdetections[0].x,
    testdetections[0].y, testdetections[0].height, testdetections[0].width};
    std::vector<int> testdetect1 = {getdetect[0].x,
    getdetect[0].y, getdetect[0].height, getdetect[0].width};

    EXPECT_EQ(testdetect, testdetect1);
}

/**
 * @brief Test case for getConfidence method. Checks for any fatal error.
 */
TEST(DetectionTest, Processconfidence) {
    cv::VideoCapture capture;
    capture.open("../person.jpg");
    cv::Mat frame;
    capture >> frame;
    Detection detection2;
    detection2.setFrame(frame);
    detection2.processFrameforHuman();

    std::vector<float> getconf1 = detection2.getConfidence();
    std::vector<float> getconf2 = detection2.getConfidence();
    EXPECT_EQ(getconf1, getconf2);
}

/**
 * @brief Test case for setFrame method of Track class. 
 */
TEST(TrackerTest, TestFrame) {
    cv::Mat test_frame = cv::imread("../person.jpg");
    EXPECT_NO_FATAL_FAILURE({
        dummytrack.setFrame(test_frame);
    });
}

/**
 * @brief Test case for initialize tracker method of Track class. Checks for any fatal error
 */
TEST(TrackerTest, Initializetracker) {
    EXPECT_NO_FATAL_FAILURE({
        dummytrack.initializeTracker();
    });
}
