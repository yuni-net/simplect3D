#include <algorithm>
#include <BoneMotion.h>

namespace si3
{


	/***
	* @brief �L�[�t���[������ǉ����܂��B
	*/
	void BoneMotion::add_key_frame(const Motion & motion)
	{
		KeyFrame key_frame;
		key_frame.frame = motion.frame_No;
		key_frame.pos.x = motion.location[0];
		key_frame.pos.y = motion.location[1];
		key_frame.pos.z = motion.location[2];
		key_frame.quate.x = motion.rotation[0];
		key_frame.quate.y = motion.rotation[1];
		key_frame.quate.z = motion.rotation[2];
		key_frame.quate.w = motion.rotation[3];

		key_frame_list.push_back(key_frame);
	}

	void BoneMotion::finish_add_key_frame()
	{
		std::sort(key_frame_list.begin(), key_frame_list.end());
	}

	/***
	* @brief ���݂̃t���[���̂��̃{�[���̍��W�ϊ��s����v�Z���܂��B
	* @param
	*  [out]trans_mat: �ϊ��s�񂪂����Ɋi�[����܂�
	*  [out]rot_mat: �ϊ��s��̉�]�����݂̂������Ɋi�[����܂�
	*  now_frame: ���݂̃t���[���w�肵�܂�
	* @return
	*  ���W��ϊ�����K�v������ꍇ��true���A���������ϊ�����K�v�������ꍇ��false��Ԃ��܂��B
	*  �Ⴆ�ΑO�̃t���[������ω���������΍��W���ēx�ϊ�����K�v�͖����킯�ł��B
	*  �������A�K���P�t���[�������ԂɃA�j���[�V�������邱�Ƃ��O��ɂȂ��Ă��܂��B
	*  �t�Đ���A�C�ӂ̃t���[���Ƀe���|�[�g�����ꍇ�͓��R�ϊ�����K�v������ł��傤�B
	*/
	bool BoneMotion::compute_trans_mat(
		Matrix & trans_mat,
		Matrix & rot_mat,
		const Coor3 & bone_pos,
		const int now_frame)
	{
		MoveData move_data;
		bool todo_trans;
		bool todo_judge_move;

		compute_move_data(move_data, todo_trans, todo_judge_move, now_frame);

		if (todo_trans == false)
		{
			return false;
		}

		if (todo_judge_move)
		{
			if (did_bone_move(move_data) == false)
			{
				return false;
			}
		}

		const float percent = compute_percent(move_data, now_frame);

		D3DXQUATERNION quate;
		rot_mat = rot_mat_of_bone(move_data, percent, quate);

		trans_mat = trans_mat_of_bone(move_data, percent, bone_pos, quate);

		return true;
	}



	void BoneMotion::compute_move_data(
		MoveData & move_data,
		bool & todo_trans,
		bool & todo_judge_move,
		const int now_frame)
	{
		// �܂��L�[�t���[��������Ȃ��ꍇ�͖ⓚ���p��
		if (key_frame_list.size() <= 0)
		{
			todo_trans = false;
			return;
		}

		if (now_frame < key_frame_list[0].frame)
		{
			todo_trans = false;
			return;
		}

		if (key_frame_list.size() == 1)
		{
			/***
			* �L�[�t���[������̏ꍇ�A�K��0�t���[���ڂŐݒ肳��Ă���Ɖ��肷��
			* ���_����0�t���[���ڂŕύX���A�ȍ~�͕ύX����K�v���Ȃ�
			*/

			if (now_frame > 0)
			{
				todo_trans = false;
				return;
			}

			move_data.beg = &(key_frame_list[0]);
			move_data.end = &(key_frame_list[0]);
			todo_trans = true;
			todo_judge_move = false;
			return;
		}

		const int final_index = key_frame_list.size() - 1;
		const int final_frame = key_frame_list[final_index].frame;
		if (now_frame > final_frame)
		{
			todo_trans = false;
			return;
		}

		// �L�[�t���[����������Ԃɑ������āA
		// ���݂̈ʒu��T��
		fw::uint beg_index = 0;
		for (fw::uint frame_No = 1; frame_No < key_frame_list.size(); ++frame_No)
		{
			const int key_frame = key_frame_list[frame_No].frame;
			if (now_frame < key_frame)
			{
				// ���݂̈ʒu���킩������E�o
				break;
			}

			// ���݈ʒu�͊m��ł͂Ȃ����b��I�ɐi�񂾂Ƃ���ɂ��Ă���
			beg_index = frame_No;
		}

		if (beg_index == final_index)
		{
			--beg_index;
		}

		move_data.beg = &(key_frame_list[beg_index]);
		move_data.end = &(key_frame_list[beg_index + 1]);

		todo_trans = true;
		todo_judge_move = true;
	}

