#ifndef WINEDATA_ETL_HPP
#define WINEDATA_ETL_HPP

#include <Eigen/Dense>
#include <string>
#include <vector>

class WineDataETL {

  std::string dataset = "";
  std::string delimiter = ",";
  bool header = false;

public:
  WineDataETL(std::string dataset, std::string delimiter, bool header)
      : dataset(dataset), delimiter(delimiter), header(header) {}

  std::vector<std::vector<std::string>> readCSV();

  Eigen::MatrixXd
  csvToEigenMatrix(std::vector<std::vector<std::string>> dataset, size_t rows,
                   size_t cols);

  std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd>
  trainTestSplit(const Eigen::MatrixXd &data, float train_size);

  Eigen::VectorXd mean(const Eigen::MatrixXd &data);
  Eigen::VectorXd stdDev(const Eigen::MatrixXd &data);
  Eigen::MatrixXd normalize(const Eigen::MatrixXd &data, bool normalizeTarget);
  void vectorToFile(const std::vector<float>& vec, const std::string& filename);
  void eigenToFile(const Eigen::MatrixXd& data, const std::string& filename);
};

#endif // WINEDATA_ETL_HPP
