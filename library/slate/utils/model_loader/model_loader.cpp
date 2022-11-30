#include "model_loader.hpp"

#include <iostream>

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

    RawMesh ModelLoader::process_mesh(aiMesh* mesh, const aiScene*) {
        RawMesh raw_mesh;

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            // pos
            glm::vec3 position;
            position.x = mesh->mVertices[i].x;
            position.y = mesh->mVertices[i].y;
            position.z = mesh->mVertices[i].z;
            raw_mesh.positions.push_back(position);

            // normal
            glm::vec3 normal;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
            raw_mesh.normals.push_back(normal);

            // uv
            glm::vec2 uv(0.0f, 0.0f);
            if (mesh->mTextureCoords[0]) {
                uv.x = mesh->mTextureCoords[0][i].x;
                uv.y = mesh->mTextureCoords[0][i].y;
            }
            raw_mesh.uvs.push_back(uv);

        }

        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                // mNumIndices should always be 3 because of aiProcess_Triangulate
                raw_mesh.indices.push_back(face.mIndices[j]);
            }
        }
        raw_mesh.material = materials[mesh->mMaterialIndex];
        return raw_mesh;
    }

    const std::vector<RawMesh>& ModelLoader::get_meshes() const {
        return meshes;
    }

    const std::vector<std::shared_ptr<Material>>& ModelLoader::get_materials() const {
        return materials;
    }
}