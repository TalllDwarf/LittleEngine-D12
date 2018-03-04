Texture2D t1 : register(t0);
SamplerState s1 : register(s0);

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

cbuffer LightBuffer : register(b1)
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
}


float4 main(VS_OUTPUT input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;

    //Sample the pixel color from the texture using the sampler at this texutre coord
    textureColor = t1.Sample(s1, input.texCoord);

    //set the default output color to the ambient ligth value for all pixels
    color = ambientColor;

    //Init the specular color
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    //Invert the light direction for calculation
    lightDir = -lightDirection;

    //Calcualte the amount of lightt on this pixel
    lightIntensity = saturate(dot(input.normal, lightDir));

    if (lightIntensity > 0.0f)
    {
        //Determing the final diffuse color based on the diffuse color and the amount of light intensity
        color += (diffuseColor * lightIntensity);

        //Saturate the ambient and diffuse color
        color = saturate(color);

        //Calculate the reflection vector based on the light intensity, normal vector and light direction
        reflection = normalize(2 * lightIntensity * input.normal - lightDir);

        //Determing the amount of specular light based on the reflection vector, viewing direction and specular power
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    }

    //Multiply the texture pixel and the input color to get the textured result
    color = color * textureColor;

    //Add the specular component last to the output color
    color = saturate(color + specular);

    return color;
}