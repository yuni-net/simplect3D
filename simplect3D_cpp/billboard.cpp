#include <billboard.h>
#include <top4.h>
#include <diffuse.h>
#include <anime_data.h>
#include <model_coor.h>
#include <Manager.h>

namespace si3
{
	billboard::billboard()
	{
		// Nothing
	}
	billboard::billboard(const Manager & manageri, const TCHAR * path)
	{
		load(manageri, path);
	}
	billboard::billboard(const Manager & manageri, const std::string & path)
	{
		load(manageri, path);
	}
	billboard::billboard(const Manager & manageri, const ::si3::image_data & imaged)
	{
		image_data(manageri, imaged);
	}









	// @Override
	void billboard::compute_world_mat() const
	{
		matrix scale_mat;
		scale_mat.scale(scale(), scale(), scale());

		const D3DXMATRIX & view_rot = pManager->camera().view_rot();
		matrix inverse_rot = inverse_matrix_of_rot(view_rot);
		matrix rot_mat;
		rot_mat.rotate_z(rot_z());
		rot_mat *= inverse_rot;

		matrix parallel_mat;
		parallel_mat.parallel(x(), y(), z());

		world_mat = scale_mat;
		world_mat *= rot_mat;
		world_mat *= parallel_mat;
	}


















#if 0
	billboard::billboard()
	{
		construct();
	}
	billboard::billboard(const Manager & manageri, const TCHAR * path)
	{
		construct();
		load(manageri, path);
	}
	billboard::billboard(const Manager & manageri, const std::string & path)
	{
		construct();
		load(manageri, path);
	}
	billboard::billboard(const Manager & manageri, const ::si3::anime_data & imaged)
	{
		construct();
		anime_data(manageri, imaged);
	}

	void billboard::piece_size(float value)
	{
		piece_size_ = value;
	}
	float billboard::piece_size() const
	{
		return piece_size_;
	}

	bool billboard::load(const Manager & manageri, const TCHAR * path)
	{
		si3m = &manageri;
		::si3::anime_data * imgd = new ::si3::anime_data();
		bool result = imgd->load(manageri, path, piece_size());
		anime_data(manageri, *imgd);
		ownership = true;
		return result;
	}
	bool billboard::load(const Manager & manageri, const std::string & path)
	{
		return load(manageri, path.c_str());
	}
	void billboard::anime_data(const Manager & manageri, const ::si3::anime_data & imaged)
	{
		release();
		si3m = &manageri;
		this->imaged = &imaged;
	}

	void billboard::x(float value)
	{
		x_ = value;
	}
	float billboard::x() const
	{
		return x_;
	}

	void billboard::y(float value)
	{
		y_ = value;
	}
	float billboard::y() const
	{
		return y_;
	}

	void billboard::z(float value)
	{
		z_ = value;
	}
	float billboard::z() const
	{
		return z_;
	}

	void billboard::scale(float value)
	{
		scale_ = value;
	}
	float billboard::scale() const
	{
		return scale_;
	}

	uint billboard::base_width() const
	{
		return this->imaged->width();
	}
	uint billboard::base_height() const
	{
		return this->imaged->height();
	}

	float billboard::width() const
	{
		return base_width()*scale();
	}
	float billboard::height() const
	{
		return base_height()*scale();
	}

	D3DMATERIAL9 & billboard::material()
	{
		return material_;
	}
	const D3DMATERIAL9 & billboard::material() const
	{
		return material_;
	}

	void billboard::setblend_normal()
	{
		BlendMode = anime_data::normal_blend;
	}
	void billboard::setblend_alpha()
	{
		BlendMode = anime_data::alpha_blend;
	}
	void billboard::setblend_add()
	{
		BlendMode = anime_data::add_blend;
	}
	void billboard::setblend_sub()
	{
		BlendMode = anime_data::sub_blend;
	}





