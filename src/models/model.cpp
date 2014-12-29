#include "model.hpp"
#include "error.hpp"
#include "shader.hpp"

#include <FreeImagePlus.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace {
    GLuint makeTexture(std::string path);
}

glc::Mesh::Mesh(
    const std::vector<glc::Vex>& vertices,
    const std::vector<glc::Tex>& textures,
    const std::vector<GLuint>& indices)
: mVertices(vertices),
  mTextures(textures),
  mIndices(indices)
{
    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mVbo);
    glGenBuffers(1, &mEbo);

    glBindVertexArray(mVao);

    auto vbytesize = mVertices.size() * sizeof(glc::Vex);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, vbytesize, mVertices.data(), GL_STATIC_DRAW);

    auto ibytesize = mIndices.size() * sizeof(GLuint);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibytesize, mIndices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glc::Vex),
        (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glc::Vex),
        (GLvoid*)offsetof(glc::Vex, norm));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glc::Vex),
        (GLvoid*)offsetof(glc::Vex, uv));

    glBindVertexArray(0);
}

void glc::Mesh::draw(glc::Shader* shader)
{
    shader->use();

    GLuint diff = 1;
    GLuint spec = 1;
    for (size_t i = 0; i < mTextures.size(); i++)
    {
        std::string symbol = "Material.";

        switch (mTextures[i].type)
        {
        case glc::TexType::DIFF:
            symbol += "texture_diffuse" + std::to_string(diff);
            diff += 1;
            break;
        case glc::TexType::SPEC:
            symbol += "texture_specular" + std::to_string(spec);
            spec += 1;
            break;
        }

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
        shader->setUniform(symbol, static_cast<GLint>(i));
    }

    glBindVertexArray(mVao);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    for (size_t i = 0; i < mTextures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}


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

void glc::Model::draw(glc::Shader* shader)
{
    for (auto &m : mMeshes)
    {
        m.draw(shader);
    }
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
    std::vector<glc::Vex> vertices;
    std::vector<glc::Tex> textures;
    std::vector<GLuint> indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        glc::Vex v;

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

std::vector<glc::Tex> glc::Model::getTex(const aiMaterial* mat, const aiTextureType type)
{
    std::vector<glc::Tex> textures;

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
            auto tex = glc::Tex();
            tex.id = ::makeTexture(mBaseDirectory + "/" + str.C_Str());
            switch (type)
            {
            case aiTextureType_SPECULAR:
                tex.type = glc::TexType::SPEC;
                break;
            case aiTextureType_DIFFUSE:
                tex.type = glc::TexType::DIFF;
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

namespace {
    GLuint makeTexture(std::string path)
    {
        GLuint id;
        glGenTextures(1, &id);

        fipImage image;
        image.load(path.c_str());
        image.convertTo32Bits();
        auto data = image.accessPixels();
        auto w = image.getWidth();
        auto h = image.getHeight();

        auto minSetting = GL_LINEAR_MIPMAP_LINEAR;
        auto magSetting = GL_LINEAR;

        glBindTexture(GL_TEXTURE_2D,id);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_BGRA,GL_UNSIGNED_BYTE,data);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minSetting);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magSetting);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,0);
        image.clear();

        return id;
    }
}
