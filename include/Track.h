#include <iostream>
#include <vector>
#include <numeric>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking/tracker.hpp>

#include <map>

/**
 * @brief 
 * 
 */
class Track
{

private:
    /**
     * @brief Private Variable for the multip tracker network
     * 
     */
    cv::Ptr<cv::MultiTracker> multiTracker;
    /**
     * @brief Private Variable for current frame
     * 
     */
    cv::Mat frame_;
    /**
     * @brief Gets the Poses from the bounding boxes in the UAV's Camera Frame.
     * @param coordinates type : std::vector<float>
     * @return std::vector<float> Retruns the coordinates for bounding boxes in the frame
     */

    std::vector<float> getCoordinatesInCameraFrame(std::vector<float> coordinates);

public:
    /**
     * @brief Construct a new Track object
     * 
     */
    Track();
    /**
     * @brief Sets current frame
     * @param frame type : cv::Mat
     * @return void
     */

    void setFrame(cv::Mat frame);
    /**
     * @brief Initializes the tracker
     * @param void
     * @return void
     */
    void initializeTracker();

    /**
     * @brief Runs the tracking algo by taking in detections and confidence scores
     * @param detections type : std::vector<cv::Rect>
     * @return void
     */
    void runTrackerAlgorithm(std::vector<cv::Rect> detections);
    /**
     * @brief Resizes bounding boxes
     * @param box type : cv::Rect gives box coordinates
     * @return void
     */
    void resizeBoxes(cv::Rect &box);
    /**
     * @brief Updates frames in the tracker
     * @param void
     * @return void
     */
    void updateTracker();
    /**
     * @brief Draws green bounding box around the tracked human
     * @param void
     * @return cv::Mat return frame with tracking bounding box
     */
    cv::Mat drawGreenBoundingBox();
    /**
     * @brief Destroy the Track object
     * 
     */
    ~Track() {}
};
