#ifndef LinearRegression_hpp
#define LinearRegression_hpp

#include <vector>
#include <Eigen/Dense>

class LinearRegression{

public:
  LinearRegression(){};

  float OLS_Cost(Eigen::MatrixXd X, Eigen::MatrixXd y, Eigen::VectorXd theta);

  std::tuple<Eigen::VectorXd, std::vector<float>> gradientDescent(
    Eigen::MatrixXd X,
    Eigen::MatrixXd y,
    Eigen::VectorXd theta,
    float alpha,
    int iters
  );

};


#endif // LinearRegression_hpp
