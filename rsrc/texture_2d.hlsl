#pragma pack_matrix(row_major)



cbuffer core:register(b0){
	matrix pm;
};



cbuffer extra:register(b1){
	matrix wm;
};



Texture2D tx;
SamplerState ss;



struct VS_OUT{
	float4 p:SV_POSITION;
	float2 t:TEXCOORD;
};



VS_OUT texture_2d_vs(float4 p:POSITION,float2 t:TEXCOORD){
	VS_OUT o={
		mul(mul(p,wm),pm),
		t
	};
	return o;
}



float4 texture_2d_ps(VS_OUT vo):SV_TARGET{
	return tx.Sample(ss,vo.t);
}
