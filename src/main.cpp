#include "ETL.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Dense>

int main(int argc, char *argv[]){
  
  std::cout << "Hello World\n";

  for(int i = 1; i < argc; ++i){
    std::cout << "argv[" << i << "] = \"" << argv[i] << "\"\n";
  }
  
  ETL etl(argv[1], argv[2], argv[3]);

  std::vector<std::vector<std::string>> data = etl.readCsv();

  size_t rows = data.size();
  size_t cols = data[0].size();

  std::cout << "rows:: " << rows << "\ncols:: " << cols << "\n";

  Eigen::MatrixXd dataMatrix = etl.csvToEigenMatrix(data, rows, cols);

  std::cout << "rows:: " << dataMatrix.rows() << "\ncols:: " << dataMatrix.cols() << "\n";

  std::cout << dataMatrix.topRows(3) << std::endl;

  return 0;
}
