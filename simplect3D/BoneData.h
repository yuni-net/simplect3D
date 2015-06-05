#ifndef si3_BoneData_h_
#define si3_BoneData_h_

#include "popular.h"
#include "si3_coor3.h"
#include "Top_pmd.h"
#include "util_MMD.h"
#include "matrix.h"

namespace si3
{
	class BoneData
	{
	public:
		/***
		* ボーンと頂点の関係データを白紙に戻します。
		*/
		void init_top_lists();

		/***
		* @brief キーフレーム情報を追加します。
		*/
		void add_key_frame(const Motion & motion);


		void compute_trans_mat(const int now_frame);

		/***
		* ボーンと頂点の関係データを新しく追加します。
		*/
		void add_associated_top(const int index, Top_pmd & top, const bool is_main);


	private:
		struct BonedTop
		{
			int index;
			float pos[3];
			float normal[3];
			float bone_weight;

			void init(const int index, const Top_pmd & top, const unsigned char bone_weight)
			{
				this->index = index;

				pos[0] = top.pos[0];
				pos[1] = top.pos[1];
				pos[2] = top.pos[2];

				normal[0] = top.normal[0];
				normal[1] = top.normal[1];
				normal[2] = top.normal[2];

				this->bone_weight = bone_weight * 0.01f;
			}
		};

		struct KeyFrame
		{
			int frame;
			coor3 pos;
			float radian;
			coor3 axis;
		};

		struct MoveData
		{
			const KeyFrame * beg;
			const KeyFrame * end;
		};


		
		fw::vector<BonedTop> associated_tops;
		fw::vector<KeyFrame> key_frame_list;



		bool did_bone_move(MoveData & move_data, const int now_frame) const;
	//	void trans_and_set(D3DVECTOR & target, const float base[3], const matrix & mat);
		matrix BoneData::rot_mat_of_bone(const MoveData & move_data) const;
		matrix BoneData::para_mat_of_bone(const MoveData & move_data) const;

	};
}

#endif