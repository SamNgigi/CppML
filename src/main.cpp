#include "etl/winedata_etl.hpp"

#include <iostream>
#include <Eigen/Dense>

int main(int argc, char *argv[]){
  
  std::cout << "Hello World!\n";

  for(int i=0; i < argc; ++i){
    std::cout << "argv[" << i << "]= \"" << argv[i] << "\"\n";
  }

  WineDataETL etl(argv[1], argv[2], argv[3]);

  std::vector<std::vector<std::string>> data = etl.readCSV();

  size_t rows = data.size();
  size_t cols = data[0].size();

  Eigen::MatrixXd dataMat = etl.csvToEigenMatrix(data, rows, cols);

  std::cout << dataMat.topRows(3) << std::endl;

  return EXIT_SUCCESS;

}
