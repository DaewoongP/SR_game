// matrix
float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
bool            g_Is2D;
texture			g_Texture;

sampler DefaultSampler = sampler_state
{
	texture = g_Texture;
	MINFILTER = linear;
	magfilter = linear;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0; 	
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	vector		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);	

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

vector PS_MAIN_TOODEE(PS_IN In) : COLOR0
{
	vector		vColor = (vector)0;

	vColor = tex2D(DefaultSampler, In.vTexUV);
    if(!g_Is2D)
    {
        vColor.rgba = 0.2f;
    }
	return vColor;
}

vector PS_MAIN_TOPDEE(PS_IN In) : COLOR0
{
	vector		vColor = (vector)0;

	vColor = tex2D(DefaultSampler, In.vTexUV);

	return vColor;
}


technique		DefaultTechnique
{
	pass Toodee
	{
        
		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader = compile ps_2_0 PS_MAIN_TOODEE();
	}
    pass Topdee
    {
        CullMode = none;
        
        VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader = compile ps_2_0 PS_MAIN_TOPDEE();
    }
}

