struct VSOutput
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
    float4 WorldPos : WORLD_POS;
};

struct PSOutput
{
    float4 Color : SV_TARGET;
};

Texture2D ColorMap : register(t0);
SamplerState WrapSmp : register(s0);

cbuffer LightBuffer : register(b2)
{
    float3 LightPosition  : packoffset(c0); //!< ライト位置
    float3 LightColor     : packoffset(c1); //!< ライトカラー
    float3 CameraPosition : packoffset(c2); //!< カメラ位置
}

cbuffer MaterialBuffer : register(b3)
{
    float3 Diffuse      : packoffset(c0);
    float  Alpha        : packoffset(c0.w);
    float3 Specular     : packoffset(c1);
    float  Shininess    : packoffset(c1.w);
}

static const float F_PI = 3.141596535f;

PSOutput main(VSOutput input)
{
    PSOutput output = (PSOutput) 0;
    
    float3 N = normalize(input.Normal);
    float3 L = normalize(LightPosition - input.WorldPos.xyz);
    float3 V = normalize(CameraPosition - input.WorldPos.xyz);
    float3 R = normalize(-reflect(V, N));
    
    float NL = saturate(dot(N, L));
    float LR = saturate(dot(L, R));
    
    float4 color    = ColorMap.Sample(WrapSmp, input.TexCoord);
    float3 diffuse  = Diffuse * NL * (1.0f / F_PI) + 0.3f; //!< 暗いので少し明るくする
    float3 specular = Specular * ((Shininess + 2.0f) / (2.0f * F_PI)) * pow(LR, Shininess);
    
    if(color.x > 0.0f || color.y > 0.0f || color.z > 0.0f)
        output.Color = float4(LightColor * color.rgb * (diffuse + specular), color.a * Alpha);
    else
        output.Color = float4(LightColor * (diffuse + specular), color.a * Alpha);
 
    return output;
}