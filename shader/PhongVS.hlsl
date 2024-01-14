cbuffer Transform : register(b0)
{
    float4x4 View  : packoffset(c0);
    float4x4 Proj  : packoffset(c4);
}
    
cbuffer WorldTransform : register(b1)
{
    float4x4 WorldInstanceData[500];
}

struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float3 Tangent : TANGENT;
    uint InstanceID : SV_InstanceID;
};

struct VSOutput
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
    float4 WorldPos : WORLD_POS;
};

VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    
    uint index = input.InstanceID;
    float4 LocalPos = float4(input.Position, 1.0f);
    float4 WorldPos = mul(WorldInstanceData[index], LocalPos);
    float4 ViewPos = mul(View, WorldPos);
    float4 ProjPos = mul(Proj, ViewPos);
    
    output.Position = ProjPos;
    output.TexCoord = input.TexCoord;
    output.WorldPos = WorldPos;
    output.Normal = normalize(mul((float3x3)WorldInstanceData[index], input.Normal));
    
    return output;
}