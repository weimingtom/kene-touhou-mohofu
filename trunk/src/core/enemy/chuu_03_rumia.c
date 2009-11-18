
#include "bullet_object.h"

/*---------------------------------------------------------
		"ルーミア",		"GFAIRY",
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
	int start_x256; /* 登場x座標 */
} RUMIA_DATA;
//	int wait2;	/* 攻撃wait */

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

/*---------------------------------------------------------
	s = ボス本体 boss sprite
	t = プレイヤーの弾 player's weapon
---------------------------------------------------------*/

static void callback_hit_rumia1(SPRITE *src/*敵自体*/, SPRITE *tama/*自弾*/)
{
	dummy_obj->x256 = tama->x256;
	dummy_obj->y256 = tama->y256;
	bakuhatsu_add_type_ddd(dummy_obj/*tama->x256,tama->y256*/,/*0,*/BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);
//
	RUMIA_DATA *data	= (RUMIA_DATA *)src->data;
//	WEAPON_BASE *w		= (WEAPON_BASE *)tama->data;
	/*data->base.*/src->base_health -= /*w->*/tama->base_weapon_strength;
	if (/*data->base.*/src->base_health <= 0)
	{
		src->base_health = 65535;/* ２回は倒せない */
		bullets_to_hosi();/* 弾全部、星アイテムにする */
	//
		const u8 item_tbl[4] =
		{
			(SP_ITEM_04_P128&0xff), 	/* [F] */
			(SP_ITEM_01_P008&0xff), 	/* [P]大 */
			(SP_ITEM_00_P001&0xff), 	/* [p]小 */
			(SP_ITEM_06_TENSU&0xff), 	/* [点] */
		};
		item_create(src, (SP_GROUP_ITEMS|(item_tbl[difficulty])), 7, ITEM_MOVE_FLAG_06_RAND_XY/*ITEM_MOVE_FLAG_01_COLLECT*/);/* ちらばる */
		player_add_score(/*data->base.*/src->base_score);
//
		bakuhatsu_add_circle(src, 0);
	//	src->type				= SP_DELETE;
		data->state1			= SS04; 		/* 上へ退場 */
		/* コールバック登録 */
		src->callback_hit_enemy = NULL; 	/* ダミーコールバック登録 */
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

//static void lose_rumia1(SPRITE *src)
//{
//
//}


/*---------------------------------------------------------
	弾幕が終わるまで待つ。
	-------------------------------------------------------

---------------------------------------------------------*/

static void danmaku_state_check_holding(SPRITE *src)
{
	RUMIA_DATA *data = (RUMIA_DATA *)src->data;
	if (DANMAKU_00 == data->boss_base.danmaku_type)
	{
		data->state1++/* = nextstate*/;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_rumia1(SPRITE *src)
{
	RUMIA_DATA *data=(RUMIA_DATA *)src->data;
	data->time_out--;
	switch (data->state1)
	{
	case SS00:	/* 上から登場 */
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
		src->y256		-= t256(2); 	/**fps_fa ctor*/
		if ( -((src->h128+src->h128)) > src->y256)
		{
			src->type = SP_DELETE;
		}
		break;
	}
	if (SP_DELETE != src->type)
	{
		src->type 			= TEKI_00_BOSS11+((data->time_out&0x30)>>4);
	}
//	src->yx_anim_frame	= ((data->time_out&0x10)>>4);
//
	danmaku_generator(src); /* 弾幕生成 */
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_chuu_rumia(STAGE_DATA *l)/*int lv*/
{
	{
		SPRITE *sakuya;
//		sakuya						= sprite_add_res(BASE_RUMIA08_PNG); 	//s->anim_speed = 3;
		sakuya						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); 	//s->anim_speed = 3;
		sakuya->type				= /*SP_CHUU*/TEKI_00_BOSS11/*SP_ZAKO_YOKAI1*/;
//		sakuya->type				= SP_CHUU/*SP_ZAKO_YOKAI1*/;
		sakuya->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		sakuya->callback_mover		= move_rumia1;
	//	sakuya->callback_loser		= lose_rumia1;
		sakuya->callback_hit_enemy	= callback_hit_rumia1;	/* コールバック登録 */
		sakuya->y256				= t256(-30);
	//	sakuya->x256				= ((l->user_x)*t256(35))+t256(40);
	//	sakuya->x256				= ((l->user_x)*t256(32))+t256(48);
	//	sakuya->x256				= ((l->user_x)<<(8+5))+t256(48);
		sakuya->x256				= ((l->user_x)<<(8));
//
		/*data->base.*/sakuya->base_health		= 200+(difficulty<<4);	/* easyでも存在感を印象づける為に 200 は必要 */ 	// 50+150*difficulty;
		/*data->base.*/sakuya->base_score		= score(100)+score(100)*difficulty;
//
		RUMIA_DATA *data;
		data								= mmalloc(sizeof(RUMIA_DATA));
		sakuya->data						= data;
		data->state1						= SS00;
//		data->time_out						= 30;
//		data->wait2 						= 10;/*0*/
		data->repeat						= (2+2+1);
		data->start_x256					= ((l->user_y)<<(8));/* t256(50) */
		#if 1
	//------------ 弾幕関連
		data->start_danmaku 				= ((l->user_1_moji+0x10/*とりあえずr27互換*/)&0x1f);
		data->boss_base.danmaku_type		= 0;
		data->boss_base.danmaku_time_out	= 0;
		#endif
	}
}
