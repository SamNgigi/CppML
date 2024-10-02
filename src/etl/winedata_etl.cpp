#include "etl/winedata_etl.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <Eigen/Dense>

std::vector<std::vector<std::string>> WineDataETL::readCSV(){

  std::ifstream file(dataset);

  // Addresses if file is not found
  if(!file.is_open()){
    throw std::runtime_error("Unable to open file: " + dataset);
  }
  // Address if file is empty
  if(file.peek() == std::ifstream::traits_type::eof()){
    throw std::runtime_error("Empty File");
  }

  std::vector<std::vector<std::string>> data;
  std::string line = "";

  while(std::getline(file, line)){
    std::vector<std::string> vec;
    boost::algorithm::split(vec, line, boost::is_any_of(delimiter));
    data.push_back(vec);
  }
  file.close();

  return data;

}

Eigen::MatrixXd WineDataETL::csvToEigenMatrix(
  std::vector<std::vector<std::string>> data,
  size_t rows,
  size_t cols
){

  if(header){
    rows = rows - 1;
  }
  

  Eigen::MatrixXd mat(cols, rows);

  for(size_t i = 0; i < rows; ++i){
    for(size_t j = 0; j < cols; ++j){
      mat(j, i) = std::stod(data[i][j]);
    }
  }

  return mat.transpose();

}


Eigen::VectorXd WineDataETL::mean(const Eigen::MatrixXd& data){
  return data.colwise().mean();
}

Eigen::VectorXd WineDataETL::stdDev(const Eigen::MatrixXd& data){
  return ((data.array().square().colwise().sum())/(data.rows()-1)).sqrt();
}


Eigen::MatrixXd WineDataETL::normalize(Eigen::MatrixXd data, bool normalizeTarget){
    Eigen::MatrixXd dataNorm;
    if(normalizeTarget==true) {
        dataNorm = data;
    } else {
        dataNorm = data.leftCols(data.cols()-1);
    }

    auto mn = mean(dataNorm);
    Eigen::MatrixXd scaled_data = dataNorm.rowwise() - mn.transpose();
    auto std = WineDataETL::stdDev(scaled_data);

    Eigen::MatrixXd norm = scaled_data.array().rowwise()/std.transpose().array();

    if(normalizeTarget==false) {
        norm.conservativeResize(norm.rows(), norm.cols()+1);
        norm.col(norm.cols()-1) = data.rightCols(1);
    }

    return norm;
}
