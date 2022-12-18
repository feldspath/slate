#include "solver.hpp"

#include <complex>
#define lapack_complex_float std::complex<float>
#define lapack_complex_double std::complex<double>
#include <OpenBLAS/cblas.h>
#include <OpenBLAS/lapacke.h>

void blas_add_in_c(std::vector<float>& c, std::vector<float>& a, int rows, int cols, float alpha, float beta) {
    cblas_sgeadd(CblasRowMajor, rows, cols, alpha, a.data(), cols, beta, c.data(), cols);
}

void blas_mmv(std::vector<float>& mat, std::vector<float>& v, std::vector<float>& res, int m, int n, const float beta, const float res_add) {
    cblas_sgemv(CblasRowMajor, CblasNoTrans, m, n, 1.0f, mat.data(), n, v.data(), 1, res_add, res.data(), 1);
}

void add_vec(std::vector<float>& a, const std::vector<float>& b, const float beta) {
    for (int i = 0; i < a.size(); ++i) {
        a[i] += beta * b[i];
    }
}

void oppos_vec(std::vector<float>& a) {
    for (int i = 0; i < a.size(); ++i) {
        a[i] = -a[i];
    }
}

float vec_norm(const std::vector<float>& v) {
    float sum = 0.0f;
    for (auto& f : v) {
        sum += f * f;
    }
    return std::sqrt(sum);
}

void solve_system(std::vector<float>& A, std::vector<float>& b, int rows, int cols) {
    std::vector<int> ipiv(rows);
    std::vector<float> save = b;
    int info = LAPACKE_sgesv(LAPACK_ROW_MAJOR, rows, 1, A.data(), cols, ipiv.data(), b.data(), 1);

    if (info != 0) throw;
}

// mass_matrix = identity
Solver::Solver(const std::vector<float>& u0, const std::vector<float>& du0, const std::vector<float>& mass_matrix_)
    : u(u0), du(du0), mass_matrix(mass_matrix_), mass_matrix_inv(mass_matrix) {}

void ImplicitNewmarkSolver::update_constants(const float dt, float& alpha_1, float& alpha_2,
    float& alpha_3, float& alpha_4, float& alpha_5,
    float& alpha_6) const {
    alpha_1 = 1.0f / (beta_newmark * dt * dt);
    alpha_2 = 1.0f / (beta_newmark * dt);
    alpha_3 = (1.0f - 2.0f * beta_newmark) / (2.0f * beta_newmark);
    alpha_4 = gamma_newmark / (beta_newmark * dt);
    alpha_5 = 1 - gamma_newmark / beta_newmark;
    alpha_6 = (1 - gamma_newmark / (2.0f * beta_newmark)) * dt;
}

void ImplicitNewmarkSolver::compute_next_step(float dt, const std::vector<float>& external_forces) {
    int r = u.size();
    std::vector<float> new_q = u;
    // Init constants
    float alpha_1, alpha_2, alpha_3, alpha_4, alpha_5, alpha_6;
    update_constants(dt, alpha_1, alpha_2, alpha_3, alpha_4, alpha_5, alpha_6);

    // For each Newton-Raphson iteration
    for (int j = 0; j < max_newton_raphson; j++) {
        // Compute forces
        std::vector<float> reduced_internal_forces = polynomial_generator->get_force(new_q);

        std::vector<float> reduced_stiffness_mat =
            polynomial_generator->get_tangent_stiffness_mat(new_q);

        std::vector<float> local_damping_mat(r * r, 0.0f);

        // local_damping_mat = mass_coeff * mass_matrix + stiffness_coeff * reduced_stiffness_mat
        blas_add_in_c(local_damping_mat, mass_matrix, r, r, mass_coeff, 1.0f);

        blas_add_in_c(local_damping_mat, reduced_stiffness_mat, r, r, stiffness_coeff, 1.0f);

        std::vector<float> system_mat = reduced_stiffness_mat;

        // system_mat = alpha_1 * mass_matrix + alpha_4 * local_damping_mat + reduced_stiffness_mat
        blas_add_in_c(system_mat, mass_matrix, r, r, alpha_1);
        blas_add_in_c(system_mat, local_damping_mat, r, r, alpha_4);

        std::vector<float> diff = new_q;
        add_vec(diff, u, -1.0f);

        std::vector<float> v1(r, 0.0f);

        // v1 = alpha_1 * (new_q - u) - alpha_2 * du - alpha_3 * ddu
        add_vec(v1, diff, alpha_1);
        add_vec(v1, du, -alpha_2);
        add_vec(v1, ddu, -alpha_3);

        std::vector<float> v2(r, 0.0f);
        // v2 = alpha_4 * (new_q - u) + alpha_5 * du + alpha_6 * ddu
        add_vec(v2, diff, alpha_4);
        add_vec(v2, du, alpha_5);
        add_vec(v2, ddu, alpha_6);

        std::vector<float> residual(r);
        // residual = mass_matrix * v1 + local_damping_mat * v2 + reduced_internal_forces - external_forces
        blas_mmv(mass_matrix, v1, residual, r, r, 1.0f, 0.0f);
        blas_mmv(local_damping_mat, v2, residual, r, r, 1.0f, 1.0f);

        add_vec(residual, reduced_internal_forces, 1.0f);
        add_vec(residual, external_forces, -1.0f);

        // If the tolerance is reached
        const float residual_norm = vec_norm(residual);

        if (std::isnan(residual_norm)) {
            throw;
        }
        if (std::isinf(residual_norm) || residual_norm < residual_tolerance) {
            break;
        }
        // Otherwise, solve dense symmetric linear system
        //std::vector<float> delta_q = system_mat.fullPivHouseholderQr().solve(-residual);
        oppos_vec(residual);
        solve_system(system_mat, residual, r, r);

        // Update q_{i+1}
        // new_q += delta_q
        add_vec(new_q, residual, 1.0f);
    }

    std::vector<float> diff = new_q;
    add_vec(diff, u, -1.0f);

    // Update velocity
    std::vector<float> new_du(r, 0.0f);
    // new_du = alpha_4 * (new_q - u) + alpha_5 * du + alpha_6 * ddu
    add_vec(new_du, diff, alpha_4);
    add_vec(new_du, du, alpha_5);
    add_vec(new_du, ddu, alpha_6);

    // Update accelerations
    std::vector<float> new_ddu(r, 0.0f);
    // ddu = alpha_1 * (new_q - u) - alpha_2 * du - alpha_3 * ddu
    add_vec(new_ddu, diff, alpha_1);
    add_vec(new_ddu, du, -alpha_2);
    add_vec(new_ddu, ddu, -alpha_3);

    // Update for real
    u = new_q;
    du = new_du;
    ddu = new_ddu;

}