#include "etl/winedata_etl.hpp"
#include "linear_regression/LinearRegression.hpp"

#include <iostream>
#include <Eigen/Dense>
#include <filesystem>

namespace fs = std::filesystem;

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
  std::cout << "\n";

  Eigen::MatrixXd normData = etl.normalize(dataMat, true);

  std::cout << "Normed Data::\n" << normData.bottomRows(3)<< std::endl;
  std::cout << "\n";

  Eigen::MatrixXd X_train, y_train, X_test, y_test;
  std::tuple<Eigen::MatrixXd,Eigen::MatrixXd,Eigen::MatrixXd,Eigen::MatrixXd> split_data = etl.trainTestSplit(normData, 0.8);
  std::tie(X_train, y_train, X_test, y_test) = split_data;

  std::cout << "Normed Data rows:: " << normData.rows() << "\n";
  std::cout <<"X_train rows:: "<<X_train.rows()<<" :: "<<"X_train cols:: "<<X_train.cols()<<"\n";
  std::cout <<"y_train rows:: "<<y_train.rows()<<" :: "<<"y_train cols:: "<<y_train.cols()<<"\n";
  std::cout <<"X_test rows:: "<<X_test.rows()<<" :: "<<"X_test cols:: "<<X_test.cols()<<"\n";
  std::cout <<"y_test rows:: "<<y_test.rows()<<" :: "<<"y_test cols:: "<<y_test.cols()<<"\n";
  std::cout << "\n";


  Eigen::VectorXd vec_train = Eigen::VectorXd::Ones(X_train.rows());
  Eigen::VectorXd vec_test = Eigen::VectorXd::Ones(X_test.rows());

  X_train.conservativeResize(X_train.rows(), X_train.cols()+1);
  X_train.col(X_train.cols()-1) = vec_train;
  X_test.conservativeResize(X_test.rows(), X_test.cols()+1);
  X_test.col(X_test.cols()-1) = vec_test;

  Eigen::VectorXd theta = Eigen::VectorXd::Zero(X_train.cols());
  float alpha = 0.01;
  int iters = 1000;

  Eigen::VectorXd thetaOut;
  std::vector<float> cost;
  LinearRegression lr;
  std::tuple<Eigen::VectorXd,std::vector<float>> gd = lr.gradientDescent(X_train, y_train, theta, alpha, iters);
  std::tie(thetaOut, cost) = gd;

  std::cout << "Theta:: " << thetaOut << "\n\n";

  std::cout << "Cost\n";
  for(auto c : cost){
    std::cout << c << "\n";
  }
  std::cout << "\n";

  fs::path data_path = fs::current_path() / "datasets" ;
/* 
  fs::path thetaOut_path = data_path / "thetaOut.txt";
  fs::path cost_path = data_path / "cost.txt";
  etl.eigenToFile(thetaOut, thetaOut_path.string());
  etl.vectorToFile(cost, cost_path.string());
 */

  auto mu_data = etl.mean(dataMat);
  auto mu_z = mu_data(11);

  auto scaled_data = dataMat.rowwise() - mu_data.transpose();

  auto sigma_data = etl.stdDev(scaled_data);
  auto sigma_z = sigma_data(11);

  Eigen::MatrixXd y_train_hat = (X_train * thetaOut * sigma_z).array()+ mu_z;
  Eigen::MatrixXd y = dataMat.col(11).topRows(1279);

  float R_Squared = lr.rSquared(y, y_train_hat);
  std::cout << "R-Squared: " << R_Squared << "\n";

  fs::path ytrain_hat_path = data_path / "y_train_hat.txt";
  // etl.eigenToFile(y_train_hat, ytrain_hat_path.string());

  return EXIT_SUCCESS;

}

