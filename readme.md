# Human Detection and Tracker For ACME's Search and Rescue Drone
[![Build Status](https://travis-ci.org/sukoonsarin/Human-Detection-and-Tracker.svg?branch=master)](https://travis-ci.org/github/sukoonsarin/Human-Detection-and-Tracker)
[![Coverage Status](https://coveralls.io/repos/github/sukoonsarin/Human-Detection-and-Tracker/badge.svg?branch=master)](https://coveralls.io/github/sukoonsarin/Human-Detection-and-Tracker?branch=master)

## MIT Licence 
[![Packagist](https://img.shields.io/packagist/l/doctrine/orm.svg)](LICENSE) 
---

## Project contibutors

1) [Sneha Nayak](https://github.com/snehanyk05)
2) [Sukoon Sarin](https://github.com/sukoonsarin)

## Link to Phase 1 Presentation Video

https://drive.google.com/drive/folders/1lJ3bBe_7tFxXyDnSp5FBbLZLtUFEFj50?usp=sharing

## Link to Phase 2 Presentation Video

https://drive.google.com/drive/folders/1G2sBzu-hkWndrr6qjHLmen06YyPdsStR?usp=sharing

## Agile Iterative Process
[![Solo Iterative Process](https://img.shields.io/badge/AIP-ClickHere-brightgreen.svg?style=flat)](https://docs.google.com/spreadsheets/d/1CHGazXVQ__rPbJ77KxTLwJCWrwvfs2NZH7G_nNfibtA/edit#gid=0) 

## Agile Planning
https://docs.google.com/document/d/1PhiaCQiU5IhbYd66ARC0uiOVwrjqZ-Kwy5KarFBFgZk/edit?usp=sharing



## Overview

A Human detection and tracking module for ACME's search and rescue drone. Detects a human in video frames, and bounds the detection with a red bounding box. To check if the person is moving, the detected human is tracked and the tracked human is bounded by a blue bounding box. The pose of the human is also returned in the frame. To achieve the detection phase we use YOLOv4 and to acheive tracking, we used OpenCV's KCF Multi-Tracker functionality. The output obtained by the module is used by the path planning and controls component of the rescue drone.

The detection module which employs the YOLOv4 object detector algorithm, is used with a pre-trained COCO Dataset.
1. Each frame in the video sequence is preprocessed to be converted into a blob, where each pixel in the image is reduced by a factor of 1/255. 

2. These blobs are then fed to the Deep Learning neural network, which then gives out a list of detections with confidence scores of how close they are to the class labels.

3. The number of bounding boxes are then reduced using non-maximum supression.

4. These detections for each frame are then passed to the Kernelized Correlation Filters Tracker to track the detected humans.

5. The KCF algorithm then tracks the humans in question and then return the pose of the human in the camera frame.

We managed to create a robust set of test cases with:

- cmake
- googletest

## Output of the Integrated Modules 
### Image Screenshots 
<p align="center">
<img src="https://github.com/sukoonsarin/Human-Detection-and-Tracker/blob/master/output/output.png">
</p> 


### Video output
Please find the videos in the below link:

![Video output](https://drive.google.com/drive/folders/1WQY_-ZKl6SaCuTw_C-mdeYvkv5fAyVub?usp=sharing) 

## Dependencies

* Ubuntu 18.04 (Operating System)
* Modern C++ 11(Programming Language)
* CMake (Build System)
* OpenCV >= 4.4 with dnn and tracking library

## License 

```
MIT License

Copyright (c) 2020 Sukoon Sarin, Sneha Nayak

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## Standard install via command-line

```
git clone --recursive https://github.com/sukoonsarin/Human-Detection-and-Tracker
cd <path to repository>
mkdir build
cd build
cmake ..
make
Run tests: ./test/cpp-test
Run program: ./app/shell-app --video=../run.mp4 (or path to video file)
```

## Building for code coverage (for assignments beginning in Week 4)
```
sudo apt-get install lcov
cmake -D COVERAGE=ON -D CMAKE_BUILD_TYPE=Debug ../
make
make code_coverage
```
This generates a index.html page in the build/coverage sub-directory that can be viewed locally in a web browser.


## Generate Doxygen Documentation

To install doxygen run the following command:
```
sudo apt-get install doxygen
```
Now from the cloned directory run:
```
doxygen doxygen
```

Generated doxygen files are in html format and you can find them in ./docs folder. With the following command
```
cd docs
cd html
google-chrome index.html
