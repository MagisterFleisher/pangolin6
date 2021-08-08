/* #ifndef PANGOLIN_EIGEN_LIBRARY_TEST_HPP
#define PANGOLIN_EIGEN_LIBRARY_TEST_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <Eigen/Dense>
#include <iostream>

void EigenTest() {
    Eigen::MatrixXd m (Eigen::MatrixXd::Random(3,3));
    m = (m + Eigen::MatrixXd::Constant(3,3,1.2)) * 50;
    std::cout << "m =" << std::endl << m << std::endl;
    Eigen::VectorXd v(3);
    v << 1, 2, 3;
    std::cout << "m * v =" << std::endl << m * v << "\n"; }

#endif //EIGEN_LIBRARY_TEST */