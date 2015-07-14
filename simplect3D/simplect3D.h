/*
 * WELCOME TO simplect3D LIBRARY !
 * 
 * Version: 5.0.1
 * FinalUpdate: Feb 8th 2015
 * Author: NH 30070 AT12A171 10
 */

#pragma once

#include "Manager.h"
#include "Model.h"
#include "model_data.h"
#include "Leaf.h"
#include "image_data.h"
#include "sphere.h"
#include "billboard.h"
#include "Anime.h"
#include "sound_data.h"
#include "unique_sound.h"
#include "se.h"
#include "bunisher.h"
#include "sky_dome.h"
#include "sky_dome_mini.h"
#include "box.h"
#include "sphere.h"
#include "Motion3D.h"

#ifndef simplect3D_developping_
#ifdef _DEBUG
#pragma comment(lib, "simplect3dd.lib")
#else
#pragma comment(lib, "simplect3d.lib")
#endif
#endif


// release note ***********************************************************

#if 0

[5.0.1]
*when Debug, bunisher does not sort

[5.0.0]
*bunisher was implemented

[3.1.0]
*unique_sound, sound_manager, sound_data, se を実装

[3.0.0]
*xファイルのサポートを廃止
*pmdファイルのサポートに変更
*Model 両面描画、ライティングOFF設定に対応
*camera view変換行列を取り出す関数を実装

[2.0.0]
*Matrixのデフォルト初期値を単位行列に設定するよう変更
*Matrixのmake_*系関数をstaticではないものに変更
*Modelのplus_*系インターフェイスを廃止
*Modelにてxyz回転の順番を指定できる機能を追加
*MatrixをD3DXMATRIXで初期化できる機能を追加
*Leafおよびbillboardにaffin関数を追加
*Matrixが生成する回転行列を修正
*Leaf::image_data(), billboard::image_data()を用いた際にManagerのポインタを取得するように修正
*Modelでカリングの有効無効を指定する機能を追加
*Modelでmodel_dataを指定するときにもManagerのインスタンスを指定するよう変更

[1.2.0]
*context colorが反映されるように修正
*モデル読み込みの際、中心点を指定できる機能を追加
*model_data draw_alpha()の際、ワールド変換行列の指定が抜けていたので追加
*materialの値を変えても反映されていなかったようなので修正
*LeafにてカリングのON, OFFを切り替える機能を追加

[1.1.0]
*Leaf release()のときownershipに応じて処理を変えるよう修正
*anime_data, Anime を実装
*Anime, Leaf で、compute_world_matの呼び出し位置を修正
*billboardに既定のコンストラクタを実装
*DisplayManagerにてcontext colorを設定することができるようになった

[1.0.2]
*Animeを追加したが、Leafをコピーしただけ。

[1.0.1]
*Leafの変更に伴ってbillboardも修正。Leafを継承する実装にした

[1.0.0]
*landのデフォルト方向をZマイナス方向にし、地面ではなくなったため名称をLeafに変更

[0.1.0]
*Leafで回転機能を追加

[0.0.1]
*model_dataのdraw_alphaでalpha値が1.0fと一致しているかどうかの判定を、==ではなくabsを用いて判定するよう変更
*model_dataの読み込みの際、最初にrelease()を呼ぶように変更

#endif