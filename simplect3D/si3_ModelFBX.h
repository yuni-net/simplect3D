#pragma once

#include <memory>
#include <vector>
#include <fbxsdk.h>
#include "si3_popular.h"
#include "si3_utility.h"
#include "si3_MyFbxMesh.h"
#include "si3_DisplayObject.h"
#include "si3_Matrix.h"

#pragma comment(lib, "libfbxsdk-mt.lib")

namespace si3
{
	class ModelFBX :public DisplayObject
	{
	public:
		float x;
		float y;
		float z;

		ModelFBX(const tstring & file_path);



		// @Override
		void draw_no_alpha() const;

		// @Override
		void draw_alpha() const;
	private:
		FbxScene * scene;
		int mesh_quantity;
		LPDIRECT3DDEVICE9 device;
		std::vector<std::unique_ptr<MyFbxMesh> > mesh_list;
		LPDIRECT3DTEXTURE9 texture;
		mutable Matrix world_mat;



		Matrix calc_world_mat() const;
		void set_parameter_before() const;
		void set_parameter_after() const;
	};




}