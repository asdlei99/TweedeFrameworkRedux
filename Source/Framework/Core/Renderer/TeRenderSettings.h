#pragma once

#include "TeCorePrerequisites.h"
#include "Math/TeVector2.h"
#include "Image/TeColor.h"

namespace te
{
    /** Determines which parts of the scene will trigger motion blur. */
    enum class TE_CORE_EXPORT MotionBlurDomain
    {
        /** Camera movement and rotation will result in full-screen motion blur. */
        CameraOnly,

        /**
         * Object movement and rotation will result in blurring of the moving object. Can be significantly more
         * expensive than just using camera blur due to the requirement to use a velocity buffer (unless some
         * other effect also requires it, in which case it will be re-used).
         */
        ObjectOnly,

        /** Both the camera movement and object movement will result in motion blur. */
        CameraAndObject
    };

    /** Type of filter to use when filtering samples contributing to a blurred pixel. */
    enum class TE_CORE_EXPORT MotionBlurFilter
    {
        /** Samples will be simply averaged together to create the blurred pixel. */
        Simple,

        /**
         * A more advanced reconstruction filter will be used. This filter provides better blur quality at a
         * performance cost. In particular the filter will improve blurring at object boundaries, allowing blur
         * to extend beyond the object silhouette. It will also try to estimate blurred background and provide
         * better weighting between background, center and foreground samples.
         */
        Reconstruction
    };

    /** Determines the number of samples to take during motion blur filtering. */
    enum class TE_CORE_EXPORT MotionBlurQuality
    {
        /** 4 samples per pixel. */
        VeryLow = 0x0,
        /** 6 samples per pixel. */
        Low = 0x1,
        /** 8 samples per pixel. */
        Medium = 0x2,
        /** 12 samples per pixel. */
        High = 0x3,
        /** 16 samples per pixel. */
        Ultra = 0x4
    };

    /** Determines the number of samples to take during motion blur filtering. */
    enum class TE_CORE_EXPORT BloomQuality
    {
        /** 
         * Bloom texture 4 times smaller samples per pixel.
         * Only 5 samples horizontally and vertically
         */
        Low = 0x0,
        /** 
         * Bloom texture 2 times smaller samples per pixel.
         * Only 5 samples horizontally and vertically
         */
        Medium = 0x1,
        /** 
          * Bloom texture same size as RenderTarget
          * Max sample amount horizontally and vertically
         */
        High = 0x2
    };

    /** AmbientOcclusion quality. */
    enum class TE_CORE_EXPORT AmbientOcclusionQuality
    {
        Low = 0x0,
        Medium = 0x1,
        High = 0x2,
        Ultra = 0x3
    };

    /** Type of output we want */
    enum class TE_CORE_EXPORT RenderOutputType
    {
        Final     = 0x0,
        Color     = 0x1,
        Velocity  = 0x2,
        Emissive  = 0x3,
        Depth     = 0x4,
        Normal    = 0x5,
        SSAO      = 0x6
    };

    enum class TE_CORE_EXPORT AntiAliasingAlgorithm
    {
        FXAA = 0x0,
        TAA = 0x1,
        None = 0x2
    };

    /** Settings that control automatic exposure (eye adaptation) post-process. */
    struct TE_CORE_EXPORT AutoExposureSettings
    {
        AutoExposureSettings() = default;
    };

    /** Settings that control tonemap post-process. */
    struct TE_CORE_EXPORT TonemappingSettings
    {
        TonemappingSettings() = default;

        bool Enabled = true;
    };

    /** Settings that control screen space ambient occlusion. */
    struct TE_CORE_EXPORT AmbientOcclusionSettings
    {
        AmbientOcclusionSettings() = default;

        /** Enables or disables the screen space ambient occlusion effect. */
        bool Enabled = true;

        /**
         * Radius (in world space, in meters) over which occluders are searched for. Smaller radius ensures better sampling
         * precision but can miss occluders. Larger radius ensures far away occluders are considered but can yield lower
         * quality or noise because of low sampling precision. Usually best to keep at around a meter, valid range
         * is roughly [0.05, 5.0].
         */
        float Radius = 1.5f;

        /**
         * Bias used to reduce false occlusion artifacts. Higher values reduce the amount of artifacts but will cause
         * details to be lost in areas where occlusion isn't high. Value is in millimeters. Usually best to keep at a few
         * dozen millimeters, valid range is roughly [0, 200].
         */
        float Bias = 1.0f;

