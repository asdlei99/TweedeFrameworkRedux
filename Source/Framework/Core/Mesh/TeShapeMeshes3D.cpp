#include "Mesh/TeShapeMeshes3D.h"
#include "Math/TeRect2.h"
#include "Mesh/TeMesh.h"
#include "Math/TeVector2.h"
#include "Math/TeQuaternion.h"
#include "Mesh/TeMeshUtility.h"
#include "Mesh/TeMeshData.h"
#include "RenderAPI/TeVertexDataDesc.h"

namespace te
{
    inline UINT8* WriteVector3(UINT8* buffer, UINT32 stride, const Vector3& value)
    {
        *(Vector3*)buffer = value;
        return buffer + stride;
    }

    inline UINT8* WriteVector2(UINT8* buffer, UINT32 stride, const Vector2& value)
    {
        *(Vector2*)buffer = value;
        return buffer + stride;
    }

    void ShapeMeshes3D::WireAABox(const AABox& box, const SPtr<MeshData>& meshData, UINT32 vertexOffset, UINT32 indexOffset)
    {
        UINT32* indexData = meshData->GetIndices32();
        UINT8* positionData = meshData->GetElementData(VES_POSITION);

        assert((vertexOffset + 8) <= meshData->GetNumVertices());
        assert((indexOffset + 24) <= meshData->GetNumIndices());

        WireAABox(box, positionData, vertexOffset, meshData->GetVertexDesc()->GetVertexStride(), indexData, indexOffset);
    }

    void ShapeMeshes3D::SolidAABox(const AABox& box, const SPtr<MeshData>& meshData, UINT32 vertexOffset, UINT32 indexOffset)
    {
        const SPtr<VertexDataDesc>& desc = meshData->GetVertexDesc();

        UINT32* indexData = meshData->GetIndices32();
        UINT8* positionData = meshData->GetElementData(VES_POSITION);
        UINT8* normalData = meshData->GetElementData(VES_NORMAL);

        UINT32 numVertices = meshData->GetNumVertices();
        UINT32 numIndices = meshData->GetNumIndices();
        UINT32 vertexStride = desc->GetVertexStride();

        assert((vertexOffset + 24) <= meshData->GetNumVertices());
        assert((indexOffset + 36) <= meshData->GetNumIndices());

        UINT8* uvData = nullptr;
        if (desc->HasElement(VES_TEXCOORD))
            uvData = meshData->GetElementData(VES_TEXCOORD);

        SolidAABox(box, positionData, normalData, uvData, vertexOffset, vertexStride, indexData, indexOffset);

        if (uvData != nullptr && desc->HasElement(VES_TANGENT))
        {
            UINT8* tangentData = meshData->GetElementData(VES_TANGENT);
            GenerateTangents(positionData, normalData, uvData, indexData, numVertices, numIndices,
                vertexOffset, indexOffset, vertexStride, tangentData);
        }
    }

    void ShapeMeshes3D::WireSphere(const Sphere& sphere, const SPtr<MeshData>& meshData, UINT32 vertexOffset, UINT32 indexOffset, UINT32 quality)
    {
        UINT32 requiredNumVertices, requiredNumIndices;
        GetNumElementsWireSphere(quality, requiredNumVertices, requiredNumIndices);

        assert((vertexOffset + requiredNumVertices) <= meshData->GetNumVertices());
        assert((indexOffset + requiredNumIndices) <= meshData->GetNumIndices());

        UINT32 verticesPerArc = (quality + 1) * 5;
        UINT32 indicesPerArc = (verticesPerArc - 1) * 2;

        WireDisc(sphere.GetCenter(), sphere.GetRadius(), Vector3::UNIT_X, meshData,
            vertexOffset, indexOffset, quality);

        WireDisc(sphere.GetCenter(), sphere.GetRadius(), Vector3::UNIT_Y, meshData,
            vertexOffset + verticesPerArc, indexOffset + indicesPerArc, quality);

        WireDisc(sphere.GetCenter(), sphere.GetRadius(), Vector3::UNIT_Z, meshData,
            vertexOffset + verticesPerArc * 2, indexOffset + indicesPerArc * 2, quality);
    }

    void ShapeMeshes3D::SolidSphere(const Sphere& sphere, const SPtr<MeshData>& meshData, UINT32 vertexOffset,
        UINT32 indexOffset, UINT32 quality)
    {
        const SPtr<VertexDataDesc>& desc = meshData->GetVertexDesc();

        UINT32* indexData = meshData->GetIndices32();
        UINT8* positionData = meshData->GetElementData(VES_POSITION);
        UINT8* normalData = meshData->GetElementData(VES_NORMAL);

        UINT32 numVertices = meshData->GetNumVertices();
        UINT32 numIndices = meshData->GetNumIndices();
        UINT32 vertexStride = desc->GetVertexStride();

        UINT32 requiredNumVertices, requiredNumIndices;
        GetNumElementsSphere(quality, requiredNumVertices, requiredNumIndices);

        assert((vertexOffset + requiredNumVertices) <= numVertices);
        assert((indexOffset + requiredNumIndices) <= numIndices);

        UINT8* uvData = nullptr;
        if (desc->HasElement(VES_TEXCOORD))
            uvData = meshData->GetElementData(VES_TEXCOORD);

        SolidSphere(sphere, positionData, normalData, uvData, vertexOffset, vertexStride, indexData, indexOffset, quality);

        if (uvData != nullptr && desc->HasElement(VES_TANGENT))
        {
            UINT8* tangentData = meshData->GetElementData(VES_TANGENT);
            GenerateTangents(positionData, normalData, uvData, indexData, numVertices, numIndices,
                vertexOffset, indexOffset, vertexStride, tangentData);
        }
    }

    void ShapeMeshes3D::WireDisc(const Vector3& center, float radius, const Vector3& normal, const SPtr<MeshData>& meshData,
        UINT32 vertexOffset, UINT32 indexOffset, UINT32 quality)
    {
        WireArc(center, radius, normal, Degree(0), Degree(360), meshData, vertexOffset, indexOffset, quality);
    }

    void ShapeMeshes3D::SolidDisc(const Vector3& center, float radius, const Vector3& normal, const SPtr<MeshData>& meshData,
        UINT32 vertexOffset, UINT32 indexOffset, UINT32 quality)
    {
        SolidArc(center, radius, normal, Degree(0), Degree(360), meshData, vertexOffset, indexOffset, quality);
    }

    void ShapeMeshes3D::WireArc(const Vector3& center, float radius, const Vector3& normal, Degree startAngle, Degree amountAngle,
        const SPtr<MeshData>& meshData, UINT32 vertexOffset, UINT32 indexOffset, UINT32 quality)
    {
        UINT32* indexData = meshData->GetIndices32();
        UINT8* positionData = meshData->GetElementData(VES_POSITION);

        UINT32 requiredNumVertices, requiredNumIndices;
        GetNumElementsWireArc(quality, requiredNumVertices, requiredNumIndices);

        assert((vertexOffset + requiredNumVertices) <= meshData->GetNumVertices());
        assert((indexOffset + requiredNumIndices) <= meshData->GetNumIndices());

        WireArc(center, radius, normal, startAngle, amountAngle, positionData, vertexOffset,
            meshData->GetVertexDesc()->GetVertexStride(), indexData, indexOffset, quality);
    }

