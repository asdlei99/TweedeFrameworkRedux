#include "Include/ForwardBase.hlsli"

cbuffer PerCameraBuffer : register(b0)
{
    float3 gViewDir;
    float  gPadding1;
    float3 gViewOrigin;
    float  gPadding2;
    matrix gMatViewProj;
    matrix gMatView;
    matrix gMatProj;
    matrix gMatPrevViewProj;
    matrix gNDCToPrevNDC;
    float4 gClipToUVScaleOffset;
    float4 gUVToClipScaleOffset;
}

cbuffer PerMaterialBuffer : register(b1)
{
    float4 gAmbient;
    float4 gDiffuse;
    float4 gEmissive;
    float4 gSpecular;
    float2 gTextureRepeat;
    float2 gTextureOffset;
    uint   gUseDiffuseMap;
    uint   gUseEmissiveMap;
    uint   gUseNormalMap;
    uint   gUseSpecularMap;
    uint   gUseBumpMap;
    uint   gUseParallaxMap;
    uint   gUseTransparencyMap;
    uint   gUseReflectionMap;
    uint   gUseOcclusionMap;
    uint   gUseEnvironmentMap;
    float  gSpecularPower;
    float  gSpecularStrength;
    float  gTransparency;
    float  gIndexOfRefraction;
    float  gRefraction;
    float  gReflection;
    float  gBumpScale;
    float  gParallaxScale;
    float  gAlphaThreshold;
    float  gPadding3;
};

cbuffer PerLightsBuffer : register(b2)
{
    LightData gLights[MAX_LIGHTS];
    uint gLightsNumber;
    float3 gPadding4;
}

cbuffer PerFrameBuffer : register(b3)
{
    float gTime;
    float gFrameDelta;
    float2 gPadding5;
    float4 gSceneLightColor;
}

SamplerState AnisotropicSampler : register(s0);

Texture2D DiffuseMap : register(t0);
Texture2D EmissiveMap : register(t1);
Texture2D NormalMap : register(t2);
Texture2D SpecularMap : register(t3);
Texture2D BumpMap : register(t4);
Texture2D ParallaxMap : register(t5);
Texture2D TransparencyMap : register(t6);
Texture2D ReflectionMap : register(t7);
Texture2D OcclusionMap : register(t8);
TextureCube EnvironmentMap : register(t9);

static const float FrameDelta = 1 / 60.0;

float4 ComputeNormalBuffer(float4 normal)
{
    return float4(normal.xyz, 1.0);
}

float4 ComputeEmissiveBuffer(float4 color, float4 emissive)
{
    if(any(emissive != (float4)0))
    {
        return emissive;
    }
    else
    {
        float brightness = dot(color.rgb, float3(0.2126, 0.7152, 0.0722));
        if(brightness > 1.0)
            return float4(color.rgb, 1.0);
        else
            return float4(0.0, 0.0, 0.0, 1.0);
    }
}

float4 ComputeVelocityBuffer(float4 position, float4 prevPosition, float alpha)
{
    float2 velocity = (float2)0;
    float4 output = float4(0.0, 0.0, 0.0, 1.0);

    if(alpha >= 1.0)
    {
        if(all(position =! prevPosition))
        {
            float2 a = float2(position.xy);
            float2 b = float2(prevPosition.xy);

            velocity = (b - a);

            if(velocity.x < -0.99) velocity.x = -0.99;
            if(velocity.y < -0.99) velocity.y = -0.99;

            if(velocity.x > 0.99) velocity.x = 0.99;
            if(velocity.y > 0.99) velocity.y = 0.99;

            velocity /= 2.0;
            velocity += 0.5;

            output.xy = velocity.xy;
        }
    }

    return output;
}

struct LightingResult
{
    float3 Diffuse;
    float3 Specular;
};

// d : distance from light
float DoAttenuation( LightData light, float d )
{
    return 1.0f / ( light.AttenuationRadius + light.LinearAttenuation * d + light.QuadraticAttenuation * d * d );
}

// V : view vector
// N : surface normal
float3 DoDiffuse( LightData light, float3 L, float3 N )
{
    float NdotL = max( 0, dot( N, L ) );
    return light.Color * NdotL * light.Intensity;
}

// V : view vector
// L : light vector
// N : surface normal
float3 DoSpecular( LightData light, float3 V, float3 L, float3 N )
{
    // Phong lighting.
    float3 R = normalize( reflect( -L, N ) );
    float RdotV = max( 0, dot( R, V ) );

    // Blinn-Phong lighting
    // float3 H = normalize( L + V );
    // float NdotH = max( 0, dot( N, H ) );

    return light.Color * pow( RdotV, gSpecularPower ) * light.Intensity * gSpecularStrength;
}

// V : view vector
// P : position vector in worldspace
// N : surface normal
LightingResult DoPointLight( LightData light, float3 V, float3 P, float3 N )
{
    LightingResult result = (LightingResult)0;

    float3 L = ( light.Position - P );
    float distance = length(L);
    L = L / distance;

    float attenuation = DoAttenuation( light, distance );
    if(attenuation > 0.00001f)
    {
        result.Diffuse = DoDiffuse( light, L, N ) * attenuation;
        result.Specular = DoSpecular( light, V, L, N ) * attenuation;
    }

    return result;
}

