#include <Leaf.h>
#include <top4.h>
#include <diffuse.h>
#include <image_data.h>
#include <model_coor.h>

namespace si3
{
	Leaf::Leaf()
	{
		construct();
	}
	Leaf::Leaf(const Manager & manageri, const TCHAR * path)
	{
		construct();
		load(manageri, path);
	}
	Leaf::Leaf(const Manager & manageri, const std::string & path)
	{
		construct();
		load(manageri, path);
	}
	Leaf::Leaf(const Manager & manageri, const ::si3::ImageData & imaged)
	{
		construct();
		image_data(manageri, imaged);
	}

	void Leaf::piece_size(float value)
	{
		piece_size_ = value;
	}
	float Leaf::piece_size() const
	{
		return piece_size_;
	}

	bool Leaf::load(const Manager & manageri, const TCHAR * path)
	{
		manager = &manageri;
		::si3::ImageData * imgd = new ::si3::ImageData();
		bool result = imgd->load(manageri, path, piece_size());
		image_data(manageri, *imgd);
		ownership = true;
		return result;
	}
	bool Leaf::load(const Manager & manageri, const std::string & path)
	{
		return load(manageri, path.c_str());
	}
	void Leaf::image_data(const Manager & manageri, const ::si3::ImageData & imaged)
	{
		manager = &manageri;
		release();
		this->imaged = &imaged;
	}

	void Leaf::x(float value)
	{
		x_ = value;
	}
	float Leaf::x() const
	{
		return x_;
	}

	void Leaf::y(float value)
	{
		y_ = value;
	}
	float Leaf::y() const
	{
		return y_;
	}

	void Leaf::z(float value)
	{
		z_ = value;
	}
	float Leaf::z() const
	{
		return z_;
	}

	void Leaf::rot_x(float value)
	{
		rot_x_ = value;
	}
	float Leaf::rot_x() const
	{
		return rot_x_;
	}

	void Leaf::rot_y(float value)
	{
		rot_y_ = value;
	}
	float Leaf::rot_y() const
	{
		return rot_y_;
	}

	void Leaf::rot_z(float value)
	{
		rot_z_ = value;
	}
	float Leaf::rot_z() const
	{
		return rot_z_;
	}

	void Leaf::scale(float value)
	{
		scale_ = value;
	}
	float Leaf::scale() const
	{
		return scale_;
	}

	uint Leaf::base_width() const
	{
		return this->imaged->width();
	}
	uint Leaf::base_height() const
	{
		return this->imaged->height();
	}

	float Leaf::width() const
	{
		return base_width()*scale();
	}
	float Leaf::height() const
	{
		return base_height()*scale();
	}

	D3DMATERIAL9 & Leaf::material()
	{
		return material_;
	}
	const D3DMATERIAL9 & Leaf::material() const
	{
		return material_;
	}

	void Leaf::setblend_normal()
	{
		blend_mode = ImageData::normal_blend;
	}
	void Leaf::setblend_alpha()
	{
		blend_mode = ImageData::alpha_blend;
	}
	void Leaf::setblend_add()
	{
		blend_mode = ImageData::add_blend;
	}
	void Leaf::setblend_sub()
	{
		blend_mode = ImageData::sub_blend;
	}

	void Leaf::culling(bool is_enable)
	{
		is_culling_on = is_enable;
	}

	void Leaf::affine(const Matrix & world_mat)
	{
		this->world_mat = world_mat;
		computed_world_mat = true;
	}
	const Matrix & Leaf::affine() const
	{
		compute_world_mat();

		return world_mat;
	}





	void Leaf::draw_no_alpha() const
	{
		compute_world_mat();
		computed_world_mat = false;

		if (blend_mode != ImageData::normal_blend)	// 半透明処理が必要だからここでは書き込まない
		{
			return;
		}

		imaged->draw_no_alpha(world_mat.dxmat(), material(), is_culling_on);
	}
	void Leaf::draw_alpha() const
	{
		// 完全不透明なら、既に書き込まれているからここでは書き込まない
		if (blend_mode == ImageData::normal_blend)
		{
			return;
		}

		imaged->draw_alpha(world_mat.dxmat(), material(), blend_mode, is_culling_on);

	}


	void Leaf::construct()
	{
		imaged = nullptr;
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

		culling(true);

		computed_world_mat = false;
	}

	void Leaf::compute_world_mat() const
	{
		if (computed_world_mat)
		{
			return;
		}

		world_mat.scale(scale(), scale(), scale());
		world_mat *= Matrix().rotate_y(rot_y());
		world_mat *= Matrix().rotate_x(rot_x());
		world_mat *= Matrix().rotate_z(rot_z());
		world_mat *= Matrix().parallel(x(), y(), z());
	}

	void Leaf::release()
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

	Leaf::~Leaf()
	{
		release();
	}
}