#include <gtest/gtest.h>
#include <fstream>
#include <Eigen/Dense>
#include <stdexcept>

#include "etl/winedata_etl.hpp"

class TestWineDataETL: public ::testing::Test {
protected:
  void SetUp() override {

    // Creates a temporary CSV file for testing
    std::ofstream testFile("test.csv");
    testFile << "A,B,C\n";
    testFile << "1.0,2.0,3.0\n";
    testFile << "4.0,5.0,6.0\n";
    testFile.close();

    // Create a CSV file with missing values
    std::ofstream missingvalueFile("missing_value.csv");
    missingvalueFile << "a,b,c\n";
    missingvalueFile << "1.0,2.0,3.0\n";
    missingvalueFile << "4.0,5.0,6.0\n";
    missingvalueFile.close();

    // Create empty file
    std::ofstream emptyFile("empty.csv");
    emptyFile.close();

    // Create a CSV file with different delimiter
    std::ofstream semicolonFile("semicolon.csv");
    semicolonFile << "a;b;c\n";
    semicolonFile << "1.0;2.0;3.0\n";
    semicolonFile.close();

  }

  void TearDown() override {
    // Cleanup by removing temp csv files
    std::remove("test.csv");
    std::remove("missing_value.csv");
    std::remove("empty.csv");
    std::remove("semicolon.csv");
  }

  WineDataETL etl{"test.csv", ",", true};

};


TEST_F(TestWineDataETL, TestFileNotFound){
  WineDataETL notFoundETL("notFound.csv", ",", true);
  ASSERT_THROW(notFoundETL.readCSV(), std::runtime_error);
}
