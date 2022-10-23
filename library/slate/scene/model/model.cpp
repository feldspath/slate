#include "model.hpp"

#include <iostream>
#include <memory>

namespace slate {
    Model::Model(const std::string path) {
        load_model(path);
    }

    static glm::vec3 assimp_col_to_glm(aiColor3D color) {
        glm::vec3 vec;
        vec.x = color.r;
        vec.y = color.g;
        vec.z = color.b;
        return vec;
    }

    void Model::load_model(const std::string path) {
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

    void Model::process_node(aiNode* node, const aiScene* scene) {
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

    MeshPtr Model::process_mesh(aiMesh* mesh, const aiScene*) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            Vertex vertex;

            // pos
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;

            // normal
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;

            // uv
            if (!mesh->mTextureCoords[0]) {
                vertex.uv = glm::vec2(0.0f);
                continue;
            }
            vertex.uv.x = mesh->mTextureCoords[0][i].x;
            vertex.uv.y = mesh->mTextureCoords[0][i].y;

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                // mNumIndices should always be 3 because of aiProcess_Triangulate
                indices.push_back(face.mIndices[j]);
            }
        }

        return std::make_shared<Mesh>(vertices, indices, materials[mesh->mMaterialIndex]);
    }

    void Model::draw(const ShaderPtr shader) const {
        for (const auto& m : meshes) {
            m->draw(shader);
        }
    }
}