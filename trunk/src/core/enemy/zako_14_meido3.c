
#include "enemy.h"

/*---------------------------------------------------------
		"メイド3",		"CURVER",	rwingx_curver
	-------------------------------------------------------
上から現れてS字をn回描いて横へ

←←
↓
→→→→→
		↓
←←←←←
↓
→→→→→
		↓
←←←←←
	-------------------------------------------------------
	バグあり。
---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE b;
	/*dou ble*/int angle512;
	/*dou ble*/int speed256;
	int state;
	int turn_count;
//
	/*dou ble*/int turnspeed512;
	/*dou ble*/int clip_left256;
	/*dou ble*/int clip_right256;
} MEIDO3_DATA;

static int static_last=0;/* 登場位置の切り替え */
static int level;
static int destoroy[2];

//	NUM_OF_ENEMIES=10;		10機
//#define NUM_OF_ENEMIES (10)
//#define NUM_OF_ENEMIES (4+difficulty)
#define NUM_OF_ENEMIES (4+3)

/*---------------------------------------------------------

---------------------------------------------------------*/

static void lose_meido3(SPRITE *s)
{
	destoroy[static_last]++;
	if ( (NUM_OF_ENEMIES-1) < destoroy[static_last]/*all_destoroy*/)
	{
		destoroy[static_last] = 0;
		item_create(s, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_meido3(SPRITE *s)
{
	MEIDO3_DATA *data = (MEIDO3_DATA *)s->data;
	switch (data->state)
	{
	case 0: 	/* 左周りで回転中 */
		data->angle512 -= data->turnspeed512/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
		mask512(data->angle512);
		/* 右(斜め上)を向いたら */
		if ( (512-16) < data->angle512 )/*eps*/ /*<= deg_360_to_512(0)*/	//deg_360_to_512(10/*90*/ /*-90*/ /*360*/ /*-10*/
		{
		//	data->angle512 = deg_360_to_512(0/*360*/ /*-10*/)/*deg_360_to_512(360)*/;
			data->state=1;
			if (0 < /*data->*/level)	{	bullet_create_n_way_dan_type(s, /*speed256*/t256(2), ANGLE_JIKINERAI_DAN/*angle512*/, /*bu_type*/BU_TYPE01_KUNAI_PNG, /*n*/(difficulty<<1)); }/* easy以外は狙い弾を打つ */
			/*bullet_create_aka_maru_jikinerai(s, t256(3+(data->level>>1)));*/
		}
		break;
	case 1: 	/* 右(斜め上)へ移動中 */
		if (0 < data->turn_count)
		{
			if (s->x256 >= data->clip_right256) {	data->state=2;	data->turn_count--; }
		}
		else	/* 退場中 */
		{
			if (s->x256 > t256(GAME_WIDTH) )
			{	s->flags &= (~(SP_FLAG_VISIBLE));}
		}
		break;
	case 2: 	/* 右周りで回転中 */
		data->angle512 += data->turnspeed512/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
		mask512(data->angle512);
		/* 左(斜め上)を向いたら */
		if ( (256+16) < data->angle512 )	//deg_360_to_512(180 /*270*/ /*90+*/ /*180-90-90*/ /*+10*/)
		{
		//	data->angle512 = deg_360_to_512(180/*+10*/);
			data->state=3;
			if (0 < /*data->*/level)	{	bullet_create_n_way_dan_type(s, /*speed256*/t256(2), ANGLE_JIKINERAI_DAN/*angle512*/, /*bu_type*/BU_TYPE01_KUNAI_PNG, /*n*/(difficulty<<1)); 	}/* easy以外は狙い弾を打つ */
			/*bullet_create_aka_maru_jikinerai(s, t256(3+(data->level>>1)));*/
		}
		break;
	case 3: 	/* 左(斜め上)へ移動中 */
		if (0 < data->turn_count)
		{
			if (s->x256 <= data->clip_left256) {	data->state=0;	data->turn_count--; 	}
		}
		else	/* 退場中 */
		{
			if (s->x256 < t256(0) )
			{	s->flags &= (~(SP_FLAG_VISIBLE));}
		}
		break;
	}
	/*以下rwingx.cと同じ*/
	s->x256+=((cos512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
	s->y256+=((sin512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
//	s->anim_frame=(deg_512_to_360(data->angle512+deg_360_to_512(270))/10)%36;
//	s->anim_frame = ((((data->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
	s->anim_frame = ((((data->angle512)&(512-1)))>>6);/*"rw ingx8.png"*/ /*現在仕様上おかしい*/
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_meido3(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
	/*data->*/level 		= lv;
//
	static_last++;
	static_last &= 1;
//
	destoroy[static_last] = 0;
//
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_AKA_MEIDO08_PNG); 	//s->anim_speed=0;/*37"rw ingx8.png""rw ingx.png"*/
		s->type 			= SP_ZAKO/*_14_MEIDO3*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_meido3;
		s->callback_loser	= lose_meido3;
		MEIDO3_DATA *data;
		data				= mmalloc(sizeof(MEIDO3_DATA));
		s->data 			= data;
		data->b.score		= score(/*25*/50*2);
	//	data->b.health		= (12*8)+(difficulty<<3/*4*/)/*5+(difficulty<<2)*/;/*よわすぎ*/
	//	data->b.health		= (12*8)+(difficulty<<2/*4*/)/*5+(difficulty<<2)*/;/*他が強いので弱くする*/
		data->b.health		= (10*8)+(difficulty<<2/*4*/)/*5+(difficulty<<2)*/;/*他が強いので弱くする*/
		data->angle512		= 256+16 /*deg_360_to_512(270)*/;
		data->speed256		= t256(1.5) /*3+difficulty*/;/*はやすぎ*/
		data->turnspeed512	= (7) /*deg_360_to_512*/ /*(4+difficulty)*/;/*簡略化(360度→512度)*/
		data->clip_left256	= (t256(static_last<<7)+t256(static_last<<6)+t256(32)) /*50*/;			/* 32,	224(128+64+32)	*/
		data->clip_right256 = (t256(static_last<<7)+t256(static_last<<6)+t256(32))+t256(128-32);	/* 128, 320(256+64)   */	 //  /*GAME_WIDTH-50-s->w*/ ウィンドウ幅の変更
		data->state 		= (static_last<<1)/*0*/;
		data->turn_count	= 4;
		s->x256 			= t256((static_last<<7))+t256((32))+((s->w*(i+1))*t256(0.75))/*-(s->w*(i+1))*0.5*/;/*くっつきすぎ*/
		s->y256 			= t256(0)/*50*/;
	}
}
#undef NUM_OF_ENEMIES
