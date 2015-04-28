#include <leaf.h>
#include <top4.h>
#include <diffuse.h>
#include <image_data.h>
#include <model_coor.h>

namespace si3
{
	leaf::leaf()
	{
		construct();
	}
	leaf::leaf(const manager & manageri, const TCHAR * path)
	{
		construct();
		load(manageri, path);
	}
	leaf::leaf(const manager & manageri, const std::string & path)
	{
		construct();
		load(manageri, path);
	}
	leaf::leaf(const manager & manageri, const ::si3::image_data & imaged)
	{
		construct();
		image_data(manageri, imaged);
	}

	void leaf::piece_size(float value)
	{
		piece_size_ = value;
	}
	float leaf::piece_size() const
	{
		return piece_size_;
	}

	bool leaf::load(const manager & manageri, const TCHAR * path)
	{
		pManager = &manageri;
		::si3::image_data * imgd = new ::si3::image_data();
		bool result = imgd->load(manageri, path, piece_size());
		image_data(manageri, *imgd);
		ownership = true;
		return result;
	}
	bool leaf::load(const manager & manageri, const std::string & path)
	{
		return load(manageri, path.c_str());
	}
	void leaf::image_data(const manager & manageri, const ::si3::image_data & imaged)
	{
		pManager = &manageri;
		release();
		this->imaged = &imaged;
	}

	void leaf::x(float value)
	{
		x_ = value;
	}
	float leaf::x() const
	{
		return x_;
	}

	void leaf::y(float value)
	{
		y_ = value;
	}
	float leaf::y() const
	{
		return y_;
	}

	void leaf::z(float value)
	{
		z_ = value;
	}
	float leaf::z() const
	{
		return z_;
	}

	void leaf::rot_x(float value)
	{
		rot_x_ = value;
	}
	float leaf::rot_x() const
	{
		return rot_x_;
	}

	void leaf::rot_y(float value)
	{
		rot_y_ = value;
	}
	float leaf::rot_y() const
	{
		return rot_y_;
	}

	void leaf::rot_z(float value)
	{
		rot_z_ = value;
	}
	float leaf::rot_z() const
	{
		return rot_z_;
	}

	void leaf::scale(float value)
	{
		scale_ = value;
	}
	float leaf::scale() const
	{
		return scale_;
	}

	uint leaf::base_width() const
	{
		return this->imaged->width();
	}
	uint leaf::base_height() const
	{
		return this->imaged->height();
	}

	float leaf::width() const
	{
		return base_width()*scale();
	}
	float leaf::height() const
	{
		return base_height()*scale();
	}

	D3DMATERIAL9 & leaf::material()
	{
		return material_;
	}
	const D3DMATERIAL9 & leaf::material() const
	{
		return material_;
	}

	void leaf::setblend_normal()
	{
		blendmode = image_data::normal_blend;
	}
	void leaf::setblend_alpha()
	{
		blendmode = image_data::alpha_blend;
	}
	void leaf::setblend_add()
	{
		blendmode = image_data::add_blend;
	}
	void leaf::setblend_sub()
	{
		blendmode = image_data::sub_blend;
	}

	void leaf::culling(bool bEnable)
	{
		bCulling = bEnable;
	}

	void leaf::affine(const matrix & world_mat)
	{
		this->world_mat = world_mat;
		bComputedWorldMat = true;
	}
	const matrix & leaf::affine() const
	{
		compute_world_mat();

		return world_mat;
	}





	void leaf::draw_no_alpha() const
	{
		compute_world_mat();
		bComputedWorldMat = false;

		if (blendmode!=image_data::normal_blend)	// 半透明処理が必要だからここでは書き込まない
		{
			return;
		}

		imaged->draw_no_alpha(world_mat.dxmat(), material(), bCulling);
	}
	void leaf::draw_alpha() const
	{
		// 完全不透明なら、既に書き込まれているからここでは書き込まない
		if (blendmode==image_data::normal_blend)
		{
			return;
		}

		imaged->draw_alpha(world_mat.dxmat(), material(), blendmode, bCulling);

	}


	void leaf::construct()
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

		bComputedWorldMat = false;
	}

	void leaf::compute_world_mat() const
	{
		if (bComputedWorldMat)
		{
			return;
		}

		world_mat.scale(scale(), scale(), scale());
		world_mat *= matrix().rotate_y(rot_y());
		world_mat *= matrix().rotate_x(rot_x());
		world_mat *= matrix().rotate_z(rot_z());
		world_mat *= matrix().parallel(x(), y(), z());
	}

	void leaf::release()
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

	leaf::~leaf()
	{
		release();
	}
}