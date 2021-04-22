#pragma once

#include "TeObjectImporterPrerequisites.h"
#include "Importer/TeBaseImporter.h"
#include "Renderer/TeRendererMeshData.h"
#include "TeObjectImportData.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace te
{
    class Skeleton;
    struct AnimationSplitInfo;

    class ObjectImporter : public BaseImporter
    {
    public:
        ObjectImporter();
        virtual ~ObjectImporter();

        /** @copydoc BaseImporter::IsExtensionSupported */
        bool IsExtensionSupported(const String& ext) const override;

        /** @copydoc BaseImporter::Import */
        SPtr<Resource> Import(const String& filePath, const SPtr<const ImportOptions> importOptions) override;

        /** @copydoc BaseImporter::ImportAll */
        Vector<SubResourceRaw> ImportAll(const String& filePath, SPtr<const ImportOptions> importOptions) override;

        /** @copydoc BaseImporter::CreateImportOptions */
        SPtr<ImportOptions> CreateImportOptions() const override;

    private:
        /** Reads the object file and outputs mesh data from the read file. Sub-mesh information will be output in @p subMeshes. */
        SPtr<RendererMeshData> ImportMeshData(const String& filePath, SPtr<const ImportOptions> importOptions, Vector<SubMesh>& subMeshes, 
            Vector<AssimpAnimationClipData>& animation, SPtr<Skeleton>& skeleton);

        /**
         * Parses an FBX scene. Find all meshes in the scene and returns mesh data object containing all vertices, indexes
         * and other mesh information. Also outputs a sub-mesh array that allows you locate specific sub-meshes within the
         * returned mesh data object. If requested animation and blend shape data is output as well.
         */
        void ParseScene(aiScene* scene, const AssimpImportOptions& options, AssimpImportScene& outputScene);

        /** Parses a mesh. Converts it from Assimp format into a mesh data object containing one or multiple sub-meshes. */
        void ParseMesh(aiMesh* mesh, AssimpImportNode* parentNode, const AssimpImportOptions& options, AssimpImportScene& outputScene);

        /**	Imports skinning information and bones for all meshes. */
        void ImportSkin(AssimpImportScene& scene, const AssimpImportOptions& options);

        /**	Imports skinning information and bones for the specified mesh. */
        void ImportSkin(AssimpImportScene& scene, aiMesh* assimpMesh, AssimpImportMesh& mesh, const AssimpImportOptions& options);

        /**	Imports all bone and blend shape animations from the FBX. */
        void ImportAnimations(aiScene* scene, AssimpImportOptions& importOptions, AssimpImportScene& importScene, const String& filePath);

        /**
         * Parses the scene and outputs a skeleton for the imported meshes using the imported raw data.
         *
         * @param[in]	scene		Scene whose meshes to parse.
         * @param[in]	sharedRoot	Determines should a shared root bone be created. Set this to true if the scene contains
         *							multiple sub-meshes (as there can't be multiple roots).
         * @return					Skeleton containing a set of bones, or null if meshes don't contain a skeleton.
         */
        SPtr<Skeleton> CreateSkeleton(const AssimpImportScene& scene, bool sharedRoot);

        /** Converts FBX animation clips into engine-ready animation curve format. */
        void ConvertAnimations(const Vector<AssimpAnimationClip>& clips, const Vector<AnimationSplitInfo>& splits,
            const SPtr<Skeleton>& skeleton, bool importRootMotion, Vector<AssimpAnimationClipData>& output);

        /**
         * Removes identical sequential keyframes for the provided set of curves. The keyframe must be identical over all
         * the curves in order for it to be removed.
         */
        template<class T>
        TAnimationCurve<T> ReduceKeyframes(TAnimationCurve<T>& curve);

        /** Converts the mesh data from the imported assimp scene into mesh data that can be used for initializing a mesh. */
        SPtr<RendererMeshData> GenerateMeshData(AssimpImportScene& scene, AssimpImportOptions& options, Vector<SubMesh>& subMeshes);

        /**	Creates an internal representation of an assimp node from an aiNode object. */
        AssimpImportNode* CreateImportNode(const AssimpImportOptions& options, AssimpImportScene& scene, aiNode* assimpNode, AssimpImportNode* parent);

        /** Convert an assimp matrix into engine matrix */
        Matrix4 ConvertToNativeType(const aiMatrix4x4& matrix);

        /** Convert an assimp vector4 into engine vector4 */
        Vector4 ConvertToNativeType(const aiColor4D& color);

        /** Convert an assimp vector3 into engine vector3 */
        Vector3 ConvertToNativeType(const aiVector3D& vector);

        /** Convert an assimp vector2 into engine vector2 */
        Vector2 ConvertToNativeType(const aiVector2D& vector);

        /** Convert an assimp color3D into engine color */
        Color ConvertToNativeType(const aiColor3D& vector);

        /** Convert an assimp quaternion into engine quaternion */
        Quaternion ConvertToNativeType(const aiQuaternion& quaternion);

        void SetMeshImportOptions(const String& filePath, MeshImportOptions& meshImportOptions);

    private:
        Vector<String> _extensions;
    };
}