    void ShapeMeshes3D::SolidArc(const Vector3& center, float radius, const Vector3& normal, Degree startAngle, Degree amountAngle,
        const SPtr<MeshData>& meshData, UINT32 vertexOffset, UINT32 indexOffset, UINT32 quality)
    {
        const SPtr<VertexDataDesc>& desc = meshData->GetVertexDesc();

        UINT32* indexData = meshData->GetIndices32();
        UINT8* positionData = meshData->GetElementData(VES_POSITION);
        UINT8* normalData = meshData->GetElementData(VES_NORMAL);

        UINT32 numVertices = meshData->GetNumVertices();
        UINT32 numIndices = meshData->GetNumIndices();
        UINT32 vertexStride = desc->GetVertexStride();

        UINT32 requiredNumVertices, requiredNumIndices;
        GetNumElementsArc(quality, requiredNumVertices, requiredNumIndices);

        assert((vertexOffset + requiredNumVertices) <= meshData->GetNumVertices());
        assert((indexOffset + requiredNumIndices) <= meshData->GetNumIndices());

        UINT8* uvData = nullptr;
        if (desc->HasElement(VES_TEXCOORD))
            uvData = meshData->GetElementData(VES_TEXCOORD);

        SolidArc(center, radius, normal, startAngle, amountAngle, positionData, normalData, uvData, vertexOffset,
            vertexStride, indexData, indexOffset, quality);

        if (uvData != nullptr && desc->HasElement(VES_TANGENT))
        {
            UINT8* tangentData = meshData->GetElementData(VES_TANGENT);
            GenerateTangents(positionData, normalData, uvData, indexData, numVertices, numIndices,
                vertexOffset, indexOffset, vertexStride, tangentData);
        }
    }

    void ShapeMeshes3D::SolidCone(const Vector3& base, const Vector3& normal, float height, float radius, Vector2 scale,
        const SPtr<MeshData>& meshData, UINT32 vertexOffset, UINT32 indexOffset, UINT32 quality)
    {
        const SPtr<VertexDataDesc>& desc = meshData->GetVertexDesc();

        UINT32* indexData = meshData->GetIndices32();
        UINT8* positionData = meshData->GetElementData(VES_POSITION);
        UINT8* normalData = meshData->GetElementData(VES_NORMAL);

        UINT32 numVertices = meshData->GetNumVertices();
        UINT32 numIndices = meshData->GetNumIndices();
        UINT32 vertexStride = desc->GetVertexStride();

        UINT32 requiredNumVertices, requiredNumIndices;
        GetNumElementsCone(quality, requiredNumVertices, requiredNumIndices);

        assert((vertexOffset + requiredNumVertices) <= meshData->GetNumVertices());
        assert((indexOffset + requiredNumIndices) <= meshData->GetNumIndices());

        UINT8* uvData = nullptr;
        if (desc->HasElement(VES_TEXCOORD))
            uvData = meshData->GetElementData(VES_TEXCOORD);

        SolidCone(base, normal, height, radius, scale, positionData, normalData, uvData, vertexOffset,
            vertexStride, indexData, indexOffset, quality);

        if (uvData != nullptr && desc->HasElement(VES_TANGENT))
        {
            UINT8* tangentData = meshData->GetElementData(VES_TANGENT);
            GenerateTangents(positionData, normalData, uvData, indexData, numVertices, numIndices,
                vertexOffset, indexOffset, vertexStride, tangentData);
        }
    }

    void ShapeMeshes3D::WireCone(const Vector3& base, const Vector3& normal, float height, float radius, Vector2 scale,
        const SPtr<MeshData>& meshData, UINT32 vertexOffset, UINT32 indexOffset, UINT32 quality)
    {
        UINT32* indexData = meshData->GetIndices32();
        UINT8* positionData = meshData->GetElementData(VES_POSITION);

        UINT32 requiredNumVertices, requiredNumIndices;
        GetNumElementsWireCone(quality, requiredNumVertices, requiredNumIndices);

        assert((vertexOffset + requiredNumVertices) <= meshData->GetNumVertices());
        assert((indexOffset + requiredNumIndices) <= meshData->GetNumIndices());

        WireCone(base, normal, height, radius, scale, positionData, vertexOffset,
            meshData->GetVertexDesc()->GetVertexStride(), indexData, indexOffset, quality);
    }

    void ShapeMeshes3D::SolidCylinder(const Vector3& base, const Vector3& normal, float height, float radius, Vector2 scale,
        const SPtr<MeshData>& meshData, UINT32 vertexOffset, UINT32 indexOffset, UINT32 quality)
    {
        const SPtr<VertexDataDesc>& desc = meshData->GetVertexDesc();

        UINT32* indexData = meshData->GetIndices32();
        UINT8* positionData = meshData->GetElementData(VES_POSITION);
        UINT8* normalData = meshData->GetElementData(VES_NORMAL);

        UINT32 numVertices = meshData->GetNumVertices();
        UINT32 numIndices = meshData->GetNumIndices();
        UINT32 vertexStride = desc->GetVertexStride();

        UINT32 requiredNumVertices, requiredNumIndices;
        GetNumElementsCylinder(quality, requiredNumVertices, requiredNumIndices);

        assert((vertexOffset + requiredNumVertices) <= meshData->GetNumVertices());
        assert((indexOffset + requiredNumIndices) <= meshData->GetNumIndices());

        UINT8* uvData = nullptr;
        if (desc->HasElement(VES_TEXCOORD))
            uvData = meshData->GetElementData(VES_TEXCOORD);

        SolidCylinder(base, normal, height, radius, scale, positionData, normalData, uvData, vertexOffset,
            vertexStride, indexData, indexOffset, quality);

        if (uvData != nullptr && desc->HasElement(VES_TANGENT))
        {
            UINT8* tangentData = meshData->GetElementData(VES_TANGENT);
            GenerateTangents(positionData, normalData, uvData, indexData, numVertices, numIndices,
                vertexOffset, indexOffset, vertexStride, tangentData);
        }
    }

    void ShapeMeshes3D::WireCylinder(const Vector3& base, const Vector3& normal, float height, float radius, Vector2 scale,
        const SPtr<MeshData>& meshData, UINT32 vertexOffset, UINT32 indexOffset, UINT32 quality)
    {
        UINT32* indexData = meshData->GetIndices32();
        UINT8* positionData = meshData->GetElementData(VES_POSITION);

        UINT32 requiredNumVertices, requiredNumIndices;
        GetNumElementsWireCylinder(quality, requiredNumVertices, requiredNumIndices);

        assert((vertexOffset + requiredNumVertices) <= meshData->GetNumVertices());
        assert((indexOffset + requiredNumIndices) <= meshData->GetNumIndices());

        WireCylinder(base, normal, height, radius, scale, positionData, vertexOffset,
            meshData->GetVertexDesc()->GetVertexStride(), indexData, indexOffset, quality);
    }

    void ShapeMeshes3D::SolidQuad(const Rect3& area, const SPtr<MeshData>& meshData, UINT32 vertexOffset, UINT32 indexOffset)
    {
        const SPtr<VertexDataDesc>& desc = meshData->GetVertexDesc();

        UINT32* indexData = meshData->GetIndices32();
        UINT8* positionData = meshData->GetElementData(VES_POSITION);
        UINT8* normalData = meshData->GetElementData(VES_NORMAL);

        UINT32 numVertices = meshData->GetNumVertices();
        UINT32 numIndices = meshData->GetNumIndices();
        UINT32 vertexStride = desc->GetVertexStride();

        assert((vertexOffset + 8) <= numVertices);
        assert((indexOffset + 12) <= numIndices);

        UINT8* uvData = nullptr;
        if (desc->HasElement(VES_TEXCOORD))
            uvData = meshData->GetElementData(VES_TEXCOORD);

        SolidQuad(area, positionData, normalData, uvData, vertexOffset,
            meshData->GetVertexDesc()->GetVertexStride(), indexData, indexOffset);

        if (uvData != nullptr && desc->HasElement(VES_TANGENT))
        {
            UINT8* tangentData = meshData->GetElementData(VES_TANGENT);
            GenerateTangents(positionData, normalData, uvData, indexData, numVertices, numIndices,
                vertexOffset, indexOffset, vertexStride, tangentData);
        }
    }

    void ShapeMeshes3D::GetNumElementsAABox(UINT32& numVertices, UINT32& numIndices)
    {
        numVertices = 24;
        numIndices = 36;
    }