// V : view vector
// P : position vector in worldspace
// N : surface normal
LightingResult DoDirectionalLight( LightData light, float3 V, float3 P, float3 N )
{
    LightingResult result = (LightingResult)0; 

    float3 L = -light.Direction.xyz;
 
    result.Diffuse = DoDiffuse( light, L, N );
    result.Specular = DoSpecular( light, V, L, N );

    return result;
}

// L : light vector
float DoSpotCone( LightData light, float3 L )
{
    float minCos = cos( light.SpotAngles.x );
    float maxCos = ( minCos + 1.0f ) / 2.0f;
    float cosAngle = dot( light.Direction.xyz, -L );
    return smoothstep( minCos, maxCos, cosAngle ); 
}

// V : view vector
// P : position vector in worldspace
// N : surface normal
LightingResult DoSpotLight( LightData light, float3 V, float3 P, float3 N )
{
    LightingResult result = (LightingResult)0;

    float3 L = ( light.Position - P );
    float distance = length(L);
    L = L / distance;

    float attenuation = DoAttenuation( light, distance );
    if(attenuation > 0.00001f)
    {
        float spotIntensity = DoSpotCone( light, L );
        if(spotIntensity > 0.001f)
        {
            result.Diffuse = DoDiffuse( light, L, N ) * attenuation * spotIntensity;
            result.Specular = DoSpecular( light, V, L, N ) * attenuation * spotIntensity;
        }
    }

    return result;
}

// P : position vector in world space
// N : normal
LightingResult ComputeLighting( float3 P, float3 N, uint castLight )
{
    float3 V = normalize( gViewOrigin - P );
    LightingResult totalResult = { {0, 0, 0}, {0, 0, 0} };

    //if(castLight == 1)
    {
        [unroll]
        for( uint i = 0; i < gLightsNumber; ++i )
        {
            LightingResult result = { {0, 0, 0}, {0, 0, 0} };

            if(gLights[i].Type == DIRECTIONAL_LIGHT)
                result = DoDirectionalLight( gLights[i], V, P, N );
            else if(gLights[i].Type == POINT_LIGHT)
                result = DoPointLight( gLights[i], V, P, N );
            else if(gLights[i].Type == SPOT_LIGHT)
                result = DoSpotLight( gLights[i], V, P, N );

            totalResult.Diffuse += result.Diffuse;
            totalResult.Specular += result.Specular;
        }

        totalResult.Diffuse = saturate(totalResult.Diffuse);
        totalResult.Specular = saturate(totalResult.Specular);
    }

    return totalResult;
}

float3 DoReflection(float3 P, float3 N)
{
    float3 I = normalize(P - gViewOrigin);
    float3 R = reflect(I, normalize(N));

    return EnvironmentMap.SampleLevel(AnisotropicSampler, R, 0).xyz * gReflection;
}

float3 DoRefraction(float3 P, float3 N)
{
    float ratio = 1.0 / gIndexOfRefraction;
    float3 I = normalize(P - gViewOrigin);
    float3 R = refract(I, normalize(N), ratio);

    return EnvironmentMap.SampleLevel(AnisotropicSampler, R, 0).xyz * gRefraction;
}

float2 DoParallaxMapping(float2 texCoords, float2 parallaxOffsetTS, int nNumSteps, float bumpScale)
{
    float2 dx = ddx( texCoords );
    float2 dy = ddy( texCoords );

    float fCurrHeight = 0.0;
    float fStepSize   = 1.0 / (float) nNumSteps;
    float fPrevHeight = 1.0;
    float fNextHeight = 0.0;

    int    nStepIndex = 0;
    bool   bCondition = true;

    float2 vTexOffsetPerStep = fStepSize * parallaxOffsetTS * bumpScale;
    float2 vTexCurrentOffset = texCoords;
    float  fCurrentBound     = 1.0;
    float  fParallaxAmount   = 0.0;

    float2 pt1 = 0;
    float2 pt2 = 0;

    float2 texOffset2 = 0;

    while ( nStepIndex < nNumSteps ) 
    {
        vTexCurrentOffset -= vTexOffsetPerStep;

        // Sample height map which in this case is stored in the alpha channel of the normal map:
        fCurrHeight = ParallaxMap.SampleGrad( AnisotropicSampler, vTexCurrentOffset, dx, dy ).r;

        fCurrentBound -= fStepSize;

        if ( fCurrHeight > fCurrentBound ) 
        {
            pt1 = float2( fCurrentBound, fCurrHeight );
            pt2 = float2( fCurrentBound + fStepSize, fPrevHeight );

            texOffset2 = vTexCurrentOffset - vTexOffsetPerStep;

            nStepIndex = nNumSteps + 1;
        }
        else
        {
            nStepIndex++;
            fPrevHeight = fCurrHeight;
        }
    }

    float fDelta2 = pt2.x - pt2.y;
    float fDelta1 = pt1.x - pt1.y;
    float fDenominator = fDelta2 - fDelta1;

    // SM 3.0 and above requires a check for divide by zero since that operation will generate an 'Inf' number instead of 0
    [flatten]if ( fDenominator == 0.0f ) 
        fParallaxAmount = 0.0f;
    else
        fParallaxAmount = ( pt1.x * fDelta2 - pt2.x * fDelta1 ) / fDenominator;

    float2 vParallaxOffset = parallaxOffsetTS  * bumpScale * ( 1.0 - fParallaxAmount );

    return texCoords - vParallaxOffset;
}

