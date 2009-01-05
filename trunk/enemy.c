#include "enemy.h"
/*
	�O���C�Y�֌W�̒ǉ��ő啝�ɕύX����Ă���B
	�e�̃f�[�^��id��ǉ��Bid��1�̒e�ŃO���C�Y����������Ȃ��悤�ɂ��邽�߁B0-999�܂ł̃����_���̒l���Ƃ�B
	�e�̃X�v���C�g�A�j���[�V�����͊�{�ꖇ�ɕύX�����B��ŕω�����e����邩���B
	�{�X��|�������ɏ�����悤�ɂ��Ă���B
	�ǉ������e
		enemy_bigbullet_create
		�������e�B1�̒e�ŃX�v���C�g��2�g���Ă���B
		�����蔻���1�ڂɂ����Ȃ��B
		
		enemy_g_bullet_create
		���e�ɕςȊp�x�w��@�\��t�������́B
		���C�t�������ǂ���}�C�i�X�̒l�����Ȃ��ˁB�Ӗ��킩���B
		�g���Ă�͓̂V�炾���Ȃ̂ŏ�����������������Ă����ł��B
		
		enemy_gr_bullet_create
		�������ɉ������Ă������e�B
		�t���[�����Ōv�Z���Ă���̂ł��Ȃ�����x�����������Ȃ��Ƒ�ςȂ��ƂɁB
*/

extern SDL_Surface *screen;
extern SPRITE *player;
extern double fps_factor;


void enemy_bullet_create(SPRITE *s, double speed)
{
	SPRITE *h;
	BULLET_DATA *data;

	h=sprite_add_file("kugel.png",1,PR_ENEMY);	
	h->type=SP_EN_BULLET;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_bullet_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(BULLET_DATA));
	h->data=data;

	data->id=rand()%1000;
	data->angle=atan2(player->y-s->y,player->x-s->x);
	data->speed=speed;

}

void enemy_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=30;
		sprintf(buffer,"30");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	BULLET_DATA *d=(BULLET_DATA *)s->data;

	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor;
	if((s->x<0)||(s->x>WIDTH2)||(s->y<0)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

void enemy_laser_create(SPRITE *s, double speed)
{
	SPRITE *h;
	LASER_DATA *data;

	h=sprite_add_file("bshoot.png",1,PR_ENEMY);	
	h->type=SP_EN_LASER;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_laser_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(LASER_DATA));
	h->data=data;

	data->id=rand()%1000;
	data->angle=atan2(player->y-s->y,player->x-s->x);
	data->speed=speed;

}

void enemy_laser_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=50;
		sprintf(buffer,"50");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	LASER_DATA *d=(LASER_DATA *)s->data;

	//s->aktframe=(radtodeg(d->angle)/10)+9;
	//s->aktframe%=18;
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor;
	if((s->x<0)||(s->x>WIDTH2)||(s->y<0)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

void enemy_homing_create(SPRITE *s)
{
	SPRITE *r;
	HOMING_DATA *data;

	r=sprite_add_file("rotating_rocket.png",20,PR_ENEMY);
	r->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	r->type=SP_EN_HOMING;
	r->x=s->x;
	r->y=s->y;
	r->mover=enemy_homing_move;
	data=mmalloc(sizeof(HOMING_DATA));
	r->data=data;
	data->b.score=5;
	data->b.health=1;
	data->angle=0;
	data->speed=2;
	data->range=200;
	data->delay=10;

	enemy_homing_update(r);
}

void enemy_homing_update(SPRITE *s)
{

	HOMING_DATA *d=(HOMING_DATA *)s->data;
	
	/* Zielwinkel erfassen */
	d->angle=atan2(player->y-s->y,player->x-s->x);
	s->aktframe=(radtodeg(d->angle)*20)/360;
	s->aktframe%=20;
}

void enemy_homing_move(SPRITE *s)
{
	SDL_Surface *tmp;
	HOMING_DATA *d=(HOMING_DATA *)s->data;

	d->delay-=fps_factor;
	if(d->delay >0) {
		s->x+=cos(d->angle)*d->speed*fps_factor;
		s->y+=sin(d->angle)*d->speed*fps_factor;
	} else {
		/* nur jeden 10ten Frame Winkel aktualisieren */
		d->delay=10;
		enemy_homing_update(s);
	}


	/* max. reichweite erreicht? self-destruct! */
	d->range-=fps_factor;
	if(d->range<=0) {
		s->type=-1;
		//tmp=sprite_getcurrimg(s);
		//parsys_add(tmp,1,1,s->x,s->y,10,0,0,30,EXPLODE|DIFFSIZE,NULL);
		//SDL_FreeSurface(tmp);
	}
}

void enemy_bigbullet_create(SPRITE *s, double ex, double ey, double speed, double angle, int ransu)
{
	/*
		ransu		10�`60���炢�܂ŁB�傫���قǋ����s�R�ȓ����ɂȂ�
					�p�x�I��+�����֗����X���������C������=>�V����`�ԁB�C������K�v�����肻���B
		angle		-2���w�肷��ƃv���C���[�Ɍ������B��{�͊p�x�w��(��)
		ex,ey		�o���ʒu
	*/
	SPRITE *h,*hu;
	BIGBULLET_DATA *data;
	BIGBULLET_S_DATA *src_d;

	h=sprite_add_file("bigkugel2.png",1,PR_ENEMY);
	h->type=SP_EN_BIGBULLET;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_bigbullet_move;
	h->aktframe=0;
	h->x=ex-h->w/2;
	h->y=ey-h->h/2;
	
	hu=sprite_add_file2("bigkugel1.png",1,PR_TMP);	
	hu->type=SP_EN_BIGBULLET_DUMMY;
	hu->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	hu->mover=enemy_bigbullet_move2;
	hu->aktframe=0;
	hu->x=ex-hu->w/2;
	hu->y=ey-hu->h/2;

	data=mmalloc(sizeof(BIGBULLET_DATA));
	h->data=data;
	
	src_d=mmalloc(sizeof(BIGBULLET_S_DATA));
	hu->data=src_d;

	data->id=rand()%1000;
	if(angle==-2)
		data->angle=atan2(player->y-s->y,player->x-s->x);
	else
		data->angle=angle;
	data->speed=speed;
	data->ransu=ransu;
	
	src_d->sw=h->w;
	src_d->sh=h->h;
	src_d->type=&h->type;
	src_d->sx=&h->x;
	src_d->sy=&h->y;
}

void enemy_bigbullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2)
	{
		s->type=-1;
		char buffer[4];
		pd->score+=100;
		sprintf(buffer,"100");
		bonus_info_text(s->x,s->y,buffer,FONT07);		//�\������ĂȂ��H
	}
	BIGBULLET_DATA *d=(BIGBULLET_DATA *)s->data;
	
	s->x+=cos(d->angle+((rand()%d->ransu-d->ransu/2)/10))*d->speed*fps_factor;
	s->y+=sin(d->angle+((rand()%d->ransu-d->ransu/2)/10))*d->speed*fps_factor;
	if((s->x<0)||(s->x>WIDTH2)||(s->y<0)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

void enemy_bigbullet_move2(SPRITE *s)
{
	BIGBULLET_S_DATA *d=(BIGBULLET_S_DATA *)s->data;
	
	s->x=*(d->sx)+d->sw/2-s->w/2;
	s->y=*(d->sy)+d->sh/2-s->h/2;
	if(*(d->type)==-1) {
		s->type=-1;
	}
}

void enemy_g_bullet_create(SPRITE *s, double speed, int state, double angle)
{
	/*
		state	�\�i���ň�̈ʂ�x�A�\�̈ʂ�y
				�w�肵���������p�x���ς��B
		angle	���W�A���w��B-2�Ńv���C���[�Ɍ������B
	*/
	SPRITE *h;
	G_BULLET_DATA *data;

	h=sprite_add_file("kugel.png",1,PR_ENEMY);	
	h->type=SP_EN_BULLET;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_g_bullet_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(G_BULLET_DATA));
	h->data=data;

	data->id=rand()%1000;
	if(angle==-2)
		data->angle=atan2(player->y-s->y,player->x-s->x);
	else
		data->angle=angle;
	data->speed=speed;
	data->state=state;
}

