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
    testFile << "1.0,2.0,3.0\n";
    testFile << "4.0,5.0,6.0\n";
    testFile.close();

    // Create a CSV file with missing values
    std::ofstream missingvalueFile("missing_value.csv");
    missingvalueFile << "a,b,c\n";
    missingvalueFile << "1.0,,3.0\n";
    missingvalueFile << "4.0,5.0,\n";
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

  WineDataETL testEtl{"test.csv", ",", false};

};


TEST_F(TestWineDataETL, TestReadNonExistentFile){
  WineDataETL notFoundETL("notFound.csv", ",", true);
  ASSERT_THROW(notFoundETL.readCSV(), std::runtime_error);
}

TEST_F(TestWineDataETL, TestReadEmptyFile){
  WineDataETL emptyETL{"empty.csv", ",", true};
  ASSERT_THROW(emptyETL.readCSV(), std::runtime_error);
}

TEST_F(TestWineDataETL, TestReadCSV){
  std::vector<std::vector<std::string>> csvData  = testEtl.readCSV();
  ASSERT_EQ(csvData.size(), 2);
  ASSERT_EQ(csvData[0], std::vector<std::string>({"1.0", "2.0", "3.0"}));
  ASSERT_EQ(csvData[1], std::vector<std::string>({"4.0", "5.0", "6.0"}));
}

TEST_F(TestWineDataETL, TestReadMissingValues){
  WineDataETL missingValsETL{"missing_value.csv", ",", true};
  auto csvData  = missingValsETL.readCSV();
  ASSERT_EQ(csvData.size(), 3);
  ASSERT_EQ(csvData[1][1], "");
  ASSERT_EQ(csvData[2][2], "");
}

TEST_F(TestWineDataETL, TestDifferentDelimiter){
  WineDataETL semicolonETL{"semicolon.csv", ";", true};
  auto csvData = semicolonETL.readCSV();

  ASSERT_EQ(csvData.size(), 2);
  ASSERT_EQ(csvData[0], std::vector<std::string>({"a", "b", "c"}));
  ASSERT_EQ(csvData[1], std::vector<std::string>({"1.0", "2.0", "3.0"}));
}

TEST_F(TestWineDataETL, TestCsvToEigenMatrix){
  auto csvData = testEtl.readCSV();
  auto mat = testEtl.csvToEigenMatrix(csvData,2, 3); 
  Eigen::MatrixXd expected(2,3);
  expected << 1.0, 2.0, 3.0,
              4.0, 5.0, 6.0;
  ASSERT_EQ(mat.rows(), 2);
  ASSERT_EQ(mat.cols(), 3);
  ASSERT_TRUE(mat.isApprox(expected));
}

