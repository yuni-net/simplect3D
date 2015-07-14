#include <Anime.h>
#include <top4.h>
#include <diffuse.h>
#include <image_data.h>
#include <model_coor.h>
#include <utility.h>

namespace si3
{
	Anime::Anime()
	{
		construct();
	}
#if 0
	Anime::Anime(const Manager & manageri, const TCHAR * path)
	{
		construct();
		load(manageri, path);
	}
	Anime::Anime(const Manager & manageri, const std::string & path)
	{
		construct();
		load(manageri, path);
	}
#endif
	Anime::Anime(const Manager & manageri, const ::si3::AnimeData & animed)
	{
		construct();
		anime_data(manageri, animed);
	}

	void Anime::piece_size(float value)
	{
		piece_size_ = value;
	}
	float Anime::piece_size() const
	{
		return piece_size_;
	}

#if 0
	bool Anime::load(const Manager & manageri, const TCHAR * path)
	{
		pManager = &manageri;
		::si3::ImageData * imgd = new ::si3::ImageData();
		bool result = imgd->load(manageri, path, piece_size());
		image_data(manageri, *imgd);
		ownership = true;
		return result;
	}
	bool Anime::load(const Manager & manageri, const std::string & path)
	{
		return load(manageri, path.c_str());
	}
#endif
	void Anime::anime_data(const Manager & manageri, const ::si3::AnimeData & animed)
	{
		const int c_nDefaultResistor = 4;

		release();
		this->animed = &animed;
		cnt.config(animed.enable_num(), c_nDefaultResistor);
		cnt.set(0);
	}

	void Anime::x(float value)
	{
		x_ = value;
	}
	float Anime::x() const
	{
		return x_;
	}

	void Anime::y(float value)
	{
		y_ = value;
	}
	float Anime::y() const
	{
		return y_;
	}

	void Anime::z(float value)
	{
		z_ = value;
	}
	float Anime::z() const
	{
		return z_;
	}

	void Anime::rot_x(float value)
	{
		rot_x_ = value;
	}
	float Anime::rot_x() const
	{
		return rot_x_;
	}

	void Anime::rot_y(float value)
	{
		rot_y_ = value;
	}
	float Anime::rot_y() const
	{
		return rot_y_;
	}

	void Anime::rot_z(float value)
	{
		rot_z_ = value;
	}
	float Anime::rot_z() const
	{
		return rot_z_;
	}

	void Anime::scale(float value)
	{
		scale_ = value;
	}
	float Anime::scale() const
	{
		return scale_;
	}

	uint Anime::base_width() const
	{
		return this->animed->width();
	}
	uint Anime::base_height() const
	{
		return this->animed->height();
	}

	float Anime::width() const
	{
		return base_width()*scale();
	}
	float Anime::height() const
	{
		return base_height()*scale();
	}

	D3DMATERIAL9 & Anime::material()
	{
		return material_;
	}
	const D3DMATERIAL9 & Anime::material() const
	{
		return material_;
	}

	void Anime::setblend_normal()
	{
		blend_mode = BlendMode::normal;
	}
	void Anime::setblend_alpha()
	{
		blend_mode = BlendMode::alpha;
	}
	void Anime::setblend_add()
	{
		blend_mode = BlendMode::add;
	}
	void Anime::setblend_sub()
	{
		blend_mode = BlendMode::sub;
	}


	void si3::Anime::animation()
	{
		cnt.move(1);
	}
	void si3::Anime::animation_once()
	{
		int pre = cnt.now();
		cnt.move(1);
		if (cnt.now() < pre)
		{
			cnt.move(-1);
		}
	}

	void si3::Anime::resistor(int v)
	{
		cnt.resistor(v);
	}
	void si3::Anime::replay()
	{
		cnt.set(0);
	}
	bool si3::Anime::is_final_frame()
	{
		return cnt.now_No() >= cnt.max_No();
	}

	void si3::Anime::set(int nNo)
	{
		cnt.set(nNo);
	}
	int si3::Anime::max_No()
	{
		return cnt.max_No();
	}





	void Anime::draw_no_alpha() const
	{
		compute_world_mat();

		if (blend_mode != BlendMode::normal)	// 半透明処理が必要だからここでは書き込まない
		{
			return;
		}


		animed->draw_no_alpha(world_mat, material(), cnt.now());
	}
	void Anime::draw_alpha() const
	{
		// 完全不透明なら、既に書き込まれているからここでは書き込まない
		if (blend_mode == BlendMode::normal)
		{
			return;
		}

		animed->draw_alpha(world_mat, material(), blend_mode, cnt.now());
	}


	void Anime::construct()
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

	void Anime::compute_world_mat() const
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

	void Anime::release()
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

	Anime::~Anime()
	{
		release();
	}
}