#include "ETL.hpp"
#include "Eigen/src/Core/Matrix.h"

#include <vector>
#include <fstream>
#include <Eigen/Dense>
#include <boost/algorithm/string.hpp>




std::vector<std::vector<std::string>> ETL::readCsv(){
  std::ifstream file(dataset);
  std::vector<std::vector<std::string>> dataString;

  std::string line = "";

  while(std::getline(file, line)){
    std::vector<std::string> vec;
    boost::algorithm::split(vec, line, boost::is_any_of(delimiter));
    dataString.push_back(vec);
  }

  file.close();

  return dataString;
} 

Eigen::MatrixXd ETL::csvToEigenMatrix(std::vector<std::vector<std::string>> dataset, size_t rows, size_t  cols){
  if(header){
    rows = rows - 1;
  }
  
  Eigen::MatrixXd mat(cols, rows);

  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; ++j){
      mat(j, i) = atof(dataset[i][j].c_str());
    }
  }

  return mat.transpose();

}



