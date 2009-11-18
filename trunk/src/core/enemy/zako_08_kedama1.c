
#include "bullet_object.h"

/*---------------------------------------------------------
		"毛玉1",	"BADGUY",
	-------------------------------------------------------
	本家毛玉
---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	int state;
	int enemy_rank;
	int tx256;
	int ty256;
	int speed256;
	int time_out;
} KEDAMA1_DATA;

//	int v x256;
//	int v y256;

//static int enemy_rank;

#define NUM_OF_ENEMIES (10)
//#define NUM_OF_ENEMIES (16)

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_kedama1(SPRITE *src)
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
	ST01,
	ST02,
	ST03,
}
;
static void move_kedama1(SPRITE *src)
{
	KEDAMA1_DATA *data = (KEDAMA1_DATA *)src->data;
	data->time_out -= 1/**fps_fa ctor*/;
//	data->time_out -= 1/**fps_fa ctor*/;
	switch (data->state)
	{
	case ST00:	/* ばらばらに登場させる */
		if (0 > data->time_out)
		{
			data->time_out	= 64;
			data->state++;	/* 登場する */
		}
		break;
	case ST01:	/* 登場する */
		{	int angle512;
			angle512=atan_512(data->ty256-src->y256-t256(64)/*+(20)*/,data->tx256-src->x256);
			src->vx256 =((sin512(angle512)));
			src->vy256 =((cos512(angle512)));
		}
		if ((src->x256 >= player->x256) ||
			(src->y256 > /*(480-80)*/t256(GAME_HEIGHT/2)/*(272-32)*/ ) ||
			(0 > data->time_out)
		 )
		{
			src->vy256 = (-src->vy256);
			data->state++/* = ST02*/;
			data->time_out	= 64;
			/* 第二目標は、 */
			data->tx256 	= t256(-100);
			data->ty256 	= t256(0)/*((player->y256)&(128-1))*/;
			data->speed256	= /*6*/t256(2.5/*5.0*/) /**fps_fa ctor*/;
		}
		break;/*??? [***090215 追加 */
	case ST02:
		if (0==(data->time_out & 0x0f))
		{
			if (ENEMY_LAST_SHOT_LINE256 > src->y256)	/* このラインより下からは敵が撃たない */
			{
				if (0 < data->enemy_rank)
				{
					bullet_create_n_way_dan_sa_type(src,
						t256(1.0), //t256(1.0)+t256((data->enemy_rank)>>1),
						ANGLE_JIKI_NERAI_DAN,
						(int)(512/24),
						BULLET_KOME_01_AOI+(src->x256&0x07),
						(3+difficulty+difficulty));
				}
			}
		}
		if (0 > data->time_out)
		{
			data->state++;
		}
		break;
	case ST03:/* 退場 */
		{	int angle512;
			angle512=atan_512(data->ty256-src->y256-t256(64)/*+(20)*/,data->tx256-src->x256);
			src->vx256 =((sin512(angle512)));
			src->vy256 =((cos512(angle512)));
		}
		if (src->x256 <= -((src->w128+src->w128)))
		{
			src->type = SP_DELETE;	/* おしまい */
		}
		break;/*??? [***090215 追加 */
	}

	/* 移動する */
	src->x256+=(((src->vx256)*data->speed256)>>8);
	src->y256+=(((src->vy256)*data->speed256)>>8);
//
	src->m_angleCCW512 += 5;
	mask512(src->m_angleCCW512);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_kedama1(STAGE_DATA *l)/*int lv*/
{
	int enemy_rank	= l->user_y;
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
		s->callback_mover		= move_kedama1;
		s->callback_loser		= lose_kedama1;
		s->callback_hit_enemy	= callback_hit_zako;
//		s->an im_frame			= 0;
		switch (i&3/*i%5*/)
		{
		case 0: s->x256 		=				   (ra_nd()&((64*256)-1))/*ra_nd()%40*/; break;
		case 1: s->x256 		= t256(GAME_WIDTH)-(ra_nd()&((64*256)-1))/*ra_nd()%40*/; break;
		case 2: s->x256 		= t256(200)+	   (ra_nd()&((64*256)-1))/*ra_nd()%40*/; break;
		case 3: s->x256 		= t256(90)+ 	   (ra_nd()&((64*256)-1))/*ra_nd()%40*/; break;
	//	case 4: s->x256 		= t256(310)+	   (ra_nd()&((64*256)-1))/*ra_nd()%40*/; break;
		}
		s->y256 				= ((ra_nd()&((32*256)-1))-t256(80));//	(ra_nd()%40-90);
		KEDAMA1_DATA *data;
		data					= mmalloc(sizeof(KEDAMA1_DATA));
		s->data 				= data;
		data->enemy_rank		= enemy_rank;
		/*data->base.*/s->base_score		= score(/*50*/5*2)*(enemy_rank+1);
		/*data->base.*/s->base_health		= (8*8)+(difficulty<<2)/*(1+(difficulty<<2))*/;/*やわらかすぎ*/
		data->state 			= ST00;
		data->time_out			= 64+(i<<4);
		/* 第一目標はプレイヤーのいた位置 */
		data->tx256 			= player->x256;
		data->ty256 			= player->y256;
		s->vx256				= (0);
		s->vy256				= (0);
	//	data->speed256			= /*fps_fa ctor**/((ra_nd()&((256*2)-1)))*(1+difficulty+enemy_rank/3);
		data->speed256			= /*fps_fa ctor**/((ra_nd()&((256*2)-1))) + ((difficulty<<8)+ (enemy_rank<<7/**86*2*/) );
	}
}
#undef NUM_OF_ENEMIES
