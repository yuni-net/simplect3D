#include <memory>
#include <fbxsdk.h>
#include <si3_ModelFBX.h>
#include <si3_MyFbxManager.h>
#include <si3_Manager.h>

namespace si3
{

	ModelFBX::ModelFBX(const tstring & file_path)
	{
		typedef unsigned long ulong;
		typedef unsigned short ushort;
		device = si3::Manager::get_dxdevice();
		scene = MyFbxManager::load_scene(file_path);

		std::unique_ptr<FbxGeometryConverter> verter(MyFbxManager::new_geometry_converter());
		verter->Triangulate(scene, true);
		mesh_quantity = scene->GetMemberCount<FbxMesh>();
		mesh_list.reserve(mesh_quantity);

		for (ulong top_index = 0; int(top_index) < mesh_quantity; ++top_index)
		{
			auto mesh = scene->GetMember<FbxMesh>(top_index);
			mesh_list.push_back(std::unique_ptr<MyFbxMesh>(new MyFbxMesh(mesh)));
		}

		HRESULT hr = D3DXCreateTextureFromFileEx(
			device,
			"data/white.png",
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			0,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0,
			NULL,
			NULL,
			&texture);

	}


	void ModelFBX::draw_no_alpha() const
	{
		world_mat = calc_world_mat();
		device->SetTransform(D3DTS_WORLD, world_mat.dxpointer());
		device->SetTexture(0, texture);
		set_parameter_before();
		for (unsigned int mesh_index = 0; mesh_index < mesh_list.size(); ++mesh_index)
		{
			mesh_list[mesh_index]->draw();
		}
		set_parameter_after();
	}

	void ModelFBX::draw_alpha() const
	{
		// todo
	}


	Matrix ModelFBX::calc_world_mat() const
	{
		return Matrix(Coor3(x, y, z));
	}

	void ModelFBX::set_parameter_before() const
	{
		device->LightEnable(0, TRUE);
		device->SetRenderState(D3DRS_LIGHTING, TRUE);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// todo
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	}


	void ModelFBX::set_parameter_after() const
	{
		// Nothing
	}

}