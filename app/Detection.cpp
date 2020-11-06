/**
 * Copyright 2020 Sneha Nayak, Sukoon Sarin
 * @file Detection.cpp
 * @author Sneha Nayak (snehanyk@umd.edu)
 * @author Sukoon Sarin (sukoon@umd.edu)
 * @brief Detection Class implementation
 * @version 0.1
 * @date 2020-10-19
 * 
 * @copyright Copyright (c) 2020 Sneha Nayak, Sukoon Sarin
 * 
 */
#include "../include/Detection.h"

/**
 * @brief Detection constructor.
 */
Detection::Detection() {
  confThreshold_ = 0.5;
  nmsThreshold_ = 0.4;
  inpWidth_ = 416;
  inpHeight_ = 416;
  loadModelandLabelClasses("../yolov4.weights",
   "../yolov4.cfg", "../coco.names");
}
/**
 * @brief Fetches all bounding boxes of detected humans in a single frame
 */
std::vector<cv::Rect> Detection::getDetections() {
  return detections;
}
/**
 * @brief Gives confidence metric for each bounding box for detected humans in a frame
 */
std::vector<float> Detection::getConfidence() {
  return confidenceDetection;
}

/**
 * @brief Initializes Confidence and non maximum suppression threshold along with width and height of the image
 */
void Detection::initializeParams(float confThreshold,
float nmsThreshold, float inpWidth, float inpHeight) {
  confThreshold_ = confThreshold;
  nmsThreshold_ = nmsThreshold;
  inpWidth_ = inpWidth;
  inpHeight_ = inpHeight;
}
/**
 * @brief Sets path to model weights file, model config file and model class files
 */
void Detection::loadModelandLabelClasses
(const std::string modelWeightsFile, const std::string
modelConfigFile, const std::string modelClassFile) {
  modelClassFile_ = modelClassFile;
  modelConfigFile_ = modelConfigFile;
  modelWeightsFile_ = modelWeightsFile;
}

/**
 * @brief Sets current frame
 */
void Detection::setFrame(cv::Mat frame) {
  frame_ = frame;
}
/**
 * @brief RUns YOLOv4 algo and detects humans and returns detections
 */
std::vector<cv::Rect> Detection::processFrameforHuman() {
  std::ifstream ifs(modelClassFile_.c_str());
  std::string line;
  while (getline(ifs, line))
    classes.push_back(line);
  cv::dnn::Net net =
  cv::dnn::readNetFromDarknet(modelConfigFile_, modelWeightsFile_);
  net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
  net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

  cv::Mat blob;
  cv::dnn::blobFromImage(frame_, blob, 1 / 255.0,
    cv::Size(inpWidth_, inpHeight_), cv::Scalar(0, 0, 0), true, false);
  net.setInput(blob);
  std::vector<cv::Mat> outs;
  net.forward(outs, getOutputsNames(net));

  detections = postProcess(outs);

  return detections;
}
/**
 * @brief Draws a red bounding box over frame from the given coordinates
 */
void Detection::drawRedBoundingBox(std::vector<int> coordinates,
int classID, float conf) {
  // Draw a rectangle displaying the bounding box
  cv::rectangle(frame_, cv::Point(coordinates[0], coordinates[1]),
   cv::Point(coordinates[2], coordinates[3]), cv::Scalar(0, 0, 255), 3);

  // Get the label for the class name and its confidence
  std::string label = cv::format("%.2f", conf);

  label = classes[classID] + ":" + label;

  // Display the label at the top of the bounding box
  int baseLine;
  cv::Size labelSize = cv::getTextSize(label,
   cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
  coordinates[1] = std::max(coordinates[1], (labelSize.height));
  cv::rectangle(frame_, cv::Point(coordinates[0], coordinates[1]
  - round(1.5 * labelSize.height)), cv::Point(coordinates[0] +
  round(1.5 * labelSize.width), coordinates[1] + baseLine),
  cv::Scalar(255, 255, 255), cv::FILLED);
  cv::putText(frame_, label, cv::Point(coordinates[0], coordinates[1]),
   cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 0), 1);
}
/**
 * @brief Gets correct detections and bounding boxes are reduced
 */

std::vector<cv::Rect> Detection::postProcess(const std::vector<cv::Mat> &outs) {
  std::vector<int> classIds;
  std::vector<float> confidences;
  std::vector<cv::Rect> boxes;

  for (size_t i = 0; i < outs.size(); ++i) {
    // Scan through all the bounding boxes output from the
    // network and keep only the ones with high confidence
    // scores. Assign the box's class label as the class
    // with the highest score for the box.
    float *data = reinterpret_cast<float *> (outs[i].data);
    for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols) {
      cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
      cv::Point classIdPoint;
      double confidence;
      // Get the value and location of the maximum score
      minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
      if (confidence > confThreshold_) {
        int centerX = static_cast<int>(data[0] * frame_.cols);
        int centerY = static_cast<int>(data[1] * frame_.rows);
        int width = static_cast<int>(data[2] * frame_.cols);
        int height = static_cast<int>(data[3] * frame_.rows);
        int left = centerX - width / 2;
        int top = centerY - height / 2;

        if (classes[classIdPoint.x] == "person") {
          classIds.push_back(classIdPoint.x);
          confidences.push_back(static_cast<float> (confidence));
          boxes.push_back(cv::Rect(left, top, width, height));
        }
      }
    }
  }

  // Perform non maximum suppression to eliminate
  // redundant overlapping boxes with lower confidences
  std::vector<int> indices;
  cv::dnn::NMSBoxes
  (boxes, confidences, confThreshold_, nmsThreshold_, indices);
  for (size_t i = 0; i < indices.size(); ++i) {
    int idx = indices[i];
    cv::Rect box = boxes[idx];

    std::vector<int> coordinates =
    {box.x, box.y, box.x + box.width, box.y + box.height};

    if (classes[classIds[idx]] == "person")
      drawRedBoundingBox(coordinates, classIds[idx], confidences[idx]);
  }

  confidenceDetection = confidences;
  return boxes;
}
/**
 * @brief Gets output names of the last layer of the neural network
 */

std::vector<cv::String>
Detection::getOutputsNames(const cv::dnn::Net &net) {
  static std::vector<cv::String> names;
  if (names.empty()) {
    // Get the indices of the output layers,
    // i.e. the layers with unconnected outputs
    std::vector<int> outLayers = net.getUnconnectedOutLayers();

    // get the names of all the layers in the network
    std::vector<cv::String> layersNames = net.getLayerNames();

    // Get the names of the output layers in names
    names.resize(outLayers.size());
    for (size_t i = 0; i < outLayers.size(); ++i)
      names[i] = layersNames[outLayers[i] - 1];
  }
  return names;
}
