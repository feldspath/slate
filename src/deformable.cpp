#include "deformable.hpp"

#include <slate/utils/model_loader/model_loader.hpp>
#include <slate/scene/object/slate_object.hpp>
#include <slate/utils/performance/performance.hpp>
#include <slate/helper_dirs.hpp>

#include <fstream>

float random_float() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

Deformable::Deformable(slate::MeshPtr mesh, slate::ShaderPtr shader) : shader(shader), mesh(mesh) {
    // VOXELIZATION
    import_voxelization(std::string(ROOT_DIR) + "resources/voxelizations/cactus_highres_vox.txt");
    vertex_mapping = std::make_shared <slate::SSBO>(sizeof(float) * voxel_vertices_indices.size(), GL_MAP_READ_BIT, voxel_vertices_indices.data());

    // 3D texture of displacements
    glGenTextures(1, &vertex_offsets_tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, vertex_offsets_tex);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, width, height, depth, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, vertex_offsets_tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    // SIMULATION
    import_simulation(std::string(ROOT_DIR) + "resources/voxelizations/cactus_highres_sim.txt");
    reduction_basis_ssbo = std::make_shared<slate::SSBO>(sizeof(float) * reduction_basis.size(), GL_MAP_READ_BIT, reduction_basis.data());

    reduced_vector = std::vector<float>(r, 0.0f);
    reduced_vector_ssbo = std::make_shared<slate::SSBO>(sizeof(float) * r, GL_DYNAMIC_STORAGE_BIT, reduced_vector.data());
    external_forces = std::vector<float>(3 * n_vertices, 0.0f);

    std::vector<float> mass_matrix(r*r, 0.0f);
    for (int i = 0; i < r; ++i) {
        mass_matrix[r * i + i] = 1.0f;
    }
    double damping_mass = 0.0f;
    double damping_stiffness = 1e-2;
    const int max_newton_raphson = 3;
    double residual_tolerance = 1e-5;
    const double beta_newmark = 0.25;
    const double gamma_newmark = 0.5;

    std::vector<float> du0(r, 0.0f);


    solver = std::make_shared<ImplicitNewmarkSolver>(reduced_vector, du0, mass_matrix, polynomial_generator, max_newton_raphson, residual_tolerance, damping_mass, damping_stiffness, beta_newmark, gamma_newmark);

    // Shader
    cs = std::make_shared<slate::ComputeShader>(std::string(ROOT_DIR) + "resources/compute_shaders/deform.glsl");
}

Deformable::~Deformable() {
    glDeleteTextures(1, &vertex_offsets_tex);
}

void Deformable::add_external_force(int vertex_index, glm::vec3 force) {
    if (!active) return;
    glm::vec3 local_force = glm::toMat4(glm::inverse(get_target()->transform.rotation)) * glm::vec4(force, 0.0f);

    external_forces[3*vertex_index+0] = local_force.x;
    external_forces[3*vertex_index+1] = local_force.y;
    external_forces[3*vertex_index+2] = local_force.z;
}

void Deformable::update(std::shared_ptr<slate::Scene>, const float dt) {
    if (!active) return;
    slate::Benchmark bench("Deform update");

    std::vector<float> reduced_external_forces(r);
    {
        slate::Benchmark bench("External Forces");

        // Compute external forces in the reduction basis
        blas_mmv(reduction_basis, external_forces, reduced_external_forces, r, 3 * n_vertices);

        // Reset forces
        std::fill(external_forces.begin(), external_forces.end(), 0.0f);
    }

    // Compute next step
    {
        slate::Benchmark bench("Solver");
        solver->compute_next_step(dt, reduced_external_forces);
    }
    

    {
        slate::Benchmark bench("Pass reduced vector");

        glBindImageTexture(0, vertex_offsets_tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
        // Pass the new reduced vector to the GPU
        reduced_vector = solver->get_u();
        reduced_vector_ssbo->update_buffer(0, reduced_vector);
    }

    {
        slate::Benchmark bench("Compute Shader");

        // Update the displacements in the compute shader
        cs->use();
        cs->bind_ssbo(vertex_mapping->get_id(), 0);
        cs->bind_ssbo(reduction_basis_ssbo->get_id(), 1);
        cs->bind_ssbo(reduced_vector_ssbo->get_id(), 2);
        cs->set_uniform("width", width);
        cs->set_uniform("height", height);
        cs->set_uniform("depth", depth);
        cs->set_uniform("r", r);
        cs->dispatch(width, height, depth);
    }
}

void Deformable::render() {
    if (!active) return;
    auto obj = get_target();
    if (!obj)  return;

    shader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, vertex_offsets_tex);

    // Model Matrix
    const glm::mat4 model_matrix = obj->transform.frame_matrix();
    shader->set_uniform("model_matrix", model_matrix);

    // Voxelization
    shader->set_uniform("width", width);
    shader->set_uniform("height", height);
    shader->set_uniform("depth", depth);
    shader->set_uniform("origin", origin);
    shader->set_uniform("voxel_size", voxel_size);

    mesh->draw(shader);
}

void Deformable::import_voxelization(const std::string& path) {
    std::fstream file;
    file.open(path, std::ios_base::in);

    file >> origin.x >> origin.y >> origin.z;
    file >> voxel_size;
    file >> width >> height >> depth >> n_vertices;
    voxel_vertices_indices.resize(width * height * depth, -1);

    for (int i = 0; i < n_vertices; ++i) {
        int x, y, z;
        file >> x >> y >> z;
        file >> voxel_vertices_indices[z * width * height + y * width + x];
    }

    file.close();
}

void Deformable::import_simulation(const std::string& path) {
    std::fstream file;
    file.open(path, std::ios_base::in);

    // Reduction Basis
    int rows, cols;
    file >> rows >> cols;
    reduction_basis.resize(rows * cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file >> reduction_basis[i*cols + j];
        }
    }

    // Polynomials
    file >> r;
    std::vector<Polynomial> polynomials(r, Polynomial(r));
    for (auto& polynomial : polynomials) {
        // linear terms
        for (int i = 0; i < r; i++) {
            file >> polynomial.get_linear(i);
        }

        // quadratic terms
        for (int i = 0; i < r; i++) {
            for (int j = i; j < r; j++) {
                file >> polynomial.get_quadratic(i, j);
            }
        }

        // cubic terms
        for (int i = 0; i < r; i++) {
            for (int j = i; j < r; j++)
                for (int k = j; k < r; k++) {
                    file >> polynomial.get_cubic(i, j, k);
                }
        }
    }

    file.close();

    polynomial_generator = std::make_shared<PolynomialGenerator>(polynomials);
}