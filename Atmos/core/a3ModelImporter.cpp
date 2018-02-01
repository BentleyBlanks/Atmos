#include <core/a3ModelImporter.h>
#include <core/log/a3Log.h>
#include <core/a3Scene.h>
#include <core/a3Settings.h>

#ifdef A3_MODEL_IMPORTOR_ASSIMP
// lib assimp
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#else
// tinyobjloader
#include <tiny_obj_loader.h>
#endif

#include <shapes/a3Triangle.h>


class a3ModelImporter::a3Importer
{
public:
    a3Importer() :scene(NULL){}

    std::vector<a3Shape*> load(const char* filePath, bool convertHandness)
    {
#ifdef A3_MODEL_IMPORTOR_ASSIMP

        a3Log::debug("Parsering model file \"%s\"...\n", filePath);

        scene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_Fast |
                                       aiProcess_ConvertToLeftHanded);

        if(!scene)
        {
            a3Log::error("Can't open file: %s\n", filePath);
            return std::vector<a3Shape*>();
        }

        if(scene->mNumMeshes < 0)
        {
            a3Log::error("The model: %s don't have any primitives\n", filePath);
            return std::vector<a3Shape*>();
        }

        std::vector<a3Shape*> shapes;

        for(int m = 0; m < scene->mNumMeshes; m++)
        {
            const struct aiMesh* mesh = scene->mMeshes[m];

            a3Log::debug("Faces: %d, Vertices: %d, Normals: %d\n", mesh->mNumFaces, mesh->mNumVertices, mesh->mNumVertices * 3);
            // 仅支持三角形作为图元
            for(int i = 0; i < mesh->mNumFaces; i++)
            {
                a3Log::info("Loading...%8.2f \r", (float) i / mesh->mNumFaces * 100.0f);

                int numIndices = mesh->mFaces[i].mNumIndices;
                if(numIndices != 3)
                {
                    a3Log::error("Atmos only supports triangles. The model imported have %d vertices\n", numIndices);

                    //A3_SAFE_DELETE(shapes);

                    return std::vector<a3Shape*>();
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

                // init texture coordinate
                if(mesh->mTextureCoords[0])
                {
                    triangle->vt0.set(mesh->mTextureCoords[0][indexV0].x, mesh->mTextureCoords[0][indexV0].y, mesh->mTextureCoords[0][indexV0].z);
                    triangle->vt1.set(mesh->mTextureCoords[0][indexV1].x, mesh->mTextureCoords[0][indexV1].y, mesh->mTextureCoords[0][indexV1].z);
                    triangle->vt2.set(mesh->mTextureCoords[0][indexV2].x, mesh->mTextureCoords[0][indexV2].y, mesh->mTextureCoords[0][indexV2].z);
                }

                triangle->set(triangle->v0, triangle->v1, triangle->v2);

                shapes.push_back(triangle);
            }

            a3Log::print("\n");
            a3Log::debug("Loading model successed\n");
        }

        aiReleaseImport(scene);

        return shapes;
#else    
        a3Log::debug("Parsering model file \"%s\"...\n", filePath);

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::string err;
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filePath);

        if(!err.empty())
            a3Log::warning("ErrMsg: %s\n", err.c_str());

        if(!ret)
        {
            a3Log::error("Can't open file: %s\n", filePath);
            return std::vector<a3Shape*>();
        }

        a3Log::debug("Total vertices: %d, normals : %d\n texcorrds : %d\n",
            (int) attrib.vertices.size() / 3,
            (int) attrib.normals.size() / 3,
            (int) attrib.texcoords.size() / 2);