        /**
         * Distance (in view space, in meters) after which AO starts fading out. The fade process will happen over the
         * range as specified by @p FadeRange.
         */
        float FadeDistance = 500.0f;

        /**
         * Range (in view space, in meters) in which AO fades out from 100% to 0%. AO starts fading out after the distance
         * specified in @p fadeDistance.
         */
        float FadeRange = 50.0f;

        /**
         * Linearly scales the intensity of the AO effect. Values less than 1 make the AO effect less pronounced, and vice
         * versa. Valid range is roughly [0.2, 2].
         */
        float Intensity = 1.0f;

        /**
         * Controls how quickly does the AO darkening effect increase with higher occlusion percent. This is a non-linear
         * control and will cause the darkening to ramp up exponentially. Valid range is roughly [1, 4], where 1 means no
         * extra darkening will occur.
         */
        float Power = 4.0f;

        /**
         * Quality level of generated ambient occlusion. In range [0, 3]. Higher levels yield higher quality AO at the cost
         * of performance.
         */
        AmbientOcclusionQuality Quality = AmbientOcclusionQuality::Ultra;
    };

    /** Base class for both sim and core thread variants of DepthOfFieldSettings. */
    struct TE_CORE_EXPORT DepthOfFieldSettings
    {
        DepthOfFieldSettings() = default;

        /** Enables or disables the depth of field effect. */
        bool Enabled = false;

        /**
         * Distance from the camera at which the focal plane is located in. Objects at this distance will be fully in focus.
         * In world units (meters).
         */
        float FocalDistance = 0.75f;

        /**
         * Determines the size of the range within which objects transition from focused to fully unfocused, at the near
         * plane. Only relevant for Gaussian and Bokeh depth of field. In world units (meters).
         */
        float NearTransitionRange = 0.25f;

        /**
         * Determines the size of the range within which objects transition from focused to fully unfocused, at the far
         * plane. Only relevant for Gaussian and Bokeh depth of field. In world units (meters).
         */
        float FarTransitionRange = 0.25f;

        /**
         * Determines the maximum size of the blur kernel, in percent of view size. Larger values cost more performance.
         * Only relevant when using Bokeh depth of field.
         */
        float MaxBokehSize = 0.025f;

        /**
         * Determines the maximum color difference between surrounding pixels allowed (as a sum of all channels) before
         * higher fidelity sampling is triggered. Increasing this value can improve performance as less higher fidelity
         * samples will be required, but may decrease quality of the effect. Only relevant when using Bokeh depth of
         * field.
         */
        float AdaptiveColorThreshold = 1.0f;

        /**
         * Determines the minimum circle of confusion size before higher fidelity sampling is triggered. Small values
         * trigger high fidelity sampling because they can otherwise produce aliasing, and they are small enough so they
         * don't cost much. Increasing this value can improve performance as less higher fidelity samples will be required,
         * but may decrease quality of the effect. Only relevant when using Bokeh depth of field.
         */
        float AdaptiveRadiusThreshold = 0.1f;

        /** Camera aperture size in mm. Only relevant when using Bokeh depth of field. */
        float ApertureSize = 50.0f;

        /** Focal length of the camera's lens (e.g. 75mm). Only relevant when using Bokeh depth of field. */
        float FocalLength = 50.0f;

        /**
         * Camera sensor width and height, in mm. Used for controlling the size of the circle of confusion. Only relevant
         * when using Bokeh depth of field.
         */
        Vector2 SensorSize = Vector2(22.2f, 14.8f);
    };

    /** Settings that control the motion blur effect. */
    struct TE_CORE_EXPORT MotionBlurSettings
    {
        MotionBlurSettings() = default;

        /** Enables or disables the motion blur effect. */
        bool Enabled = true;

        /** Determines which parts of the scene will trigger motion blur. */
        MotionBlurDomain Domain = MotionBlurDomain::CameraOnly; // TODO

        /** Type of filter to use when filtering samples contributing to a blurred pixel. */
        MotionBlurFilter Filter = MotionBlurFilter::Reconstruction; // TODO

        /**
         * Determines the number of samples to take during motion blur filtering. Increasing this value will
         * yield higher quality blur at the cost of the performance.
         */
        MotionBlurQuality Quality = MotionBlurQuality::Ultra;
    };

    /** Settings that control the bloom effect. Bloom adds an extra highlight to bright areas of the scene. */
    struct TE_CORE_EXPORT BloomSettings
    {
        BloomSettings() = default;

        /** Enables or disables the bloom effect. */
        bool Enabled = true;

