#include "model_loader.hpp"

#include <iostream>
#include <array>

namespace slate {

    static glm::vec3 assimp_col_to_glm(aiColor3D color) {
        glm::vec3 vec;
        vec.x = color.r;
        vec.y = color.g;
        vec.z = color.b;
        return vec;
    }

    ModelLoader::ModelLoader(const std::string path) {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "Error::Assimp: " << import.GetErrorString() << '\n';
            return;
        }

        for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
            aiMaterial* mat = scene->mMaterials[i];
            auto slate_mat = std::make_shared<Material>();

            aiColor3D color(0.f, 0.f, 0.f);

            // diffuse
            mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
            slate_mat->diffuse = assimp_col_to_glm(color);

            // ambient
            mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
            slate_mat->ambient = assimp_col_to_glm(color);

            // specular
            mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
            slate_mat->specular = assimp_col_to_glm(color);

            materials.push_back(slate_mat);
        }

        process_node(scene->mRootNode, scene);
    }

    void ModelLoader::process_node(aiNode* node, const aiScene* scene) {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(process_mesh(mesh, scene));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            process_node(node->mChildren[i], scene);
        }
    }

    static glm::vec3 compute_normal(const std::array<glm::vec3, 3>& triangle) {
        const glm::vec3 e0 = triangle[1] - triangle[0];
        const glm::vec3 e1 = triangle[2] - triangle[0];

        glm::vec3 normal = glm::normalize(glm::cross(e0, e1));
        return normal;
    }

    void ModelLoader::compute_normals(RawMesh& raw_mesh, const aiMesh* mesh) {
        raw_mesh.normals.resize(mesh->mNumVertices, glm::vec3(0.0f));

        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];

            // Retrieve triangle
            std::array<glm::vec3, 3> triangle;
            for (unsigned int j = 0; j < 3; ++j) {
                triangle[j] = raw_mesh.positions[face.mIndices[j]];
            }

            // Compute normal
            const glm::vec3 normal = compute_normal(triangle);

            // Add it to every vertex
            for (unsigned int j = 0; j < 3; ++j) {
                raw_mesh.normals[face.mIndices[j]] += normal;
            }
        }

        // Normalize computed normals
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            raw_mesh.normals[i] = glm::normalize(raw_mesh.normals[i]);
        }
    }

    RawMesh ModelLoader::process_mesh(aiMesh* mesh, const aiScene*) {
        RawMesh raw_mesh;
        
        // Allocate vectors
        raw_mesh.positions.resize(mesh->mNumVertices);
        raw_mesh.normals.resize(mesh->mNumVertices);
        raw_mesh.uvs.resize(mesh->mNumVertices, glm::vec2(0.0f));
        raw_mesh.indices.resize(mesh->mNumFaces * 3);

        bool has_normals = mesh->mNormals;

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            // pos
            raw_mesh.positions[i].x = mesh->mVertices[i].x;
            raw_mesh.positions[i].y = mesh->mVertices[i].y;
            raw_mesh.positions[i].z = mesh->mVertices[i].z;

            // normal
            if (has_normals) {
                raw_mesh.normals[i].x = mesh->mNormals[i].x;
                raw_mesh.normals[i].y = mesh->mNormals[i].y;
                raw_mesh.normals[i].z = mesh->mNormals[i].z;
            }

            // uv
            if (mesh->mTextureCoords[0]) {
                raw_mesh.uvs[i].x = mesh->mTextureCoords[0][i].x;
                raw_mesh.uvs[i].y = mesh->mTextureCoords[0][i].y;
            }
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            // Indices
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < 3; ++j) {
                // mNumIndices should always be 3 because of aiProcess_Triangulate
                raw_mesh.indices[3 * i + j] = (face.mIndices[j]);
            }
        }

        raw_mesh.material = materials[mesh->mMaterialIndex];

        if (!has_normals) {
            compute_normals(raw_mesh, mesh);
        }

        return raw_mesh;
    }

    const std::vector<RawMesh>& ModelLoader::get_meshes() const {
        return meshes;
    }

    const std::vector<std::shared_ptr<Material>>& ModelLoader::get_materials() const {
        return materials;
    }
}