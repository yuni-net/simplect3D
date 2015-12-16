#pragma once

#include "si3_popular.h"
#include <fbxsdk.h>
#include "si3_utility.h"
#include <memory>

#pragma comment(lib, "libfbxsdk-mt.lib")

namespace si3
{
	class MyFbxManager
	{
	public:
		/**
		 @brief fbxファイルを読み込み、sceneデータを返します。
		 @param 読み込むfbxファイルのパスを指定します。
		 @return
			成功するとsceneデータの先頭アドレスを返します。
			解放の責任はユーザーにゆだねられます。
			失敗するとnullptrを返します。
		 */
		static FbxScene * load_scene(const tstring & file_path);

		static std::unique_ptr<FbxGeometryConverter> new_geometry_converter();







		~MyFbxManager();
	private:
		FbxManager * fbx_manager;

		static MyFbxManager & get_instance();
		MyFbxManager();
		MyFbxManager(const MyFbxManager & another);
		MyFbxManager & operator=(const MyFbxManager & another);
	};




}