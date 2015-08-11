#include <si3_Seal.h>

namespace si3
{
	Seal::Seal()
	{
		construct();
	}
	Seal::Seal(const TCHAR * path)
	{
		construct();
		load(path);
	}
	Seal::Seal(const std::string & path)
	{
		construct();
		load(path);
	}
	Seal::Seal(const ::si3::SealData & imaged)
	{
		construct();
		set_seal_data(imaged);
	}

	void Seal::piece_size(float value)
	{
		piece_size_ = value;
	}
	float Seal::piece_size() const
	{
		return piece_size_;
	}

	bool Seal::load(const TCHAR * path)
	{
		::si3::SealData * imgd = new ::si3::SealData();
		bool result = imgd->load(path, piece_size());
		set_seal_data(*imgd);
		ownership = true;
		return result;
	}
	bool Seal::load(const std::string & path)
	{
		return load(path.c_str());
	}
	void Seal::set_seal_data(const ::si3::SealData & imaged)
	{
		release();
		this->imaged = &imaged;
	}


	void Seal::set_scale(float value)
	{
		scale_x = value;
		scale_y = value;
	}

	uint Seal::base_width() const
	{
		return this->imaged->width();
	}
	uint Seal::base_height() const
	{
		return this->imaged->height();
	}

	float Seal::width() const
	{
		return base_width()*scale_x;
	}
	float Seal::height() const
	{
		return base_height()*scale_y;
	}

	D3DMATERIAL9 & Seal::material()
	{
		return material_;
	}
	const D3DMATERIAL9 & Seal::material() const
	{
		return material_;
	}

	void Seal::setblend_normal()
	{
		blend_mode = SealData::normal_blend;
	}
	void Seal::setblend_alpha()
	{
		blend_mode = SealData::alpha_blend;
	}
	void Seal::setblend_add()
	{
		blend_mode = SealData::add_blend;
	}
	void Seal::setblend_sub()
	{
		blend_mode = SealData::sub_blend;
	}


	void Seal::affine(const Matrix & world_mat)
	{
		this->world_mat = world_mat;
		computed_world_mat = true;
	}
	const Matrix & Seal::affine() const
	{
		compute_world_mat();

		return world_mat;
	}





	void Seal::draw_no_alpha() const
	{
		compute_world_mat();
		computed_world_mat = false;

		if (blend_mode != SealData::normal_blend)	// 半透明処理が必要だからここでは書き込まない
		{
			return;
		}

		imaged->draw_no_alpha(world_mat.dxmat(), material());
	}
	void Seal::draw_alpha() const
	{
		// 完全不透明なら、既に書き込まれているからここでは書き込まない
		if (blend_mode == SealData::normal_blend)
		{
			return;
		}

		imaged->draw_alpha(world_mat.dxmat(), material(), blend_mode);

	}


	void Seal::construct()
	{
		imaged = nullptr;
		piece_size(200.0f);
		z = 0.0f;
		set_scale(1.0f);
		radian = 0.0f;

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

		computed_world_mat = false;
	}

	void Seal::compute_world_mat() const
	{
		if (computed_world_mat)
		{
			return;
		}

		world_mat.scale(scale_x, scale_y, 1.0f);
		world_mat *= Matrix().rotate_z(radian);
		world_mat *= Matrix().parallel(x, y, z);
	}

	void Seal::release()
	{
		if (imaged)
		{
			if (ownership)
			{
				delete imaged;
			}
		}

		imaged = nullptr;
		ownership = false;
	}

	Seal::~Seal()
	{
		release();
	}

}