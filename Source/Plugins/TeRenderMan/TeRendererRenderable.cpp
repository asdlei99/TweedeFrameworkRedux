#include "TeRendererRenderable.h"
#include "Mesh/TeMesh.h"
#include "Utility/TeBitwise.h"
#include "Renderer/TeRendererUtility.h"

namespace te
{ 
    PerInstanceParamDef gPerInstanceParamDef;
    PerMaterialParamDef gPerMaterialParamDef;
    PerObjectParamDef gPerObjectParamDef;

    void PerObjectBuffer::UpdatePerObject(SPtr<GpuParamBlockBuffer>& buffer, const Matrix4& tfrm,
        const Matrix4& prevTfrm, Renderable* renderable)
    {
        const Matrix4& tfrmNoScale = renderable->GetMatrixNoScale();
        const UINT32 layer = Bitwise::mostSignificantBit(renderable->GetLayer());

        gPerObjectParamDef.gMatWorld.Set(buffer, tfrm);
        gPerObjectParamDef.gMatInvWorld.Set(buffer, tfrm.InverseAffine());
        gPerObjectParamDef.gMatWorldNoScale.Set(buffer, tfrmNoScale);
        gPerObjectParamDef.gMatInvWorldNoScale.Set(buffer, tfrmNoScale.InverseAffine());
        gPerObjectParamDef.gMatPrevWorld.Set(buffer, prevTfrm);
        gPerObjectParamDef.gLayer.Set(buffer, (INT32)layer);
        gPerObjectParamDef.gHasAnimation.Set(buffer, (UINT32)renderable->IsAnimated() ? 1 : 0);
        gPerObjectParamDef.gWriteVelocity.Set(buffer, (UINT32)renderable->GetWriteVelocity() ? 1 : 0);
        gPerObjectParamDef.gCastLight.Set(buffer, (UINT32)renderable->GetCastLight() ? 1 : 0);
    }

    void PerObjectBuffer::UpdatePerInstance(SPtr<GpuParamBlockBuffer>& perObjectBuffer, 
        SPtr<GpuParamBlockBuffer>& perInstanceBuffer, PerInstanceData* instanceData, UINT32 instanceCounter)
    {
        for (size_t i = 0; i < instanceCounter; i++)
            gPerInstanceParamDef.gInstances.Set(perInstanceBuffer, instanceData[i], (UINT32)i);
    }

    void PerObjectBuffer::UpdatePerMaterial(SPtr<GpuParamBlockBuffer>& perMaterialBuffer, const MaterialProperties& properties)
    {
        MaterialData data = ConvertMaterialProperties(properties);

        gPerMaterialParamDef.gAmbient.Set(perMaterialBuffer, data.gAmbient);
        gPerMaterialParamDef.gDiffuse.Set(perMaterialBuffer, data.gDiffuse);
        gPerMaterialParamDef.gEmissive.Set(perMaterialBuffer, data.gEmissive);
        gPerMaterialParamDef.gSpecular.Set(perMaterialBuffer, data.gSpecular);
        gPerMaterialParamDef.gTextureRepeat.Set(perMaterialBuffer, data.gTextureRepeat);
        gPerMaterialParamDef.gTextureOffset.Set(perMaterialBuffer, data.gTextureOffset);
        gPerMaterialParamDef.gUseDiffuseMap.Set(perMaterialBuffer, data.gUseDiffuseMap);
        gPerMaterialParamDef.gUseEmissiveMap.Set(perMaterialBuffer, data.gUseEmissiveMap);
        gPerMaterialParamDef.gUseNormalMap.Set(perMaterialBuffer, data.gUseNormalMap);
        gPerMaterialParamDef.gUseSpecularMap.Set(perMaterialBuffer, data.gUseSpecularMap);
        gPerMaterialParamDef.gUseBumpMap.Set(perMaterialBuffer, data.gUseBumpMap);
        gPerMaterialParamDef.gUseParallaxMap.Set(perMaterialBuffer, data.gUseParallaxMap);
        gPerMaterialParamDef.gUseTransparencyMap.Set(perMaterialBuffer, data.gUseTransparencyMap);
        gPerMaterialParamDef.gUseReflectionMap.Set(perMaterialBuffer, data.gUseReflectionMap);
        gPerMaterialParamDef.gUseOcclusionMap.Set(perMaterialBuffer, data.gUseOcclusionMap);
        gPerMaterialParamDef.gUseEnvironmentMap.Set(perMaterialBuffer, data.gUseEnvironmentMap);
        gPerMaterialParamDef.gUseIrradianceMap.Set(perMaterialBuffer, data.gUseIrradianceMap);
        gPerMaterialParamDef.gUseGlobalIllumination.Set(perMaterialBuffer, data.gUseGlobalIllumination);
        gPerMaterialParamDef.gSpecularPower.Set(perMaterialBuffer, data.gSpecularPower);
        gPerMaterialParamDef.gSpecularStrength.Set(perMaterialBuffer, data.gSpecularStrength);
        gPerMaterialParamDef.gTransparency.Set(perMaterialBuffer, data.gTransparency);
        gPerMaterialParamDef.gIndexOfRefraction.Set(perMaterialBuffer, data.gIndexOfRefraction);
        gPerMaterialParamDef.gRefraction.Set(perMaterialBuffer, data.gRefraction);
        gPerMaterialParamDef.gReflection.Set(perMaterialBuffer, data.gReflection);
        gPerMaterialParamDef.gBumpScale.Set(perMaterialBuffer, data.gBumpScale);
        gPerMaterialParamDef.gParallaxScale.Set(perMaterialBuffer, data.gParallaxScale);
        gPerMaterialParamDef.gAlphaThreshold.Set(perMaterialBuffer, data.gAlphaThreshold);
        gPerMaterialParamDef.gParallaxSamples.Set(perMaterialBuffer, data.gParallaxSamples);
    }

