#include <iostream>
#include <fstream>

#include "polynomial.hpp"

PolynomialGenerator::PolynomialGenerator(const std::vector<Polynomial>& polynomials) : r(polynomials.size()), polynomials(polynomials) {}


std::vector<float> PolynomialGenerator::get_force(const std::vector<float>& q, const bool use_quadratic,
    const bool use_cubic) const {
    assert(q.size() == r);
    std::vector<float> R(r);
    // Add linear terms
    for (int c = 0; c < r; c++) {
        for (int i = 0; i < r; i++) {
            R[c] += polynomials.at(c).get_linear(i) * q[i];
        }
    }

    // Add quadratic terms
    if (use_quadratic) {
        for (int c = 0; c < r; c++) {
            for (int i = 0; i < r; i++) {
                for (int j = i; j < r; j++) {
                    R[c] += polynomials.at(c).get_quadratic(i, j) * q[i] * q[j];
                }
            }
        }
    }

    // Add cubic terms
    if (use_cubic) {
        for (int c = 0; c < r; c++) {
            for (int i = 0; i < r; i++) {
                for (int j = i; j < r; j++) {
                    for (int k = j; k < r; k++) {
                        R[c] += polynomials.at(c).get_cubic(i, j, k) * q[i] * q[j] *
                            q[k];
                    }
                }
            }
        }
    }

    return R;
}

// TODO: fix double indexing
std::vector<float> PolynomialGenerator::get_tangent_stiffness_mat(const std::vector<float>& q) const {
    assert(q.size() == r);
    std::vector<float> K(r * r, 0.0f);

    // Add linear terms
    for (int a = 0; a < r; a++) {
        for (int l = 0; l < r; l++) {
            K[a * r + l] += polynomials.at(a).get_linear(l);
        }
    }

    // Add quadratic terms
    for (int a = 0; a < r; a++) {
        for (int l = 0; l < r; l++) {
            for (int i = 0; i < r; i++) {
                K[a * r + l] += polynomials.at(a).get_quadratic(l, i) * q[i];
            }
        }
    }

    // Add cubic terms
    for (int a = 0; a < r; a++) {
        for (int l = 0; l < r; l++) {
            for (int i = 0; i < r; i++) {
                for (int j = 0; j < r; j++) {
                    K[a * r + l] += polynomials.at(a).get_cubic(l, i, j) * q[i] * q[j];
                }
            }
        }
    }

    return K;
}