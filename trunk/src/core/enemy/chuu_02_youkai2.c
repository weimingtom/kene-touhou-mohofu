
#include "bullet_object.h"

/*---------------------------------------------------------
		"妖怪2",		"GFAIRY",
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	BOSS_BASE boss_base;
//------------ 移動関連
	int state1;
	int time_out;	/* 行動wait */
	int repeat; 	/* 繰り返し回数 */
	int start_danmaku;
	int start_x256;	/* 登場x座標 */
	SPRITE *s2;
} YOKAI1_DATA;
//	int wait2;	/* 攻撃wait */


/*---------------------------------------------------------
	s = ボス本体 boss sprite
	t = プレイヤーの弾 player's weapon
---------------------------------------------------------*/

static void callback_hit_youkai1(SPRITE *src/*敵自体*/, SPRITE *tama/*自弾*/)
{
	dummy_obj->x256 = tama->x256;
	dummy_obj->y256 = tama->y256;
	bakuhatsu_add_type_ddd(dummy_obj/*tama->x256,tama->y256*/,/*0,*/BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);
//
	YOKAI1_DATA *data	= (YOKAI1_DATA *)src->data;
//	WEAPON_BASE *w		= (WEAPON_BASE *)tama->data;
	/*data->base.*/src->base_health -= /*w->*/tama->base_weapon_strength;
	if (/*data->base.*/src->base_health <= 0)
	{
		bullets_to_hosi();/* 弾全部、星アイテムにする */
	//
		item_create(src, SP_ITEM_06_TENSU, 7, ITEM_MOVE_FLAG_06_RAND_XY/*ITEM_MOVE_FLAG_01_COLLECT*/);/*点数を出す*/	/* ちらばる */
		player_add_score(/*data->base.*/src->base_score);
//
		bakuhatsu_add_circle(src, 0);
		src->type				= SP_DELETE;
		data->s2->type			= SP_DELETE;
		/* コールバック登録 */
		src->callback_hit_enemy = NULL; 	/* ダミーコールバック登録 */
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

//static void lose_youkai1(SPRITE *src)
//{
//
//}


/*---------------------------------------------------------
	弾幕が終わるまで待つ。
	-------------------------------------------------------

---------------------------------------------------------*/

static void danmaku_state_check_holding(SPRITE *src)
{
	YOKAI1_DATA *data = (YOKAI1_DATA *)src->data;
	if (DANMAKU_00 == data->boss_base.danmaku_type)
	{
		data->state1++/* = nextstate*/;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
enum
{
	SS00 = 0,
	SS01,
	SS02,
	SS03,
	SS04,
};
//	ST02,
//	ST02bbb,
//	ST03,

#if 0
			;/* 8偶数弾 */
			data->wait2--;
			if (1 > data->wait2)
			{
				/* 0: 20 == 20-(0*4) */
				/* 1: 16 == 20-(1*4) */
				/* 2: 12 == 20-(2*4) */
				/* 3:  8 == 20-(3*4) */
				data->wait2 = (20-((difficulty)<<2))/*8*/ /*10*/;
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
				#else
				voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*テキトー*/
				#endif
				bullet_create_n_way_dan_sa_type(src,
					(t256(3.0)+((difficulty)<<6)),
					ANGLE_JIKI_NERAI_DAN,
					(int)(512/24),
					BULLET_UROKO14_03_MIDORI,
					8);
			}
#endif

#if 0
			;/* 7奇数弾 */
			data->wait2--;
			if (1 > data->wait2)
			{
				data->wait2 = (20-((difficulty)<<2))/*8*/ /*10*/;
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
				#else
				voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*テキトー*/
				#endif
				bullet_create_n_way_dan_sa_type(src,
					t256(3.25)+((difficulty)<<6),
					ANGLE_JIKI_NERAI_DAN,
					(int)(512/24),
					BULLET_UROKO14_04_MIZUIRO,
					7);
			}
#endif
static void move_youkai1(SPRITE *src)
{
	YOKAI1_DATA *data=(YOKAI1_DATA *)src->data;
	data->time_out--;
	switch (data->state1)
	{
	case SS00:	/* 上から登場 */
		data->s2->y256	+= t256(2); 	/**fps_fa ctor*/
		src->y256		+= t256(2); 	/**fps_fa ctor*/
		if (data->start_x256 < src->y256)	{	data->time_out=30;	data->state1 = SS03; }
		break;
//---------
	case SS01:	/* 弾幕セット */
			/*data->wait2=8 10*/
			data->state1++;/* = SS02;*/
			;/* 8偶数弾 */
			;/* 7奇数弾 */
			data->boss_base.danmaku_type		= data->start_danmaku/*DANMAKU_01*/;			/* 禊弾幕をセット */
			data->boss_base.danmaku_time_out	= DANMAKU_01_SET_TIME;	/* 禊弾幕の発生時間 */
		break;
	case SS02:	/* 禊弾幕が終わるまで待機 */
		danmaku_state_check_holding(src);	/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/
		break;
	case SS03:	/* しばし待つ */
		if (1 > data->time_out)
		{
			data->time_out=100/*150*/;
			/* 規定の繰り返し回数こなしたら退場 */
			data->repeat--;
			if ((0) != data->repeat)	{	data->state1	= SS01; 		}/* 繰り返し */
			else						{	data->state1++/* = SS04*/;		}/* 上へ退場 */
		}
		break;
//---------
	case SS04:	/* 上へ退場 */
		data->s2->y256	-= t256(2); 	/**fps_fa ctor*/
		src->y256		-= t256(2); 	/**fps_fa ctor*/
		if ( -((src->h128+src->h128)) > src->y256)
		{
			src->type = SP_DELETE;
			data->s2->type = SP_DELETE;
		}
		break;
	}
//	src->an im_frame 	= ((data->time_out&0x10)>>4);
	if (SP_DELETE != src->type)
	{
		src->type 			= TEKI_54_CHOU1+((data->time_out&0x10)>>4);
	}
//
	data->s2->m_angleCCW512++;
	mask512(data->s2->m_angleCCW512);
//
	danmaku_generator(src); /* 弾幕生成 */
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static SPRITE *create_usiro_no_mahojin(SPRITE *src) //魔方陣グラフィック生成
{
	SPRITE *s2; 		// 魔方陣グラフィックのスプライト
//	s2					= sp rite_add_res(BASE_MAHOUJIN_0_PNG);		//s2->anim_speed	= 0;/*"boss04-lo.png"*/
//	s2					= sprite_add_gu(TAMA_TYPE_BULLET_JIPPOU32_PNG); 	//s2->anim_speed	= 0;/*"boss04-lo.png"*/
	s2					= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); 			//s2->anim_speed	= 0;/*"boss04-lo.png"*/
	s2->type			= TEKI_51_MAHOJIN1/*SP_MUTEKI*/;
//	s2->type			= MAHOU_JIN_00_aaa/*SP_MUTEKI*/;
	s2->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	s2->an im_frame		= 0;
	s2->color32 		= 0xaa0000ff;		/* 赤っぽく */		/*	s2->alpha			= 0x00;*/
	s2->x256			= src->x256/*+((src->w128-s2->w128))*/-t256(8);
	s2->y256			= src->y256/*+((src->h128-s2->h128))*/-t256(8);
	return (s2);
}

void add_chuu_youkai2(STAGE_DATA *l)/*int lv*/
{
	{
		SPRITE *s1;
		SPRITE *s2;
	//	s1->x256				= ((l->user_x)*t256(35))+t256(40);
	//	s1->x256				= ((l->user_x)*t256(32))+t256(48);
	//	s1->x256				= ((l->user_x)<<(8+5))+t256(48);
		dummy_obj->x256 		= ((l->user_x)<<(8));
		dummy_obj->y256 		= t256(-30);

		s2						= create_usiro_no_mahojin(dummy_obj);

//		s1						= sp rite_add_res(BASE_GREAT_FAIRY02_PNG);	//s->anim_speed = 3;
		s1						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);	//s->anim_speed = 3;
		s1->type				= /*SP_CHUU*/TEKI_54_CHOU1/*SP_ZAKO_YOKAI1*/;
//		s1->type				= SP_CHUU/*SP_ZAKO_YOKAI1*/;
		s1->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s1->callback_mover		= move_youkai1;
	//	s1->callback_loser		= lose_youkai1;
		s1->callback_hit_enemy	= callback_hit_youkai1; 	/* コールバック登録 */
		s1->x256				= dummy_obj->x256;
		s1->y256				= dummy_obj->y256;
//
		/*data->base.*/s1->base_health		= 200+(difficulty<<4);	/* easyでも存在感を印象づける為に 200 は必要 */ 	// 50+150*difficulty;
		/*data->base.*/s1->base_score		= score(100)+score(100)*difficulty;
//
		YOKAI1_DATA *data;
		data								= mmalloc(sizeof(YOKAI1_DATA));
		s1->data						= data;
		data->state1						= SS00;
//		data->time_out						= 30;
//		data->wait2 						= 10;/*0*/
		data->repeat						= (2+2+1);
		data->start_x256					= ((l->user_y)<<(8));/* t256(50) */
		data->s2							= s2;
		#if 1
	//------------ 弾幕関連
		data->start_danmaku 				= ((l->user_1_moji+0x10/*とりあえずr27互換*/)&0x1f);
		data->boss_base.danmaku_type		= 0;
		data->boss_base.danmaku_time_out	= 0;
		#endif
	}
}
