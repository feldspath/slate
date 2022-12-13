#pragma once

#include <array>
#include <algorithm>
#include <vector>
#include <assert.h>

// requires i <= j
static inline int triangle_indexing(int i, int j) { return j * (j + 1) / 2 + i; }

// requires i <= j <= k
static inline int triangle_indexing(int i, int j, int k) { return k * (k + 1) * (k + 2) / 6 + j * (j + 1) / 2 + i; }

static inline void order3(int& i, int& j, int& k) {
    if (j < i)
        std::swap(i, j);
    if (k < i) {
        std::swap(k, i);
        std::swap(k, j);
    }
    else if (k < j)
        std::swap(k, j);

    assert(i <= j || j <= k);
}

static inline void order2(int& i, int& j) {
    if (j < i)
        std::swap(i, j);

    assert(i <= j);
}

class Polynomial {
private:
    std::vector<double> linear_terms;
    std::vector<double> quadratic_terms;
    std::vector<double> cubic_terms;

public:
    int r;

    Polynomial(int _r) : r{ _r } {
        linear_terms.resize(r, 0.);
        quadratic_terms.resize(triangle_indexing(r - 1, r - 1) + 1, 0.);
        cubic_terms.resize(triangle_indexing(r - 1, r - 1, r - 1) + 1, 0.);
    }

    double& get_linear(int i) {
        return linear_terms[i];
    }

    double& get_quadratic(int i, int j) {
        order2(i, j);
        return quadratic_terms[triangle_indexing(i, j)];
    }

    double& get_cubic(int i, int j, int k) {
        order3(i, j, k);
        return cubic_terms[triangle_indexing(i, j, k)];
    }

    double get_linear(int i) const { return linear_terms[i]; }

    double get_quadratic(int i, int j) const {
        order2(i, j);
        return quadratic_terms[triangle_indexing(i, j)];
    }

    double get_cubic(int i, int j, int k) const {
        order3(i, j, k);
        return cubic_terms[triangle_indexing(i, j, k)];
    }
};

class PolynomialGenerator {
public:
    PolynomialGenerator(const std::vector<Polynomial>& polynomials);

    // Returns force in reduced coordinates
    std::vector<float> get_force(const std::vector<float>& q, const bool use_quadratic = true,
        const bool use_cubic = true) const;

    // Returns tangent stiffness matrix in reduced coordinates
    std::vector<float> get_tangent_stiffness_mat(const std::vector<float>& q) const;

private:
    const int r;
    std::vector<Polynomial> polynomials;
};