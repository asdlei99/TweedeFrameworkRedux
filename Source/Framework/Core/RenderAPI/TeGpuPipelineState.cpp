#include "TeGpuPipelineState.h"
#include "RenderAPI/TeRenderStateManager.h"
#include "RenderAPI/TeGpuProgram.h"
#include "RenderAPI/TeRasterizerState.h"
#include "RenderAPI/TeBlendState.h"
#include "RenderAPI/TeGpuParamDesc.h"
#include "RenderAPI/TeGpuPipelineParamInfo.h"
#include "RenderAPI/TeRenderStateManager.h"

namespace te
{
    GraphicsPipelineState::GraphicsPipelineState(const PIPELINE_STATE_DESC& desc, GpuDeviceFlags deviceMask)
        : _data(desc)
        , _deviceMask(deviceMask)
    { }

    GraphicsPipelineState::~GraphicsPipelineState()
    { }

    void GraphicsPipelineState::Initialize()
    {
        GPU_PIPELINE_PARAMS_DESC paramsDesc;

        if (_data.vertexProgram != nullptr)
            paramsDesc.VertexParams = _data.vertexProgram->GetParamDesc();
        if (_data.pixelProgram != nullptr)
            paramsDesc.PixelParams = _data.pixelProgram->GetParamDesc();
        if (_data.geometryProgram != nullptr)
            paramsDesc.GeometryParams = _data.geometryProgram->GetParamDesc();
        if (_data.hullProgram != nullptr)
            paramsDesc.HullParams = _data.hullProgram->GetParamDesc();
        if (_data.domainProgram != nullptr)
            paramsDesc.DomainParams = _data.domainProgram->GetParamDesc();

        _paramInfo = GpuPipelineParamInfo::Create(paramsDesc);

        CoreObject::Initialize();
    }

    SPtr<GraphicsPipelineState> GraphicsPipelineState::Create(const PIPELINE_STATE_DESC& desc, GpuDeviceFlags deviceMask)
    {
        return RenderStateManager::Instance().CreateGraphicsPipelineState(desc, deviceMask);
    }

    ComputePipelineState::ComputePipelineState()
        : _program(nullptr)
    { }

    ComputePipelineState::ComputePipelineState(const SPtr<GpuProgram>& program, GpuDeviceFlags deviceMask)
        : _program(program)
        , _deviceMask(deviceMask)
    { }

    void ComputePipelineState::Initialize()
    {
        GPU_PIPELINE_PARAMS_DESC paramsDesc;
        paramsDesc.ComputeParams = _program->GetParamDesc();

        _paramInfo = GpuPipelineParamInfo::Create(paramsDesc, _deviceMask);

        CoreObject::Initialize();
    }

    SPtr<ComputePipelineState> ComputePipelineState::Create(const SPtr<GpuProgram>& program,
        GpuDeviceFlags deviceMask)
    {
        return RenderStateManager::Instance().CreateComputePipelineState(program, deviceMask);
    }
}
