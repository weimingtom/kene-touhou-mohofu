
#include "game_main.h"

/*---------------------------------------------------------
  東方模倣風 〜 Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	弾システム
	-------------------------------------------------------
	(r33)もう一回汎用的な弾発射システムを構築し直してみる。
	旧 bullet_angle.c と
	旧 bullet_vector.c は
	ここに統合される予定。
	レーザー関連は統合されないで、別システムになる予定。
	-------------------------------------------------------
	(r32-)喰み出しチェックについて、
	「移動時に」喰み出しチェックをしない事が前提で設計されている弾幕が多い。
	この為「移動時に」システムで喰み出しチェックをしません。
	例えば画面端で跳ね返る弾幕は「移動時に」喰み出しチェックをしない事により実現している機能です。
	「移動と別で」弾幕リストを調べて「喰み出しチェック」を行います。
---------------------------------------------------------*/

/*---------------------------------------------------------
	交差弾は色々弾によって違いが多すぎる。(個々の時間とか)
	全部システム側で吸収すると、遅くなる気がする。
	交差弾システム側で吸収するなら、角度弾と統合して角度弾無くすべき
	だけど(将来はともかく)現状では角度弾汎用にすると遅すぎる。
	-------------------------------------------------------
	現状の速度なら、交差弾は弾幕リスト書き換えで対応すべき。
	(パチェとか書き換えの嵐になりそうですが。)
	システム側交差弾対応は、無くなりそう。
---------------------------------------------------------*/

/*---------------------------------------------------------
	角度弾、基本定義
	-------------------------------------------------------
	角度弾では、ベクトル移動をしない。
	代わりに基点座標として使う。
---------------------------------------------------------*/

#if 1/* 角度弾規格(策定案tama_system) */
	#define tx256				vx256/* 基点座標x */
	#define ty256				vy256/* 基点座標y */

#endif

/*---------------------------------------------------------
	ベクトル弾、基本定義
---------------------------------------------------------*/

#if 1/* ベクトル弾規格(策定案) */


#endif

/*---------------------------------------------------------
	ベクトル弾
	-------------------------------------------------------
	ベクトル弾の移動を行う。
---------------------------------------------------------*/

#if 0
/* 何もしない(等速直線移動) */
static void move_bullet_vector_wide100(SPRITE *src)
{
	src->cx256 += (src->tx256);/*fps_factor*/
	src->cy256 += (src->ty256);/*fps_factor*/
}
#endif


/*---------------------------------------------------------
	角度弾
	-------------------------------------------------------
	角度弾の移動を行う。
---------------------------------------------------------*/

#define tama_system_kousadan_angle65536 tmp_angleCCW1024

/*---------------------------------------------------------
	#001 角度弾の移動を行う(通常弾用)「正直分けたくないけど、分けてみる。」
---------------------------------------------------------*/

static void tama_system_move_angle_001(SPRITE *src)
{
	#if 1/* 検討中(無くても出来るけど、あった方が簡単) */
	src->tama_system_speed65536 		+= (src->tama_system_tra65536); 		/* 加減速調整 */
	#endif
//	src->tama_system_radius256			+= (src->tama_system_speed256); 		/* 速度 */
	src->tama_system_radius256			+= (src->tama_system_speed65536>>8);	/* 速度 */
	#if 0/* 検討中(無くても出来るけど、あった方が簡単) */
	/* パチェ() とか ルーミア(交差弾) とかに必要(無くても出来るけど、あった方が簡単) */
	/* 交差弾の場合この方式だとグラが難しい。(?) */
	src->rotationCCW1024				+= ((src->tama_system_add_rotate1024)>>8);			/* 回転角度調整 */
	#endif
//
	src->cx256 = (src->tx256) + ((sin1024((src->rotationCCW1024))*(src->tama_system_radius256))>>8);/*fps_factor*/
	src->cy256 = (src->ty256) + ((cos1024((src->rotationCCW1024))*(src->tama_system_radius256))>>8);/*fps_factor*/
}

//	move_bullet_vector_wide100(src);
	#if 0/* 検討中 */
	check_bullet_angle01(src);
	#endif

//	src->base_time_out--;/*fps_factor*/
//	if (src->base_time_out < 0)
//	{
//		src->callback_mover 				= move_bullet_vector_wide100;
//	}
//	/* 表示用 */


//
//	mask1024(src->rotationCCW1024);


/*---------------------------------------------------------
	発弾エフェクト
	-------------------------------------------------------
	発弾エフェクトを行う。
---------------------------------------------------------*/