        /** Determines the intensity of the bloom effect. Ideally should be in [0, 4] range but higher values are allowed.*/
        float Intensity = 1.0f;

        /**
         * Determines the number of samples to take during bloom. Increasing this value will
         * yield higher quality blur at the cost of the performance. Reduce also generated bloom texture size.
         */
        BloomQuality Quality = BloomQuality::Medium;
    };

    /** Various options that control shadow rendering for a specific view. */
    struct TE_CORE_EXPORT ShadowsSettings
    {
        ShadowsSettings() = default;

        /**
         * Maximum distance that directional light shadows are allowed to render at. Decreasing the distance can yield
         * higher quality shadows nearer to the viewer, as the shadow map resolution isn't being used up on far away
         * portions of the scene. In world units (meters).
         */
        float DirectionalShadowDistance = 250.0f;

        /**
         * Number of cascades to use for directional shadows. Higher number of cascades increases shadow quality as each
         * individual cascade has less area to cover, but can significantly increase performance cost, as well as a minor
         * increase in memory cost. Valid range is roughly [1, 6].
         */
        UINT32 NumCascades = 4;

        /**
         * Allows you to control how are directional shadow cascades distributed. Value of 1 means the cascades will be
         * linearly split, each cascade taking up the same amount of space. Value of 2 means each subsequent split will be
         * twice the size of the previous one (meaning cascades closer to the viewer cover a smaller area, and therefore
         * yield higher resolution shadows). Higher values increase the size disparity between near and far cascades at
         * an exponential rate. Valid range is roughly [1, 4].
         */
        float CascadeDistributionExponent = 3.0f;

        /**
         * Determines the number of samples used for percentage closer shadow map filtering. Higher values yield higher
         * quality shadows, at the cost of performance. Valid range is [1, 4].
         */
        UINT32 ShadowFilteringQuality = 4;
    };

    /** Settings that control temporal anti-aliasing. */
    struct TE_CORE_EXPORT TemporalAASettings
    {
        TemporalAASettings() = default;

        /**
         * Number of different jittered positions to use. Each frame will use one position and subsequent frames
         * will use subsequent positions until this number of reached, at which point the positions start getting
         * re-used from the start.
         */
        UINT32 JitteredPositionCount = 8;

        /** Determines the distance between temporal AA samples. Larger values result in a sharper image. */
        float Sharpness = 1.0f;
    };

    /**
     * Settings that control the screen space reflections effect. Screen space reflections provide high quality mirror-like
     * reflections at low performance cost. They should be used together with reflection probes as the effects complement
     * each other. As the name implies, the reflections are only limited to geometry drawn on the screen and the system will
     * fall back to refl. probes when screen space data is unavailable. Similarly the system will fall back to refl. probes
     * for rougher (more glossy rather than mirror-like) surfaces. Those surfaces require a higher number of samples to
     * achieve the glossy look, so we instead fall back to refl. probes which are pre-filtered and can be quickly sampled.
     */
    struct TE_CORE_EXPORT ScreenSpaceReflectionsSettings
    {
        ScreenSpaceReflectionsSettings() = default;

        /** Enables or disables the SSR effect. */
        bool Enabled = true;

        /**
         * Quality of the SSR effect. Higher values cast more sample rays, and march those rays are lower increments for
         * better precision. This results in higher quality, as well as a higher performance requirement. Valid range is
         * [0, 4], default is 2.
         */
        UINT32 Quality = 2;

        /** Intensity of the screen space reflections. Valid range is [0, 1]. Default is 1 (100%). */
        float intensity = 1.0f;

        /**
         * Roughness at which screen space reflections start fading out and become replaced with refl. probes. Valid range
         * is [0, 1]. Default is 0.8.
         */
        float MaxRoughness = 0.8f;
    };

    /** RenderSettings struct */
    struct TE_CORE_EXPORT RenderSettings
    {
        RenderSettings() = default;

        /**
         * Determines should automatic exposure be applied to the HDR image. When turned on the average scene brightness
         * will be calculated and used to automatically expose the image to the optimal range. Use the parameters provided
         * by autoExposure to customize the automatic exposure effect. You may also use exposureScale to
         * manually adjust the automatic exposure. When automatic exposure is turned off you can use exposureScale to
         * manually set the exposure.
         */
        bool EnableAutoExposure = false;

        /**
         * Parameters used for customizing automatic scene exposure.
         *
         * @see	enableAutoExposure
         */
         AutoExposureSettings AutoExposure; // TODO not used currently

