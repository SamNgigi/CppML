#ifndef ETL_HPP
#define ETL_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <Eigen/Dense>


class ETL{
  std::string dataset;
  std::string delimiter;
  bool header;

public:
  ETL(std::string dataset, std::string delimiter, bool header): dataset(dataset), delimiter(delimiter), header(header) {}

  std::vector<std::vector<std::string>> readCsv();

  Eigen::MatrixXd csvToEigenMatrix(std::vector<std::vector<std::string>> dataset, size_t rows, size_t cols);

  Eigen::MatrixXd normalize(Eigen::MatrixXd data, bool normalizeTarget);
};


#endif // ETL_HPP
