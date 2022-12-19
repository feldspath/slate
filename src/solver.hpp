#pragma once

#include "polynomial.hpp"

#include <iostream>
#include <memory>
#include <vector>

void blas_add_in_c(std::vector<float>& c, std::vector<float>& a, int rows, int cols, float alpha = 1.0f, float beta = 1.0f);

void blas_mmv(std::vector<float>& mat, std::vector<float>& v, std::vector<float>& res, int m, int n, const float beta = 1.0f, const float res_add = 0.0f);

void add_vec(std::vector<float>& a, const std::vector<float>& b, const float beta);

void oppos_vec(std::vector<float>& a);

float vec_norm(const std::vector<float>& v);

void solve_system(std::vector<float>& A, std::vector<float>& b, int rows, int cols);

class Solver {
protected:
    // current time position and speed values
    std::vector<float> u;
    std::vector<float> du;

    // constant fields
    std::vector<float> mass_matrix;
    std::vector<float> mass_matrix_inv;
public:
    Solver(const std::vector<float>& u0, const std::vector<float>& du0, const std::vector<float>& mass_matrix_);

    virtual void compute_next_step(float dt, const std::vector<float>& external_forces) = 0;

    const std::vector<float>& get_u() const { return u; }
    const std::vector<float>& get_du() const { return du; }
};

class ImplicitNewmarkSolver : public Solver {

public:
    ImplicitNewmarkSolver(const std::vector<float>& u0, const std::vector<float>& du0,
        const std::vector<float>& mass_matrix,
        std::shared_ptr<PolynomialGenerator> _polynomial_generator,
        int _max_newton_raphson, float _residual_tolerance, float _mass_coeff,
        float _stiffness_coeff, float _beta_newmark, float _gamma_newmark)
        : polynomial_generator{ _polynomial_generator }, max_newton_raphson{ _max_newton_raphson },
        residual_tolerance{ _residual_tolerance }, mass_coeff(_mass_coeff),
        stiffness_coeff(_stiffness_coeff), beta_newmark{ _beta_newmark },
        gamma_newmark{ _gamma_newmark }, ddu(u0.size(), 0.0f), Solver(u0, du0, mass_matrix) {
    }

    virtual void compute_next_step(float dt, const std::vector<float>& external_forces) override;

private:
    int max_newton_raphson = 2;
    float residual_tolerance = 1e-5f;
    std::shared_ptr<PolynomialGenerator> polynomial_generator;

    float mass_coeff;
    float stiffness_coeff;

    float beta_newmark = 0.25f;
    float gamma_newmark = 0.5f;

    std::vector<float> ddu;

    void update_constants(const float dt, float& alpha_1, float& alpha_2, float& alpha_3,
        float& alpha_4, float& alpha_5, float& alpha_6) const;
};