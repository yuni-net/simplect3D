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
		key_frame.radian = motion.rotation[0];
		key_frame.axis.x = motion.rotation[1];
		key_frame.axis.y = motion.rotation[2];
		key_frame.axis.z = motion.rotation[3];

		key_frame_list.add(key_frame);
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
		matrix & trans_mat,
		matrix & rot_mat,
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

		rot_mat = rot_mat_of_bone(move_data);
		matrix para_mat = para_mat_of_bone(move_data);
		trans_mat = rot_mat * para_mat;

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

		if (beg.radian != end.radian)
		{
			return true;
		}

		if (beg.axis != end.axis)
		{
			return true;
		}

		return false;
	}



	matrix BoneMotion::rot_mat_of_bone(const MoveData & move_data) const
	{
		// beg��end�̉�]�̎�������ł���Ƃ������Ƃ��O��ł���_�ɒ���

		const float beg_rot = move_data.beg->radian;
		const float end_rot = move_data.end->radian;
		const float ave_rot = (beg_rot + end_rot)*0.5f;

		const auto & axis = move_data.beg->axis;
		D3DXQUATERNION quate(ave_rot, axis.x, axis.y, axis.z);

		D3DXMATRIX rot_mat;
		D3DXMatrixRotationQuaternion(&rot_mat, &quate);

		return matrix(rot_mat);
	}
	matrix BoneMotion::para_mat_of_bone(const MoveData & move_data) const
	{
		const coor3 & beg_pos = move_data.beg->pos;
		const coor3 & end_pos = move_data.end->pos;
		coor3 ave_pos = (beg_pos + end_pos)*0.5f;
		return matrix().parallel(ave_pos.x, ave_pos.y, ave_pos.z);
	}

}