#ifndef PANGOLIN_EIGEN_LIBRARY_TEST_HPP
#define PANGOLIN_EIGEN_LIBRARY_TEST_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <Eigen/Dense>
#include <iostream>

void EigenTest() {
    Eigen::MatrixXd m (Eigen::MatrixXd::Random(3,3));
    m = (m + Eigen::MatrixXd::Constant(3,3,1.2)) * 50;
    cout << "m =" << endl << m << endl;
    Eigen::VectorXd v(3);
    v << 1, 2, 3;
    cout << "m * v =" << endl << m * v << "\n"; }

#end//EIGEN_LIBRARY_TEST