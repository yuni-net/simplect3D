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
		 @brief fbx�t�@�C����ǂݍ��݁Ascene�f�[�^��Ԃ��܂��B
		 @param �ǂݍ���fbx�t�@�C���̃p�X���w�肵�܂��B
		 @return
			���������scene�f�[�^�̐擪�A�h���X��Ԃ��܂��B
			����̐ӔC�̓��[�U�[�ɂ䂾�˂��܂��B
			���s�����nullptr��Ԃ��܂��B
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