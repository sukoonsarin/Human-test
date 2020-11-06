/**
 * Copyright 2020 Sneha Nayak, Sukoon Sarin
 * @file DataLoader.cpp
 * @author Sneha Nayak (snehanyk@umd.edu)
 * @author Sukoon Sarin (sukoon@umd.edu)
 * @brief Data loader Class implementation
 * @version 0.2
 * @date 2020-11-01
 * 
 * @copyright Copyright (c) 2020 Sneha Nayak, Sukoon Sarin
 * 
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/DataLoader.h"
#include "../include/Detection.h"
#include "../include/Track.h"

Detection detection;
Track tracker;
/**
 * @brief Dataloader constructor.
 */
DataLoader::DataLoader() {
    setPath("../person.jpg");
    frame_ = cv::imread(path_, cv::IMREAD_COLOR);
}
/**
 * @brief Dataloader constructor with path and method parameters.
 */
DataLoader::DataLoader(const std::string path, const std::string method) {
    setInputStreamMethod(method);
    setPath(path);
}
/**
 * @brief Set the Input Stream Method object
 */
void DataLoader::setInputStreamMethod(const std::string method) {
    method_ = method;
}
/**
 * @brief Get the Input Stream Method object. Fetches input method 
 */
std::string DataLoader::getInputStreamMethod() {
    return method_;
}

/**
 * @brief Set the Input Stream Method object
 */
void DataLoader::setPath(const std::string path) {
    path_ = path;
}

/**
 * @brief: Updates the isVideo and isImage value and sets the imagePath and videoPath.
 */
int DataLoader::checkParser(cv::CommandLineParser parser) {
    if (parser.has("image")) {
        path_ = parser.get<std::string>("image");
        std::cout << "Data input method is Image" << std::endl;
        return 0;
    } else if (parser.has("video")) {
        path_ = parser.get<std::string>("video");
        std::cout << "Data input method is Video" << std::endl;
        return 0;
    } else {
        return -1;
    }
}

/**
 * @brief: Processes the video and updates the video frames with bounding boxes.
 */
void DataLoader::processInput(cv::CommandLineParser parser) {
    // Open a video file or an image file or a camera stream.
    cv::VideoCapture capture;
    cv::VideoWriter video;
    try {
        // outputFile = "yolo_out_cpp.avi";
        if (parser.has("image")) {
            // Open the input file
            std::ifstream inputfile(path_);
            if (!inputfile)
                throw("error: Image or video file required");
            capture.open(path_);
            path_.replace(path_.end() - 4, path_.end(),
            "_YOLOv4_output_cpp.jpg");
            outputFile = path_;
        } else if (parser.has("video")) {
            // Open the video file
            std::ifstream inputfile(path_);
            if (!inputfile)
                throw("error: Image or video file required");
            capture.open(path_);
            path_.replace(path_.end() - 4, path_.end(),
            "_YOLOv4_output_cpp.avi");
            outputFile = path_;
            // Get the video writer initialized to save the output video
            video.open(outputFile, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
            28, cv::Size(capture.get(cv::CAP_PROP_FRAME_WIDTH),
            capture.get(cv::CAP_PROP_FRAME_HEIGHT)));
        } else {
            // Open the default input file

            if (method_ == "image") {
                std::ifstream inputfile(path_);
                if (!inputfile)
                    throw("error: Image or video file required");
                capture.open(path_);
                path_.replace(path_.end() - 4, path_.end(),
                "_YOLOv4_output_cpp.jpg");
                outputFile = path_;
            } else {
                std::ifstream inputfile(path_);
                if (!inputfile)
                    throw("error: Image or video file required");
                capture.open(path_);
                path_.replace(path_.end() - 4, path_.end(),
                "_YOLOv4_output_cpp.avi");
                outputFile = path_;
                // Get the video writer initialized to save the output video
                video.open(outputFile,
                cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 28,
                cv::Size(capture.get(cv::CAP_PROP_FRAME_WIDTH),
                capture.get(cv::CAP_PROP_FRAME_HEIGHT)));
            }
        }
    }
    catch (...) {
        std::cout << "Could not open the input image/video stream" << std::endl;
    }

    // Create a window
    static const std::string kWinName = "Human Detection";
    cv::namedWindow(kWinName, cv::WINDOW_NORMAL);
    int frameNumber = 1;

    tracker.initializeTracker();
    std::vector<cv::Rect> detections;
    std::vector<float> confidenceDetection;
    while (cv::waitKey(1) < 0) {
        // perform analysis
        capture >> frame_;
        frameNumber++;
        if (frame_.empty()) {
            std::cout << "Output file is stored as " << outputFile << std::endl;
            cv::waitKey(3000);
            break;
        }
        detection.setFrame(frame_);
        tracker.setFrame(frame_);
        if (frameNumber % 45 == 0) {
            detections.clear();
            detection.setFrame(frame_);
            detections = detection.processFrameforHuman();
            tracker.setFrame(frame_);
            tracker.runTrackerAlgorithm(detections);
            // write frame to video
        } else {
            tracker.updateTracker();
        }
        frame_ = tracker.drawGreenBoundingBox();
        cv::Mat finalFrame;
        frame_.convertTo(finalFrame, CV_8U);
        if (parser.has("image")) {
            cv::imwrite(outputFile, finalFrame);
        } else if (parser.has("video")) {
            video.write(finalFrame);
        }
        cv::imshow(kWinName, frame_);
    }
    capture.release();
    if (parser.has("video"))
        video.release();
}
