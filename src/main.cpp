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

  std::cout << "Data::\n" << dataMat.topRows(3) << std::endl;

  Eigen::MatrixXd normData = etl.normalize(dataMat, true);

  std::cout << "Normed Data::\n" << normData.bottomRows(3)<< std::endl;

  Eigen::MatrixXd X_train, y_train, X_test, y_test;
  std::tuple<Eigen::MatrixXd,Eigen::MatrixXd,Eigen::MatrixXd,Eigen::MatrixXd> split_data = etl.trainTestSplit(normData, 0.8);
  std::tie(X_train, y_train, X_test, y_test) = split_data;

  std::cout << "Normed Data rows:: " << normData.rows() << "\n";
  std::cout <<"X_train rows:: "<<X_train.rows()<<" :: "<<"X_train cols:: "<<X_train.cols()<<"\n";
  std::cout <<"y_train rows:: "<<y_train.rows()<<" :: "<<"y_train cols:: "<<y_train.cols()<<"\n";
  std::cout <<"X_test rows:: "<<X_test.rows()<<" :: "<<"X_test cols:: "<<X_test.cols()<<"\n";
  std::cout <<"y_test rows:: "<<y_test.rows()<<" :: "<<"y_test cols:: "<<y_test.cols()<<"\n";


  return EXIT_SUCCESS;

}
