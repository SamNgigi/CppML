#ifndef WINEDATA_ETL_HPP
#define WINEDATA_ETL_HPP

#include <vector>
#include <string>
#include <Eigen/Dense>

class WineDataETL{

  std::string dataset = "";
  std::string delimiter = ",";
  bool header = false;

public:
  WineDataETL(std::string dataset, std::string delimiter, bool header): dataset(dataset), delimiter(delimiter), header(header) {}
  
  std::vector<std::vector<std::string>> readCSV();

  Eigen::MatrixXd csvToEigenMatrix(
    std::vector<std::vector<std::string>> data,
    size_t rows,
    size_t cols
  );

};

#endif // WINEDATA_ETL_HPP
