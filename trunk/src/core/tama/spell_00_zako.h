
/*---------------------------------------------------------
 東方模倣風 〜 Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	雑魚の弾幕を生成します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	アリス(仮)	洗濯機弾幕
	-------------------------------------------------------
	テキトー
---------------------------------------------------------*/
static void spell_create_09_zako_sentakki(SPRITE *src)
{
	if (0==((src->boss_base_spell_time_out)&0x03))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		src->tmp_angleCCW1024 -= (int)(1024/24);
		mask1024(src->tmp_angleCCW1024);
	//
			obj_send1->cx256							= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
			obj_send1->cy256							= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
		#if 0
			br.BULLET_REGIST_05_regist_type 			= REGIST_TYPE_00_MULTI_VECTOR;
			br.BULLET_REGIST_06_n_way					= (8);									/* [8way] */
			br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/24);						/* 分割角度 */
	//
			br.BULLET_REGIST_00_speed256				= (t256(2.0)+((src->boss_base_spell_time_out)<<2)); /* 弾速 */
		{
			br.BULLET_REGIST_02_VECTOR_angle1024		= (src->tmp_angleCCW1024);								/* 角度 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_UROKO14_03_AOI;				/* [青鱗弾] */
			bul let_reg ist_vec tor();
		//
			br.BULLET_REGIST_02_VECTOR_angle1024		= ((-src->tmp_angleCCW1024)&(1024-1));					/* 角度 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_UROKO14_04_MIZUIRO;			/* [水鱗弾] */
			bul let_reg ist_vec tor();
		}
		#endif
		#if 1
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
			br.BULLET_REGIST_06_n_way					= (8);									/* [8way] */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/24);						/* 分割角度 */
			br.BULLET_REGIST_01_speed_offset			= t256(1);		/* 調整減速弾 */	/* この方式になるか検討中 */
	//
			br.BULLET_REGIST_00_speed256				= (t256(1.0)+((src->boss_base_spell_time_out)<<2)); /* 弾速 */
		{
			int first_angle65536 = ((src->tmp_angleCCW1024)<<6);
			br.BULLET_REGIST_02_angle65536				= (first_angle65536);								/* 角度 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_UROKO14_03_AOI;				/* [青鱗弾] */
			tama_system_regist_katayori_n_way();/* (r33-) */
		//
			br.BULLET_REGIST_02_angle65536				= ((-first_angle65536)&(65536-1));					/* 角度 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_UROKO14_04_MIZUIRO;			/* [水鱗弾] */
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
		#endif
	}
}


