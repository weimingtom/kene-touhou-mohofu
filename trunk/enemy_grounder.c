#include "enemy.h"

extern SPRITE *player;
extern double fps_factor;
extern int difficulty;		//***090124		追加

typedef struct {
	ENEMY_BASE b;
	int state;
	int tx;
	int ty;
	double speed;
	int level;
	int wait;		//***090124		追加
	int n;
} GROUNDER_DATA;

void enemy_grounder_add(int lv, char spd) //actually lv is the x coord
{
	SPRITE *s;
	GROUNDER_DATA *data;

	s=sprite_add_file("grounder.png",9,PR_GROUNDER);
	s->type=SP_EN_GROUNDER;
	s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	s->mover=enemy_grounder_move;
	s->anim_speed=1;
	s->aktframe=0;
	s->x=lv/1000;
	s->y=-s->h-lv%1000;
	data=mmalloc(sizeof(GROUNDER_DATA));
	s->data=data;
	data->b.score=200;
	data->b.health=25+(difficulty*15);
	data->state=0;
	data->tx=player->x;
	data->ty=player->y;
	data->wait=100;		//***090124		追加
	data->n=0;
	switch(spd) {
                case '1':  data->speed=0.2; break;
                case '2':  data->speed=0.4; break;
                case '3':  data->speed=0.6; break;
                case '4':  data->speed=0.8; break;
                case '5':  data->speed=1; break;
                case '6':  data->speed=1.2; break;
                case '7':  data->speed=1.4; break;
                case '8':  data->speed=1.6; break;
                case '9':  data->speed=1.8; break;
                default:   data->speed=1;
        }


	data->level=0;

}

void enemy_grounder_move(SPRITE *s)		//***090124		攻撃パターンを変える
{
	//double angle;
	GROUNDER_DATA *d=(GROUNDER_DATA *)s->data;
	int al,sp;
	
	switch(d->state) {
		case 0:
			if((s->y >= player->y)||(s->y>100)) {
				d->state=1;
				enemy_bullet_create(s,1);
			}
			
		case 1:
			if(s->y > 290) {
				s->type=-1;
			}
	}
	
	if(d->n<(difficulty*7)+10){		//***090128		変更
		if(d->wait<0)
		{
			playChunk(10);
			al=rand()%360;
			sp=rand()%4+1;
			if(sp<4){
				sp=rand()%4+1;
				if(sp<4){
					sp=rand()%4+1;
				}
			}
			enemy_stop_bullet_create(s, sp, degtorad(al), 0.03);
			d->wait=25-(difficulty*5);
		}
		else
			d->wait--;
	}

	//angle=atan2(d->ty-s->y,d->tx-s->x);
	//s->x+=cos(angle)*d->speed;
	//s->y+=sin(angle)*d->speed;
	s->y+=fps_factor*d->speed;
}