/*---------------------------------------------------------
	発弾エフェクト(共通部分)
---------------------------------------------------------*/
static void tama_system_common_hatudan_000(SPRITE *src)
{
	int aaa = (src->tama_system_hatsudan_counter);
	src->cx256 = (src->tx256) + ((sin1024((src->rotationCCW1024))*(256-aaa)) );/*fps_factor*/
	src->cy256 = (src->ty256) + ((cos1024((src->rotationCCW1024))*(256-aaa)) );/*fps_factor*/
//	src->cx256 = (src->tx256);/*fps_factor*/
//	src->cy256 = (src->ty256);/*fps_factor*/
	src->color32		= ((aaa)<<(24))|0x00ffffff;
	src->m_zoom_x256	= t256(3.0)-(aaa)-(aaa);
	src->m_zoom_y256	= t256(3.0)-(aaa)-(aaa);
	src->tama_system_hatsudan_counter += 4;
}

/*---------------------------------------------------------
	発弾エフェクト	傾かない弾
---------------------------------------------------------*/
/*---------------------------------------------------------
	発弾エフェクト	傾き弾(通常)
---------------------------------------------------------*/

static void move_bullet_hatsudan100(SPRITE *src)
{
	tama_system_common_hatudan_000(src);
	if (255 < src->tama_system_hatsudan_counter)
	{
		src->tama_system_hatsudan_counter = (0xff);
		src->flags |= (SP_FLAG_COLISION_CHECK);/* あたり判定有効 */
		src->callback_mover 			= tama_system_move_angle_001;
		if (src->tama_system_tama_data & TAMA_DATA_8000_NON_TILT)/* 非傾き弾 */
		{
			src->m_zoom_y256 = M_ZOOM_Y256_NO_TILT;/* 特殊機能で傾かないようシステム拡張(r33)。 */
		}
	}
}





/*---------------------------------------------------------
	弾を登録する。
	-------------------------------------------------------
	関数を(クラスっぽく)隠蔽。
	弾発動時のエフェクト等の関係上、発弾部分は全部纏める必要がある。
	-------------------------------------------------------
	ベクトル弾と角度弾を統合するかもしれない。
---------------------------------------------------------*/


/*---------------------------------------------------------
	標準の角度弾を複数(n way弾)登録する。
	-------------------------------------------------------
	弾の発弾方式は、1ヶ所に纏める予定なのだが、
	現在仕様が決まっていない。
	弾の発弾が、1ヶ所に纏まっていないと、発弾のエフェクトが作れない。
	「発弾のエフェクトが作れない」というのは、
	弾の発弾が、1ヶ所に纏まっていないのに強引に「作ると遅くなる」からやりたくないという意味。
	現在(r33)も速度がネック(遅い)になっているので、そういう事はしない。というか出来ない。
	もちろん1ヶ所に纏まって速度も解消できたら発弾のエフェクトを作る予定。
	現在(r33)の速度低下は、
		サイドのパネルがSDL描画。
		会話がSDL描画。
		Gu回転描画で横sliceしてない。
		ボス中にも雑魚の処理(リスト処理)を行う。
		ボスがスペカシステムに移行出来てないので無駄な処理がある。
		スクリプト処理が移動と描画が分離できてない。
	あたりが、主な処理落ち。
	-------------------------------------------------------
	あくまで暫定仕様(r32)
	-------------------------------------------------------
	BULLET_REGIST_06_n_way: 		n way弾の総弾数
		一度に弾を撃つ数。1つしか撃たないなら1。
	BULLET_REGIST_02_angle65536:		開始向き
		１周を65536分割した値(0 ... 65535)でnway弾の発弾開始角度を指定する。
		下向きが0度で半時計回り。(下CCW)
	BULLET_REGIST_04_bullet_obj_type:	弾の種類(具体的にはグラ)の番号

	BULLET_REGIST_00_speed256:		初速(打ち出し速度)

	BULLET_REGIST_07_div_angle65536:	n way弾の分割角度

	BULLET_REGIST_01_speed_offset:	加速度(又は減速度)

---------------------------------------------------------*/
//		h->rotationCCW1024				= (0);/* (i<<4) deg_360_to_512(90) */
//		h->tmp_angleCCW1024 			= ((br.BULLET_REGIST_02_angle65536)>>6);/* (i<<4) deg_360_to_512(90) */

/* (r33) */
//static void regist_01_non_tilt_bullet(SPRITE		*h)/* 傾かない弾 */
//{
//		h->callback_mover				= move_bullet_hatsudan100;
//}
//static void regist_00_tilt_bullet(SPRITE		*h)/* 傾き弾(通常) */
//{
//		h->callback_mover				= move_bullet_hatsudan000;
//}

