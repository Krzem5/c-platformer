import os
import json
import struct



DXGI_FORMAT_UNKNOWN=0
DXGI_FORMAT_R32G32B32A32_TYPELESS=1
DXGI_FORMAT_R32G32B32A32_FLOAT=2
DXGI_FORMAT_R32G32B32A32_UINT=3
DXGI_FORMAT_R32G32B32A32_SINT=4
DXGI_FORMAT_R32G32B32_TYPELESS=5
DXGI_FORMAT_R32G32B32_FLOAT=6
DXGI_FORMAT_R32G32B32_UINT=7
DXGI_FORMAT_R32G32B32_SINT=8
DXGI_FORMAT_R16G16B16A16_TYPELESS=9
DXGI_FORMAT_R16G16B16A16_FLOAT=10
DXGI_FORMAT_R16G16B16A16_UNORM=11
DXGI_FORMAT_R16G16B16A16_UINT=12
DXGI_FORMAT_R16G16B16A16_SNORM=13
DXGI_FORMAT_R16G16B16A16_SINT=14
DXGI_FORMAT_R32G32_TYPELESS=15
DXGI_FORMAT_R32G32_FLOAT=16
DXGI_FORMAT_R32G32_UINT=17
DXGI_FORMAT_R32G32_SINT=18
DXGI_FORMAT_R32G8X24_TYPELESS=19
DXGI_FORMAT_D32_FLOAT_S8X24_UINT=20
DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS=21
DXGI_FORMAT_X32_TYPELESS_G8X24_UINT=22
DXGI_FORMAT_R10G10B10A2_TYPELESS=23
DXGI_FORMAT_R10G10B10A2_UNORM=24
DXGI_FORMAT_R10G10B10A2_UINT=25
DXGI_FORMAT_R11G11B10_FLOAT=26
DXGI_FORMAT_R8G8B8A8_TYPELESS=27
DXGI_FORMAT_R8G8B8A8_UNORM=28
DXGI_FORMAT_R8G8B8A8_UNORM_SRGB=29
DXGI_FORMAT_R8G8B8A8_UINT=30
DXGI_FORMAT_R8G8B8A8_SNORM=31
DXGI_FORMAT_R8G8B8A8_SINT=32
DXGI_FORMAT_R16G16_TYPELESS=33
DXGI_FORMAT_R16G16_FLOAT=34
DXGI_FORMAT_R16G16_UNORM=35
DXGI_FORMAT_R16G16_UINT=36
DXGI_FORMAT_R16G16_SNORM=37
DXGI_FORMAT_R16G16_SINT=38
DXGI_FORMAT_R32_TYPELESS=39
DXGI_FORMAT_D32_FLOAT=40
DXGI_FORMAT_R32_FLOAT=41
DXGI_FORMAT_R32_UINT=42
DXGI_FORMAT_R32_SINT=43
DXGI_FORMAT_R24G8_TYPELESS=44
DXGI_FORMAT_D24_UNORM_S8_UINT=45
DXGI_FORMAT_R24_UNORM_X8_TYPELESS=46
DXGI_FORMAT_X24_TYPELESS_G8_UINT=47
DXGI_FORMAT_R8G8_TYPELESS=48
DXGI_FORMAT_R8G8_UNORM=49
DXGI_FORMAT_R8G8_UINT=50
DXGI_FORMAT_R8G8_SNORM=51
DXGI_FORMAT_R8G8_SINT=52
DXGI_FORMAT_R16_TYPELESS=53
DXGI_FORMAT_R16_FLOAT=54
DXGI_FORMAT_D16_UNORM=55
DXGI_FORMAT_R16_UNORM=56
DXGI_FORMAT_R16_UINT=57
DXGI_FORMAT_R16_SNORM=58
DXGI_FORMAT_R16_SINT=59
DXGI_FORMAT_R8_TYPELESS=60
DXGI_FORMAT_R8_UNORM=61
DXGI_FORMAT_R8_UINT=62
DXGI_FORMAT_R8_SNORM=63
DXGI_FORMAT_R8_SINT=64
DXGI_FORMAT_A8_UNORM=65
DXGI_FORMAT_R1_UNORM=66
DXGI_FORMAT_R9G9B9E5_SHAREDEXP=67
DXGI_FORMAT_R8G8_B8G8_UNORM=68
DXGI_FORMAT_G8R8_G8B8_UNORM=69
DXGI_FORMAT_BC1_TYPELESS=70
DXGI_FORMAT_BC1_UNORM=71
DXGI_FORMAT_BC1_UNORM_SRGB=72
DXGI_FORMAT_BC2_TYPELESS=73
DXGI_FORMAT_BC2_UNORM=74
DXGI_FORMAT_BC2_UNORM_SRGB=75
DXGI_FORMAT_BC3_TYPELESS=76
DXGI_FORMAT_BC3_UNORM=77
DXGI_FORMAT_BC3_UNORM_SRGB=78
DXGI_FORMAT_BC4_TYPELESS=79
DXGI_FORMAT_BC4_UNORM=80
DXGI_FORMAT_BC4_SNORM=81
DXGI_FORMAT_BC5_TYPELESS=82
DXGI_FORMAT_BC5_UNORM=83
DXGI_FORMAT_BC5_SNORM=84
DXGI_FORMAT_B5G6R5_UNORM=85
DXGI_FORMAT_B5G5R5A1_UNORM=86
DXGI_FORMAT_B8G8R8A8_UNORM=87
DXGI_FORMAT_B8G8R8X8_UNORM=88
DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM=89
DXGI_FORMAT_B8G8R8A8_TYPELESS=90
DXGI_FORMAT_B8G8R8A8_UNORM_SRGB=91
DXGI_FORMAT_B8G8R8X8_TYPELESS=92
DXGI_FORMAT_B8G8R8X8_UNORM_SRGB=93
DXGI_FORMAT_BC6H_TYPELESS=94
DXGI_FORMAT_BC6H_UF16=95
DXGI_FORMAT_BC6H_SF16=96
DXGI_FORMAT_BC7_TYPELESS=97
DXGI_FORMAT_BC7_UNORM=98
DXGI_FORMAT_BC7_UNORM_SRGB=99
DXGI_FORMAT_AYUV=100
DXGI_FORMAT_Y410=101
DXGI_FORMAT_Y416=102
DXGI_FORMAT_NV12=103
DXGI_FORMAT_P010=104
DXGI_FORMAT_P016=105
DXGI_FORMAT_420_OPAQUE=106
DXGI_FORMAT_YUY2=107
DXGI_FORMAT_Y210=108
DXGI_FORMAT_Y216=109
DXGI_FORMAT_NV11=110
DXGI_FORMAT_AI44=111
DXGI_FORMAT_IA44=112
DXGI_FORMAT_P8=113
DXGI_FORMAT_A8P8=114
DXGI_FORMAT_B4G4R4A4_UNORM=115
DXGI_FORMAT_P208=130
DXGI_FORMAT_V208=131
DXGI_FORMAT_V408=132
DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE=189
DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE=190
DXGI_FORMAT_FORCE_UINT=0xfffffff