        std::vector<a3Shape*> triShapes;
        // Loop over shapes
        for(size_t s = 0; s < shapes.size(); s++)
        {
            int numFaceVertices = shapes[s].mesh.num_face_vertices.size();
            a3Log::debug("shape[%d] faces: %d\n", s, (int) numFaceVertices);

            // Loop over faces(polygon)
            // --!FAQ: https://github.com/syoyo/tinyobjloader
            size_t index_offset = 0;
            for(size_t f = 0; f < numFaceVertices; f++)
            {
                a3Log::info("Loading...%8.2f \r", (float) f / numFaceVertices * 100.0f);

                int fv = shapes[s].mesh.num_face_vertices[f];

                if(fv != 3)
                {
                    // 防止内存泄漏
                    if(triShapes.size() > 0)
                    {
                        for(std::vector<a3Shape*>::iterator it = triShapes.begin(); it != triShapes.end();)
                        {
                            A3_SAFE_DELETE(*it);
                            it = triShapes.erase(it);
                        }
                    }

                    a3Log::error("Atmos only supports triangles. The model imported have %d vertices\n", fv);
                    return std::vector<a3Shape*>();
                }

                // access to vertex
                a3Triangle* triangle = new a3Triangle();

                // -----------------------------indices-----------------------------
                tinyobj::index_t idx0 = shapes[s].mesh.indices[index_offset + 0];
                tinyobj::index_t idx1 = shapes[s].mesh.indices[index_offset + 1];
                tinyobj::index_t idx2 = shapes[s].mesh.indices[index_offset + 2];

                if(idx0.vertex_index == -1 || idx1.vertex_index == -1 || idx2.vertex_index == -1)
                {
                    a3Log::error("The model doesn't have any vertives\n");
                    return std::vector<a3Shape*>();
                }

                // -----------------------------vertex component-----------------------------
                float vx0 = attrib.vertices[3 * idx0.vertex_index + 0];
                float vy0 = attrib.vertices[3 * idx0.vertex_index + 1];
                float vz0 = attrib.vertices[3 * idx0.vertex_index + 2];

                float vx1 = attrib.vertices[3 * idx1.vertex_index + 0];
                float vy1 = attrib.vertices[3 * idx1.vertex_index + 1];
                float vz1 = attrib.vertices[3 * idx1.vertex_index + 2];

                float vx2 = attrib.vertices[3 * idx2.vertex_index + 0];
                float vy2 = attrib.vertices[3 * idx2.vertex_index + 1];
                float vz2 = attrib.vertices[3 * idx2.vertex_index + 2];

                // -----------------------------v0 v1 v2-----------------------------
                t3Vector3f v0, v1, v2;

                if(convertHandness)
                {
                    v0.set(vx0, vz0, vy0);
                    v1.set(vx1, vz1, vy1);
                    v2.set(vx2, vz2, vy2);
                }
                else
                {
                    v0.set(vx0, vy0, vz0);
                    v1.set(vx1, vy1, vz1);
                    v2.set(vx2, vy2, vz2);
                }

                triangle->v0 = v0;
                triangle->v1 = v1;
                triangle->v2 = v2;

                // using face normal if there are no vertex normal
                t3Vector3f normal = (v1 - v0).getCrossed(v2 - v0).getNormalized();

                // -----------------------------normal-----------------------------
                if(idx0.normal_index != -1 && idx1.normal_index != -1 && idx2.normal_index != -1)
                {
                    float nx0 = attrib.normals[3 * idx0.normal_index + 0];
                    float ny0 = attrib.normals[3 * idx0.normal_index + 1];
                    float nz0 = attrib.normals[3 * idx0.normal_index + 2];

                    float nx1 = attrib.normals[3 * idx1.normal_index + 0];
                    float ny1 = attrib.normals[3 * idx1.normal_index + 1];
                    float nz1 = attrib.normals[3 * idx1.normal_index + 2];

                    float nx2 = attrib.normals[3 * idx2.normal_index + 0];
                    float ny2 = attrib.normals[3 * idx2.normal_index + 1];
                    float nz2 = attrib.normals[3 * idx2.normal_index + 2];

                    if(convertHandness)
                    {
                        triangle->n0.set(nx0, nz0, ny0);
                        triangle->n1.set(nx1, nz1, ny1);
                        triangle->n2.set(nx2, nz2, ny2);
                    }
                    else
                    {
                        triangle->n0.set(nx0, ny0, nz0);
                        triangle->n1.set(nx1, ny1, nz1);
                        triangle->n2.set(nx2, ny2, nz2);
                    }
                }
                else
                {
                    if(convertHandness)
                    {
                        // face normal
                        triangle->n0 = -normal;
                        triangle->n1 = -normal;
                        triangle->n2 = -normal;
                    }
                    else
                    {
                        // face normal
                        triangle->n0 = normal;
                        triangle->n1 = normal;
                        triangle->n2 = normal;
                    }
                }
                
                // texture indices
                if(idx0.texcoord_index != -1 && idx1.texcoord_index != -1 && idx2.texcoord_index != -1)
                {
                    float tx0 = attrib.texcoords[2 * idx0.texcoord_index + 0];
                    float ty0 = attrib.texcoords[2 * idx0.texcoord_index + 1];

                    float tx1 = attrib.texcoords[2 * idx1.texcoord_index + 0];
                    float ty1 = attrib.texcoords[2 * idx1.texcoord_index + 1];

                    float tx2 = attrib.texcoords[2 * idx2.texcoord_index + 0];
                    float ty2 = attrib.texcoords[2 * idx2.texcoord_index + 1];

                    // init 2d texture coordinate
                    triangle->vt0.set(tx0, ty0, 0);
                    triangle->vt1.set(tx1, ty1, 0);
                    triangle->vt2.set(tx2, ty2, 0);
                }

                triangle->set(triangle->v0, triangle->v1, triangle->v2);

                triShapes.push_back(triangle);

                index_offset += fv;
            }
        }

        a3Log::print("\n");
        a3Log::debug("Loading model successed\n");

        return triShapes;
#endif
    }

    const struct aiScene* scene;
};

a3ModelImporter::a3ModelImporter()
{
    importer = new a3Importer();

    convertHandness = false;
}

a3ModelImporter::~a3ModelImporter()
{
    A3_SAFE_DELETE(importer);
}

std::vector<a3Shape*> a3ModelImporter::load(const char* filePath)
{
    return importer->load(filePath, convertHandness);
}


 