/*---------------------------------------------------------
	弾システム:
	基本弾発生
	-------------------------------------------------------
	仕様： 引数は破壊しない。
---------------------------------------------------------*/

global void tama_system_regist_single(void)
{
	SPRITE		*h;
	h									= obj_add_00_tama_error();
	if (NULL != h)
	{
		h->tx256						= obj_send1->cx256;
		h->ty256						= obj_send1->cy256;
		h->type 						= (br.BULLET_REGIST_04_bullet_obj_type);
		h->flags &= ~(SP_FLAG_COLISION_CHECK);/* あたり判定無効(発弾エフェクト用) */
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
	//
//	#if 1
//		void (*aaa[(TAMA_TYPE_99_MAX)])(SPRITE	*h) =
//		{
//			regist_00_tilt_bullet,		// TAMA_TYPE_00_ANGLE_TILT
//			regist_01_non_tilt_bullet,	// TAMA_TYPE_01_ANGLE_NON_TILT	// 非傾き弾
//		};
//		(*aaa[ (int)((br.BULLET_REGIST_05_regist_type)&(4-1)) ])(h);	/* エフェクトの動きの種類によって分岐する */
//	#else
	//	h->callback_mover						= move_bullet_hatsudan000;
		h->callback_mover						= move_bullet_hatsudan100;
//	#endif
	//
		h->tama_system_kousadan_angle65536		= (br.BULLET_REGIST_02_angle65536); /* 交差弾用 */
		h->rotationCCW1024						= ((br.BULLET_REGIST_02_angle65536)>>6);/* (i<<4) deg_360_to_512(90) */
		h->m_Hit256R							= TAMA_ATARI_JIPPOU32_PNG;
	//
	//	h->tama_system_speed256 				= ((br.BULLET_REGIST_00_speed256)	 ); 	/* 速度 */	/* 初速(打ち出し速度) */
		h->tama_system_speed65536				= ((br.BULLET_REGIST_00_speed256)<<8);	/* 速度 */	/* 初速(打ち出し速度) */
		h->tama_system_hatsudan_counter 		= (0);
//		h->tama_system_base_time_out			= (120);/*(100)*/
		h->tama_system_radius256				= t256(0);/* 半径 */
//		h->tama_system_tra65536 				= ((br.BU LLET_REGIST_speed_offset)<<8);	/* t256形式で。 調整減速弾 */	/* この方式になるか検討中 */
		h->tama_system_tra65536 				= ((br.BULLET_REGIST_01_speed_offset)); 	/* t256形式で。 調整減速弾 */	/* この方式になるか検討中 */
		h->tama_system_tama_data				= ((br.BULLET_REGIST_03_tama_data));		/* 角度弾規格B(仮策定案)特殊機能 この方式になるか検討中 */
		//
	//	h->tama_system_add_rotate1024			= ((br.BULLET_REGIST_angle_offset1024)>>(8-6)); 	/* t256形式で。 交差弾 */		/* この方式になるか検討中 */
	}
}

/*---------------------------------------------------------
	弾システム:
	偏り n way 弾
	紅魔郷の n way 弾システムは、どうもこういうものっぽい。
	★ 5面ボス咲夜の「通常攻撃1」及び「通常攻撃2」で
	咲夜が生成した魔方陣は、 5way とか 6way とか 弾を撃ってくるが、
	一気に描かないで、左右で2回にかけて描く。
	つまり左右でプライオリティー(弾の重なり具合)が違う。

	★ 5面中-ボス咲夜の「通常攻撃」は(青ナイフ、赤クナイ共に)左右が固定の固定弾なので、
	こういう n way 弾だと都合が良い。(旧作のなぎ払いを再現するのも都合が良い)
	★ 5面中-ボス咲夜の「ミスディレクション」も、正確な自機狙いが保証されるので、
	こういう n way 弾だと都合が良い。
	-------------------------------------------------------
	以下は基本部分で撃てるのか、それとも若干改変したものなのか不明(たぶん基本部分で撃てる)

	★ 5面ボス咲夜の「通常攻撃1」のピンクのナイフは基本部分で撃てるのか
	それとも基本部分を若干改変したものかは判らないが殆ど同じもの。

	★ ルーミアの紅魔郷の「ナイトバード」は基本部分で撃てるのか
	それとも基本部分を若干改変したものかは判らないが殆ど同じもの。
	-------------------------------------------------------
	仕様： 下記の引数を破壊するので注意
	br.BULLET_REGIST_02_angle65536	[破壊]
---------------------------------------------------------*/

