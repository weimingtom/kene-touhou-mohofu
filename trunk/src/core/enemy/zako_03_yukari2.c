
#include "enemy.h"

/*---------------------------------------------------------
		"紫編隊2",		"GR EETER",
	-------------------------------------------------------
	バグあり？？？
	-------------------------------------------------------
	５機一列に続がり、編隊飛行してくるザコ敵
	-------------------------------------------------------
	mi ng_gr eeter
	rw ingx_curver(mi ng)に似てる
---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE b;
	/*dou ble*/int angle512;
	/*dou ble*/int speed256;
	int state;
	int level;
//
	/*dou ble*/int max_y256;
} YUKARI2_DATA;
static int destoroy;

#define NUM_OF_ENEMIES (5)

/*---------------------------------------------------------

---------------------------------------------------------*/

static void lose_yukari2(SPRITE *s)
{
	item_create(s, enemy_get_random_item(), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
//
	destoroy++;
	if ( (NUM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
	{
		destoroy = 0;
		if (rand_percent(30))
		{
			item_create(/*zzz*/ s, SP_ITEM_00_P001, 1, ITEM_MOVE_FLAG_06_RAND_XY);
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_yukari2(SPRITE *s)
{
	YUKARI2_DATA *d=(YUKARI2_DATA *)s->data;
	switch (d->state)
	{
	case 0: /* nach unten */
		if (s->y256 >= d->max_y256)
		{
			d->state=1;
			d->angle512 = atan_512(t256(0)-s->y256,t256(GAME_WIDTH/2)-s->x256); 			//ウィンドウ幅の変更
			if (t256(1.5/*2.0*/) < d->speed256)
			{	/*減速*/
				d->speed256 -= t256(0.2);
			//	d->speed256 = t256(2)/*3+difficulty*/;								// [***090201		変更
			}
		}
		if (d->level)
		{
		//	if (0==(ra_nd()%(105-(d->level-2+difficulty)*10)))	// [***090126		若干変更
		//	if (0==(ra_nd()%(11-(d->level-2+difficulty) ))) 	// [***090126		若干変更
		//	if (0==(ra_nd()%(16-(d->level-2+difficulty) ))) 	// [***090126		若干変更
		//	if ((d->level-2+difficulty) >= (ra_nd()&(16-1)))	// [***090126		若干変更
			if ((d->level-2+difficulty) >= (ra_nd()&(64-1)))	// [***090126		若干変更
			{
				bullet_create_aka_maru_jikinerai(s, t256(1)+t256(difficulty)+(d->level<<6)/*t256(d->level/3)*/ );
			}
		}
		break;
	case 1:
		if (s->y256 < -((s->h)<<8) )
		{
			s->flags	&= (~(SP_FLAG_VISIBLE));
		}
		break;
	}
	/*似てるがちょっと違う--以下rwingx.cと同じ*/
	s->x256+=((cos512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
	s->y256+=((sin512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
//	s->anim_frame=(deg_512_to_360(d->angle512+deg_360_to_512(270))/10)%36;
//	s->anim_frame = ((((d->angle512/*+deg_360_to_512(270)*/)&(512-1))*(36/2))>>8);
//	s->anim_frame = ((((d->angle512/*+deg_360_to_512(270)*/)&(512-1))*(32/2))>>8);
	s->anim_frame = ((((d->angle512/*+deg_360_to_512(270)*/)&(512-1)))>>4);
}
#if 0
	case 0: 	/* 右へ移動中 */
		if (s->x >= d->clip_right)
		{	d->state=1;}
		break;
	case 1: 	/* 右周りで回転中 */
		d->angle512 -= d->turnspeed512/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
		if (d->angle512 <= deg_360_to_512(180))
		{
			d->angle512 = deg_360_to_512(180);
			d->state=2;
			if (d->level>0)
			{	bullet_create_aka_maru_jikinerai(s, t256(3)+(d->level<<7) );}
		}
		break;
	case 2: 	/* 左へ移動中 */
		if (s->x <= d->clip_left)
		{	d->state=3;}
		break;
	case 3: 	/* 左周りで回転中 */
		d->angle512 += d->turnspeed512/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
		if (d->angle512 >= deg_360_to_512(360) )
		{
			d->angle512 = deg_360_to_512(0);
			d->state=0/*4*/;
			if (d->level>0)
			{	bullet_create_aka_maru_jikinerai(s, t256(3)+(d->level<<7) );}
		}
		break;

	case 8: 	/* 右へ移動中 */
		if (s->x > GAME_WIDTH)		//ウィンドウ幅の変更
		{	s->flags &= (~(SP_FLAG_VISIBLE));}
		break;
#endif

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_yukari2(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	destoroy = 0;
	static int static_last=0;/* 登場位置の切り替え */
	static_last++;
	static_last &= 1;
//
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_YUKARI32_PNG);	//s->anim_speed=0;/*36"mi ng.png"*/
		s->type 			= SP_ZAKO/*_03_YUKARI2*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_yukari2;
		s->callback_loser	= lose_yukari2;
		if (0==static_last) {	s->x256=t256(0);						}	//右上から登場
		else				{	s->x256=t256(GAME_WIDTH)-t256(s->w);	}	//左上から登場
		s->y256 			= -i*s->h*256;
		YUKARI2_DATA *data;
		data				= mmalloc(sizeof(YUKARI2_DATA));
		s->data 			= data;
		data->angle512		= atan_512((t256(GAME_HEIGHT)-((s->h)<<8)-t256(60))-s->y256,t256(GAME_WIDTH/2)-s->x256);	//ウィンドウ幅の変更
		data->max_y256		= (t256(GAME_HEIGHT)-((s->h)<<8)-t256(60));
		data->speed256		= (t256(2.5/*3.0*/)+((difficulty)<<4) ) /*4*/;/*始めだけは速い*/
		data->state 		= 0;
		data->b.score		= score(5*2);
		data->b.health		= 1+(difficulty<<2);
		data->level 		= lv;
	}
}
#undef NUM_OF_ENEMIES