	/***
	* move_data��beg��end���قȂ�l�ł��邩�ǂ����𔻒肵�܂��B
	* �قȂ�ꍇ��true���A�����ł���ꍇ��false��Ԃ��܂��B
	*/
	bool BoneMotion::did_bone_move(MoveData & move_data) const
	{
		const KeyFrame & beg = *(move_data.beg);
		const KeyFrame & end = *(move_data.end);

		if (beg.pos != end.pos)
		{
			return true;
		}

		if (beg.quate.x != end.quate.x ||
			beg.quate.y != end.quate.y || 
			beg.quate.z != end.quate.z || 
			beg.quate.w != end.quate.w )
		{
			return true;
		}

		return false;
	}

	float BoneMotion::compute_percent(MoveData & move_data, const int now_frame) const
	{
		const KeyFrame & beg = *(move_data.beg);
		const KeyFrame & end = *(move_data.end);

		const int rel_frame = now_frame - beg.frame;

		if (rel_frame == 0)
		{
			return 0.0f;
		}

		const int gap_frame = end.frame - beg.frame;

		return static_cast<float>(rel_frame) / gap_frame;
	}




	/***
	* @brief move_data�Ɋ�Â����݂̃t���[���̉�]�s����v�Z���Ă����Ԃ��B
	* @param
	*  move_data: MoveData�̃C���X�^���X���w�肵�܂��B
	*  percent: beg�t���[����end�t���[���̊Ԃ̂ǂ��ɋ���̂��������Ŏw�肵�܂�(0.0f-1.0f)�B
	*  [out]quate: �v�Z�̌��ʓ���ꂽ�N�H�[�^�j�I���������Ɋi�[����܂��B
	* @return ���݂̃t���[���̉�]�s���Ԃ��B
	*/
	Matrix BoneMotion::rot_mat_of_bone(const MoveData & move_data, const float percent, D3DXQUATERNION & quate) const
	{
		const Quaternion & quate_beg = move_data.beg->quate;
		const Quaternion & quate_end = move_data.end->quate;
		Quaternion quate_now = quate_beg + (quate_end - quate_beg)*percent;
		
		D3DXQUATERNION quate_dx(quate_now.x, quate_now.y, quate_now.z, quate_now.w);
		quate = quate_dx;

		D3DXMATRIX rot_mat;
		D3DXMatrixRotationQuaternion(&rot_mat, &quate);

		return Matrix(rot_mat);
	}

#if 0
	Matrix BoneMotion::para_mat_of_bone(const MoveData & move_data, const float percent) const
	{
		const Coor3 & beg_pos = move_data.beg->pos;
		const Coor3 & end_pos = move_data.end->pos;
		Coor3 now_pos = (beg_pos + end_pos)*percent;
		return Matrix().parallel(now_pos.x, now_pos.y, now_pos.z);
	}
#endif

	/***
	* @brief move_data�Ɋ�Â����݂̃t���[���̕ϊ��s����v�Z���Ă����Ԃ��B
	* @param
	*  move_data: MoveData�̃C���X�^���X���w�肵�܂��B
	*  percent: beg�t���[����end�t���[���̊Ԃ̂ǂ��ɋ���̂��������Ŏw�肵�܂�(0.0f-1.0f)�B
	*  bone_top: �{�[���̒��_���w�肵�܂��B���ꂪ��]�̒��S���W�ƂȂ�܂��B
	*  quate: ��]�Ɏg�p����N�H�[�^�j�I���������Ɏw�肵�܂��B
	* @return ���݂̃t���[���̕ϊ��s���Ԃ��B
	*/
	Matrix BoneMotion::trans_mat_of_bone(const MoveData & move_data, const float percent, const Coor3 & bone_top, const D3DXQUATERNION & quate) const
	{
		const Coor3 & beg_pos = move_data.beg->pos;
		const Coor3 & end_pos = move_data.end->pos;
		Coor3 now_pos = beg_pos + (end_pos - beg_pos)*percent;

		D3DXVECTOR3 scaling;
		scaling.x = 1.0f;
		scaling.y = 1.0f;
		scaling.z = 1.0f;

		D3DXVECTOR3 bone_top_dx;
		bone_top_dx.x = bone_top.x;
		bone_top_dx.y = bone_top.y;
		bone_top_dx.z = bone_top.z;

		D3DXVECTOR3 translation;
		translation.x = now_pos.x;
		translation.y = now_pos.y;
		translation.z = now_pos.z;

		D3DXMATRIX trans_mat;
		D3DXMatrixTransformation(
			&trans_mat,
			NULL,
			NULL,
			&scaling,
			&bone_top_dx,
			&quate,
			&translation);

		return Matrix(trans_mat);
	}

	bool BoneMotion::axis_is_unit(const Coor3 & axis) const
	{
		if (abs(axis.x) >= 0.00001f)
		{
			return false;
		}

		if (abs(axis.y) >= 0.00001f)
		{
			return false;
		}

		if (abs(axis.z - 1.0f) >= 0.00001f)
		{
			return false;
		}

		return true;
	}

}