#include <si3_MyFbxMesh.h>
#include <si3_Manager.h>
#include <si3_DxTop.h>

namespace si3
{
	static const DWORD model_fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	MyFbxMesh::MyFbxMesh(FbxMesh * mesh)
	{
		device = si3::Manager::get_dxdevice();
		polygon_quantity = mesh->GetPolygonCount();

		HRESULT result;
		result = device->CreateVertexBuffer(
			sizeof(DxTop)* polygon_quantity * 3,
			D3DUSAGE_WRITEONLY,
			model_fvf,
			D3DPOOL_MANAGED,
			&vertbuff,
			NULL);
		if (FAILED(result)){ return; }

		DxTop * top_head = nullptr;
		result = vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&top_head), 0);
		if (FAILED(result)){ return; }

		add_pos(mesh, top_head);
		add_normal(mesh, top_head);
		add_uv(mesh, top_head);
		
		vertbuff->Unlock();

		init_material();
	}

	void MyFbxMesh::draw() const
	{
		device->SetFVF(model_fvf);
		device->SetMaterial(&get_material());
		device->SetStreamSource(0, vertbuff, 0, sizeof(DxTop));
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, polygon_quantity);
	}





	void MyFbxMesh::init_material()
	{
		material = {
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 0.6f, 0.6f, 0.6f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
			1.0f
		};
	}

	void MyFbxMesh::add_pos(FbxMesh * mesh, DxTop * top_head)
	{
		for (int polygon_index = 0; polygon_index < polygon_quantity; ++polygon_index)
		{
			top_head->pos = get_pos(mesh, polygon_index, 0);
			++top_head;
			top_head->pos = get_pos(mesh, polygon_index, 1);
			++top_head;
			top_head->pos = get_pos(mesh, polygon_index, 2);
			++top_head;
		}
	}

	D3DVECTOR MyFbxMesh::get_pos(FbxMesh * mesh, const int polygon_index, const int index_of_3tops) const
	{
		auto pos_index = mesh->GetPolygonVertex(polygon_index, index_of_3tops);
		auto pos = mesh->GetControlPointAt(pos_index);
		D3DVECTOR result;
		result.x = float(pos[0]);
		result.y = float(pos[1]);
		result.z = float(pos[2]);
		// pos[3]つまりwが0.0以外のケースについては現在未対応
		return result;
	}


	void MyFbxMesh::add_normal(FbxMesh * mesh, DxTop * top_head)
	{
		auto element_count = mesh->GetElementNormalCount();
		if (element_count != 1)
		{
			// 法線要素が1以外のケースについては現在未対応
			return; 
		}

		auto element = mesh->GetElementNormal();
		auto mapping_mode = element->GetMappingMode();
		auto reference_mode = element->GetReferenceMode();
		const auto & index_array = element->GetIndexArray();
		const auto & direct_array = element->GetDirectArray();

		if (reference_mode != FbxGeometryElement::eDirect &&
			reference_mode != FbxGeometryElement::eIndexToDirect)
		{
			// リファレンスモードがeDirect, eIndexToDirect以外のケースについては現在未対応
			return;
		}

		if (mapping_mode == FbxGeometryElement::eByControlPoint)
		{
			do_map_normal_by_control_point(reference_mode, index_array, direct_array, top_head);
		}
		else if (mapping_mode == FbxGeometryElement::eByPolygonVertex)
		{
			do_map_normal_by_polygon_vertex(mesh, reference_mode,index_array, direct_array, top_head);
		}
		else
		{
			// マッピングモードがeByControlPoint, eByPolygonVertex以外のケースについては現在未対応
			return;
		}
	}

	void MyFbxMesh::add_uv(FbxMesh * mesh, DxTop * top_head)
	{
		for (auto uv_index = 0; uv_index < mesh->GetElementUVCount(); ++uv_index)
		{
			add_uv_each(mesh, top_head, uv_index);
		}
	}

	void MyFbxMesh::add_uv_each(FbxMesh * mesh, DxTop * top_head, int uv_index)
	{
		auto element = mesh->GetElementUV(uv_index);
		auto mapping_mode = element->GetMappingMode();
		auto reference_mode = element->GetReferenceMode();
		const auto & index_array = element->GetIndexArray();
		const auto & direct_array = element->GetDirectArray();

		if ((reference_mode != FbxGeometryElement::eDirect) &&
			(reference_mode != FbxGeometryElement::eIndexToDirect))
		{
			// リファレンスモードがeDirect, eIndexToDirect以外のケースについては現在未対応
			return;
		}

		if (mapping_mode == FbxGeometryElement::eByControlPoint)
		{
			do_map_uv_by_control_point(reference_mode, index_array, direct_array, top_head);
		}
		else if (mapping_mode == FbxGeometryElement::eByPolygonVertex)
		{
			do_map_uv_by_polygon_vertex(mesh, reference_mode, index_array, direct_array, top_head);
		}
		else
		{
			// マッピングモードがeByControlPoint, eByPolygonVertex以外のケースについては現在未対応
			return;
		}
	}

	void MyFbxMesh::do_map_uv_by_control_point(
		FbxLayerElement::EReferenceMode reference_mode,
		const FbxLayerElementArrayTemplate<int> & index_array,
		const FbxLayerElementArrayTemplate<FbxVector2> & direct_array,
		DxTop * top_head)
	{
		for (int index = 0; index < polygon_quantity; ++index)
		{
			int uv_index = index;
			if (reference_mode == FbxGeometryElement::eIndexToDirect)
			{
				uv_index = index_array.GetAt(index);
			}
			auto uv = direct_array.GetAt(uv_index);
			top_head->u = float(uv[0]);
			top_head->v = float(uv[1]);
			++top_head;
		}
	}

	void MyFbxMesh::do_map_uv_by_polygon_vertex(
		FbxMesh * mesh,
		FbxLayerElement::EReferenceMode reference_mode,
		const FbxLayerElementArrayTemplate<int> & index_array,
		const FbxLayerElementArrayTemplate<FbxVector2> & direct_array,
		DxTop * top_head)
	{
		auto index_by_polygon_vertex = 0;
		for (int i = 0; i < polygon_quantity; ++i)
		{
			auto polygon_size = mesh->GetPolygonSize(i);
			for (int j = 0; j < polygon_size; ++j)
			{
				auto uv_index = index_by_polygon_vertex;
				if (reference_mode == FbxGeometryElement::eIndexToDirect)
				{
					uv_index = index_array.GetAt(index_by_polygon_vertex);
				}
				auto uv = direct_array.GetAt(uv_index);
				top_head->u = float(uv[0]);
				top_head->v = float(uv[1]);
				++top_head;
				++index_by_polygon_vertex;
			}
		}
	}




	void MyFbxMesh::do_map_normal_by_control_point(
		FbxLayerElement::EReferenceMode reference_mode,
		const FbxLayerElementArrayTemplate<int> & index_array,
		const FbxLayerElementArrayTemplate<FbxVector4> & direct_array,
		DxTop * top_head)
	{
		for (int index = 0; index < polygon_quantity; ++index)
		{
			int normal_index = index;
			if (reference_mode == FbxGeometryElement::eIndexToDirect)
			{
				index = index_array.GetAt(index);
			}
			auto normal = direct_array.GetAt(normal_index);
			top_head->normal.x = float(normal[0]);
			top_head->normal.y = float(normal[1]);
			top_head->normal.z = float(normal[2]);
			++top_head;
			if (normal[3] != 1.0f)
			{
				// wが1.0でないケースについては現在未対応
				return;
			}
		}
	}

	void MyFbxMesh::do_map_normal_by_polygon_vertex(
		FbxMesh * mesh,
		FbxLayerElement::EReferenceMode reference_mode,
		const FbxLayerElementArrayTemplate<int> & index_array,
		const FbxLayerElementArrayTemplate<FbxVector4> & direct_array,
		DxTop * top_head)
	{
		auto index_by_polygon_vertex = 0;
		for (int i = 0; i < polygon_quantity; ++i)
		{
			auto polygon_size = mesh->GetPolygonSize(i);
			for (int j = 0; j < polygon_size; ++j)
			{
				auto normal_index = index_by_polygon_vertex;
				if (reference_mode == FbxGeometryElement::eIndexToDirect)
				{
					normal_index = index_array.GetAt(index_by_polygon_vertex);
				}
				auto normal = direct_array.GetAt(normal_index);
				top_head->normal.x = float(normal[0]);
				top_head->normal.y = float(normal[1]);
				top_head->normal.z = float(normal[2]);
				++top_head;
				if (normal[3] != 1.0f)
				{
					// wが1.0でないケースについては現在未対応
					return;
				}
				++index_by_polygon_vertex;
			}
		}
	}

	D3DMATERIAL9 MyFbxMesh::get_material() const
	{
		return material;
	}


}