[earlydepthstencil]
PS_OUTPUT main( PS_INPUT IN )
{
    PS_OUTPUT OUT = (PS_OUTPUT)0;
    float alpha = gTransparency;
    uint writeVelocity = (uint)IN.Other.x;
    uint castLight = (uint)IN.Other.y;

    if(gUseTransparencyMap == 1)
        alpha = TransparencyMap.Sample( AnisotropicSampler, IN.Texture ).r;

    if(alpha < gAlphaThreshold)
    {
        OUT.Scene = (float4)0;
        OUT.Normal = (float4)0;
        OUT.Emissive = (float4)0;
        OUT.Velocity = (float4)0;
    }
    else
    {
        OUT.Scene  = float4(1.0f, 1.0f, 1.0f, 1.0f);

        float3 albedo      = gDiffuse.rgb;
        float3 ambient     = gAmbient.rgb;
        float3 diffuse     = gDiffuse.rgb;
        float3 emissive    = gEmissive.rgb;
        float3 specular    = gSpecular.rgb;
        float3 environment = (float3)0;
        float3 normal      = IN.Normal;
        float2 texCoords   = (IN.Texture * gTextureRepeat) + gTextureOffset;

        float3x3 TBN = float3x3(IN.Tangent.xyz, IN.BiTangent.xyz, IN.Normal.xyz);

        if(gUseParallaxMap == 1)
        {
            // Utilize dynamic flow control to change the number of samples per ray 
            // depending on the viewing angle for the surface. Oblique angles require 
            // smaller step sizes to achieve more accurate precision for computing displacement.
            // We express the sampling rate as a linear function of the angle between 
            // the geometric normal and the view direction ray:
            int parallaxSteps = (int)lerp( PARALLAX_MAX_SAMPLE, PARALLAX_MIN_SAMPLE, dot( IN.ViewDirWS, IN.Normal ) );

            texCoords = DoParallaxMapping(texCoords, IN.ParallaxOffsetTS, parallaxSteps, gParallaxScale);
        }
        if(gUseReflectionMap == 1)
            { /* TODO */ }
        if(gUseNormalMap == 1)
            normal = DoNormalMapping(TBN, NormalMap, AnisotropicSampler, texCoords);
        if(gUseBumpMap == 1)
            normal = DoBumpMapping(TBN, BumpMap, AnisotropicSampler, texCoords, gBumpScale);
        if(gUseDiffuseMap == 1)
        {
            albedo = DiffuseMap.Sample(AnisotropicSampler, texCoords).rgb;
            ambient = albedo;
        }
        if(gUseSpecularMap == 1)
            specular.rgb = SpecularMap.Sample(AnisotropicSampler, texCoords).xyz;
        if(gUseEmissiveMap == 1)
            emissive = emissive * EmissiveMap.Sample(AnisotropicSampler, texCoords).rgb;
        if(gUseOcclusionMap == 1)
            albedo = albedo * OcclusionMap.Sample(AnisotropicSampler, texCoords).rgb;

        LightingResult lit = ComputeLighting(IN.PositionWS.xyz, normalize(normal), castLight);

        if(gUseEnvironmentMap == 1)
        {
            if(gIndexOfRefraction != 0.0)
                environment = DoRefraction(IN.PositionWS.xyz, normal);
            if(gReflection != 0.0)
                environment = environment + DoReflection(IN.PositionWS.xyz, normal);

            float reflectAndRefract = gReflection + gRefraction;
            if(reflectAndRefract > 1.0) reflectAndRefract = 1.0;
            albedo = albedo * (1.0 - reflectAndRefract);
        }

        diffuse = diffuse * lit.Diffuse.rgb;
        specular = specular * lit.Specular.rgb;

        OUT.Scene.rgb = (gSceneLightColor.rgb * ambient + emissive + diffuse + specular) * (albedo + environment);
        OUT.Scene.a = alpha;

        float3 NDCPos = (IN.CurrPosition / IN.CurrPosition.w).xyz;
        float3 PrevNDCPos = (IN.PrevPosition / IN.PrevPosition.w).xyz;

        OUT.Normal = ComputeNormalBuffer(float4(normal, 0.0f));
        OUT.Emissive = ComputeEmissiveBuffer(OUT.Scene, float4(emissive, 1.0));

        if(writeVelocity == 1)
            OUT.Velocity = ComputeVelocityBuffer(float4(NDCPos, 0.0), float4(PrevNDCPos, 0.0), alpha);
        else
            OUT.Velocity = float4(0.0, 0.0, 0.0, 1.0);
    }

    return OUT;
}
