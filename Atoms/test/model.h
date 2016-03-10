#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <core/log/a3Log.h>

const struct aiScene* scene = NULL;

int main()
{
    scene = aiImportFile("./cbox_smallbox.obj", aiProcessPreset_TargetRealtime_MaxQuality);

    const struct aiMesh* mesh = scene->mMeshes[0];

    aiVector3D* vertices = mesh->mVertices;

    for(int i = 0; i < mesh->mNumVertices; i++)
    {
        a3Log::print("v[%d](%f, %f, %f)\n", i, mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
    }

    a3Log::print("\n");

    for(int i = 0; i < mesh->mNumVertices; i++)
    {
        a3Log::print("n[%d](%f, %f, %f)\n", i, mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
    }

    a3Log::print("\n");

    for(int i = 0; i < mesh->mNumFaces; i++)
    {
        a3Log::print("f[%d]( ", i);
        for(int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
        {
            a3Log::print("%d ", mesh->mFaces[i].mIndices[j]);
        }
        a3Log::print(")\n");
    }

    return 0;
}