#include "model.hpp"
#include "error.hpp"
#include "common.hpp"

#include <assimp/Importer.hpp>

glc::Model::Model(std::string path)
: mMeshes(),
  mLoadedTextures(),
  mBaseDirectory(path.substr(0, path.find_last_of("/")))
{
    Assimp::Importer import;
    auto scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (! scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || ! scene->mRootNode)
    {
        throw glc::MalformedModel(path, import.GetErrorString());
    }

    this->processNode(scene, scene->mRootNode);
}

void glc::Model::draw(glc::Shader shader)
{
    // for (auto &m : mMeshes)
    // {

    // }
}

void glc::Model::processNode(const aiScene* scene, const aiNode* node)
{
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        this->processMesh(scene, mesh);
    }

    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(scene, node->mChildren[i]);
    }
}

void glc::Model::processMesh(const aiScene* scene, const aiMesh* mesh)
{
    std::vector<glc::vtx> vertices;
    std::vector<glc::tex> textures;
    std::vector<GLuint> indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        glc::vtx v;

        v.pos = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z);

        v.norm = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z);

        if (mesh->mTextureCoords[0])
        {
            v.uv = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y);
        }
        else
        {
            v.uv = glm::vec2(0.0f, 0.0f);
        }

        vertices.emplace_back(v);
    }

    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        auto face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++)
        {
            indices.emplace_back(face.mIndices[j]);
        }
    }

    auto mat = scene->mMaterials[mesh->mMaterialIndex];
    auto diffMaps = getTex(mat, aiTextureType_DIFFUSE);
    auto specMaps = getTex(mat, aiTextureType_SPECULAR);
    textures.insert(textures.end(), diffMaps.begin(), diffMaps.end());
    textures.insert(textures.end(), specMaps.begin(), specMaps.end());

    mMeshes.emplace_back(vertices, textures, indices);
}

std::vector<glc::tex> glc::Model::getTex(const aiMaterial* mat, const aiTextureType type)
{
    std::vector<glc::tex> textures;

    for (size_t i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        if (mLoadedTextures.count(str.C_Str()))
        {
            textures.emplace_back(mLoadedTextures[str.C_Str()]);
        }
        else
        {
            glc::tex tex;
            tex.id = glc::makeTexture(mBaseDirectory + "/" + str.C_Str());
            switch (type)
            {
            case aiTextureType_SPECULAR:
                tex.type = glc::textype::SPECULAR;
                break;
            case aiTextureType_DIFFUSE:
                tex.type = glc::textype::DIFFUSE;
                break;
            default:
                // Sooooon...
                break;
            }
            mLoadedTextures.emplace(str.C_Str(), tex);
            textures.emplace_back(tex);
        }
    }

    return textures;
}
