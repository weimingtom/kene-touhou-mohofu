
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"���C�h1",		"RWINGX",	rwingx
	-------------------------------------------------------
	�ォ�牡�ɓ��Ԋu�Ő��񂵂Č���A�v���C���[�Ɍ�����
	������x�߂Â��ƁA�e�u���Ĕ��]���ē�����B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_meido1(SPRITE *src)
{
	if (512+0 > src->jyumyou)
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
	else
	if (512+1024 > src->jyumyou)
	{
		src->speed256	+= t256(0.1);/*�������Ȃ��� �v���C���[�ɓ˂�����ł���*/
		if (src->cy256 >= t256(GAME_HEIGHT-150))
		{
		//	if (src->enemy_rank)
			{
				obj_send1->cx256 					= src->cx256;
				obj_send1->cy256 					= src->cy256;
				br.BULLET_REGIST_speed256			= t256(2.8);				/* �e�� */	 // //t256(1.0)+t256(src->enemy_rank)/*t256(3)+t256(src->enemy_rank)*/	/*���߂�*/
			//	br.BULLET_REGIST_speed256			= speed256; 				/* �e�� */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* ���@�_���e */	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			//	br.BULLET_REGIST_div_angle1024		= (0);						/* �_�~�[�p�x(���g�p) */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_01_AKA;		/* [�Ԓe] */ /* �e�O�� */
				br.BULLET_REGIST_n_way				= (1);						/* [1way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
			src->jyumyou = (512+1023);
			src->tmp_angleCCW1024 += (1024/2);	/* (1024/2) == [180/360 �x] ���]���ē����� */
			mask1024(src->tmp_angleCCW1024);
		//	src->speed256 = /*t256(6+src->enemy_rank)*/;/*���߂�*/
		}
	}
	else
	if (512+2048 > src->jyumyou)
	{
		/* �v���C���[�ɓ˂�����ł��� / schnell richtung player */
		src->jyumyou = (512+2047);
		tmp_angleCCW1024_jiki_nerai(src);
	//	src->speed256	= /*t256(3+(src->enemy_rank>>1))*/ /*(4+src->enemy_rank)*/;/*���߂�*/
	}
	else
	if (512+2049 > src->jyumyou)/* 2048+1 */
	{
		if (0 < src->speed256)
		{
			src->speed256 -= (5);/*t256(0.1)*/ /*����*/
		}
	}
	else
	if (512+2109 > src->jyumyou)/* 2048+1+60 */
	{
		if ( src->speed256 < t256(1.0))
		{
			src->speed256 += (5);/*t256(0.1)*/ /*����*/
		}
	}
//
	/*�ȉ�rwingx.c�Ɠ���*/
/* CCW�̏ꍇ */
	src->cx256 += ((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
	src->cy256 += ((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
	/* �A�j���[�V���� */
	zako_anime_type01(src, TEKI_36_YOUSEI3_1);
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_meido1(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_36_YOUSEI3_1;	/*SP_ZAKO*/ /*_12_MEIDO1*/
		h->callback_mover		= move_meido1;
	//
		h->tmp_angleCCW1024 	= (0);/* ������ */		/* CCW�̏ꍇ */
	//
		h->speed256 			= t256(0);//t256(0.5);
		h->jyumyou				= (512+2209);//(100);//20;
	}
}