global void tama_system_regist_katayori_n_way(void)
{
	int i_angle65536;
	i_angle65536	= (br.BULLET_REGIST_02_angle65536);/* 始めの開始角度、向き */
	int i;
	for (i=(0); i<(br.BULLET_REGIST_06_n_way); i++)/* n way弾を登録する。 */
	{
		br.BULLET_REGIST_02_angle65536 = i_angle65536;
		tama_system_regist_single();
		/* 次の角度 */
		i_angle65536 += (br.BULLET_REGIST_07_div_angle65536);
		mask65536(i_angle65536);
	}
}
/*
	弾システム:
	n way 弾
	n way 弾は、一気に生成しないで、偏り n way 弾を2回呼ぶ事で生成する。
	これは奇数弾の場合の狙い誤差を最小にする為に必要な措置。
	奇数弾の積算誤差が大きいと自機狙い弾のつもりで生成しても
	多弾生成時の積算誤差で角度がずれるので、自機を狙わなくなってしまう。
	プライオリティー(弾の重なり具合)を調べると原作でもこの方式を採用している事がわかる。
	原作では弾の生成順にプライオリティー(ZバッファなのでZ値)を決めている。
	-------------------------------------------------------
	仕様： 下記の引数を破壊するので注意
	br.BULLET_REGIST_02_angle65536		[破壊]
	br.BULLET_REGIST_06_n_way			[破壊]
	br.BULLET_REGIST_07_div_angle65536	[破壊]
*/
global void tama_system_regist_n_way(void)
{
	const int j_angle65536	= (br.BULLET_REGIST_02_angle65536);/* 開始向き */
	const int j_n_way		= (br.BULLET_REGIST_06_n_way)&0xff;/* 弾数 */
	const int j_n_way_harf	= (j_n_way>>1)&0xff;/* 弾数 */
	//
	{
		/* 偶数弾の場合の差分を生成
			偶数弾の場合、分割角度の半分の角度がオフセット角度。
			奇数弾の場合オフセット角度は必ずゼロ。
		*/
	//	int j_guusuu_harf_angle_offset;
	//	j_guusuu_harf_angle_offset = 0;
	//	const int j_guusuu_harf_angle_offset = ((br.BULLET_REGIST_07_div_angle65536)>>1);	/* 偶数弾なら分割角の半分が差分、奇数弾はなし */
		const int j_guusuu_harf_angle_offset = (0==(j_n_way&1)) ? ((br.BULLET_REGIST_07_div_angle65536)>>1) : (0);	/* 偶数弾なら分割角の半分が差分、奇数弾はなし */

		/* 右半分(差分が正値の場合) */
		br.BULLET_REGIST_02_angle65536 = j_angle65536 + j_guusuu_harf_angle_offset;/* 偶数弾 & 奇数弾 */
		br.BULLET_REGIST_06_n_way		= (j_n_way - (j_n_way_harf));	/* 例えば 7way なら 4弾描く。 8way なら 4弾描く。 */
		tama_system_regist_katayori_n_way();
		#if 0/* デバッグ用 */
			br.BULLET_REGIST_04_bullet_obj_type ^= 2;/* デバッグ用: 同じグループだけど、左右で違う色の弾にしてみる。 */
		#endif
		/* 差分角度を逆にする。(逆周りに描くway弾になる) */
		br.BULLET_REGIST_07_div_angle65536 = (65536)-(br.BULLET_REGIST_07_div_angle65536);
		#if 1
		br.BULLET_REGIST_02_angle65536 = j_angle65536 - j_guusuu_harf_angle_offset;/* 偶数弾 & 奇数弾 */
		/* 左半分(差分が正値の場合) */
		if (0==(j_n_way&1))/* 偶数弾 */
		{;/* なにもしない */}
		else/* 奇数弾 */
		{
			br.BULLET_REGIST_02_angle65536 += br.BULLET_REGIST_07_div_angle65536;/* 奇数弾(は1弾少ないのでずらす) */
		}
	//	br.BULLET_REGIST_02_angle65536 = j_angle65536 + ((0==(j_n_way&1))?(-j_guusuu_harf_angle_offset):(br.BULLET_REGIST_07_div_angle65536));
		br.BULLET_REGIST_06_n_way		= ( 		 (j_n_way_harf));	/* 例えば 7way なら 3弾描く。 8way なら 4弾描く。 */
		tama_system_regist_katayori_n_way();
		#endif
	}
}