        /**
         * Determines should the image be tonemapped. Tonemapping converts an HDR image into LDR image by applying
         * a filmic curve to the image, simulating the effect of film cameras. Filmic curve improves image quality by
         * tapering off lows and highs, preventing under- and over-exposure. This is useful if an image contains both
         * very dark and very bright areas, in which case the global exposure parameter would leave some areas either over-
         * or under-exposed. Use #tonemapping to customize how tonemapping performed.
         *
         * If this is disabled, then color grading and white balancing will not be enabled either. Only relevant for HDR
         * images.
         */
        TonemappingSettings Tonemapping;

        /** Parameters used for customizing screen space ambient occlusion. */
        AmbientOcclusionSettings AmbientOcclusion;

        /** Parameters used for customizing screen space reflections. */
        ScreenSpaceReflectionsSettings ScreenSpaceReflections;

        /** Parameters used for customizing the bloom effect. */
        BloomSettings Bloom;

        /** Parameters used for customizing the motion blur effect. */
        MotionBlurSettings MotionBlur;

        /** Parameters used for customizing the gaussian depth of field effect. */
        DepthOfFieldSettings DepthOfField;

        /** Parameters used for customizing the temporal anti-aliasing effect. */
        TemporalAASettings TemporalAA;

        /** Parameters used for customizing shadow rendering. */
        ShadowsSettings ShadowSettings;

        /** For debug purpose, we could want to see different output */
        RenderOutputType OutputType = RenderOutputType::Final;

        /** Enables the fast approximate anti-aliasing effect. */
        AntiAliasingAlgorithm AntialiasingAglorithm = AntiAliasingAlgorithm::FXAA;

        /**
         * Log2 value to scale the eye adaptation by (for example 2^0 = 1). Smaller values yield darker image, while larger
         * yield brighter image. Allows you to customize exposure manually, applied on top of eye adaptation exposure (if
         * enabled). In range [-8, 8].
         */
        float ExposureScale = 0.85f;

        /**
         * Gamma value to adjust the image for. Larger values result in a brighter image. When tonemapping is turned
         * on the best gamma curve for the output device is chosen automatically and this value can by used to merely tweak
         * that curve. If tonemapping is turned off this is the exact value of the gamma curve that will be applied.
         */
        float Gamma =1.0f;

        /**
         * High dynamic range allows light intensity to be more correctly recorded when rendering by allowing for a larger
         * range of values. The stored light is then converted into visible color range using exposure and a tone mapping
         * operator.
         */
        bool EnableHDR = true;

        /**
         * Determines if scene objects will be lit by lights. If disabled everything will be rendered using their albedo
         * texture with no lighting applied.
         */
        bool EnableLighting = true;

        /** Determines if shadows cast by lights should be rendered. Only relevant if lighting is turned on. */
        bool EnableShadows = true;

        /** 
         * It's possible to generate at each frame a dynamic cubemap environment texture. For each material, if
         * UseDynamicEnvironmentMap is set, renderer will try to find a lighprobe for this object and material. 
         * If no lightprobe has been found a cubemap will be generated and for each object using this material, this texture will
         * be bound on texture slot "EnvironmentMap"
         * Only renderables with UseForDynamicEnvMapping set to true will be use for rendering
         */
        bool EnableDynamicEnvMapping = false;

        /**
         * Signals the renderer to only render overlays (like GUI), and not scene objects. Such rendering doesn't require
         * depth buffer or multi-sampled render targets and will not render any scene objects. This can improve performance
         * and memory usage for overlay-only views.
         */
        bool OverlayOnly = false;

        /**
         * If enabled the camera will use the skybox for rendering the background. A skybox has to be present in the scene.
         * When disabled the camera will use the clear color for rendering the background.
         */
        bool EnableSkybox = true;

        /**
         * Add contrast to image output
         */
        float Contrast = 2.0f;

        /**
         * make image brighter
         */
        float Brightness = 0.1f;

        /**
         * The absolute base cull-distance for objects rendered through this camera in world units. Objects will use this
         * distance and apply their own factor to it to determine whether they should be visible.
         */
        float CullDistance = 5000.0f;

        /** 
         * It's possible to define a scene color which will be used on every object rendered with this camera
         * It's useful to control globally brightness of a scene without using to much lights
         */
        Color SceneLightColor = Color(1.0f, 1.0f, 1.0f, 1.0f);

        /** 
         * If Skybox is enabled for this camera and if a Skybox with a valid CubeMap is present in the scene, 
         * we will use this Skybox to achieve Global Illumination of each object rendered
         */
        bool UseGlobalIllumination = true;
    };
}
