#pragma pack_matrix(row_major)



cbuffer core:register(b0){
	matrix pm;
};



cbuffer extra:register(b1){
	matrix wm;
};



struct VS_OUT{
	float4 p:SV_POSITION;
	float4 c:COLOR;
};



VS_OUT color_2d_vs(float4 p:POSITION,float4 c:COLOR){
	VS_OUT o={
		mul(mul(p,wm),pm),
		c
	};
	return o;
}



float4 color_2d_ps(float4 p:SV_POSITION,float4 c:COLOR):SV_TARGET{
	return c;
}
