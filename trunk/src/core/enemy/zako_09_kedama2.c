
#include "bullet_object.h"

/*---------------------------------------------------------
		"毛玉2",	"BADGUY",	(r27から新規追加)
	-------------------------------------------------------
	本家毛玉
---------------------------------------------------------*/

#define USE_X_HOUKOU (1)

typedef struct
{
//	ENEMY_BASE base;
	int state;
	int time_out;
	int kougeki_type;
} KEDAMA1_DATA;


//	int t x256;
//	int t y256;
//	int v x256;
//	int v y256;
//	int speed256;

//static int enemy_rank;		/* 設定ファイルからの敵の強さ */

#define NUM_OF_ENEMIES (6)

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_kedama2(SPRITE *src)
{
//	case SP_ZAKO_08_KEDAMA1:
//	if (rand_percent(10))
	{
		item_create(src, enemy_get_random_item(), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
	}
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
enum
{
	ST00 = 0,
	ST02,
	ST03,
};

static void move_kedama2(SPRITE *src)
{
	KEDAMA1_DATA *data = (KEDAMA1_DATA *)src->data;

	data->time_out -= 1/**fps_fa ctor*/;
//

//
	switch (data->state)
	{
	case ST00:	/* ばらばらに登場させる */
		if (
			(0 >= src->x256)
			|| (t256(GAME_WIDTH) < src->x256)
			#if (1==USE_X_HOUKOU)
			|| (0 >= src->y256)
			|| (t256(GAME_HEIGHT) < src->y256)
			#endif
		)
		{
			/*画面外*/;
		}
		else
		{
			data->time_out	= 64;
			data->state++;	/* 登場する */
		}
		break;
	case ST02:	/* 攻撃 */
		{			enum
			{
				K00_KOUGEKI_KANKAKU_MASK = 0,
				K01_SUKIMA_KAKUDO,
				K02_TAMA_KAZU,
				K99_MAX/* 最大数 */
			};
			const u32 kougeki_tbl[(4*2)][K99_MAX] =
			{
				/* 0:紅っぽい毛玉(狭角度 奇数弾) */
				{0x3f, (u32)(512/12), (5/*+difficulty+difficulty*/)},	/* (u32)(512/12):広角なので簡単 */
				{0x1f, (u32)(512/16), (3/*+difficulty+difficulty*/)},	/* 0x0f:普通に避けれる間隔 */
				{0x0f, (u32)(512/20), (3/*+difficulty+difficulty*/)},	/* 0x0f:普通に避けれる間隔 */
				{0x07, (u32)(512/24), (5/*+difficulty+difficulty*/)},	/* (u32)(512/24):普通の狭角 */
				/* 1:妖っぽい毛玉(90度 4方向) (無駄弾なので難易度は低い筈) */
				{0x3f, (u32)(512/8),  (8)}, 	/* 8:見せ弾(無駄弾なので難易度は低い筈) */
				{0x1f, (u32)(512/8),  (8)}, 	/* 0x1f:普通に避けれる間隔 */
				{0x07, (u32)(512/4),  (4)},
				{0x03, (u32)(512/4),  (4)}, 	/* 0x03:連続弾っぽく */
			};
			if (0==(data->time_out & kougeki_tbl[data->kougeki_type][K00_KOUGEKI_KANKAKU_MASK]))/*0x0f*/
			{
				bullet_create_n_way_dan_sa_type(src,
					(t256(2.5)+((difficulty)<<6)),/* [2.5 - 3.25] */		//	(t256(2.0)+((difficulty)<<6)),/* [2.0 - 2.75] */
					ANGLE_JIKI_NERAI_DAN,
					kougeki_tbl[data->kougeki_type][K01_SUKIMA_KAKUDO], 	//	(int)(512/24),
					BULLET_KOME_01_AOI+(src->x256&0x07),
					kougeki_tbl[data->kougeki_type][K02_TAMA_KAZU]);		//	(3/*+difficulty+difficulty*/)
			}
		}
		if (0 > data->time_out)
		{
			data->state++;
		}
		break;
	case ST03:
		/* 画面外に出たら終わり */
		if (
			(0 >= src->x256)
			|| (t256(GAME_WIDTH) < src->x256)
			#if (1==USE_X_HOUKOU)
			|| (0 >= src->y256)
			|| (t256(GAME_HEIGHT) < src->y256)
			#endif
		)
		{
			src->type = SP_DELETE;	/* おしまい */
		}
		break;
	}

	/* 移動する */
	src->x256 += (src->vx256);
	src->y256 += (src->vy256);
//
	src->m_angleCCW512 += 5;
	mask512(src->m_angleCCW512);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_kedama2(STAGE_DATA *l)/*int lv*/
{
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
//		s						= sp rite_add_res(BASE_KEDAMA16_PNG);	//s->anim_speed=5; /*3*/ /*9"ba dguy.png"*/
		s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);	//s->anim_speed=5; /*3*/ /*9"ba dguy.png"*/
		s->type 				= /*SP_ZAKO*/TEKI_59_HAI_KEDAMA/*_08_KEDAMA1*/;
//		s->type 				= SP_ZAKO/*_08_KEDAMA1*/;
//		s->anim_speed			= /*-*/3; /*逆転アニメ禁止に変更*/
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->color32 				= 0xaaffffff;		/*白っぽく */
		s->callback_mover		= move_kedama2;
		s->callback_loser		= lose_kedama2;
		s->callback_hit_enemy	= callback_hit_zako;
//		s->an im_frame			= 0;
		//
		s->y256 				= ((l->user_y)<<8);
		s->vx256				= ((l->user_x));
		s->vy256				= (0);
		//
		{
			if (0 < (s->vx256))
			{/* [引数 user_y が正方向の場合、右へ移動(→)] [引数yが負方向の場合、下へ移動(↓)] */
				s->x256 			= t256(-30)-(i<<(5+8));
			}
			else
			{/* [引数 user_y が正方向の場合、左へ移動(←)] [引数yが負方向の場合、上へ移動(↑)] */
				s->x256 			= t256(360)+(i<<(5+8));/* 360 > 352(x_max) > 272(y_max) */
			}
		}
		//
		if (0 < (s->y256))
		{	;	}
		else
		{
			/* y座標が負方向の場合は、x座標指定なので(x座標 y座標 を逆にする) */
			{
				int s_sss;
				s_sss				= s->x256;
				s->x256 			= -(s->y256);
				s->y256 			= s_sss;
			}
			s->vy256			= (s->vx256);
			s->vx256			= (0);
		}
		//
		KEDAMA1_DATA *data;
		data					= mmalloc(sizeof(KEDAMA1_DATA));
		s->data 				= data;
		/*data->base.*/s->base_score		= score(/*50*/5*2*4)/**(enemy_rank+1)*/;
		/*data->base.*/s->base_health		= (8/**8*/)/*+(difficulty<<2)*/ /*(1+(difficulty<<2))*/;/*やわらかすぎ*/
		data->state 			= ST00;
		data->time_out			= 64+(i<<4);
		data->kougeki_type		= (((l->user_1_moji)&1)<<2)|(difficulty/*&0x03*/);
	}
}
#undef NUM_OF_ENEMIES
//			data->anime_houkou	= 0x50;
//			data->anime_houkou	= 0x00;

	//	s->x256 				= t256(200)+	   (ra_nd()&((64*256)-1))/*ra_nd()%40*/;
	//	s->y256 				= ((ra_nd()&((32*256)-1))-t256(80));//	(ra_nd()%40-90);
	//	s->vx256			= (0);
	//	s->vy256			= (0);
//	/*data->*/enemy_rank 	= l->user_y;

//		data->anime_houkou	= 0x20;

//		data->tx256 			= player->x256;
//		data->ty256 			= player->y256;

