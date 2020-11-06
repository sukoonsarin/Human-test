/**
 * Copyright 2020 Sneha Nayak, Sukoon Sarin
 * @file main.cpp
 * @author Sneha Nayak (sneha@umd.com)
 * @author Sukoon Sarin (sukoon@umd.com)
 * @brief Main file that run all tests
 * @version 0.1 //SHOULD WE KEP TRACK OF THE VERSION OR NOT ??
 * @date 2020-10-19
 * 
 * @copyright Copyright (c) 2020 Sneha Nayak, Sukoon Sarin
 * 
 */
#include <gtest/gtest.h>

/**
 * @brief Main function for test.cpp
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @return int : Exit code 1 or 0
 */
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
