#include <core/a3ModelImporter.h>

// lib assimp
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <core/log/a3Log.h>
#include <core/a3Scene.h>
#include <shapes/a3Triangle.h>

class a3ModelImporter::a3Importer
{
public:
    a3Importer() :scene(NULL){}

    std::vector<a3Shape*>* load(const char* filePath)
    {
        a3Log::debug("Parsering model file...\n");

        scene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);

        if(!scene)
        {
            a3Log::error("无法打开文件%s\n", filePath);
            return NULL;
        }

        if(scene->mNumMeshes < 0)
            return NULL;

        const struct aiMesh* mesh = scene->mMeshes[0];

        std::vector<a3Shape*>* shapes = new std::vector<a3Shape*>();

        a3Log::debug("faces: %d, vertices: %d, normals: %d\n", mesh->mNumFaces, mesh->mNumVertices, mesh->mNumVertices * 3);
        // 仅支持三角形作为图元
        for(int i = 0; i < mesh->mNumFaces; i++)
        {
            a3Log::info("Loading...%8.2f \r", (float)i / mesh->mNumFaces * 100.0f);

            int numIndices = mesh->mFaces[i].mNumIndices;
            if(numIndices != 3)
            {
                a3Log::error("Atoms仅支持三角形作为片元, 而当前模型面由%d个顶点组成\n", numIndices);

                delete shapes;
                shapes = NULL;
                return NULL;
            }

            a3Triangle* triangle = new a3Triangle();

            int indexV0 = mesh->mFaces[i].mIndices[0], indexV1 = mesh->mFaces[i].mIndices[1], indexV2 = mesh->mFaces[i].mIndices[2];

            // init vertices
            triangle->v0.set(mesh->mVertices[indexV0].x, mesh->mVertices[indexV0].y, mesh->mVertices[indexV0].z);
            triangle->v1.set(mesh->mVertices[indexV1].x, mesh->mVertices[indexV1].y, mesh->mVertices[indexV1].z);
            triangle->v2.set(mesh->mVertices[indexV2].x, mesh->mVertices[indexV2].y, mesh->mVertices[indexV2].z);

            // init normals
            triangle->n0.set(mesh->mNormals[indexV0].x, mesh->mNormals[indexV0].y, mesh->mNormals[indexV0].z);
            triangle->n1.set(mesh->mNormals[indexV1].x, mesh->mNormals[indexV1].y, mesh->mNormals[indexV1].z);
            triangle->n2.set(mesh->mNormals[indexV2].x, mesh->mNormals[indexV2].y, mesh->mNormals[indexV2].z);

            //sceneController->addShape(triangle);
            shapes->push_back(triangle);
        }

        a3Log::print("\n");
        a3Log::debug("Loading model successed\n");

        return shapes;
    }

    const struct aiScene* scene;
};

a3ModelImporter::a3ModelImporter()
{
    importer = new a3Importer();
}

std::vector<a3Shape*>* a3ModelImporter::load(const char* filePath)
{
    return importer->load(filePath);
}

