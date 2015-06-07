#include <Motion3D.h>
#include <manager.h>

namespace si3
{
	/***
	** ���f���f�[�^�ƃ��[�V�����f�[�^��ǂݍ��݂܂��B
	** ���f���f�[�^��pmd�`���A���[�V�����f�[�^��vmd�`���̂ݑΉ��ł��B
	** ���̎��_�ŃA�j���[�V�����ʒu���擪�ɐݒ肳��܂��B
	*/
	void Motion3D::load(manager & si3m, const char * model_path, const char * motion_path)
	{
		device = si3m.get_dxdevice();
		model_data.load(si3m, model_path, motion_data, bone_map);
		motion_data.load(si3m, motion_path, bone_map);
		seek_first();
		radian.x = 0.0f;
		radian.y = 0.0f;
		radian.z = 0.0f;
	}

	/***
	** �A�j���[�V�������Đ���Ԃɂ��܂�
	*/
	void Motion3D::play()
	{
		motion_data.play();
	}

	/***
	** �A�j���[�V�������ꎞ��~��Ԃɂ��܂�
	*/
	void Motion3D::pause()
	{
		motion_data.pause();
	}

	/***
	** �A�j���[�V�����ʒu��擪�ɐݒ肵�܂�
	*/
	void Motion3D::seek_first()
	{
		motion_data.seek_first();
	}

	// @override
	void Motion3D::draw_no_alpha() const
	{
		model_data.animation(motion_data);

		compute_world_mat();

		// ���[���h�ϊ��s��ݒ�
		device->SetTransform(D3DTS_WORLD, world_mat.dxpointer());

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// Z�o�b�t�@���X�V����
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// ���e�X�g�֎~
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// �������\���֎~(���u�����f�B���O������)
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		// �}�e���A���ƃe�N�X�`����rgba�����|�����킹��ݒ�
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

		model_data.draw_no_alpha();
	}

	// @override
	void Motion3D::draw_alpha() const
	{
		// ���[���h�ϊ��s��ݒ�
		device->SetTransform(D3DTS_WORLD, world_mat.dxpointer());

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// Z�o�b�t�@���X�V���Ȃ�
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// ���e�X�g�L��
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

		// �������`��L��
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// �}�e���A���ƃe�N�X�`����rgba�����|�����킹��ݒ�
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

		model_data.draw_alpha();
	}





	void Motion3D::compute_world_mat() const
	{
		matrix para_mat;
		para_mat.parallel(pos.x, pos.y, pos.z);

		matrix rot_mat =
			rot_mat.rotate_y(radian.y) *
			rot_mat.rotate_x(radian.x) *
			rot_mat.rotate_z(radian.z);

		world_mat = rot_mat * para_mat;
	}
}