    MaterialData PerObjectBuffer::ConvertMaterialProperties(const MaterialProperties& properties)
    {
        MaterialData data;
        data.gAmbient = properties.Ambient.GetAsVector4();
        data.gDiffuse = properties.Diffuse.GetAsVector4();
        data.gEmissive = properties.Emissive.GetAsVector4();
        data.gSpecular = properties.Specular.GetAsVector4();
        data.gTextureRepeat = properties.TextureRepeat;
        data.gTextureOffset = properties.TextureOffset;
        data.gUseDiffuseMap = (UINT32)properties.UseDiffuseMap ? 1 : 0;
        data.gUseEmissiveMap = (UINT32)properties.UseEmissiveMap ? 1 : 0;
        data.gUseNormalMap = (UINT32)properties.UseNormalMap ? 1 : 0;
        data.gUseSpecularMap = (UINT32)properties.UseSpecularMap ? 1 : 0;
        data.gUseBumpMap = (UINT32)properties.UseBumpMap ? 1 : 0;
        data.gUseParallaxMap = (UINT32)properties.UseParallaxMap ? 1 : 0;
        data.gUseTransparencyMap = (UINT32)properties.UseTransparencyMap ? 1 : 0;
        data.gUseReflectionMap = (UINT32)properties.UseReflectionMap ? 1 : 0;
        data.gUseOcclusionMap = (UINT32)properties.UseOcclusionMap ? 1 : 0;
        data.gUseEnvironmentMap = (UINT32)properties.UseEnvironmentMap ? 1 : 0;
        data.gUseIrradianceMap = (UINT32)properties.UseIrradianceMap ? 1 : 0;
        data.gUseGlobalIllumination = (UINT32)properties.UseGlobalIllumination ? 1 : 0;
        data.gSpecularPower = properties.SpecularPower;
        data.gSpecularStrength = properties.SpecularStrength;
        data.gTransparency = properties.Transparency;
        data.gIndexOfRefraction = properties.IndexOfRefraction;
        data.gRefraction = properties.Refraction;
        data.gReflection = properties.Reflection;
        data.gBumpScale = properties.BumpScale;
        data.gParallaxScale = properties.ParallaxScale;
        data.gAlphaThreshold = properties.AlphaThreshold;
        data.gParallaxSamples = properties.ParallaxSamples;

        return data;
    }

    RenderableElement::RenderableElement(bool createPerMaterialBuffer)
        : RenderElement()
    {
        if(createPerMaterialBuffer)
            PerMaterialParamBuffer = gPerMaterialParamDef.CreateBuffer();
    }

    void RenderableElement::Draw() const
    {
        gRendererUtility().Draw(MeshElem, *SubMeshElem, InstanceCount);
    }

    RendererRenderable::RendererRenderable()
    {
        PerObjectParamBuffer = gPerObjectParamDef.CreateBuffer();
    }

    RendererRenderable::~RendererRenderable()
    { }

    void RendererRenderable::UpdatePerObjectBuffer()
    {
        PerObjectBuffer::UpdatePerObject(PerObjectParamBuffer, WorldTfrm, PrevWorldTfrm, RenderablePtr);
    }

    void RendererRenderable::UpdatePerInstanceBuffer(PerInstanceData* instanceData, UINT32 instanceCounter, UINT32 blockId)
    {
        PerObjectBuffer::UpdatePerInstance(PerObjectParamBuffer, gPerInstanceParamBuffer[blockId], instanceData, instanceCounter);
    }
}
