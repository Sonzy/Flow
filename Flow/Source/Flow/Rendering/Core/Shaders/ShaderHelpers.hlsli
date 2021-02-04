
// Constant Buffers /////////////////////////////////////////////////////////////////

cbuffer CB_Camera : register(b9)
{
    float3 m_cameraPosition;
}

cbuffer PointLightCBuf : register(b12)
{
	float3 m_point_lightPosition;
    float3 m_point_diffuseColor;
    float m_point_diffuseIntensity;
    float m_point_specularIntensity;
    float m_point_aConstant;
    float m_point_aLinear;
    float m_point_aQuadratic;
};

cbuffer DirectionalLightCBuffer : register(b13)
{
    float3 m_directional_lightDirection;
    float3 m_directional_ambientColor;
    float3 m_directional_diffuseColor;
    float3 m_directional_specularColor;
    float m_directional_diffuseIntensity;
    float m_directional_ambientIntensity;
    float m_directional_specularIntensity;
}

cbuffer ObjectCBuf : register(b11)
{
    float3 specularColor;
    float specularWeight;
    float specularGloss;
};

// Structs /////////////////////////////////////////////////////////////////

struct LightVectorData
{
	float3 vToL;
	float3 dirToL;
	float distToL;
};


// Functions /////////////////////////////////////////////////////////////////

float3 MapNormal(
	const in float3 tan,
	const in float3 bitan,
	const in float3 normal,
	const in float2 tc,
	uniform Texture2D nmap,
	uniform SamplerState splr)
{
	// build the tranform (rotation) into same space as tan/bitan/normal (target space)
	const float3x3 tanToTarget = float3x3(tan, bitan, normal);
	// sample and unpack the normal from texture into target space   
	const float3 normalSample = nmap.Sample(splr, tc).xyz;
	const float3 tanNormal = normalSample * 2.0f - 1.0f;
	// bring normal from tanspace into target space
	return normalize(mul(tanNormal, tanToTarget));
}

float3 CalculateDirectionalLight(float3 position, float3 normal, float3 lightDirection)
{
    float3 directional_ambient = float3(0.0f, 0.0f, 0.0f);
    float3 directional_diffuse = float3(0.0f, 0.0f, 0.0f);
    float3 directional_specular = float3(0.0f, 0.0f, 0.0f);
	
	//Ambient
	{
        directional_ambient = m_directional_ambientColor * m_directional_ambientIntensity;
    }
	
	//Diffuse
	{   
        float diffusePower = max(dot(-lightDirection, normal), 0.0f);
        directional_diffuse = m_directional_diffuseColor * diffusePower * m_directional_diffuseIntensity;
    }
	
	//Specular
	{
        float3 viewDirection = normalize(m_cameraPosition - position);
        float3 reflectionDirection = reflect(lightDirection, normal);
      
        float specular = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 32);
        directional_specular = m_directional_specularIntensity * specular * m_directional_specularColor;
    }
	
   return (directional_ambient + directional_diffuse + directional_specular);
}

float3 CalculatePointLight(float3 position, float3 normal)
{
    float3 point_ambient = float3(0.0f, 0.0f, 0.0f);
    float3 point_diffuse = float3(0.0f, 0.0f, 0.0f);
    float3 point_specular = float3(0.0f, 0.0f, 0.0f);
    
    float3 point_direction = normalize(position - m_point_lightPosition);
    float point_distance = length(position - m_point_lightPosition);
    float point_attenuation = 1.0 / (m_point_aConstant + m_point_aLinear * point_distance + m_point_aQuadratic * (point_distance * point_distance));
	
	//Diffuse
	{           
        float diffusePower = max(dot(-point_direction, normal), 0.0f);
        point_diffuse = m_point_diffuseColor * diffusePower * m_point_diffuseIntensity;
        
        point_diffuse *= point_attenuation;

    }
	
	//Specular
	{
        float3 viewDirection = normalize(m_cameraPosition - position);
        float3 reflectionDirection = reflect(point_direction, normal);
      
        float specular = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 32);
        point_specular = m_point_specularIntensity * specular;
        
        point_specular *= point_attenuation;
    }
	
    return (point_ambient + point_diffuse + point_specular);
}