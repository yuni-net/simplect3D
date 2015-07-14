#include <dxlight.h>

namespace si3
{
	bool DxLight::init(LPDIRECT3DDEVICE9 dxdevice)
	{
#if 1
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(light));

		light.Type = D3DLIGHT_POINT;
		light.Diffuse.r = 1.0f;
		light.Diffuse.g = 1.0f;
		light.Diffuse.b = 1.0f;
		light.Diffuse.a = 1.0f;
		light.Ambient.r = 1.0f;
		light.Ambient.g = 1.0f;
		light.Ambient.b = 1.0f;
		light.Ambient.a = 1.0f;
		light.Specular.r = 1.0f;
		light.Specular.g = 1.0f;
		light.Specular.b = 1.0f;
		light.Specular.a = 1.0f;

		light.Position.x = 0.0f;
		light.Position.y = 10.0f;
		light.Position.z = -140.0f;

		light.Attenuation0 = 1.0f;
		light.Range = 1600.0f;
#else
		// ライティング設定
		D3DLIGHT9 light = { D3DLIGHT_DIRECTIONAL };	// 平行光源
		light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &D3DXVECTOR3(-1.0f, -1.0f, 0.5f));
#endif





		dxdevice->SetLight(0, &light);





#if 1
		dxdevice->LightEnable(0, TRUE);
		dxdevice->SetRenderState(D3DRS_LIGHTING, TRUE);
#else
		dxdevice->LightEnable(0, FALSE);
		dxdevice->SetRenderState(D3DRS_LIGHTING, FALSE);
#endif




		dxdevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		dxdevice->SetRenderState(D3DRS_AMBIENT, 0x00101010);

		return true;
	}



}