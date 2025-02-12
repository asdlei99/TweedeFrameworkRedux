#include "Audio/TeAudioClip.h"
#include "Audio/TeAudio.h"
#include "Utility/TeDataStream.h"
#include "Resources/TeResourceManager.h"

namespace te
{
    AudioClip::AudioClip(const SPtr<DataStream>& samples, UINT32 streamSize, UINT32 numSamples, const AUDIO_CLIP_DESC& desc)
        : Resource(TID_AudioClip)
        , _desc(desc)
        , _numSamples(numSamples)
        , _streamSize(streamSize)
        , _streamData(samples)
    {
        if (samples != nullptr)
            _streamOffset = (UINT32)samples->Tell();
    }

    void AudioClip::Initialize()
    {
        _length = _numSamples / _desc.NumChannels / (float)_desc.Frequency;
        Resource::Initialize();
    }

    HAudioClip AudioClip::Create(const SPtr<DataStream>& samples, UINT32 streamSize, UINT32 numSamples, const AUDIO_CLIP_DESC& desc)
    {
        return static_resource_cast<AudioClip>(gResourceManager()._createResourceHandle(CreatePtr(samples, streamSize, numSamples, desc)));
    }

    SPtr<AudioClip> AudioClip::CreatePtr(const SPtr<DataStream>& samples, UINT32 streamSize, UINT32 numSamples, const AUDIO_CLIP_DESC& desc)
    {
        SPtr<AudioClip> newClip = gAudio().CreateClip(samples, streamSize, numSamples, desc);
        newClip->SetThisPtr(newClip);
        newClip->Initialize();

        return newClip;
    }

    SPtr<AudioClip> AudioClip::CreateEmpty()
    {
        AUDIO_CLIP_DESC desc;
        SPtr<AudioClip> newClip = gAudio().CreateClip(nullptr, 0, 0, desc);
        newClip->SetThisPtr(newClip);

        return newClip;
    }
}
