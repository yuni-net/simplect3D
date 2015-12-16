#include <si3_MyFbxManager.h>

namespace si3
{
	FbxScene * MyFbxManager::load_scene(const tstring & file_path)
	{
		auto importer = FbxImporter::Create(get_instance().fbx_manager, "");
		bool did_succeed = importer->Initialize(file_path.c_str(), -1, get_instance().fbx_manager->GetIOSettings());
		if (did_succeed == false)
		{
			return nullptr;
		}

		auto scene = FbxScene::Create(get_instance().fbx_manager, "original_scene");
		importer->Import(scene);
		importer->Destroy();

		return scene;
	}

	std::unique_ptr<FbxGeometryConverter> MyFbxManager::new_geometry_converter()
	{
		return std::unique_ptr<FbxGeometryConverter>(new FbxGeometryConverter(get_instance().fbx_manager));
	}





	MyFbxManager::~MyFbxManager()
	{
		fbx_manager->Destroy();
	}


	MyFbxManager & MyFbxManager::get_instance()
	{
		static MyFbxManager me;
		return me;
	}


	MyFbxManager::MyFbxManager()
	{
		fbx_manager = FbxManager::Create();
		auto ios = FbxIOSettings::Create(fbx_manager, IOSROOT);
		fbx_manager->SetIOSettings(ios);
	}

}