    void ShapeMeshes3D::GetNumElementsSphere(UINT32 quality, UINT32& numVertices, UINT32& numIndices)
    {
        numVertices = 20 * (3 * ((UINT32)std::pow(4, quality)));
        numIndices = numVertices;

        // Extra for the seam fix. 4 initial triangles each split 4 times per quality level, one vertex each
        numVertices += (3 * (UINT32)pow(4, quality));
    }

    void ShapeMeshes3D::GetNumElementsWireSphere(UINT32 quality, UINT32& numVertices, UINT32& numIndices)
    {
        GetNumElementsWireArc(quality, numVertices, numIndices);
        numVertices *= 3;
        numIndices *= 3;
    }

    void ShapeMeshes3D::GetNumElementsArc(UINT32 quality, UINT32& numVertices, UINT32& numIndices)
    {
        numVertices = ((quality + 1) * 5 + 1) * 2;
        numIndices = ((quality + 1) * 5) * 6;
    }

    void ShapeMeshes3D::GetNumElementsWireArc(UINT32 quality, UINT32& numVertices, UINT32& numIndices)
    {
        numVertices = (quality + 1) * 5;
        numIndices = ((quality + 1) * 5 - 1) * 2;
    }

    void ShapeMeshes3D::GetNumElementsDisc(UINT32 quality, UINT32& numVertices, UINT32& numIndices)
    {
        GetNumElementsArc(quality, numVertices, numIndices);
    }

    void ShapeMeshes3D::GetNumElementsWireDisc(UINT32 quality, UINT32& numVertices, UINT32& numIndices)
    {
        GetNumElementsWireArc(quality, numVertices, numIndices);
    }

    void ShapeMeshes3D::GetNumElementsCone(UINT32 quality, UINT32& numVertices, UINT32& numIndices)
    {
        numVertices = ((quality + 1) * 4) * 3 + 1;
        numIndices = ((quality + 1) * 4) * 6;
    }

    void ShapeMeshes3D::GetNumElementsWireCone(UINT32 quality, UINT32& numVertices, UINT32& numIndices)
    {
        numVertices = (quality + 1) * 4 + 5;
        numIndices = ((quality + 1) * 4 + 4) * 2;
    }

    void ShapeMeshes3D::GetNumElementsCylinder(UINT32 quality, UINT32& numVertices, UINT32& numIndices)
    {
        numVertices = ((quality + 1) * 4 + 1) * 4;
        numIndices = ((quality + 1) * 4) * 12;
    }

    void ShapeMeshes3D::GetNumElementsWireCylinder(UINT32 quality, UINT32& numVertices, UINT32& numIndices)
    {
        numVertices = ((quality + 1) * 4) * 2;
        numIndices = ((quality + 1) * 4) * 6;
    }

    void ShapeMeshes3D::GetNumElementsQuad(UINT32& numVertices, UINT32& numIndices)
    {
        numVertices = 8;
        numIndices = 12;
    }

