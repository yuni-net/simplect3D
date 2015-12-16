#pragma once

#include "si3_popular.h"
#include <fbxsdk.h>
#include <si3_DxTop.h>

#pragma comment(lib, "libfbxsdk-mt.lib")

namespace si3
{
	class MyFbxMesh
	{
	public:
		MyFbxMesh(FbxMesh * mesh);
		void draw() const;





	private:
		LPDIRECT3DDEVICE9 device;
		int polygon_quantity;
		IDirect3DVertexBuffer9 * vertbuff;
		D3DMATERIAL9 material;

		void init_material();

		void add_pos(FbxMesh * mesh, DxTop * top_head);
		D3DVECTOR MyFbxMesh::get_pos(FbxMesh * mesh, const int polygon_index, const int index_of_3tops) const;

		void add_normal(FbxMesh * mesh, DxTop * top_head);
		void do_map_normal_by_control_point(
			FbxLayerElement::EReferenceMode reference_mode,
			const FbxLayerElementArrayTemplate<int> & index_array,
			const FbxLayerElementArrayTemplate<FbxVector4> & direct_array,
			DxTop * top_head);
		void do_map_normal_by_polygon_vertex(
			FbxMesh * mesh,
			FbxLayerElement::EReferenceMode reference_mode,
			const FbxLayerElementArrayTemplate<int> & index_array,
			const FbxLayerElementArrayTemplate<FbxVector4> & direct_array,
			DxTop * top_head);

		void add_uv(FbxMesh * mesh, DxTop * top_head);
		void add_uv_each(FbxMesh * mesh, DxTop * top_head, int uv_index);
		void MyFbxMesh::do_map_uv_by_control_point(
			FbxLayerElement::EReferenceMode reference_mode,
			const FbxLayerElementArrayTemplate<int> & index_array,
			const FbxLayerElementArrayTemplate<FbxVector2> & direct_array,
			DxTop * top_head);
		void MyFbxMesh::do_map_uv_by_polygon_vertex(
			FbxMesh * mesh,
			FbxLayerElement::EReferenceMode reference_mode,
			const FbxLayerElementArrayTemplate<int> & index_array,
			const FbxLayerElementArrayTemplate<FbxVector2> & direct_array,
			DxTop * top_head);



		D3DMATERIAL9 get_material() const;
	};
}