D3D11_RESOURCE_DIMENSION_UNKNOWN=0
D3D11_RESOURCE_DIMENSION_BUFFER=1
D3D11_RESOURCE_DIMENSION_TEXTURE1D=2
D3D11_RESOURCE_DIMENSION_TEXTURE2D=3
D3D11_RESOURCE_DIMENSION_TEXTURE3D=4



def GetSurfaceInfo(w,h,f):
	b=False
	p=False
	bpb=0
	if (f in [DXGI_FORMAT_BC1_TYPELESS,DXGI_FORMAT_BC1_UNORM,DXGI_FORMAT_BC1_UNORM_SRGB,DXGI_FORMAT_BC4_TYPELESS,DXGI_FORMAT_BC4_UNORM,DXGI_FORMAT_BC4_SNORM]):
		b=True
		bpb=8
	elif (f in [DXGI_FORMAT_BC2_TYPELESS,DXGI_FORMAT_BC2_UNORM,DXGI_FORMAT_BC2_UNORM_SRGB,DXGI_FORMAT_BC3_TYPELESS,DXGI_FORMAT_BC3_UNORM,DXGI_FORMAT_BC3_UNORM_SRGB,DXGI_FORMAT_BC5_TYPELESS,DXGI_FORMAT_BC5_UNORM,DXGI_FORMAT_BC5_SNORM,DXGI_FORMAT_BC6H_TYPELESS,DXGI_FORMAT_BC6H_UF16,DXGI_FORMAT_BC6H_SF16,DXGI_FORMAT_BC7_TYPELESS,DXGI_FORMAT_BC7_UNORM,DXGI_FORMAT_BC7_UNORM_SRGB]):
		b=True
		bpb=16
	elif (f==DXGI_FORMAT_R8G8_B8G8_UNORM or f==DXGI_FORMAT_G8R8_G8B8_UNORM):
		p=True
	if (b==True):
		if (w<=0 or h<=0):
			raise RuntimeError
		return (max(1,(w+3)//4)*bpb,max(1,(h+3)//4))
	if (p==True):
		return (((w+1)>>1)*4,h)
	if (f==[DXGI_FORMAT_R32G32B32A32_TYPELESS,DXGI_FORMAT_R32G32B32A32_FLOAT,DXGI_FORMAT_R32G32B32A32_UINT,DXGI_FORMAT_R32G32B32A32_SINT]):
		bpp=128
	elif (f==[DXGI_FORMAT_R32G32B32_TYPELESS,DXGI_FORMAT_R32G32B32_FLOAT,DXGI_FORMAT_R32G32B32_UINT,DXGI_FORMAT_R32G32B32_SINT]):
		bpp=96
	elif (f==[DXGI_FORMAT_R16G16B16A16_TYPELESS,DXGI_FORMAT_R16G16B16A16_FLOAT,DXGI_FORMAT_R16G16B16A16_UNORM,DXGI_FORMAT_R16G16B16A16_UINT,DXGI_FORMAT_R16G16B16A16_SNORM,DXGI_FORMAT_R16G16B16A16_SINT,DXGI_FORMAT_R32G32_TYPELESS,DXGI_FORMAT_R32G32_FLOAT,DXGI_FORMAT_R32G32_UINT,DXGI_FORMAT_R32G32_SINT,DXGI_FORMAT_R32G8X24_TYPELESS,DXGI_FORMAT_D32_FLOAT_S8X24_UINT,DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,DXGI_FORMAT_X32_TYPELESS_G8X24_UINT]):
		bpp=64
	elif (f in [DXGI_FORMAT_R10G10B10A2_TYPELESS,DXGI_FORMAT_R10G10B10A2_UNORM,DXGI_FORMAT_R10G10B10A2_UINT,DXGI_FORMAT_R11G11B10_FLOAT,DXGI_FORMAT_R8G8B8A8_TYPELESS,DXGI_FORMAT_R8G8B8A8_UNORM,DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,DXGI_FORMAT_R8G8B8A8_UINT,DXGI_FORMAT_R8G8B8A8_SNORM,DXGI_FORMAT_R8G8B8A8_SINT,DXGI_FORMAT_R16G16_TYPELESS,DXGI_FORMAT_R16G16_FLOAT,DXGI_FORMAT_R16G16_UNORM,DXGI_FORMAT_R16G16_UINT,DXGI_FORMAT_R16G16_SNORM,DXGI_FORMAT_R16G16_SINT,DXGI_FORMAT_R32_TYPELESS,DXGI_FORMAT_D32_FLOAT,DXGI_FORMAT_R32_FLOAT,DXGI_FORMAT_R32_UINT,DXGI_FORMAT_R32_SINT,DXGI_FORMAT_R24G8_TYPELESS,DXGI_FORMAT_D24_UNORM_S8_UINT,DXGI_FORMAT_R24_UNORM_X8_TYPELESS,DXGI_FORMAT_X24_TYPELESS_G8_UINT,DXGI_FORMAT_R9G9B9E5_SHAREDEXP,DXGI_FORMAT_R8G8_B8G8_UNORM,DXGI_FORMAT_G8R8_G8B8_UNORM,DXGI_FORMAT_B8G8R8A8_UNORM,DXGI_FORMAT_B8G8R8X8_UNORM,DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM,DXGI_FORMAT_B8G8R8A8_TYPELESS,DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,DXGI_FORMAT_B8G8R8X8_TYPELESS,DXGI_FORMAT_B8G8R8X8_UNORM_SRGB]):
		bpp=32
	elif (f in [DXGI_FORMAT_R8G8_TYPELESS,DXGI_FORMAT_R8G8_UNORM,DXGI_FORMAT_R8G8_UINT,DXGI_FORMAT_R8G8_SNORM,DXGI_FORMAT_R8G8_SINT,DXGI_FORMAT_R16_TYPELESS,DXGI_FORMAT_R16_FLOAT,DXGI_FORMAT_D16_UNORM,DXGI_FORMAT_R16_UNORM,DXGI_FORMAT_R16_UINT,DXGI_FORMAT_R16_SNORM,DXGI_FORMAT_R16_SINT,DXGI_FORMAT_B5G6R5_UNORM,DXGI_FORMAT_B5G5R5A1_UNORM,DXGI_FORMAT_B4G4R4A4_UNORM]):
		bpp=16
	elif (f in [DXGI_FORMAT_R8_TYPELESS,DXGI_FORMAT_R8_UNORM,DXGI_FORMAT_R8_UINT,DXGI_FORMAT_R8_SNORM,DXGI_FORMAT_R8_SINT,DXGI_FORMAT_A8_UNORM,DXGI_FORMAT_BC2_TYPELESS,DXGI_FORMAT_BC2_UNORM,DXGI_FORMAT_BC2_UNORM_SRGB,DXGI_FORMAT_BC3_TYPELESS,DXGI_FORMAT_BC3_UNORM,DXGI_FORMAT_BC3_UNORM_SRGB,DXGI_FORMAT_BC5_TYPELESS,DXGI_FORMAT_BC5_UNORM,DXGI_FORMAT_BC5_SNORM,DXGI_FORMAT_BC6H_TYPELESS,DXGI_FORMAT_BC6H_UF16,DXGI_FORMAT_BC6H_SF16,DXGI_FORMAT_BC7_TYPELESS,DXGI_FORMAT_BC7_UNORM,DXGI_FORMAT_BC7_UNORM_SRGB]):
		bpp=8
	elif (f in [DXGI_FORMAT_BC1_TYPELESS,DXGI_FORMAT_BC1_UNORM,DXGI_FORMAT_BC1_UNORM_SRGB,DXGI_FORMAT_BC4_TYPELESS,DXGI_FORMAT_BC4_UNORM,DXGI_FORMAT_BC4_SNORM]):
		bpp=4
	elif (f==DXGI_FORMAT_R1_UNORM):
		bpp=1
	else:
		raise RuntimeError
	return ((w*bpp+7)//8,h)



for k in os.listdir("rsrc"):
	if (k[-4:]==".dds"):
		with open(f"rsrc\\{k}","rb") as f:
			dt=f.read()
		if (dt[:4]==b"DDS "):
			_h=struct.unpack("<31L",dt[4:128])
			sz,f,h,w,sz_t,d,mmc=_h[:7]
			px_sz,px_f,px_cc,px_bc,px_rm,px_gm,px_bm,px_am,c,c2,c3,c4=_h[18:30]
			if (sz!=124 or px_sz!=32):
				raise RuntimeError
			if ((px_f&0x4)==0x4 and px_cc==0x30315844):
				print("DX10")
				raise RuntimeError("Unimplemented (DX10 ext)")
			elif ((px_f&0x41)==0x41):
				if (px_bc!=32):
					raise RuntimeError("RGBA 16-bit")
				if (px_rm==0x000000ff and px_gm==0x0000ff00 and px_bm==0x00ff0000 and px_am==0xff000000):
					f=DXGI_FORMAT_R8G8B8A8_UNORM
				elif (px_rm==0x00ff0000 and px_gm==0x0000ff00 and px_bm==0x000000ff and px_am==0xff000000):
					f=DXGI_FORMAT_B8G8R8A8_UNORM
				else:
					print(hex(px_rm),hex(px_gm),hex(px_bm),hex(px_am))
					raise RuntimeError
				rb,nr=GetSurfaceInfo(w,h,f)
				with open(f"rsrc\\{k[:-4]}.c","w") as wf:
					o=f"#include <common.h>\n#include <{k[:-4]}.h>\n#include <texture.h>\n\n\n\nconst uint8_t {k[:-4].upper().replace(' ','_')}_TEX_DATA[]={{\n"
					dt=dt[128:rb*nr+128]
					i=len(dt)
					while (i>0):
						if (i>=6):
							o+=f"\t{str(dt[-i]).rjust(3,' ')}, {str(dt[-i+1]).rjust(3,' ')}, {str(dt[-i+2]).rjust(3,' ')}, {str(dt[-i+3]).rjust(3,' ')}, {str(dt[-i+4]).rjust(3,' ')}, {str(dt[-i+5]).rjust(3,' ')},\n"
						else:
							o+="\t"
							while (i>0):
								o+=f"{str(dt[-i]).rjust(3,' ')}{(', ' if i>1 else '')}"
								i-=1
						i-=6
					o+=f"}};\n\n\n\nconst RawTexture {k[:-4].upper().replace(' ','_')}_TEX={{\n\t{f},\n\t{w},\n\t{h},\n\t{rb},\n\t{rb*nr},\n\t{k[:-4].upper().replace(' ','_')}_TEX_DATA\n}};\n\n\n\nconst TextureMapData {k[:-4].upper().replace(' ','_')}_TEX_MAP_DATA[]={{\n"
					with open(f"rsrc\\{k[:-4]}.json","r") as jf:
						m_dt=json.loads(jf.read())
						for i,e in enumerate(m_dt["frames"].values()):
							o+=f"\t{{\n\t\t{e['frame']['x']/m_dt['meta']['size']['w']}f,\n\t\t{e['frame']['y']/m_dt['meta']['size']['h']}f,\n\t\t{(e['frame']['x']+e['frame']['w'])/m_dt['meta']['size']['w']}f,\n\t\t{(e['frame']['y']+e['frame']['h'])/m_dt['meta']['size']['h']}f\n\t}},\n"
						o+=f"}};\n"
					wf.write(o)
				with open(f"rsrc\\{k[:-4]}.h","w") as wf:
					o=f"#ifndef __{k[:-4].upper().replace(' ','_')}_H__\n#define __{k[:-4].upper().replace(' ','_')}_H__\n#include <common.h>\n#include <texture.h>\n\n\n\nenum {k[:-4].upper().replace(' ','_')}_TEX_IMG{{\n"
					with open(f"rsrc\\{k[:-4]}.json","r") as jf:
						m_dt=json.loads(jf.read())
						for i,e in enumerate(m_dt["frames"].keys()):
							o+=f"\t{k[:-4].upper().replace(' ','_')}_TEX_IMG_{e.split('.')[0].upper().replace(' ','_')}={hex(i)},\n"
						o+=f"}};\n\n\n\nextern const uint8_t {k[:-4].upper().replace(' ','_')}_TEX_DATA[];\n\n\n\nextern const RawTexture {k[:-4].upper().replace(' ','_')}_TEX;\n\n\n\nextern const TextureMapData {k[:-4].upper().replace(' ','_')}_TEX_MAP_DATA[];\n\n\n\n#endif\n"
					wf.write(o)
			elif ((px_f&0x40)==0x40):
				raise RuntimeError("RGB")
			elif ((px_f&0x20000)==0x20000):
				raise RuntimeError("LUMINANCE")
			elif ((px_f&0x02)==0x02):
				raise RuntimeError("ALPHA")
			elif ((px_f&0x04)==0x04):
				raise RuntimeError
			else:
				raise RuntimeError(px_f)