    void ShapeMeshes3D::WireAABox(const AABox& box, UINT8* outVertices, UINT32 vertexOffset, UINT32 vertexStride, UINT32* outIndices, UINT32 indexOffset)
    {
        outVertices += vertexOffset * vertexStride;

        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_LEFT_BOTTOM));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_RIGHT_BOTTOM));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_RIGHT_TOP));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_LEFT_TOP));

        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_RIGHT_BOTTOM));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_LEFT_BOTTOM));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_LEFT_TOP));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_RIGHT_TOP));

        outIndices += indexOffset;

        // Front
        outIndices[0] = vertexOffset + 0;
        outIndices[1] = vertexOffset + 1;

        outIndices[2] = vertexOffset + 1;
        outIndices[3] = vertexOffset + 2;

        outIndices[4] = vertexOffset + 2;
        outIndices[5] = vertexOffset + 3;

        outIndices[6] = vertexOffset + 3;
        outIndices[7] = vertexOffset + 0;

        // Center
        outIndices[8] = vertexOffset + 0;
        outIndices[9] = vertexOffset + 5;

        outIndices[10] = vertexOffset + 1;
        outIndices[11] = vertexOffset + 4;

        outIndices[12] = vertexOffset + 2;
        outIndices[13] = vertexOffset + 7;

        outIndices[14] = vertexOffset + 3;
        outIndices[15] = vertexOffset + 6;

        // Back
        outIndices[16] = vertexOffset + 4;
        outIndices[17] = vertexOffset + 5;

        outIndices[18] = vertexOffset + 5;
        outIndices[19] = vertexOffset + 6;

        outIndices[20] = vertexOffset + 6;
        outIndices[21] = vertexOffset + 7;

        outIndices[22] = vertexOffset + 7;
        outIndices[23] = vertexOffset + 4;
    }

    void ShapeMeshes3D::SolidAABox(const AABox& box, UINT8* outVertices, UINT8* outNormals, UINT8* outUVs,
        UINT32 vertexOffset, UINT32 vertexStride, UINT32* outIndices, UINT32 indexOffset)
    {
        outVertices += (vertexOffset * vertexStride);

        // Front face
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_LEFT_BOTTOM));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_RIGHT_BOTTOM));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_RIGHT_TOP));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_LEFT_TOP));

        // Back face
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_RIGHT_BOTTOM));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_LEFT_BOTTOM));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_LEFT_TOP));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_RIGHT_TOP));

        // Left face
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_LEFT_BOTTOM));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_LEFT_BOTTOM));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_LEFT_TOP));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_LEFT_TOP));

        // Right face
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_RIGHT_BOTTOM));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_RIGHT_BOTTOM));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_RIGHT_TOP));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_RIGHT_TOP));

        // Top face
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_LEFT_TOP));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_LEFT_TOP));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_RIGHT_TOP));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_RIGHT_TOP));

        // Bottom face
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_LEFT_BOTTOM));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::FAR_RIGHT_BOTTOM));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_RIGHT_BOTTOM));
        outVertices = WriteVector3(outVertices, vertexStride, box.GetCorner(AABox::NEAR_LEFT_BOTTOM));

        // Normals
        static const Vector3 faceNormals[6] =
        {
            Vector3(0, 0, 1),
            Vector3(0, 0, -1),
            Vector3(-1, 0, 0),
            Vector3(1, 0, 0),
            Vector3(0, 1, 0),
            Vector3(0, -1, 0)
        };

        if (outNormals != nullptr)
        {
            outNormals += (vertexOffset * vertexStride);
            for (UINT32 face = 0; face < 6; face++)
            {
                outNormals = WriteVector3(outNormals, vertexStride, faceNormals[face]);
                outNormals = WriteVector3(outNormals, vertexStride, faceNormals[face]);
                outNormals = WriteVector3(outNormals, vertexStride, faceNormals[face]);
                outNormals = WriteVector3(outNormals, vertexStride, faceNormals[face]);
            }
        }

        // UV
        if (outUVs != nullptr)
        {
            outUVs += (vertexOffset * vertexStride);
            for (UINT32 face = 0; face < 6; face++)
            {
                outUVs = WriteVector2(outUVs, vertexStride, Vector2(0.0f, 1.0f));
                outUVs = WriteVector2(outUVs, vertexStride, Vector2(1.0f, 1.0f));
                outUVs = WriteVector2(outUVs, vertexStride, Vector2(1.0f, 0.0f));
                outUVs = WriteVector2(outUVs, vertexStride, Vector2(0.0f, 0.0f));
            }
        }

        // Indices
        UINT32* indices = outIndices + indexOffset;
        for (UINT32 face = 0; face < 6; face++)
        {
            UINT32 faceVertOffset = vertexOffset + face * 4;

            indices[face * 6 + 0] = faceVertOffset + 2;
            indices[face * 6 + 1] = faceVertOffset + 1;
            indices[face * 6 + 2] = faceVertOffset + 0;
            indices[face * 6 + 3] = faceVertOffset + 0;
            indices[face * 6 + 4] = faceVertOffset + 3;
            indices[face * 6 + 5] = faceVertOffset + 2;
        }
    }

    void ShapeMeshes3D::SolidSphere(const Sphere& sphere, UINT8* outVertices, UINT8* outNormals, UINT8* outUV,
        UINT32 vertexOffset, UINT32 vertexStride, UINT32* outIndices, UINT32 indexOffset, UINT32 quality)
    {
        // Create icosahedron
        static const float x = 0.525731112119133606f;
        static const float z = 0.850650808352039932f;

        static const Vector3 vertices[12] =
        {
            Vector3(-x,		0.0f,	z),
            Vector3(x,		0.0f,	z),
            Vector3(-x,		0.0f,	-z),
            Vector3(x,		0.0f,	-z),
            Vector3(0.0f,	z,		x),
            Vector3(0.0f,	z,		-x),
            Vector3(0.0f,	-z,		x),
            Vector3(0.0f,	-z,		-x),
            Vector3(z,		x,		0.0f),
            Vector3(-z,		x,		0.0f),
            Vector3(z,		-x,		0.0f),
            Vector3(-z,		-x,		0.0f)
        };

        static const UINT32 triangles[20][3] =
        {
                { 0, 4, 1 },  { 0, 9, 4 },	{ 9, 5, 4 },	{ 4, 5, 8 },
                { 4, 8, 1 },  { 8, 10, 1 }, { 8, 3, 10 },	{ 5, 3, 8 },
                { 5, 2, 3 },  { 2, 7, 3 },	{ 7, 10, 3 },	{ 7, 6, 10 },
                { 7, 11, 6 }, { 11, 0, 6 }, { 0, 1, 6 },	{ 6, 1, 10 },
                { 9, 0, 11 }, { 9, 11, 2 }, { 9, 2, 5 },	{ 7, 2, 11 }
        };

        // Tessellate it
        UINT32 curVertOffset = vertexOffset;
        for (int i = 0; i < 20; ++i)
        {
            curVertOffset += SubdivideTriangleOnSphere(sphere.GetCenter(), sphere.GetRadius(), quality,
                vertices[triangles[i][2]], vertices[triangles[i][1]], vertices[triangles[i][0]],
                outVertices, outNormals, curVertOffset, vertexStride);
        }

        // Create UV if required
        if (outUV != nullptr)
        {
            UINT32 numVertices = curVertOffset - vertexOffset;

            outUV += (vertexOffset * vertexStride);
            UINT8* curUV = outUV;
            for (UINT32 i = 0; i < numVertices; i++)
            {
                Vector3 position = *(Vector3*)&outVertices[(vertexOffset + i) * vertexStride];
                Vector3 normal = Vector3::Normalize(position);

                Vector2 uv;
                uv.x = 0.5f - atan2(normal.x, normal.z) / Math::TWO_PI;
                uv.y = 0.5f - asin(normal.y) / Math::PI;

                curUV = WriteVector2(curUV, vertexStride, uv);
            }
        }

        // Create indices
        outIndices += indexOffset;

        UINT32 numIndices = 20 * (3 * (UINT32)std::pow(4, quality));
        for (UINT32 i = 0; i < numIndices; i += 3)
        {
            outIndices[i] = vertexOffset + i + 2;
            outIndices[i + 1] = vertexOffset + i + 1;
            outIndices[i + 2] = vertexOffset + i + 0;
        }

        // Fix UV seams
        UINT8* extraPositions = outVertices + curVertOffset * vertexStride;

        UINT8* extraNormals = nullptr;
        if (outNormals)
            extraNormals = outNormals + curVertOffset * vertexStride;

        UINT8* extraUV = nullptr;
        if (outUV)
            extraUV = outUV + curVertOffset * vertexStride;

        const UINT32 maxExtraVerts = 3 * (UINT32)pow(4, quality);
        UINT32 extraVertIdx = 0;
        if (outUV != nullptr)
        {
            // Note: This only fixes seams for tileable textures. To properly fix seams for all textures the triangles
            // would actually need to be split along the UV seam. This is ignored as non-tileable textures won't look
            // good on a sphere regardless of the seam.
            for (UINT32 i = 0; i < numIndices; i += 3)
            {
                const Vector2& uv0 = *(Vector2*)&outUV[(i + 0) * vertexStride];
                const Vector2& uv1 = *(Vector2*)&outUV[(i + 1) * vertexStride];
                const Vector2& uv2 = *(Vector2*)&outUV[(i + 2) * vertexStride];

                UINT32 indexToSplit = (UINT32)-1;
                float offset = 1.0f;
                if (fabs(uv2.x - uv0.x) > 0.5f)
                {
                    if (uv0.x < 0.5f)
                    {
                        // 2 is the odd-one out, > 0.5
                        if (uv1.x < 0.5f)
                        {
                            indexToSplit = 2;
                            offset = -1.0f;
                        }
                        // 0 is the odd-one out, < 0.5
                        else
                        {
                            indexToSplit = 0;
                            offset = 1.0f;
                        }
                    }
                    else
                    {
                        // 2 is the odd-one out, < 0.5
                        if (uv1.x > 0.5f)
                        {
                            indexToSplit = 2;
                            offset = 1.0f;
                        }
                        // 0 is the odd-one out, > 0.5
                        else
                        {
                            indexToSplit = 0;
                            offset = -1.0f;
                        }

                    }
                }
                else if (fabs(uv1.x - uv0.x) > 0.5f)
                {
                    if (uv0.x < 0.5f)
                    {
                        // 1 is the odd-one out, > 0.5
                        if (uv2.x < 0.5f)
                        {
                            indexToSplit = 1;
                            offset = -1.0f;
                        }
                        // 0 is the odd-one out, < 0.5
                        else
                        {
                            indexToSplit = 0;
                            offset = 1.0f;
                        }
                    }
                    else
                    {
                        // 1 is the odd-one out, < 0.5
                        if (uv2.x > 0.5f)
                        {
                            indexToSplit = 1;
                            offset = 1.0f;
                        }
                        // 0 is the odd-one out, > 0.5
                        else
                        {
                            indexToSplit = 0;
                            offset = -1.0f;
                        }
                    }
                }
                else if (fabs(uv1.x - uv2.x) > 0.5f)
                {
                    if (uv2.x < 0.5f)
                    {
                        // 1 is the odd-one out, > 0.5
                        if (uv0.x < 0.5f)
                        {
                            indexToSplit = 1;
                            offset = -1.0f;
                        }
                        // 2 is the odd-one out, < 0.5
                        else
                        {
                            indexToSplit = 2;
                            offset = 1.0f;
                        }
                    }
                    else
                    {
                        // 1 is the odd-one out, < 0.5
                        if (uv0.x > 0.5f)
                        {
                            indexToSplit = 1;
                            offset = 1.0f;
                        }
                        // 2 is the odd-one out, > 0.5
                        else
                        {
                            indexToSplit = 2;
                            offset = -1.0f;
                        }
                    }
                }

                if (indexToSplit != (UINT32)-1)
                {
                    Vector3 position = *(Vector3*)&outVertices[(vertexOffset + i + indexToSplit) * vertexStride];
                    extraPositions = WriteVector3(extraPositions, vertexStride, position);

                    if (extraNormals)
                    {
                        Vector3 normal = *(Vector3*)&outNormals[(vertexOffset + i + indexToSplit) * vertexStride];
                        extraNormals = WriteVector3(extraNormals, vertexStride, normal);
                    }

                    Vector2 uv = *(Vector2*)&outUV[(i + indexToSplit) * vertexStride];
                    uv.x += offset;

                    extraUV = WriteVector2(extraUV, vertexStride, uv);

                    // Index 0 maps to vertex 2, index 1 to vertex 1, index 2 to vertex 0
                    if (indexToSplit == 0)
                        indexToSplit = 2;
                    else if (indexToSplit == 2)
                        indexToSplit = 0;

                    outIndices[i + indexToSplit] = vertexOffset + numIndices + extraVertIdx;

                    assert(extraVertIdx < maxExtraVerts);
                    extraVertIdx++;
                }
            }
        }

        // Fill out the remaining extra vertices, just so they aren't uninitialized
        for (; extraVertIdx < maxExtraVerts; extraVertIdx++)
        {
            extraPositions = WriteVector3(extraPositions, vertexStride, sphere.GetCenter());

            if (extraNormals)
                extraNormals = WriteVector3(extraNormals, vertexStride, Vector3::UNIT_Z);

            if (extraUV)
                extraUV = WriteVector2(extraUV, vertexStride, Vector2::ZERO);
        }
    }

    void ShapeMeshes3D::WireArc(const Vector3& center, float radius, const Vector3& normal, Degree startAngle, Degree amountAngle,
        UINT8* outVertices, UINT32 vertexOffset, UINT32 vertexStride, UINT32* outIndices, UINT32 indexOffset, UINT32 quality)
    {
        UINT32 numVertices = (quality + 1) * 5;

        GenerateArcVertices(center, normal, radius, startAngle, amountAngle, Vector2::ONE,
            numVertices, outVertices, vertexOffset, vertexStride);

        outIndices += indexOffset;
        UINT32 numLines = numVertices - 1;
        for (UINT32 i = 0; i < numLines; i++)
        {
            outIndices[i * 2 + 0] = vertexOffset + i;
            outIndices[i * 2 + 1] = vertexOffset + i + 1;
        }
    }

    void ShapeMeshes3D::SolidArc(const Vector3& center, float radius, const Vector3& normal, Degree startAngle,
        Degree amountAngle, UINT8* outVertices, UINT8* outNormals, UINT8* outUV, UINT32 vertexOffset, UINT32 vertexStride,
        UINT32* outIndices, UINT32 indexOffset, UINT32 quality)
    {
        outVertices += vertexOffset * vertexStride;
        outNormals += vertexOffset * vertexStride;
        outIndices += indexOffset;

        bool reverseOrder = amountAngle.ValueDegrees() < 0.0f;
        Vector3 visibleNormal = normal;

        outVertices = WriteVector3(outVertices, vertexStride, center);
        outNormals = WriteVector3(outNormals, vertexStride, visibleNormal);

        UINT32 numArcVertices = (quality + 1) * 5;
        GenerateArcVertices(center, normal, radius, startAngle, amountAngle, Vector2::ONE,
            numArcVertices, outVertices, vertexOffset, vertexStride);

        UINT8* otherSideVertices = outVertices + (numArcVertices * vertexStride);
        UINT8* otherSideNormals = outNormals + (numArcVertices * vertexStride);

        otherSideVertices = WriteVector3(otherSideVertices, vertexStride, center);
        otherSideNormals = WriteVector3(otherSideNormals, vertexStride, -visibleNormal);

        for (UINT32 i = 0; i < numArcVertices; i++)
        {
            otherSideVertices = WriteVector3(otherSideVertices, vertexStride, *(Vector3*)&outVertices[i * vertexStride]);

            outNormals = WriteVector3(outNormals, vertexStride, visibleNormal);
            otherSideNormals = WriteVector3(otherSideNormals, vertexStride, -visibleNormal);
        }

        // UV
        if (outUV != nullptr)
        {
            outUV += vertexOffset * vertexStride;

            // Center
            outUV = WriteVector2(outUV, vertexStride, Vector2(0.5f, 0.5f));

            Vector3 arcNormal = normal;
            Vector3 right, top;
            arcNormal.OrthogonalComplement(right, top);

            for (UINT32 i = 0; i < numArcVertices; i++)
            {
                Vector3 vec = *(Vector3*)&outVertices[i * vertexStride];
                Vector3 diff = Vector3::Normalize(vec - center);

                Vector2 uv;
                uv.x = Vector3::Dot(diff, right) * 0.5f + 0.5f;
                uv.y = Vector3::Dot(diff, top) * 0.5f + 0.5f;

                outUV = WriteVector2(outUV, vertexStride, uv);
            }

            // Reverse
            outUV = WriteVector2(outUV, vertexStride, Vector2(0.5f, 0.5f));

            for (UINT32 i = 0; i < numArcVertices; i++)
            {
                Vector3 vec = *(Vector3*)&outVertices[(numArcVertices + i + 1) * vertexStride];
                Vector3 diff = Vector3::Normalize(vec - center);

                Vector2 uv;
                uv.x = Vector3::Dot(diff, -right) * 0.5f + 0.5f;
                uv.y = Vector3::Dot(diff, -top) * 0.5f + 0.5f;

                outUV = WriteVector2(outUV, vertexStride, uv);
            }
        }

        UINT32 numTriangles = numArcVertices;

        // If angle is negative the order of vertices is reversed so we need to reverse the indexes too
        UINT32 frontSideOffset = vertexOffset + (reverseOrder ? (numArcVertices + 1) : 0);
        UINT32 backSideOffset = vertexOffset + (!reverseOrder ? (numArcVertices + 1) : 0);

        for (UINT32 i = 0; i < numTriangles; i++)
        {
            outIndices[i * 6 + 0] = frontSideOffset + 0;
            outIndices[i * 6 + 1] = frontSideOffset + i;
            outIndices[i * 6 + 2] = frontSideOffset + i + 1;

            outIndices[i * 6 + 3] = backSideOffset + 0;
            outIndices[i * 6 + 4] = backSideOffset + i + 1;
            outIndices[i * 6 + 5] = backSideOffset + i;
        }
    }

    void ShapeMeshes3D::SolidCone(const Vector3& base, const Vector3& normal, float height, float radius, Vector2 scale,
        UINT8* outVertices, UINT8* outNormals, UINT8* outUV, UINT32 vertexOffset, UINT32 vertexStride, UINT32* outIndices,
        UINT32 indexOffset, UINT32 quality)
    {
        outVertices += vertexOffset * vertexStride;
        outIndices += indexOffset;

        if (outUV != nullptr)
            outUV += vertexOffset * vertexStride;

        if (outNormals != nullptr)
            outNormals += vertexOffset * vertexStride;

        // Generate base disc
        UINT32 numArcVertices = (quality + 1) * 4;

        GenerateArcVertices(base, normal, radius, Degree(0), Degree(360), scale,
            numArcVertices + 1, outVertices, 0, vertexStride);

        if (outUV != nullptr)
        {
            Vector3 arcNormal = normal;
            Vector3 right, top;
            arcNormal.OrthogonalComplement(right, top);

            for (UINT32 i = 0; i < numArcVertices; i++)
            {
                Vector3 vec = *(Vector3*)&outVertices[i * vertexStride];
                Vector3 diff = Vector3::Normalize(vec - base);

                Vector2 uv;
                uv.x = Vector3::Dot(diff, right) * 0.5f + 0.5f;
                uv.y = Vector3::Dot(diff, top) * 0.5f + 0.5f;

                outUV = WriteVector2(outUV, vertexStride, uv);
            }

            // Center base
            outUV = WriteVector2(outUV, vertexStride, Vector2(0.5f, 0.5f));
        }

        outVertices += numArcVertices * vertexStride;
        outVertices = WriteVector3(outVertices, vertexStride, base); // Write base vertex

        UINT32 baseIdx = numArcVertices;

        if (outNormals != nullptr)
        {
            UINT32 totalNumBaseVertices = numArcVertices + 1;
            for (UINT32 i = 0; i < totalNumBaseVertices; i++)
                outNormals = WriteVector3(outNormals, vertexStride, -normal);
        }

        UINT32 numTriangles = numArcVertices;
        for (UINT32 i = 0; i < numTriangles - 1; i++)
        {
            outIndices[i * 3 + 0] = vertexOffset + baseIdx;
            outIndices[i * 3 + 1] = vertexOffset + i + 1;
            outIndices[i * 3 + 2] = vertexOffset + i;
        }

        {
            UINT32 i = numTriangles - 1;
            outIndices[i * 3 + 0] = vertexOffset + baseIdx;
            outIndices[i * 3 + 1] = vertexOffset + 0;
            outIndices[i * 3 + 2] = vertexOffset + i;
        }

        //// Generate cone
        // Base vertices
        GenerateArcVertices(base, normal, radius, Degree(0), Degree(360), scale,
            numArcVertices + 1, outVertices, 0, vertexStride);

        Vector3 topVertex = base + normal * height;

        // Normals
        if (outNormals != nullptr)
        {
            UINT8* outNormalsBase = outNormals;
            UINT8* outNormalsTop = outNormals + numArcVertices * vertexStride;
            for (INT32 i = 0; i < (INT32)numArcVertices; i++)
            {
                int offsetA = i == 0 ? numArcVertices - 1 : i - 1;
                int offsetB = i;
                int offsetC = (i + 1) % numArcVertices;

                Vector3* a = (Vector3*)(outVertices + (offsetA * vertexStride));
                Vector3* b = (Vector3*)(outVertices + (offsetB * vertexStride));
                Vector3* c = (Vector3*)(outVertices + (offsetC * vertexStride));

                Vector3 toTop = topVertex - *b;

                Vector3 normalLeft = Vector3::Cross(*a - *b, toTop);
                normalLeft.Normalize();

                Vector3 normalRight = Vector3::Cross(toTop, *c - *b);
                normalRight.Normalize();

                Vector3 triNormal = Vector3::Normalize(normalLeft + normalRight);

                outNormalsBase = WriteVector3(outNormalsBase, vertexStride, triNormal);
                outNormalsTop = WriteVector3(outNormalsTop, vertexStride, triNormal);
            }
        }

        // UV
        if (outUV != nullptr)
        {
            float angle = 0.0f;
            float angleIncrement = Math::TWO_PI / numArcVertices;

            // Bottom
            for (UINT32 i = 0; i < numArcVertices; i++)
            {
                Vector2 uv;
                uv.x = angle / Math::TWO_PI;
                uv.y = 1.0f;

                outUV = WriteVector2(outUV, vertexStride, uv);
                angle += angleIncrement;
            }

            // Top
            angle = 0.0f;
            for (UINT32 i = 0; i < numArcVertices; i++)
            {
                Vector2 uv;
                uv.x = angle / Math::TWO_PI;
                uv.y = 0.0f;

                outUV = WriteVector2(outUV, vertexStride, uv);
                angle += angleIncrement;
            }
        }

        // Top vertices (All same position, but need them separate because of different normals & uv)
        outVertices += numArcVertices * vertexStride;

        for (UINT32 i = 0; i < numArcVertices; i++)
            outVertices = WriteVector3(outVertices, vertexStride, topVertex);

        outIndices += numTriangles * 3;
        UINT32 curVertBaseOffset = vertexOffset + numArcVertices + 1;
        UINT32 curVertTopOffset = curVertBaseOffset + numArcVertices;
        for (UINT32 i = 0; i < numTriangles - 1; i++)
        {
            outIndices[i * 3 + 0] = curVertTopOffset + i;
            outIndices[i * 3 + 1] = curVertBaseOffset + i;
            outIndices[i * 3 + 2] = curVertBaseOffset + i + 1;
        }

        {
            UINT32 i = numTriangles - 1;
            outIndices[i * 3 + 0] = curVertTopOffset + i;
            outIndices[i * 3 + 1] = curVertBaseOffset + i;
            outIndices[i * 3 + 2] = curVertBaseOffset + 0;
        }
    }

    void ShapeMeshes3D::WireCone(const Vector3& base, const Vector3& normal, float height, float radius, Vector2 scale,
        UINT8* outVertices, UINT32 vertexOffset, UINT32 vertexStride, UINT32* outIndices, UINT32 indexOffset, UINT32 quality)
    {
        outVertices += vertexOffset * vertexStride;
        outIndices += indexOffset;

        // Generate arc vertices
        UINT32 numArcVertices = (quality + 1) * 4;

        GenerateArcVertices(base, normal, radius, Degree(0), Degree(360), scale,
            numArcVertices + 1, outVertices, 0, vertexStride);

        outVertices += numArcVertices * vertexStride;

        UINT32 numLines = numArcVertices;
        for (UINT32 i = 0; i < numLines; i++)
        {
            outIndices[i * 2 + 0] = vertexOffset + i;
            outIndices[i * 2 + 1] = vertexOffset + i + 1;
        }

        outIndices += numLines * 2;

        // Generate cone vertices
        GenerateArcVertices(base, normal, radius, Degree(0), Degree(360), scale,
            5, outVertices, 0, vertexStride);

        // Cone point
        outVertices += 4 * vertexStride;
        outVertices = WriteVector3(outVertices, vertexStride, base + normal * height);

        vertexOffset += numArcVertices;

        for (UINT32 i = 0; i < 4; i++)
        {
            outIndices[i * 2 + 0] = vertexOffset + 4;
            outIndices[i * 2 + 1] = vertexOffset + i;
        }
    }

    void ShapeMeshes3D::SolidCylinder(const Vector3& base, const Vector3& normal, float height, float radius, Vector2 scale,
        UINT8* outVertices, UINT8* outNormals, UINT8* outUV, UINT32 vertexOffset, UINT32 vertexStride, UINT32* outIndices,
        UINT32 indexOffset, UINT32 quality)
    {
        outVertices += vertexOffset * vertexStride;
        outIndices += indexOffset;

        if (outUV != nullptr)
            outUV += vertexOffset * vertexStride;

        if (outNormals != nullptr)
            outNormals += vertexOffset * vertexStride;

        // Generate base disc
        UINT32 numArcVertices = (quality + 1) * 4;

        GenerateArcVertices(base, normal, radius, Degree(0), Degree(360), scale,
            numArcVertices + 1, outVertices, 0, vertexStride);

        if (outUV != nullptr)
        {
            Vector3 arcNormal = normal;
            Vector3 right, top;
            arcNormal.OrthogonalComplement(right, top);

            for (UINT32 i = 0; i < numArcVertices; i++)
            {
                Vector3 vec = *(Vector3*)&outVertices[i * vertexStride];
                Vector3 diff = Vector3::Normalize(vec - base);

                Vector2 uv;
                uv.x = Vector3::Dot(diff, right) * 0.5f + 0.5f;
                uv.y = Vector3::Dot(diff, top) * 0.5f + 0.5f;

                outUV = WriteVector2(outUV, vertexStride, uv);
            }

            outUV = WriteVector2(outUV, vertexStride, Vector2(0.5f, 0.5f));
        }

        outVertices += numArcVertices * vertexStride;
        outVertices = WriteVector3(outVertices, vertexStride, base); // Write base vertex

        UINT32 baseIdx = numArcVertices;

        if (outNormals != nullptr)
        {
            UINT32 totalNumBaseVertices = numArcVertices + 1;
            for (UINT32 i = 0; i < totalNumBaseVertices; i++)
                outNormals = WriteVector3(outNormals, vertexStride, -normal);
        }

        UINT32 numTriangles = numArcVertices;
        for (UINT32 i = 0; i < numTriangles - 1; i++)
        {
            outIndices[i * 3 + 0] = vertexOffset + baseIdx;
            outIndices[i * 3 + 1] = vertexOffset + i + 1;
            outIndices[i * 3 + 2] = vertexOffset + i;
        }

        {
            UINT32 i = numTriangles - 1;
            outIndices[i * 3 + 0] = vertexOffset + baseIdx;
            outIndices[i * 3 + 1] = vertexOffset + 0;
            outIndices[i * 3 + 2] = vertexOffset + i;
        }

        outIndices += numTriangles * 3;
        UINT32 vertexOffsetBase = vertexOffset + numArcVertices + 1;

        // Generate cap disc
        Vector3 topVertex = base + normal * height;

        GenerateArcVertices(topVertex, normal, radius, Degree(0), Degree(360), scale,
            numArcVertices + 1, outVertices, 0, vertexStride);

        if (outUV != nullptr)
        {
            Vector3 arcNormal = normal;
            Vector3 right, top;
            arcNormal.OrthogonalComplement(right, top);

            for (UINT32 i = 0; i < numArcVertices; i++)
            {
                Vector3 vec = *(Vector3*)&outVertices[i * vertexStride];
                Vector3 diff = Vector3::Normalize(vec - topVertex);

                Vector2 uv;
                uv.x = Vector3::Dot(diff, right) * 0.5f + 0.5f;
                uv.y = Vector3::Dot(diff, top) * 0.5f + 0.5f;

                outUV = WriteVector2(outUV, vertexStride, uv);
            }

            outUV = WriteVector2(outUV, vertexStride, Vector2(0.5f, 0.5f));
        }

        outVertices += numArcVertices * vertexStride;
        outVertices = WriteVector3(outVertices, vertexStride, topVertex); // Write top vertex

        if (outNormals != nullptr)
        {
            UINT32 totalNumBaseVertices = numArcVertices + 1;
            for (UINT32 i = 0; i < totalNumBaseVertices; i++)
                outNormals = WriteVector3(outNormals, vertexStride, normal);
        }

        for (UINT32 i = 0; i < numTriangles - 1; i++)
        {
            outIndices[i * 3 + 0] = vertexOffsetBase + baseIdx;
            outIndices[i * 3 + 1] = vertexOffsetBase + i;
            outIndices[i * 3 + 2] = vertexOffsetBase + i + 1;
        }

        {
            UINT32 i = numTriangles - 1;
            outIndices[i * 3 + 0] = vertexOffsetBase + baseIdx;
            outIndices[i * 3 + 1] = vertexOffsetBase + i;
            outIndices[i * 3 + 2] = vertexOffsetBase + 0;
        }

        outIndices += numTriangles * 3;
        UINT32 vertexOffsetCap = vertexOffsetBase + numArcVertices + 1;

        // Generate cylinder
        GenerateArcVertices(base, normal, radius, Degree(0), Degree(360), scale,
            numArcVertices + 1, outVertices, 0, vertexStride);

        GenerateArcVertices(topVertex, normal, radius, Degree(0), Degree(360), scale,
            numArcVertices + 1, outVertices, numArcVertices + 1, vertexStride);

        // Normals
        if (outNormals != nullptr)
        {
            UINT8* outNormalsBase = outNormals;
            UINT8* outNormalsCap = outNormals + (numArcVertices + 1) * vertexStride;
            for (UINT32 i = 0; i < numArcVertices + 1; i++)
            {
                UINT32 offsetA = i == 0 ? numArcVertices - 1 : i - 1;
                UINT32 offsetB = i;
                UINT32 offsetC = i == numArcVertices ? 1 : i + 1;

                Vector3* a = (Vector3*)(outVertices + (offsetA * vertexStride));
                Vector3* b = (Vector3*)(outVertices + (offsetB * vertexStride));
                Vector3* c = (Vector3*)(outVertices + (offsetC * vertexStride));

                Vector3 toTop = normal;

                Vector3 normalLeft = Vector3::Cross(*a - *b, toTop);
                normalLeft.Normalize();

                Vector3 normalRight = Vector3::Cross(toTop, *c - *b);
                normalRight.Normalize();

                Vector3 triNormal = Vector3::Normalize(normalLeft + normalRight);

                outNormalsBase = WriteVector3(outNormalsBase, vertexStride, triNormal);
                outNormalsCap = WriteVector3(outNormalsCap, vertexStride, triNormal);
            }
        }

        // UV
        if (outUV != nullptr)
        {
            float angle = 0.0f;
            float angleIncrement = Math::TWO_PI / numArcVertices;

            for (UINT32 i = 0; i < numArcVertices + 1; i++)
            {
                Vector2 uv;
                uv.x = angle / Math::TWO_PI;
                uv.y = 1.0f;

                outUV = WriteVector2(outUV, vertexStride, uv);
                angle += angleIncrement;
            }

            angle = 0.0f;
            for (UINT32 i = 0; i < numArcVertices + 1; i++)
            {
                Vector2 uv;
                uv.x = angle / Math::TWO_PI;
                uv.y = 0.0f;

                outUV = WriteVector2(outUV, vertexStride, uv);
                angle += angleIncrement;
            }
        }

        vertexOffsetBase = vertexOffsetCap;
        vertexOffsetCap = vertexOffsetCap + numArcVertices + 1;

        for (UINT32 i = 0; i < numTriangles; i++)
        {
            outIndices[i * 6 + 0] = vertexOffsetCap + i;
            outIndices[i * 6 + 1] = vertexOffsetBase + i;
            outIndices[i * 6 + 2] = vertexOffsetBase + i + 1;

            outIndices[i * 6 + 3] = vertexOffsetCap + i;
            outIndices[i * 6 + 4] = vertexOffsetBase + i + 1;
            outIndices[i * 6 + 5] = vertexOffsetCap + i + 1;
        }
    }

    void ShapeMeshes3D::WireCylinder(const Vector3& base, const Vector3& normal, float height, float radius, Vector2 scale,
        UINT8* outVertices, UINT32 vertexOffset, UINT32 vertexStride, UINT32* outIndices, UINT32 indexOffset, UINT32 quality)
    {
        outVertices += vertexOffset * vertexStride;
        outIndices += indexOffset;

        // Generate base and cap discs
        UINT32 numArcVertices = (quality + 1) * 4;

        Degree angleAmount = Degree(360) - Degree(360) / (float)(numArcVertices);

        GenerateArcVertices(base, normal, radius, Degree(0), angleAmount, scale,
            numArcVertices, outVertices, 0, vertexStride);

        Vector3 topVertex = base + normal * height;

        GenerateArcVertices(topVertex, normal, radius, Degree(0), angleAmount, scale,
            numArcVertices, outVertices, numArcVertices, vertexStride);

        UINT32 vertexOffsetBase = vertexOffset;
        UINT32 vertexOffsetCap = vertexOffset + numArcVertices;

        UINT32 numLines = numArcVertices;
        for (UINT32 i = 0; i < numLines - 1; i++)
        {
            outIndices[i * 4 + 0] = vertexOffsetBase + i;
            outIndices[i * 4 + 1] = vertexOffsetBase + i + 1;
            outIndices[i * 4 + 2] = vertexOffsetCap + i;
            outIndices[i * 4 + 3] = vertexOffsetCap + i + 1;
        }
        {
            UINT32 i = numLines - 1;
            outIndices[i * 4 + 0] = vertexOffsetBase + i;
            outIndices[i * 4 + 1] = vertexOffsetBase + 0;
            outIndices[i * 4 + 2] = vertexOffsetCap + i;
            outIndices[i * 4 + 3] = vertexOffsetCap + 0;
        }

        // Generate cylinder
        outIndices += numLines * 4;
        for (UINT32 i = 0; i < numLines; i++)
        {
            outIndices[i * 2 + 0] = vertexOffsetBase + i;
            outIndices[i * 2 + 1] = vertexOffsetCap + i;
        }
    }

    void ShapeMeshes3D::SolidQuad(const Rect3& area, UINT8* outVertices, UINT8* outNormals, UINT8* outUV,
        UINT32 vertexOffset, UINT32 vertexStride, UINT32* outIndices, UINT32 indexOffset)
    {
        outVertices += (vertexOffset * vertexStride);

        Vector3 topLeft = area.GetCenter() - area.GetAxisHorz() * area.GetExtentHorz() + area.GetAxisVert() * area.GetExtentVertical();
        Vector3 topRight = area.GetCenter() + area.GetAxisHorz() * area.GetExtentHorz() + area.GetAxisVert() * area.GetExtentVertical();
        Vector3 botRight = area.GetCenter() + area.GetAxisHorz() * area.GetExtentHorz() - area.GetAxisVert() * area.GetExtentVertical();
        Vector3 botLeft = area.GetCenter() - area.GetAxisHorz() * area.GetExtentHorz() - area.GetAxisVert() * area.GetExtentVertical();

        outVertices = WriteVector3(outVertices, vertexStride, topLeft);
        outVertices = WriteVector3(outVertices, vertexStride, topRight);
        outVertices = WriteVector3(outVertices, vertexStride, botRight);
        outVertices = WriteVector3(outVertices, vertexStride, botLeft);

        outVertices = WriteVector3(outVertices, vertexStride, topLeft);
        outVertices = WriteVector3(outVertices, vertexStride, topRight);
        outVertices = WriteVector3(outVertices, vertexStride, botRight);
        outVertices = WriteVector3(outVertices, vertexStride, botLeft);

        Vector3 normal = area.GetAxisHorz().Cross(area.GetAxisVert());
        Vector3 reverseNormal = -normal;

        outNormals += (vertexOffset * vertexStride);
        outNormals = WriteVector3(outNormals, vertexStride, normal);
        outNormals = WriteVector3(outNormals, vertexStride, normal);
        outNormals = WriteVector3(outNormals, vertexStride, normal);
        outNormals = WriteVector3(outNormals, vertexStride, normal);

        outNormals = WriteVector3(outNormals, vertexStride, reverseNormal);
        outNormals = WriteVector3(outNormals, vertexStride, reverseNormal);
        outNormals = WriteVector3(outNormals, vertexStride, reverseNormal);
        outNormals = WriteVector3(outNormals, vertexStride, reverseNormal);

        if (outUV != nullptr)
        {
            outUV += (vertexOffset * vertexStride);
            outUV = WriteVector2(outUV, vertexStride, Vector2(0.0f, 0.0f));
            outUV = WriteVector2(outUV, vertexStride, Vector2(1.0f, 0.0f));
            outUV = WriteVector2(outUV, vertexStride, Vector2(1.0f, 1.0f));
            outUV = WriteVector2(outUV, vertexStride, Vector2(0.0f, 1.0f));

            outUV = WriteVector2(outUV, vertexStride, Vector2(0.0f, 0.0f));
            outUV = WriteVector2(outUV, vertexStride, Vector2(1.0f, 0.0f));
            outUV = WriteVector2(outUV, vertexStride, Vector2(1.0f, 1.0f));
            outUV = WriteVector2(outUV, vertexStride, Vector2(0.0f, 1.0f));
        }

        outIndices += indexOffset;
        outIndices[0] = vertexOffset;
        outIndices[1] = vertexOffset + 1;
        outIndices[2] = vertexOffset + 2;

        outIndices[3] = vertexOffset;
        outIndices[4] = vertexOffset + 2;
        outIndices[5] = vertexOffset + 3;

        outIndices[6] = vertexOffset + 4;
        outIndices[7] = vertexOffset + 6;
        outIndices[8] = vertexOffset + 5;

        outIndices[9] = vertexOffset + 4;
        outIndices[10] = vertexOffset + 7;
        outIndices[11] = vertexOffset + 6;
    }

    Vector3 ShapeMeshes3D::CalcCenter(UINT8* vertices, UINT32 numVertices, UINT32 vertexStride)
    {
        Vector3 center = Vector3::ZERO;
        for (UINT32 i = 0; i < numVertices; i++)
        {
            Vector3* curVert = (Vector3*)vertices;
            center += *curVert;

            vertices += vertexStride;
        }

        center /= (float)numVertices;
        return center;
    }

    UINT32 ShapeMeshes3D::SubdivideTriangleOnSphere(const Vector3& center, float radius, UINT32 numLevels,
        const Vector3& a, const Vector3& b, const Vector3& c,
        UINT8* outVertices, UINT8* outNormals, UINT32 vertexOffset, UINT32 vertexStride)
    {
        outVertices += (vertexOffset * vertexStride);

        if (outNormals != nullptr)
            outNormals += (vertexOffset * vertexStride);

        UINT32 numVertices = 0;

        if (numLevels > 0)
        {
            Vector3 sub1 = Vector3::Normalize((a + b) * 0.5f);
            Vector3 sub2 = Vector3::Normalize((b + c) * 0.5f);
            Vector3 sub3 = Vector3::Normalize((c + a) * 0.5f);

            numLevels--;

            numVertices += SubdivideTriangleOnSphere(center, radius, numLevels, a, sub1, sub3, outVertices,
                outNormals, numVertices, vertexStride);
            numVertices += SubdivideTriangleOnSphere(center, radius, numLevels, sub1, b, sub2, outVertices,
                outNormals, numVertices, vertexStride);
            numVertices += SubdivideTriangleOnSphere(center, radius, numLevels, sub1, sub2, sub3, outVertices,
                outNormals, numVertices, vertexStride);
            numVertices += SubdivideTriangleOnSphere(center, radius, numLevels, sub3, sub2, c, outVertices,
                outNormals, numVertices, vertexStride);
        }
        else
        {
            *((Vector3*)outVertices) = center + a * radius;
            outVertices += vertexStride;

            *((Vector3*)outVertices) = center + b * radius;
            outVertices += vertexStride;

            *((Vector3*)outVertices) = center + c * radius;
            outVertices += vertexStride;

            if (outNormals != nullptr)
            {
                *((Vector3*)outNormals) = a;
                outNormals += vertexStride;

                *((Vector3*)outNormals) = b;
                outNormals += vertexStride;

                *((Vector3*)outNormals) = c;
                outNormals += vertexStride;
            }

            numVertices += 3;
        }

        return numVertices;
    }

    void ShapeMeshes3D::GenerateArcVertices(const Vector3& center, const Vector3& up, float radius, Degree startAngle,
        Degree angleAmount, Vector2 scale, UINT32 numVertices, UINT8* outVertices, UINT32 vertexOffset, UINT32 vertexStride)
    {
        assert(numVertices >= 2);

        Quaternion alignWithStart = Quaternion(-Vector3::UNIT_Y, startAngle);
        Quaternion alignWithUp = Quaternion::GetRotationFromTo(Vector3::UNIT_Y, up);

        Vector3 right = alignWithUp.Rotate(alignWithStart.Rotate(Vector3::UNIT_X));
        right.Normalize();

        Quaternion increment(-up, angleAmount / (float)(numVertices - 1));

        outVertices += vertexOffset * vertexStride;
        Vector3 curDirection = right * radius;
        for (UINT32 i = 0; i < numVertices; i++)
        {
            // Note: Ignoring scale
            outVertices = WriteVector3(outVertices, vertexStride, (center + curDirection));
            curDirection = increment.Rotate(curDirection);
        }
    }

    void ShapeMeshes3D::GenerateTangents(UINT8* positions, UINT8* normals, UINT8* uv, UINT32* indices,
        UINT32 numVertices, UINT32 numIndices, UINT32 vertexOffset, UINT32 indexOffset, UINT32 vertexStride, UINT8* tangents)
    {
        Vector3* tempTangents = te_allocate<Vector3>(numVertices * sizeof(Vector3));
        Vector3* tempBitangents = te_allocate<Vector3>(numVertices * sizeof(Vector3));

        MeshUtility::CalculateTangents(
            (Vector3*)(positions + vertexOffset * vertexStride),
            (Vector3*)(normals + vertexOffset * vertexStride),
            (Vector2*)(uv + vertexOffset * vertexStride),
            (UINT8*)(indices + indexOffset),
            numVertices, numIndices, tempTangents, tempBitangents, 4, vertexStride);

        for (UINT32 i = 0; i < (UINT32)numVertices; i++)
        {
            Vector3 normal = *(Vector3*)&normals[(vertexOffset + i) * vertexStride];
            Vector3 tangent = tempTangents[i];
            Vector3 bitangent = tempBitangents[i];

            Vector3 engineBitangent = Vector3::Cross(normal, tangent);
            float sign = Vector3::Dot(engineBitangent, bitangent);

            Vector4 packedTangent(tangent.x, tangent.y, tangent.z, sign > 0 ? 1.0f : -1.0f);
            memcpy(tangents + (vertexOffset + i) * vertexStride, &packedTangent, sizeof(Vector4));
        }

        te_deallocate(tempBitangents);
        te_deallocate(tempTangents);
    }
}
