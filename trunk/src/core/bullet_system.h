#ifndef _BULLET_SYSTEM_H_
#define _BULLET_SYSTEM_H_

/*---------------------------------------------------------
	敵弾管理システム
---------------------------------------------------------*/

/* MAX_BULLET_POOL: 2のn乗の必要あり(1024とか2048とか) */
#define MAX_BULLET_POOL 1024
extern TGameSprite bullet_pool[MAX_BULLET_POOL];


extern void bullet_system_init(void);
extern void bullet_system_exit(void);
#endif /* _BULLET_SYSTEM_H_ */