void enemy_g_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=30;
		sprintf(buffer,"30");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	G_BULLET_DATA *d=(G_BULLET_DATA *)s->data;

	s->x+=cos(d->angle)*d->speed*fps_factor+(d->state%10);
	s->y+=sin(d->angle)*d->speed*fps_factor+(d->state/10);
	if((s->x<0)||(s->x>WIDTH2)||(s->y<0)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}


void enemy_gr_bullet_create(SPRITE *s, double speed, double angle, double gra)
{
	/*
		angle	-2�Ńv���C���[
				��{�̓��W�A��
		gra		�t���[�����ɉ������ɂǂꂾ���������邩
	*/
	SPRITE *h;
	GR_BULLET_DATA *data;

	h=sprite_add_file("bshoot.png",1,PR_ENEMY);	
	h->type=SP_EN_BULLET;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_gr_bullet_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(GR_BULLET_DATA));
	h->data=data;

	data->id=rand()%1000;
	if(angle==-2)
		data->angle=atan2(player->y-s->y,player->x-s->x);
	else
		data->angle=angle;
	data->speed=speed;
	data->gra=gra;
	data->sum=0;
}

void enemy_gr_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=50;
		sprintf(buffer,"50");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	GR_BULLET_DATA *d=(GR_BULLET_DATA *)s->data;
	d->sum+=d->gra;
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor+d->sum;
	if((s->x<0)||(s->x>WIDTH2)||(s->y<-50)||(s->y>HEIGHT+50)) {
		s->type=-1;
	}
}

void explosion_add(int x, int y, double wait, int type)
{
	SPRITE *ex=NULL;
	EXPLOSION_DATA *d;

	d=mmalloc(sizeof(EXPLOSION_DATA));
	switch(type) {
	case 0:
		ex=sprite_add_file("ex.png",29,PR_BONUS);
		d->framewait=2;
		break;
	case 1:
		ex=sprite_add_file("tr_blue.png",6,PR_BONUS);
		d->framewait=1;
		break;
	case 2:
		ex=sprite_add_file("tr_red.png",6,PR_BONUS);
		d->framewait=1;
		break;
	case 3:
		ex=sprite_add_file("tr_green.png",6,PR_BONUS);
		d->framewait=1;
		break;
	default:
		error(ERR_WARN,"explosion_add: unknown type: %d",type);
	}
	
	
	ex->flags&=~SP_FLAG_VISIBLE;
	ex->anim_speed=0;
	ex->aktframe=-1;
	ex->type=SP_ETC;
	ex->x=x-(ex->w/2);
	ex->y=y-(ex->h/2);
	ex->mover=explosion_move;
	ex->data=d;
	d->wait=wait;
	d->framestat=0;
}

void explosion_move(SPRITE *c)
{
	EXPLOSION_DATA *d=(EXPLOSION_DATA *)c->data;

	if(d->wait>0)
		d->wait-=fps_factor;
	else {
		d->framestat+=fps_factor;
		if(d->framestat>d->framewait) {
			d->framestat=0;
			c->flags|=SP_FLAG_VISIBLE;
			if(c->aktframe<c->frames-1)
				c->aktframe++;
			else
				c->type=-1;
		}
	}
}