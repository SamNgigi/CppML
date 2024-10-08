#include "linear_regression/LinearRegression.hpp"

#include <iostream>
#include <cmath>
#include <vector>
#include <Eigen/Dense>


float LinearRegression::OLS_Cost(Eigen::MatrixXd X, Eigen::MatrixXd y, Eigen::VectorXd theta){
  /* 
   * MEAN SQUARED ERROR
   *
   * Measures how close the regression line is to a set of data points.
   * It is a risk function/cost function corresponding to the expected value of
   * the squared error loss.
   *
   * Which means it measures the amount of error in a statistical model by calculating
   * the average squared difference between the observed and predicted values
   *
   * In the context of gradient descent algorithms, it is common to introduct a factor of 1/2 
   * to the MSE for ease of computation after taking the derivative.
   * So a value which is technically half the mean of squared errors may be called the MSE
  */ 
  
  Eigen::MatrixXd inner = pow(((X*theta)-y).array(), 2);

  return (inner.sum()/(2*X.rows()));
}


std::tuple<Eigen::VectorXd, std::vector<float>> LinearRegression::gradientDescent(
  Eigen::MatrixXd X,
  Eigen::MatrixXd y,
  Eigen::VectorXd theta,
  float alpha,
  int iters
){

  Eigen::MatrixXd temp = theta;

  int parameters = theta.rows();

  std::vector<float> cost;
  cost.reserve(1010);
  cost.push_back(OLS_Cost(X,y,theta));

  for(int i = 0; i < iters; ++i){
    Eigen::MatrixXd error = X*theta - y;
    for(int j = 0; j < parameters; ++j){
      Eigen::MatrixXd X_i = X.col(j);
      Eigen::MatrixXd term = error.cwiseProduct(X_i);
      temp(j, 0) = theta(j, 0) - ((alpha/X.rows())*term.sum());
    }
    theta = temp;
    cost.push_back(OLS_Cost(X,y,theta));
  }

  return std::make_tuple(theta, cost);
}

float LinearRegression::rSquared(Eigen::MatrixXd y, Eigen::MatrixXd y_hat){
  auto num = pow((y-y_hat).array(), 2).sum();
  auto den = pow(y.array().transpose()-y.mean(), 2).sum();
  return 1 - num/den;
}