	void billboard::draw_no_alpha() const
	{
		if (material().Diffuse.a < 1.0f)	// 半透明処理が必要だからここでは書き込まない
		{
			return;
		}

		D3DXMATRIX scale_mat;
		D3DXMatrixScaling(&scale_mat, scale(), scale(), scale());

#if 0
		D3DXMATRIX rotate_mat;
		//D3DXMatrixRotationX(&rotate_mat, 3.14159265f);
		D3DXMatrixRotationX(&rotate_mat, 3.141592653589f / 2.0f);
		D3DXMATRIX view_rot = si3m->camera().view_rot();
#else
		const D3DXMATRIX & view_rot = si3m->camera().view_rot();
		D3DXMATRIX inverse_rot = inverse_matrix_of_rot(view_rot);

		D3DXMATRIX base_rot;
		D3DXMatrixRotationX(&base_rot, 3.141592653589f*3.0f/2.0f);

		D3DXMATRIX rotate_mat;
		D3DXMatrixMultiply(&rotate_mat, &base_rot, &inverse_rot);
#endif

		D3DXMATRIX trans_mat;
		D3DXMatrixTranslation(&trans_mat, x(), y(), z());

		D3DXMATRIX scale_rot;
		D3DXMatrixMultiply(&scale_rot, &scale_mat, &rotate_mat);

		D3DXMATRIX world_mat;
		D3DXMatrixMultiply(&world_mat, &scale_rot, &trans_mat);

		imaged->draw_no_alpha(world_mat, material());
	}
	void billboard::draw_alpha() const
	{
		if (abs(material().Diffuse.a - 1.0f) <= 0.0000001f)	// 完全不透明なら、既に書き込まれているからここでは書き込まない
		{
			return;
		}

		D3DXMATRIX scale_mat;
		D3DXMatrixScaling(&scale_mat, scale(), scale(), scale());

#if 0
		D3DXMATRIX rotate_mat;
		//D3DXMatrixRotationX(&rotate_mat, 3.14159265f);
		D3DXMatrixRotationX(&rotate_mat, 3.141592653589f / 2.0f);
		D3DXMATRIX view_rot = si3m->camera().view_rot();
#else
		const D3DXMATRIX & view_rot = si3m->camera().view_rot();
		D3DXMATRIX inverse_rot = inverse_matrix_of_rot(view_rot);

		D3DXMATRIX base_rot;
		D3DXMatrixRotationX(&base_rot, 3.141592653589f*3.0f / 2.0f);

		D3DXMATRIX rotate_mat;
		D3DXMatrixMultiply(&rotate_mat, &base_rot, &inverse_rot);
#endif

		D3DXMATRIX trans_mat;
		D3DXMatrixTranslation(&trans_mat, x(), y(), z());

		D3DXMATRIX scale_rot;
		D3DXMatrixMultiply(&scale_rot, &scale_mat, &rotate_mat);

		D3DXMATRIX world_mat;
		D3DXMatrixMultiply(&world_mat, &scale_rot, &trans_mat);

		imaged->draw_alpha(world_mat, material(), BlendMode);
	}


	void billboard::construct()
	{
		imaged = nullptr;
		piece_size(200.0f);
		scale(1.0f);

		material().Diffuse.a = 1.0f;
		material().Diffuse.r = 1.0f;
		material().Diffuse.g = 1.0f;
		material().Diffuse.b = 1.0f;

		material().Ambient.a = 1.0f;
		material().Ambient.r = 1.0f;
		material().Ambient.g = 1.0f;
		material().Ambient.b = 1.0f;

		material().Specular.a = 0.0f;
		material().Specular.r = 0.0f;
		material().Specular.g = 0.0f;
		material().Specular.b = 0.0f;

		material().Emissive.a = 1.0f;
		material().Emissive.r = 1.0f;
		material().Emissive.g = 1.0f;
		material().Emissive.b = 1.0f;

		material().Power = 1.0f;

		setblend_normal();
	}

	void billboard::release()
	{
		if (imaged)
		{
			delete imaged;
		}

		imaged = nullptr;
		ownership = false;
	}

	billboard::~billboard()
	{
		release();
	}

#endif
}