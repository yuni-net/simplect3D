#include <anime.h>
#include <top4.h>
#include <diffuse.h>
#include <image_data.h>
#include <model_coor.h>
#include <utility.h>

namespace si3
{
	anime::anime()
	{
		construct();
	}
#if 0
	anime::anime(const Manager & manageri, const TCHAR * path)
	{
		construct();
		load(manageri, path);
	}
	anime::anime(const Manager & manageri, const std::string & path)
	{
		construct();
		load(manageri, path);
	}
#endif
	anime::anime(const Manager & manageri, const ::si3::anime_data & animed)
	{
		construct();
		anime_data(manageri, animed);
	}

	void anime::piece_size(float value)
	{
		piece_size_ = value;
	}
	float anime::piece_size() const
	{
		return piece_size_;
	}

#if 0
	bool anime::load(const Manager & manageri, const TCHAR * path)
	{
		pManager = &manageri;
		::si3::image_data * imgd = new ::si3::image_data();
		bool result = imgd->load(manageri, path, piece_size());
		image_data(manageri, *imgd);
		ownership = true;
		return result;
	}
	bool anime::load(const Manager & manageri, const std::string & path)
	{
		return load(manageri, path.c_str());
	}
#endif
	void anime::anime_data(const Manager & manageri, const ::si3::anime_data & animed)
	{
		const int c_nDefaultResistor = 4;

		release();
		this->animed = &animed;
		cnt.config(animed.enable_num(), c_nDefaultResistor);
		cnt.set(0);
	}

	void anime::x(float value)
	{
		x_ = value;
	}
	float anime::x() const
	{
		return x_;
	}

	void anime::y(float value)
	{
		y_ = value;
	}
	float anime::y() const
	{
		return y_;
	}

	void anime::z(float value)
	{
		z_ = value;
	}
	float anime::z() const
	{
		return z_;
	}

	void anime::rot_x(float value)
	{
		rot_x_ = value;
	}
	float anime::rot_x() const
	{
		return rot_x_;
	}

	void anime::rot_y(float value)
	{
		rot_y_ = value;
	}
	float anime::rot_y() const
	{
		return rot_y_;
	}

	void anime::rot_z(float value)
	{
		rot_z_ = value;
	}
	float anime::rot_z() const
	{
		return rot_z_;
	}

	void anime::scale(float value)
	{
		scale_ = value;
	}
	float anime::scale() const
	{
		return scale_;
	}

	uint anime::base_width() const
	{
		return this->animed->width();
	}
	uint anime::base_height() const
	{
		return this->animed->height();
	}

	float anime::width() const
	{
		return base_width()*scale();
	}
	float anime::height() const
	{
		return base_height()*scale();
	}

	D3DMATERIAL9 & anime::material()
	{
		return material_;
	}
	const D3DMATERIAL9 & anime::material() const
	{
		return material_;
	}

	void anime::setblend_normal()
	{
		nBlendmode = blendmode::normal;
	}
	void anime::setblend_alpha()
	{
		nBlendmode = blendmode::alpha;
	}
	void anime::setblend_add()
	{
		nBlendmode = blendmode::add;
	}
	void anime::setblend_sub()
	{
		nBlendmode = blendmode::sub;
	}


	void si3::anime::animation()
	{
		cnt.move(1);
	}
	void si3::anime::animation_once()
	{
		int pre = cnt.now();
		cnt.move(1);
		if (cnt.now() < pre)
		{
			cnt.move(-1);
		}
	}

	void si3::anime::resistor(int v)
	{
		cnt.resistor(v);
	}
	void si3::anime::replay()
	{
		cnt.set(0);
	}
	bool si3::anime::ifFinalFrame()
	{
		return cnt.nowNo() >= cnt.maxNo();
	}

	void si3::anime::set(int nNo)
	{
		cnt.set(nNo);
	}
	int si3::anime::maxNo()
	{
		return cnt.maxNo();
	}





	void anime::draw_no_alpha() const
	{
		compute_world_mat();

		if (nBlendmode!=blendmode::normal)	// 半透明処理が必要だからここでは書き込まない
		{
			return;
		}


		animed->draw_no_alpha(world_mat, material(), cnt.now());
	}
	void anime::draw_alpha() const
	{
		// 完全不透明なら、既に書き込まれているからここでは書き込まない
		if (nBlendmode==blendmode::normal)
		{
			return;
		}

		animed->draw_alpha(world_mat, material(), nBlendmode, cnt.now());
	}


	void anime::construct()
	{
		animed = nullptr;
		piece_size(200.0f);
		scale(1.0f);
		rot_x(0.0f);
		rot_y(0.0f);
		rot_z(0.0f);

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

		material().Emissive.a = 0.0f;
		material().Emissive.r = 0.0f;
		material().Emissive.g = 0.0f;
		material().Emissive.b = 0.0f;

		material().Power = 1.0f;
	}

	void anime::compute_world_mat() const
	{
		D3DXMATRIX scale_mat;
		D3DXMatrixScaling(&scale_mat, scale(), scale(), scale());

		D3DXMATRIX rotate_mat_x;
		D3DXMatrixRotationX(&rotate_mat_x, rot_x());
		D3DXMATRIX rotate_mat_y;
		D3DXMatrixRotationY(&rotate_mat_y, rot_y());
		D3DXMATRIX rotate_mat_z;
		D3DXMatrixRotationZ(&rotate_mat_z, rot_z());
		D3DXMATRIX rotate_mat;
		D3DXMatrixMultiply(&rotate_mat, &rotate_mat_x, &rotate_mat_y);
		D3DXMatrixMultiply(&rotate_mat, &rotate_mat, &rotate_mat_z);

		D3DXMATRIX trans_mat;
		D3DXMatrixTranslation(&trans_mat, x(), y(), z());

		D3DXMATRIX scale_rot;
		D3DXMatrixMultiply(&scale_rot, &scale_mat, &rotate_mat);

		D3DXMatrixMultiply(&world_mat, &rotate_mat, &scale_mat);

		D3DXMatrixMultiply(&world_mat, &world_mat, &trans_mat);
	}

	void anime::release()
	{
		if (animed)
		{
			if (ownership)
			{
				delete animed;
			}
		}

		animed = nullptr;
		ownership = false;
	}

	anime::~anime()
	{
		release();
	}
}