
/*---------------------------------------------------------
	東方模倣風 〜 Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	咲夜のカードを定義します。
---------------------------------------------------------*/


/*---------------------------------------------------------
	紅5面中-ボス 咲夜 第一攻撃にちょっとだけ似たカード
	-------------------------------------------------------
	このカードの本質は、
	１．比較的速い(というか中速度)の青ナイフ弾の壁をくぐりながら、
	異常に遅いかつ微加速なので長く留まって避け難い赤クナイ弾を避ける事。
	２．青ナイフ弾と赤クナイ弾の速度が違うので、重なって避け難い場合が発生する事。
	３．しかしながら、重なりはある程度の時間を観察していれば避けられる。
	４．(３．)に矛盾して、観察時間が短い(こういう場合も必ず発生する)と運が悪いと避けられない。
	-------------------------------------------------------
	■ 青ナイフ
		青ナイフは曲がった壁にならないと魅力は無い。
		比較的速い速度だが、速過ぎると魅力は無い。微妙にいらいらする遅さも必要。
	■ 赤クナイ
		赤クナイにはいらいらする遅さが必要。
		画面下で避けていた場合に、「画面下あたりでのみ急激に速度が変化する」。
		こういう加速度で調整する必要あり。
		前記２−４は主に画面下あたりで発生する現象。
		画面中あたりでは、その分敵に近いので反射神経は要るが、前記２−４の現象は発生しにくい。
	-------------------------------------------------------
	■ 赤クナイ
		なんか知らんが原作やり直したら「減速弾」だった。
		「加速弾」＋「減速弾」って事かなぁ？
	-------------------------------------------------------
	原作と違い、そもそも上に行けないから、上避けは出来ない。
	(pspの縦解像度では上に行くスペースがない)
	下避けもpspの縦解像度が全然足りないから、本家風のチョン避けは無理。
	ある程度左右に動き回って避けて貰う事を想定している。(だから弾速遅い)
	それに併せて密着弾はあえてぜんぜん違う。(別変数とるのが面倒だった事もある)
	-------------------------------------------------------
	青ナイフの弾速はこれくらいかも？赤クナイの弾速は速過ぎる。
	赤クナイ弾の周期は原作はだいぶずれてくるので変えたいんだけど、
	「カードシステム二重化」しないと上手くいかないかも？。
	-------------------------------------------------------
	「カードシステム二重化」：2種類の異なったカードが同時に撃てるカードシステム。
	そういうものにするべきかしないべきか検討中。(パチェ)
	-------------------------------------------------------
	まだ禊(脅し)なので、(プレイヤーにこんな処で)死んでもらっては困る。
	-------------------------------------------------------
    青ナイフspeed256    speed_offset    /   赤クナイspeed256    speed_offset
    t256(2.5)           (0)             /   (0)                 (4)             // 良いんだけど本物より難しい。
    t256(2.0)           (0)             /   (0)                 (2)             // 赤クナイの初速遅すぎるかなあ。// 青ナイフと赤クナイがずれすぎ。本物より難しい。
    t256(1.75)          (0)             /   (0)                 (1)
    -------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。
	REG_09_REG1 	aaa_angle65536
	REG_0a_REG2 	aka_kunai_timer 	赤クナイタイマー
	REG_0b_REG3 	ao_knife_timer		青ナイフタイマー
---------------------------------------------------------*/

//#define ADJUST3916	((65536/20)+(640))
#define ADJUST3916	(4000)
//#define ADJUST4444	(4200)
#define ADJUST4444	(4190)
local OBJ_CALL_FUNC(spell_create_01_sakuya_misogi_normal)
{
//	SPELL_TIME_0256 SPELL_TIME_0128
	if (127 == (REG_10_BOSS_SPELL_TIMER))/* 初期値にリセット。 */
	{
		#if 1
		/* 青ナイフ: 右上側から撃ち始める。右側なので90度ずらして、hatudan_system_regist_katayori_n_way()の補正も行う。 */
	//	src->tmp_angleCCW65536	= (65536+(65536/4)-((65536*8)/20))+(ADJUST4444);				// 青ナイフ
	//	src->tmp_angleCCW65536	= (((65536*2)/20))+(ADJUST4444);				// 青ナイフ 	hatudan_system_regist_katayori_n_way();で下を基準に、時計回りに(2/20)から撃ち始める。
		src->tmp_angleCCW65536	= (((65536*1)/18))+(ADJUST4444);				// 青ナイフ 	hatudan_system_regist_katayori_n_way();で下を基準に、時計回りに(2/20)から撃ち始める。
		// 本物は打ち始めの青ナイフの端が右上60度くらいから打ち始め、
		// 打ち終わりは、左上60度くらいの位置になる。
		#endif
		#if 1
		/* 赤クナイ: */
	//	REG_09_REG1 		= (65536/2);		// 赤クナイ
		REG_09_REG1 		= (65536/4)-(0);	// 赤クナイ (右側から)
		#endif
		REG_0b_REG3 = 0;	// 青ナイフ
//		REG_0a_REG2 = 0;	// 赤クナイ
	}
//	if (DANMAKU_0096_TIME > (REG_10_BOSS_SPELL_TIMER))/* 暫く何もしない。 */
	if (100 > (REG_10_BOSS_SPELL_TIMER))/* 暫く何もしない。 */
	{
		REG_0b_REG3--;
		if (0 >= REG_0b_REG3)
	//	if (0==((REG_10_BOSS_SPELL_TIMER)&0x07))
		{
		//	REG_0b_REG3 = (12);
			REG_0b_REG3 = (8);
			/*	青ナイフ */
		//	src->tmp_angleCCW1024 -= (int)(256/5)+(10); /*CCW*/
		//	src->tmp_angleCCW1024 -= (int)(1024/20)+(10); /*CCW*/
		//	src->tmp_angleCCW65536 -= (int)(65536/20)+(640); /*CCW*/
		//	src->tmp_angleCCW65536 -= (int)(65536/20)+(640)+(128); /*CCW*/	/* 3276== (65536/20) */
			src->tmp_angleCCW65536 -= (ADJUST4444); /*CCW*/
			mask65536(src->tmp_angleCCW65536);
		//	HATSUDAN_01_speed256			= (t256(2.5));					/* 弾速 (t256(2.5)参考。紅ノーマル5面) */
		//	HATSUDAN_01_speed256			= (t256(2.0));					/* 弾速 pspは縦解像度がないのでとにかく遅くしないと。 */
//			HATSUDAN_01_speed256			= (t256(1.75)); 				/* 弾速 pspは縦解像度がないのでとにかく遅くしないと。 */
		//	HATSUDAN_01_speed256			= (t256(1.00)); 				/* 弾速 pspは縦解像度がないのでとにかく遅くしないと。 */
			HATSUDAN_01_speed256			= (t256(1.25)); 				/* 弾速 pspは縦解像度がないのでとにかく遅くしないと。 */
		//	HATSUDAN_02_speed_offset		= t256(0.0);/* てすと */
		//	HATSUDAN_02_speed_offset		= t256(1.0);/* 加速してみるてすと */
		//	HATSUDAN_02_speed_offset		= t256(6.0);/* 加速してみるてすと */
			HATSUDAN_02_speed_offset		= t256(4.0);/* 加速してみるてすと */
			HATSUDAN_03_angle65536			= (src->tmp_angleCCW65536); 	/* 弾源角度65536 */ 		/* [螺旋状弾] */
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI;		/* [青ナイフ弾] */
			HATSUDAN_06_n_way				= (8);							/* [8way] */
		//	HATSUDAN_07_div_angle65536		= (int)(65536/(5*4));
		//	HATSUDAN_07_div_angle65536		= (int)(65536/20);				/* 分割角度([360/360]度を20分割) */
			HATSUDAN_07_div_angle65536		= (int)(65536/18);				/* 分割角度([360/360]度を20分割)[打ち終わりが左斜め上になる] */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		//	if (0==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* reset (1/32) */
		//	{
		//	//	aaa_angle1024	= (src->tmp_angleCCW1024)-(int)((1024/2));
		//		REG_09_REG1 	= (src->tmp_angleCCW65536)-(int)((65536/2));
		//	}
			#if (1)
		//	voice_play(VOICE14_SAKUYA_SHOT02, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);
			#endif
		}
	}
	REG_0a_REG2--;
	if (0 > REG_0a_REG2) /* REG_0a_REG2 が負の場合 */
	{
		REG_0a_REG2 = 32/*64*/+(ra_nd()&0x3f);
	}
	else /* REG_0a_REG2 が正の場合 */
//	/* 赤クナイの方が遅いので、青ナイフより先に撃ち始める事にする。 */
//	if (DANMAKU_0032_TIME > (REG_0a_REG2))/* 暫く何もしない。 */
//	if ((41) > (REG_0a_REG2))/* 暫く何もしない。 */
	if ((41-16+(((unsigned)REG_0f_GAME_DIFFICULTY)<<4)) > (u32)(REG_0a_REG2))/* 暫く何もしない。 */
//	if ((128-40) < (REG_0a_REG2))/* 最後は暫く何もしない。 */
	{
	//	else/* こっちの弾は本当はずれて出る感じにしたい。 */
//	//	if ((10-1)>((((u8)REG_0a_REG2)^0x20)&0x2f))/* x 10set */
//		if ((10-1)>(((REG_0a_REG2)	  )&0x0f))/* x 10set */
		/* 4回に一回、計10set */
		if ((0)==(((REG_0a_REG2)	  )&0x03))/* x 10set */
		{
			/* こっちの赤クナイは全然ダメ． */
		//	aaa_angle1024 -= (int)(256/5);
		//	aaa_angle1024 -= (int)(1024/20);
			REG_09_REG1 -= (int)(65536/20);
		//	if (0 > (int)REG_09_REG1)	{	REG_09_REG1 = (65536/2);	}
			mask65536(REG_09_REG1);
		//	HATSUDAN_01_speed256			= (t256(2.0));						/* 弾速 速過ぎる */
		//	HATSUDAN_01_speed256			= (t256(1.0));						/* 弾速 (出始めは遅い) */
		//	HATSUDAN_01_speed256			= (t256(0.5));						/* 弾速 (出始めは遅い) */
		//	HATSUDAN_01_speed256			= (t256(0.25)); 					/* 弾速 (出始めは遅い) */
		//	HATSUDAN_03_angle65536			= (((REG_09_REG1)+(65536*3/4))&(65536-1));	/* 弾源角度65536 */ 	/*下CCW*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [赤クナイ弾] */
			HATSUDAN_06_n_way				= (4);								/* [4way] */
			HATSUDAN_07_div_angle65536		= (int)(512-64);					/* [密着弾]の分割角度(r32) */
//-------------------------
	// ■  [加速弾]
		//	HATSUDAN_01_speed256			= (0);								/* 弾速 (出始めは遅い) */
//			HATSUDAN_01_speed256			= (t256(0.25)); 					/* 弾速 (出始めは遅い) */
		//	HATSUDAN_01_speed256			= (t256(3.25)); 					/* 弾速 (出始めは遅い) */
		//	HATSUDAN_01_speed256			= (t256(3.00)); 					/* 弾速 (出始めは遅い) */
			HATSUDAN_01_speed256			= (t256(2.00)); 					/* 弾速 (出始めは遅い) */

		//	HATSUDAN_02_speed_offset		= t256(3/*0*/);/* 加速してみる てすと */
//			HATSUDAN_02_speed_offset		= t256(2);/* 微加速？(加減速弾な事は間違いない？) */
		//	HATSUDAN_02_speed_offset		= t256(1);/* 微加速？(加減速弾な事は間違いない？) */
			HATSUDAN_02_speed_offset		= t256(-1);/* 減速？(加減速弾な事は間違いない？) */
			HATSUDAN_03_angle65536			= (REG_09_REG1);	/* 弾源角度65536 */ 	/*下CCW*/
			hatudan_system_regist_katayori_n_way();/* (r33-) */
//-------------------------
	// ■  [減速弾]
		//	HATSUDAN_01_speed256			= (t256(0.25)); 					/* 弾速 (出始めは遅い) */
		//	HATSUDAN_01_speed256			= (t256(1.25)); 					/* 弾速 (出始めは遅い) */
			HATSUDAN_01_speed256			= (t256(2.25)); 					/* 弾速 (出始めは遅い) */
			HATSUDAN_02_speed_offset		= t256(-1);/* 減速？(加減速弾な事は間違いない？) */
			HATSUDAN_03_angle65536			= (REG_09_REG1);	/* 弾源角度65536 */ 	/*下CCW*/
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}


/*---------------------------------------------------------
	紅5面中-ボス 咲夜 第一攻撃にちょっとだけ似たカード
	-------------------------------------------------------
	模倣風なんだから少し模倣風なカードをちょっとテストで創ってみた。
	難度的にはpsp-2000なら、紅normal5面に通常カードとして出てきたとして同じくらいか、
	ちょっぴり難しいかなって感じ。タブン。
	簡単でもないけど、難しくもないって辺りを狙ってみたんだけど、難易度良く解からん。
	psp-1000の方向キーは始めからlunaticだしアナログだと位置が悪いから疲れるしな。
	-------------------------------------------------------
	設計したのは、
	赤クナイはいつまでも画面に残るので嫌な(めんどくさい)感じ。
	青ナイフは通常、赤ナイフは反対周り。ナイフは必ず一定間隔。
	ナイフ３塊。画面下だとナイフ塊、微妙に減速するので速度が嫌な感じ。
	３塊にしたのは意図的で２塊避けて反対側に切り返すパターン(くの字避け)がある。
	(２塊だと少ないし、４塊だとムリゲっぽい)
	(始めて見た時は３塊直線的に避けるだろうし、ちょっと慣れれば直ぐ気が付く筈、
	ただしこの「くの字避け」。思ったより間隔が狭いので、慣れるけどタイミングが若干シビア。)
	-------------------------------------------------------
	lunaticといえ、こんな所で死んで貰っては困るので基本的にこけおどし。
	一見難しそうだけど、塊のナイフ群を抜けるだけだから、
	割とこけおどしで意外と簡単かと。(タブン)
	基本的に目先のナイフの速度に騙されて。赤クナイ良く見てないとしぬるタイプ。
	赤クナイ発生はランダムなんだから、考えて先読みしないと追い詰められる。
	追い詰められた場合ボムっても良いけど、ある程度下ならナイフに隙間が多いので隙間を抜けられる。
	追い詰められてボムる場合、ナイフの速度が速いんだから、当然(他のカードでボムる場合より)速く反応
	しないとならないが(喰らいボム時間がやたら長い霊夢は除く)、微妙に嫌なパターンの赤クナイで
	判断鈍ってるのがポイントといえばポイントかな？
	ちなみに幽々子で避けれる速度に調整してあります。
	ナイフを３塊直線的に避けるパターン以外に、
	ナイフを２塊避けて反対側に切り返すパターンがあるのだから、
	その辺、臨機応変に切り替えるのがコツといえばコツかな。
	(ナイフ塊をジグザグにいくパターンは一応出来るけど、かなり難しいと思うよ。
	そーゆーのの方が得意な人もいるけど一般的(?)には大道芸っぽい感じ)
	(ナイフを１塊避けて反対側に切り返すパターンもあるけど、使い所が難しい。知らなくてもいいかな。
	その分あとで赤クナイが出るのだから、赤クナイを早くから先読みしとかないとならない。
	先読み失敗した場合、ジグザグに逃げるのは技術的に難しいのだから、その分不利になる。)
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。
	REG_09_REG1 	aaa_angle65536
	REG_0a_REG2 	aka_kunai_timer 	赤クナイタイマー
	REG_0b_REG3 	knife_kaiten		0(青ナイフ) or 1(赤ナイフ)
//	sta tic int ao_knife_timer; 		// 青ナイフタイマー
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_2d_sakuya_misogi_lunatic)
{
//	SPELL_TIME_0256 SPELL_TIME_0128
	if (127 == (REG_10_BOSS_SPELL_TIMER))/* 初期値にリセット。 */
	{
		#if 1
		/* 青ナイフ: 右上側から撃ち始める。右側なので90度ずらして、hatudan_system_regist_katayori_n_way()の補正も行う。 */
	//	src->tmp_angleCCW65536	= (65536+(65536/4)-((65536*8)/20))+(ADJUST3916);				// 青ナイフ
	//	src->tmp_angleCCW65536	= (((65536*2)/20))+(ADJUST3916);				// 青ナイフ 	hatudan_system_regist_katayori_n_way();で下を基準に、時計回りに(2/20)から撃ち始める。
		src->tmp_angleCCW65536	= (((65536*1)/18))+(ADJUST3916);				// 青ナイフ 	hatudan_system_regist_katayori_n_way();で下を基準に、時計回りに(2/20)から撃ち始める。
		// 本物は打ち始めの青ナイフの端が右上60度くらいから打ち始め、
		// 打ち終わりは、左上60度くらいの位置になる。
		#endif
		#if 1
		/* 赤クナイ: */
	//	REG_09_REG1 		= (65536/2);		// 赤クナイ
		REG_09_REG1 		= (65536/4)-(0);	// 赤クナイ (右側から)
		#endif
//		ao_knife_timer = 0; 	// 青ナイフ
//		REG_0a_REG2 = 0;	// 赤クナイ
		REG_0b_REG3 &= 1;/* 念のためマスク */
		REG_0b_REG3 ^= 1;/* 反転 */
	}
//	if (DANMAKU_0096_TIME > (REG_10_BOSS_SPELL_TIMER))/* 暫く何もしない。 */
	if (48 > (REG_10_BOSS_SPELL_TIMER))/* 暫く何もしない。 */
	{
//		ao_knife_timer--;
//		if (0 >= ao_knife_timer)
	//	if (0==((REG_10_BOSS_SPELL_TIMER)&0x07))
		{
		//	ao_knife_timer = (12);
//			ao_knife_timer = (8);
			/*	青ナイフ */
			if (0==REG_0b_REG3)
			{	/* 0(青ナイフ) */
			//	src->tmp_angleCCW1024 -= (int)(256/5)+(10); /*CCW*/
			//	src->tmp_angleCCW1024 -= (int)(1024/20)+(10); /*CCW*/
			//	src->tmp_angleCCW65536 -= (int)(65536/20)+(640); /*CCW*/
			//	src->tmp_angleCCW65536 -= (int)(65536/20)+(640)+(128); /*CCW*/	/* 3276== (65536/20) */
				src->tmp_angleCCW65536 -= (ADJUST3916); /*CCW*/
			}
			else	/* 1(赤ナイフ) */
			{
				src->tmp_angleCCW65536 += (ADJUST3916); /*CCW*/
			}
			mask65536(src->tmp_angleCCW65536);
			HATSUDAN_03_angle65536			= (src->tmp_angleCCW65536); 	/* 弾源角度65536 */ 		/* [螺旋状弾] */
			//
		//	HATSUDAN_01_speed256			= (t256(2.5));						/* 弾速 (t256(2.5)参考。紅ノーマル5面) */
		//	HATSUDAN_01_speed256			= (t256(2.0));						/* 弾速 pspは縦解像度がないのでとにかく遅くしないと。 */
//			HATSUDAN_01_speed256			= (t256(1.75)); 					/* 弾速 pspは縦解像度がないのでとにかく遅くしないと。 */
			HATSUDAN_01_speed256			= (t256(1.00)); 					/* 弾速 pspは縦解像度がないのでとにかく遅くしないと。 */
			HATSUDAN_02_speed_offset		= t256(6);/*加速してみるてすと*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI+REG_0b_REG3;			/* [青ナイフ弾] */
			HATSUDAN_06_n_way				= (8);								/* [8way] */
		//	HATSUDAN_07_div_angle65536		= (int)(65536/(5*4));
		//	HATSUDAN_07_div_angle65536		= (int)(65536/20);					/* 分割角度([360/360]度を20分割) */
			HATSUDAN_07_div_angle65536		= (int)(65536/18);					/* 分割角度([360/360]度を20分割)[打ち終わりが左斜め上になる] */
			hatudan_system_regist_katayori_n_way();/* (r33-) */

		//	if (0==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* reset (1/32) */
		//	{
		//	//	aaa_angle1024	= (src->tmp_angleCCW1024)-(int)((1024/2));
		//		REG_09_REG1 = (src->tmp_angleCCW65536)-(int)((65536/2));
		//	}
			#if (1)
		//	voice_play(VOICE14_SAKUYA_SHOT02, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);
			#endif
		}
	}
	REG_0a_REG2--;
	if (0 > REG_0a_REG2)
	{
		REG_0a_REG2 = 64+(ra_nd()&0x3f);
	}
	else
//	/* 赤クナイの方が遅いので、青ナイフより先に撃ち始める事にする。 */
//	if (DANMAKU_0032_TIME > (REG_0a_REG2))/* 暫く何もしない。 */
	if (41 > (REG_0a_REG2))/* 暫く何もしない。 */
//	if ((128-40) < (REG_0a_REG2))/* 最後は暫く何もしない。 */
	{
	//	else/* こっちの弾は本当はずれて出る感じにしたい。 */
//	//	if ((10-1)>((((u8)REG_0a_REG2)^0x20)&0x2f))/* x 10set */
//		if ((10-1)>(((REG_0a_REG2)	  )&0x0f))/* x 10set */
		/* 4回に一回、計10set */
		if ((0)==(((REG_0a_REG2)	  )&0x03))/* x 10set */
		{
			/* こっちは赤クナイ */
		//	aaa_angle1024 -= (int)(256/5);
		//	aaa_angle1024 -= (int)(1024/20);
			REG_09_REG1 -= (int)(65536/20);
		//	if (0 > (int)REG_09_REG1)	{	REG_09_REG1 = (65536/2);	}
			mask65536(REG_09_REG1);
		//	HATSUDAN_03_angle65536			= (((REG_09_REG1)+(65536*3/4))&(65536-1));	/* 弾源角度65536 */ 	/*下CCW*/
		//	HATSUDAN_03_angle65536			= (((REG_09_REG1)+(65536*3/4))&(65536-1));	/* 弾源角度65536 */ 	/*下CCW*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [赤クナイ弾] */
			HATSUDAN_06_n_way				= (4);								/* [4way] */
			HATSUDAN_07_div_angle65536		= (int)(512-64);					/* [密着弾]の分割角度(r32) */
//-------------------------
	// ■  [加速弾]
			HATSUDAN_01_speed256			= (t256(2.00)); 							/* 弾速 (出始めは遅い) */
			HATSUDAN_02_speed_offset		= t256(-1);/* 減速？(加減速弾な事は間違いない？) */
			HATSUDAN_03_angle65536			= (REG_09_REG1);	/* 弾源角度65536 */ 	/*下CCW*/
			hatudan_system_regist_katayori_n_way();/* (r33-) */
//-------------------------
	// ■  [減速弾]
			HATSUDAN_01_speed256			= (t256(2.25)); 							/* 弾速 (出始めは遅い) */
			HATSUDAN_02_speed_offset		= t256(-1);/* 減速？(加減速弾な事は間違いない？) */
			HATSUDAN_03_angle65536			= (REG_09_REG1);	/* 弾源角度65536 */ 	/*下CCW*/
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}


/*---------------------------------------------------------
	紅5面中-ボス 咲夜「奇術：ミスディレクション」にちょっとだけ似たカード
	-------------------------------------------------------
	Lunatic 赤クナイ弾 19way
	-------------------------------------------------------

---------------------------------------------------------*/
/*
------------------
		/	256
		/		赤クナイ
		/	//(-r33)208 208==	64+144==256-48
27*8	/	216 == 64+152== 256-40//(r34-)
		/		なにもしない
14*8	/	112 112==	64+48
		/		青ナイフ
		/	64	64==	64+0
		/	------------
		/	64	64==	((64+256)-256)
		/		なにもしない
		/	48	48==	((64+240)-256)
		/		青ナイフの音だけ。
		/	0	0== 	((64+192)-256)
		/		反転移動
		/	0	0== 	((64+192)-256)
*/
/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_init_22_sakuya_miss_direction)
{
	if(1<REG_0f_GAME_DIFFICULTY)
	// Hard or Lunatic only.
	{
		card.danmaku_callback[1] = danmaku_01_standard_angle_sayuu_hansya_mover;/*(通常弾用)*/	/*(画面外なら反射減速)*/
	}
	else
	{
		// Easy or Normal only.(標準状態)
		card.danmaku_callback[1] = danmaku_00_standard_angle_mover;/*(通常弾用)*/
	}
//	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}

local OBJ_CALL_FUNC(spell_create_22_sakuya_miss_direction)
{
//	if (/*256-64-48*/(64+144)< REG_10_BOSS_SPELL_TIMER)
	if (/*256-64-48*/(256-40)< REG_10_BOSS_SPELL_TIMER)
	{
		/*---------------------------------------------------------
			自機狙い、全方向24分割、赤クナイ弾
		---------------------------------------------------------*/
		/* 赤クナイ弾 */
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
		{
		//	発弾位置をボス中心へセット。
			//	HATSUDAN_01_speed256				= (t256(2.0)+((REG_10_BOSS_SPELL_TIMER)<<2)); /* 弾速 */
				HATSUDAN_01_speed256				= (t256(2.0)-((REG_10_BOSS_SPELL_TIMER)));	/* 弾速 */
				HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(TAMA_SPEC_1000_EFFECT_MIDDLE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
				HATSUDAN_05_bullet_obj_type 		= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [赤クナイ弾] */
				HATSUDAN_06_n_way					= (24); 							/* [24way] */
			//	Lunatic 赤クナイ弾 19way
				HATSUDAN_07_div_angle65536			= (int)(65536/24);					/* 分割角度 */
			// lunatic は[19way]
		//
			unsigned int ii;
			for (ii=0; ii<((((unsigned)REG_0f_GAME_DIFFICULTY)+1)); ii++)
			{
			//	REG_02_DEST_X							+= (ii<<10);	/* 弾源x256 */
				REG_02_DEST_X							+= (1<<10); 	/* 弾源x256 */		//	発弾位置をずらす。
			//
				HATSUDAN_02_speed_offset			= t256(1)+(ii<<8);/*(テスト)*/
				calculate_jikinerai();/* 自機狙い角作成 */
				#if 1/*(r35のシステムでは、省略可能)*/
			//	HATSUDAN_03_angle65536				= (HATSUDAN_03_angle65536); /* 弾源角度65536 */ 	/* 自機狙い弾 */
				#endif
				hatudan_system_regist_katayori_n_way();/* (r33-) */
			}
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
			#endif
		}
	}
	else
//	if (256-192< REG_10_BOSS_SPELL_TIMER)
	if ((64+48)< REG_10_BOSS_SPELL_TIMER)
	{;}/* なにもしない */
	else
//	if (256-192< REG_10_BOSS_SPELL_TIMER)
	if ((64+0)< REG_10_BOSS_SPELL_TIMER)
	{
		/* 青ナイフ */
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			REG_02_DEST_X	= ((src->center.x256));
			REG_03_DEST_Y	= ((src->center.y256));
			calculate_jikinerai();/* 1:いちいち作成するっぽい。 */
				int int_aaa 						= ((HATSUDAN_03_angle65536));		/* 弾源角度1024 */
				HATSUDAN_02_speed_offset		= t256(0);/*(テスト)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
				HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI;			/* [青ナイフ弾] */
			int jj;
			for (jj=(0); jj<(3); jj++)
			{
				// 弾1 弾2	1.0 (1.5 2.0)
				HATSUDAN_01_speed256			= (t256(1.0)+(jj<<7));					/* 弾速 */
				HATSUDAN_03_angle65536			= (int_aaa);			/* [破壊] */	/* 弾源角度1024 */
				HATSUDAN_06_n_way				= (11); 				/* [破壊] */	/* [11way] */
				HATSUDAN_07_div_angle65536		= (int)(65536/(7*4));	/* [破壊] */	/* 分割角度([90/360]度を7分割==28分割) */
				hatudan_system_regist_n_way();/* (r33-) */
			}
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);/* 青ナイフの音はずらす必要がある */
			#endif
		}
	}
	#if 0
	else
	if (/*256-64*/(48/*(64+240)-256*/) < REG_10_BOSS_SPELL_TIMER)
	{;}/* なにもしない */
	else
	//if (/*256-64*/(0/*(64+192)-256*/) < REG_10_BOSS_SPELL_TIMER)
	{
		/* 青ナイフの音だけ(3回) */
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);/* 青ナイフの音はずらす必要がある */
			#endif
		}
	}
	#endif
}


#if 0/*(旧版、未使用。雑魚スペカとしても再利用は無理がある)*/
/*---------------------------------------------------------
	紅5面中-ボス 咲夜「奇術：ミスディレクション(1/2)」にちょっとだけ似たカード
	-------------------------------------------------------
	自機狙い、全方向24分割、赤クナイ弾
---------------------------------------------------------*/
//SPELL_TIME_0048
local OBJ_CALL_FUNC(spell_create_02_24nerai)
{
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
	{
		REG_02_DEST_X	= ((src->center.x256));
		REG_03_DEST_Y	= ((src->center.y256));
		calculate_jikinerai();/* 1:いちいち作成するっぽい。 */
	//	HATSUDAN_01_speed256			= (t256(2.0)+((REG_10_BOSS_SPELL_TIMER)<<2)); /* 弾速 */
		HATSUDAN_01_speed256			= (t256(2.0)-((REG_10_BOSS_SPELL_TIMER)));	/* 弾速 */
		HATSUDAN_02_speed_offset		= t256(1);/*(テスト)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_2000_EFFECT_MINI)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [赤クナイ弾] */
		HATSUDAN_06_n_way				= (24); 							/* [24way] */
		HATSUDAN_07_div_angle65536		= (int)(65536/24);					/* 分割角度 */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
}
/*---------------------------------------------------------
	紅5面中-ボス 咲夜 「奇術：ミスディレクション(2/2)」にちょっとだけ似たカード
	-------------------------------------------------------
	自機狙い、11弾(中心と、左右に5弾ずつ狙い)
	-------------------------------------------------------
	原作やりなおしたら、ここは３回撃つ。
---------------------------------------------------------*/
//SPELL_TIME_0064
local OBJ_CALL_FUNC(spell_create_03_11nife)
{
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x07))
	{
		REG_02_DEST_X	= ((src->center.x256));
		REG_03_DEST_Y	= ((src->center.y256));
		calculate_jikinerai();/* 1:いちいち作成するっぽい。 */
		HATSUDAN_01_speed256				= (t256(2.0));						/* 弾速 */
		HATSUDAN_02_speed_offset			= t256(0);/*(テスト)*/
		HATSUDAN_03_angle65536				= ((HATSUDAN_03_angle65536))-(int)(65536*6/(7*4));		/* 弾源角度1024 */
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_2000_EFFECT_MINI)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		HATSUDAN_05_bullet_obj_type 		= BULLET_KNIFE20_04_AOI;			/* [青ナイフ弾] */
		HATSUDAN_06_n_way					= (11); 							/* [11way] */
		HATSUDAN_07_div_angle65536			= (int)(65536/(7*4));				/* 分割角度([90/360]度を7分割==28分割) */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
}
#endif


/*---------------------------------------------------------
	紅5面ボス 咲夜 「通常攻撃1(1/2)」にちょっとだけ似たカード(予定)
	-------------------------------------------------------
	24way =(右12way)+(左12way)
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_04_pink_hearts)
{
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x07))/* 本物は 8 弾(128==0x80==spell_time_out) */
	{
			HATSUDAN_02_speed_offset		= t256(1/*0*/);/*(テスト)*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_06_YUKARI; 		/* [ピンクナイフ弾] */
			REG_03_DEST_Y					= (src->center.y256);	/* 弾源y256 ボス中心から発弾。 */
		//	発弾初期位置の修正
			REG_02_DEST_X					-= t256(4.0);		/* 弾源x256 ボス左側へ発弾。 */
		//
		int jj;
		jj = 0;
		int ii;
		for (ii=0; ii<(13*12*4)/*144*/; ii+= (13*4) )/*12方向*/ 						/* 角度(128[(90+a)/360度]を 10 分割) */
		{
			HATSUDAN_01_speed256			= (((160*4)-ii));						/* 弾速 */
			//
		//	REG_02_DEST_X					= (src->center.x256) + t256(4.0);			/* 弾源x256 ボス右側から発弾。 */
			REG_02_DEST_X					+= t256(8.0);			/* 弾源x256 ボス右側から発弾。 */
			HATSUDAN_03_angle65536			= (jj/*(0+i+i)&(65536-1)*/);
			hatudan_system_regist_single();/* (r33-) */
			//
		//	REG_02_DEST_X					= (src->center.x256) - t256(4.0);			/* 弾源x256 ボス左側から発弾。 */
			REG_02_DEST_X					-= t256(8.0);			/* 弾源x256 ボス左側から発弾。 */
			HATSUDAN_03_angle65536			= ((0-jj)&(65536-1));
			hatudan_system_regist_single();/* (r33-) */
			//
			jj += (13*2*64);
		}
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);
		#endif
	}
}


/*---------------------------------------------------------
	紅5面ボス 咲夜 「通常攻撃1(2/2)」にちょっとだけ似たカード(予定)
	-------------------------------------------------------
	32方向に分割、偶数は低速弾、奇数は高速弾。
	16方向を2回登録する。
	-------------------------------------------------------
	32方向	角度(1024[360/360度]を 32 分割)1024 == 32 x 32
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_05_32way_dual)
{
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x3f))
	{
		HATSUDAN_02_speed_offset			= t256(1/*0*/);/*(テスト)*/
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		HATSUDAN_05_bullet_obj_type 		= BULLET_KNIFE20_04_AOI;			/* [青ナイフ弾] */
		HATSUDAN_06_n_way					= (16); 							/* [16way] */
		HATSUDAN_07_div_angle65536			= (int)(65536/(16));		/* 分割角度(65536[360/360度]を 16 分割) */	/* 1周をn分割した角度 */
	//
		HATSUDAN_01_speed256				= (t256(0.75)); 		/* 偶数は低速弾 */
		HATSUDAN_03_angle65536				= (0);					/* 弾源角度65536 */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	//
		HATSUDAN_01_speed256				= (t256(1.25)); 		/* 奇数は高速弾 */
		HATSUDAN_03_angle65536				= (65536/32);			/* 弾源角度65536 */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
}


/*---------------------------------------------------------
	3「通常攻撃1」遅い弾との組み合わせ
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_2a_sakuya_baramaki1)
{
//	if (64 == (REG_10_BOSS_SPELL_TIMER))
//	{
//	}
//	else
	if (64 > (REG_10_BOSS_SPELL_TIMER))
	{
		spell_create_04_pink_hearts(src);
	}
	else
	if (0 == ((REG_10_BOSS_SPELL_TIMER)&0x7f))	/* 128 256 384 512 (640 768) */
	{
		spell_create_05_32way_dual(src);
	}
}


/*---------------------------------------------------------
	5「通常攻撃3」速い弾との組み合わせ
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_2b_sakuya_baramaki2)
{
	if (64 > (REG_10_BOSS_SPELL_TIMER))
	{
		spell_create_04_pink_hearts(src);
	}
	else
//	if (0 == ((REG_10_BOSS_SPELL_TIMER)&0xff))	/* 256 512 (768) */
	if (0 == ((REG_10_BOSS_SPELL_TIMER)&0xff))	/* 256 512 (768) */
	{
		spell_create_05_32way_dual(src);
	}
}

/*---------------------------------------------------------
	幻在「クロックコープス(1/2)」
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	幻在「クロックコープス(2/2)」
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	幻象「ルナクロック(1/2)」
	-------------------------------------------------------
	32way 固定連弾	x 4回
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_06_luna_clock_32way)
{
	if (0x40==((REG_10_BOSS_SPELL_TIMER)&0xcf))/* 4回 */
	{
		HATSUDAN_01_speed256			= (t256(1.0));				/* 弾速 */
		HATSUDAN_02_speed_offset		= t256(1/*0*/);/*(テスト)*/
		HATSUDAN_03_angle65536			= (0);						/* 発射中心角度 / 特殊機能(自機狙い/他) */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		HATSUDAN_05_bullet_obj_type 	= BULLET_CAP16_04_TUTU_SIROI;	/* [青白実包弾] */	/* 弾グラ */
		HATSUDAN_06_n_way				= (32); 					/* [32way] */		/* 発弾数 */
		HATSUDAN_07_div_angle65536		= (int)(65536/(32));		/* 分割角度(1024[360/360度]を 32 分割) */	/* 1周をn分割した角度 */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
}


/*---------------------------------------------------------
	幻象「ルナクロック(2/2)」
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	「通常攻撃3(1/2)」
	-------------------------------------------------------
	拡散型全方位ナイフ弾
	80分割弾 == (8way[hi to 0]+8way[slow to 0]) x 5way(5分割)
	-------------------------------------------------------
	本物は画面上と画面左と画面右の端でナイフが一度だけ反射する。
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_07_80way_dual_five)
{
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x3f))
	{
			HATSUDAN_02_speed_offset		= t256(1/*0*/);/*(テスト)*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI;				/* [青ナイフ弾] */
	//
		int ii; 	/* 1周を65536分割した角度 */
		int j;		/* カウンタ */
		j=0;
		for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)	12.8==(1024/80) 80分割 [5分割] */	/* 204.8==(1024/5) 角度(1024[72/360度]を 5 分割) */
		{
			j++;
			j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
			HATSUDAN_01_speed256			= (t256(3.0)-((j)<<5)-((j&1)<<7));		/*奇数は高速弾*/
			HATSUDAN_03_angle65536			= (((0-ii))&(65536-1)); 			/* 角度 */
			hatudan_system_regist_single();/* (r33-) */
		}
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
}
/*---------------------------------------------------------
	通常攻撃、青赤ナイフ
	青はCW、赤はCCW
---------------------------------------------------------*/
/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_init_2c_sakuya_blue_red_knife)
{
	card.danmaku_callback[1] = danmaku_01_standard_angle_sayuu_hansya_mover;/*(通常弾用)*/	/*(画面外なら反射減速)*/
//	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}
/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_2c_sakuya_blue_red_knife)
{
	if ((0!=((REG_10_BOSS_SPELL_TIMER)&0x0180)))
	{
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x3f))
		{
			unsigned int kk;
			kk = (((REG_10_BOSS_SPELL_TIMER)&0x0100)>>8);
				HATSUDAN_02_speed_offset		= t256(1/*0*/);/*(テスト)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
				HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI+kk; 			/* [青ナイフ弾] */
		//
			int ii; 	/* 1周を65536分割した角度 */
			int j;		/* カウンタ */
			j = 0;
			for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)	12.8==(1024/80) 80分割 [5分割] */	/* 204.8==(1024/5) 角度(1024[72/360度]を 5 分割) */
			{
				j++;
				j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
				HATSUDAN_01_speed256			= (t256(3.0)-((j)<<5)-((j&1)<<7));		/*奇数は高速弾*/
				HATSUDAN_03_angle65536			= ((kk?(0-ii):(ii))&(65536-1)); 			/* 角度 */
				hatudan_system_regist_single();/* (r33-) */
			}
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
			#endif
		}
	}
}


#if 0
/*---------------------------------------------------------
	第11形態: 最終形態(その3)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_47_sakuya_meek)
{
	if (0==(REG_10_BOSS_SPELL_TIMER))
	{
		add_za ko_sakuya_doll_01_laser(src);/* レーザー追加 */
	}
//
	src->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	src->alpha = 0xff;*/
//
}
#endif

/*---------------------------------------------------------
	メイド秘技「操りドール」
---------------------------------------------------------*/
#if 0
#endif





/*
	幻幽「ジャック・ザ・ルドビレ」の研究。
	このカードは始めに[赤大弾]をランダムっぽくばら撒く。
	このばら撒きは、何て事のないいつもの東方ばら撒きなんだけど。
	どういう仕組みなのかさっぱりわからんので、研究してみる。
	まず、ばら撒いた処で適当にスクショ撮影。
	画面に2/3位しか弾が入らない。
	ここでチマチマ数を数えると20弾〜26弾くらい。
	画面半分程で16〜18弾くらいなので、多分全部で32弾〜36弾なのでは？と推測。
	32弾、36弾。どっちもありそうなので何度か数えてみる。よく判らないが32弾っぽい。
	今ゲームフィールドが384x448, 咲夜さんの座標が291x49,
	判る限りの座標を目視(画像中心座標)＋お絵かきソフトで計測する事にした。
	大弾なので、お絵かきソフトで内接円になるように四角を描く。そのあと対角線に線を描いて中心座標を割り出す。
	----------------
	番号,画像中心座標,	相対座標
	 1弾目が 94x 87,	-(291- 94)x 87-49,	==(-197,+38)
	 2弾目が171x 69,	-(291-171)x 69-49,	==(-120,+20)
	 3弾目が192x 65,	-(291-192)x 65-49,	==(- 99,+16)
	 4弾目が151x119,	-(291-151)x119-49,	==(-140,+70)
	 5弾目が181x116,	-(291-181)x116-49,	==(-110,+67)
	 6弾目が221x 80,	-(291-221)x 80-49,	==(- 70,+31)
	 7弾目が175x139,	-(291-175)x139-49,	==(-116,+90)
	 8弾目が159x153,	-(291-159)x153-49,	==(-132,+104)
	 9弾目が210x152,	-(291-210)x152-49,	==(- 81,+103)
	10弾目が258x119,	-(291-258)x119-49,	==(- 33,+70)
	11弾目が255x152,	-(291-255)x152-49,	==(- 36,+103)
	12弾目が215x178,	-(291-215)x178-49,	==(- 76,+129)
	13弾目が254x170,	-(291-254)x170-49,	==(- 37,+121)
	14弾目が206x236,	-(291-206)x236-49,	==(- 85,+187)
	15弾目が233x273,	-(291-233)x273-49,	==(- 58,+224)
	16弾目が280x255,	-(291-280)x255-49,	==(- 11,+206)
	------------------この辺が半分っぽい。
	ここまで測定。
	相対座標から角度を割り出してみる。よくわかんないけど、
	tan(197/38)で角度が出るのかな？
//							(x/y)									tan(x/y)	たぶん角度
	 1弾目が(-197,+38)		5.18421052631578947368421052631579		0.0907292755019070511126536298905576
	 2弾目が(-120,+20)		6										0.105104235265676462511502380139882
	 3弾目が(- 99,+16)		6.1875									0.108414028737611378659694904675177
	 4弾目が(-140,+70)		2										0.0349207694917477305004026257737253
	 5弾目が(-110,+67)		1.64179104477611940298507462686567		0.0286625046177796138852122880504812
	 6弾目が(- 70,+31)		2.25806451612903225806451612903226		0.0394310774284007674631847355929523
	 7弾目が(-116,+90)		1.28888888888888888888888888888889		0.0224991500955683949518891752039385
	 8弾目が(-132,+104) 	1.26923076923076923076923076923077		0.0221558801384605992607822428428949
	 9弾目が(- 81,+103) 	0.786407766990291262135922330097087 	0.0137262667569302851091532667488299
	10弾目が(- 33,+70)		0.471428571428571428571428571428571 	0.00822816644161695626063183252739627
	11弾目が(- 36,+103) 	0.349514563106796116504854368932039 	0.00610025557803738263689086699115253
	12弾目が(- 76,+129) 	0.589147286821705426356589147286822 	0.010282922344805178445884641481058
	13弾目が(- 37,+121) 	0.305785123966942148760330578512397 	0.00533700788847012290939546575203758
	14弾目が(- 85,+187) 	0.454545454545454545454545454545455 	0.00793348122027169612990511645481441
	15弾目が(- 58,+224) 	0.258928571428571428571428571428571 	0.00451918686372985895525414665154542
	16弾目が(- 11,+206) 	0.0533980582524271844660194174757282	0.000931972200504819213720633933431873
//
	Windowsの電卓を使ってみる。
	Windowsの電卓でtan(1)は0.0174550649282175857651288952197278だそうな。
	tan(1)は(45度)の筈だから、ラジアン(１周が2πの単位系)として
	(2*pai)/360==tan(1)？？？。よくわからんな。
	どういうtanなのかよくわからない。
	tan(99)==-6.31375151467504309897946424476819()
	tan(90)==エラー無効な値です。
	tan(89)==57.2899616307594246872781475371126
	tan(88)==28.6362532829156035507565093209464
	tan(55)==1.4281480067421145021606184849985
//
わかった。arc_tan()じゃないと駄目だ。	Windowsの電卓は、tan(360度の角度)だわ。
	----------------
とにかく円状に広がらないので、BULLET_REGIST_01_speed_offsetに差がかなりあるのかも？
*/
/*---------------------------------------------------------
	幻幽「ジャック・ザ・ルドビレ」になるかもしれないテスト
	-------------------------------------------------------
---------------------------------------------------------*/



/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
	赤大弾用
---------------------------------------------------------*/

local OBJ_CALL_FUNC(sakuya_danmaku_01_callback)
{
	#if (0)/*(r35u1バグ。 原因はコンティニューで道中読みこみモードを強制onにしていた為。
	コンティニュー後の場合、道中処理が起動していた。)*/
	if (0 >= REG_10_BOSS_SPELL_TIMER)
	{
		return;
	}
	#endif
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
	//	/*[(24-12)x8]*/if ((32-8)==(REG_09_REG1))	// 192== (64*3)
	if ((HATUDAN_ITI_NO_JIKAN-((24-18)*8)) == src->jyumyou)/* 発弾エフェクト後から[??]カウント経過した弾 */
	{
		/* 青大弾を、全てストップ */
		/* 青大弾を、赤大弾へ色変更。 */
		src->hatudan_register_tra65536				= t256(0);		/* 調整加速弾 */
		src->hatudan_register_speed65536			= t256(0);		/* 弾速 */
		src->obj_type_set							= (BULLET_OODAMA32_01_AKA); 	/* [赤大弾](赤==動脈) */
		reflect_sprite_spec(src, OBJ_BANK_SIZE_00_TAMA);	/* 弾グラと弾あたり判定を変更する。 */
//	}
//	else
//	if ((32-8)==(REG_08_REG0))	// 192== (64*3)
//	{
	//	仮の発弾位置を設定。	/* 弾源x256 y256 定位置01へ向けて発弾。 */
		REG_00_SRC_X	= t256(0);					/* 弾源x256 */
		REG_01_SRC_Y	= t256(256);				/* 弾源y256 */
		goto exchange_damnaku_sakuya_tuika;/*ナイフ追加*/
	}
	else
	//	/*[(25-12)x8]*/if ((32-7)==(REG_09_REG1))		// 192== (64*3)
	if ((HATUDAN_ITI_NO_JIKAN-((25-18)*8)) == src->jyumyou)/* 発弾エフェクト後から[??]カウント経過した弾 */
	{
	//	仮の発弾位置を設定。	/* 弾源x256 y256 定位置02へ向けて発弾。 */
		REG_00_SRC_X	= t256(GAME_WIDTH); 		/* 弾源x256 */
		REG_01_SRC_Y	= t256(256);				/* 弾源y256 */
		goto exchange_damnaku_sakuya_tuika;/*ナイフ追加*/
	}
	else
	//	/*[(26-12)x8]*/if ((32-6)==(REG_09_REG1))		// 192== (64*3)
	if ((HATUDAN_ITI_NO_JIKAN-((26-18)*8)) == src->jyumyou)/* 発弾エフェクト後から[??]カウント経過した弾 */
	{
	//	仮の発弾位置を設定。		/* 弾源x256 y256 プレイヤー中心へ向けて発弾。 */
		REG_00_SRC_X	= zzz_player->center.x256;		/* 弾源x256 */
		REG_01_SRC_Y	= zzz_player->center.y256;		/* 弾源y256 */
		goto exchange_damnaku_sakuya_tuika;/*ナイフ追加*/
	}
	else
	/*(ナイフの発弾エフェクトの分と大玉の発弾エフェクトの分で２回分ずらす。)*/
	//	if ((32-5)==(REG_09_REG1))		// 192== (64*3)
	//	/*[(29-12)x8]*/if ((32-3)==(REG_09_REG1))		// 192== (64*3)
	if ((HATUDAN_ITI_NO_JIKAN-((29-18)*8)) == src->jyumyou)/* 発弾エフェクト後から[??]カウント経過した弾 */
	{/* 29 */
		/*(再加速)*/
	//	int jj = 0;
	//	jj++;
	//	jj &= 0x07; 	// 2 3 4 5	6 7 8 9
	//	jj &= 0x03; 	// 3 4 5 6
	//	src->hatudan_register_tra65536	= t256(6);			/* 調整加速弾 */
	//	src->hatudan_register_tra65536	= (jj<<8)+t256(2);	/* (2 ... 9) 調整加速弾 */
	//	src->hatudan_register_tra65536	= (jj<<8)+t256(3);	/* (3 ... 6) 調整加速弾 */
		src->hatudan_register_tra65536	= t256(3);			/* (3 ... 6) 調整加速弾 */
		/* (1は遅すぎたので除外) 9 だと速過ぎるかも */
		/* (通常弾へ変身する) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	//	goto my_quit;
	}
	goto my_quit;
	/*---------------------------------------------------------
		ナイフ追加サブルーチン処理。(カードスクリプト化した場合、サブルーチン処理は出来無いので吸収する必要あり)
	---------------------------------------------------------*/
exchange_damnaku_sakuya_tuika:
	{
		/* 弾の現在座標にナイフ弾を登録する。 */
		REG_02_DEST_X	= ((src->center.x256)); /* 弾の現在座標 */
		REG_03_DEST_Y	= ((src->center.y256)); /* 弾の現在座標 */
		if (
			( (16*256) > abs(REG_02_DEST_X-REG_00_SRC_X) ) &&
			( (16*256) > abs(REG_03_DEST_Y-REG_01_SRC_Y) )
		)/*(スクリプトからあたり判定を取る機能がありそうだな。)*/
		{
			;/*(発弾位置が近すぎる場合、追加しない)*/
		}
		else
		{
			HATSUDAN_01_speed256		= (t256(0));			/* 弾速 */
			HATSUDAN_02_speed_offset	= t256(0);/*(テスト)*/
			HATSUDAN_04_tama_spec		= (DANMAKU_LAYER_02)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type = BULLET_KNIFE20_04_AOI;			/* [青ナイフ弾] */
		//	発弾位置を受け取る。
		//	発弾位置を設定する。
			{
				tmp_angleCCW65536_src_nerai();/* src狙い角作成 */
				hatudan_system_regist_single();/* (r33-) */
			}
			voice_play(VOICE16_BOSS_KYUPIN, TRACK04_TEKIDAN);/*(ナイフ追加音)*/
		}
	}
my_quit:
	;
	#if (0)
	/* 発弾後 512 カウント以上は弾を消す。 */
	if ((HATUDAN_ITI_NO_JIKAN-512) > src->jyumyou/*(寿命なので自動で減る)*/)/* 発弾エフェクト後から[512]カウント以上経過した弾 */
	{
		/* (通常弾へ変身する) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	}
	#endif
//	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
}


/*---------------------------------------------------------
	[弾幕グループ(2)セクション]
	-------------------------------------------------------
	追加したナイフ弾用
---------------------------------------------------------*/

local OBJ_CALL_FUNC(sakuya_danmaku_02_callback)
{
	/*(無駄に8回やるがとりあえずの対応)*/
	if (29==REG_09_REG1)
	{
		/*(止めてある青ナイフが、緑ナイフになり動き出す。)*/
	//	src->hatudan_register_tra65536	= (jj<<8)+t256(3);	/* (3 ... 6) 調整加速弾 */
		src->hatudan_register_tra65536	= t256(2);			/* (3 ... 6) 調整加速弾 */
		//
		src->obj_type_set						= BULLET_KNIFE20_07_MIDORI;/* (青→緑ナイフに変身) */
		reflect_sprite_spec(src, OBJ_BANK_SIZE_00_TAMA);	/* 弾グラと弾あたり判定を変更する。 */
		//
		/* (通常弾へ変身する) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	}
	#if (0)
	/* 発弾後 512 カウント以上は弾を消す。 */
	if ((HATUDAN_ITI_NO_JIKAN-512) > src->jyumyou/*(寿命なので自動で減る)*/)/* 発弾エフェクト後から[512]カウント以上経過した弾 */
	{
		/* (通常弾へ変身する) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	}
	#endif
//	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
}
/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_14_sakuya_jack_oodama32)
{
	REG_0a_REG2 	= const_init_nan_ido_table [tama_const_H08_NUMS_SAKUYA_JACK 	+(REG_0f_GAME_DIFFICULTY)];
	card.danmaku_callback[1] = sakuya_danmaku_01_callback;/*(赤大弾用)*/
	card.danmaku_callback[2] = sakuya_danmaku_02_callback;/*(ナイフ弾用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}
/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。(8回に1回発弾する)
	REG_09_REG1 	カウンタ。(32回でループ==(SPELL_TIME_0256==256/8))
	REG_0a_REG2 	難易度別定数1
	REG_0b_REG3 	カウンタ。(ii)残り回数。
	REG_0c_REG4 	カウンタ。(kk)
	REG_0d_REG5 	乱数角度、一時保存用。(ra_nd32)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_14_sakuya_jack_oodama32)
{
	count_up_limit_NUM(REG_NUM_08_REG0, 8);//	カウンタ。(8回に1回発弾する)
	if (1==(REG_08_REG0))	/* 1回目( 0 に初期化されて、カウントアップして 1 になるので。) */
	{
		count_up_limit_NUM(REG_NUM_09_REG1, 32);//	カウンタ。(32回でループ)
		/* (r33)発弾エフェクトが追加されたので、追加時間を早めて、辻褄合わせ。 */
	//	if (((12)+(HATUDAN_FRAME64/8))==(REG_08_REG0))	// 192== (64*3)
		/*[12x8]*/if (((32-20))==(REG_09_REG1)) 		// 192== (64*3)
		{
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
			HATSUDAN_05_bullet_obj_type 	= BULLET_OODAMA32_00_AOI;			/* [青大弾](青==静脈) */
		//
			REG_0c_REG4 = (0);
		//	mm = 0; /* カウンタ */
		//	jj = (1024-960);
			REG_0d_REG5 = ra_nd();/*(乱数角)*/
			REG_0b_REG3 = REG_0a_REG2;/*(難易度別に残り回数を設定。)*/	/* 32弾ばら撒く */
		loop:
			{
			/* 960==30720/32,  30720 == 32768 -1024 -1024 */
			/* 936 = 936.228571428571428571428571428571 == (32768/(32+2左右の余裕分))	*/
			//	jj += (960);/* (=936)< 1024 [180度(32768)より若干狭い範囲にばら撒く] */
			//	REG_0c_REG4 += 386;
				REG_0c_REG4 += REG_0d_REG5;/*(乱数角加算)*/
			//	REG_0c_REG4 += 42857142;
			//	mm += 42857142;
			//	HATSUDAN_02_speed_offset		= t256(0);/*(テスト)*/
			//	HATSUDAN_02_speed_offset		= t256(8);/*(テスト)*/
			//	HATSUDAN_02_speed_offset		= t256(12);/*(テスト)*/
				HATSUDAN_02_speed_offset		= (t256(4)+((REG_0c_REG4&0x07)<<8));/*(テスト)*/
			//	HATSUDAN_01_speed256			= ((REG_0c_REG4)&(512-1))+t256(0.25);	/*奇数は高速弾*/
				HATSUDAN_01_speed256			= ((REG_0c_REG4)&(1024-1))+t256(0.25);	/*奇数は高速弾*/
				HATSUDAN_03_angle65536			= ((REG_0c_REG4)&(32768-1))+(int)(65536*3/4);	/* 角度 */
			//	HATSUDAN_03_angle65536			= ((mm)&(1024-1))+jj+(int)(65536*3/4);			/* 角度 */
				hatudan_system_regist_single();/* (r33-) */
			}
			REG_0b_REG3--;/*(残り回数減らす。)*/
			if (0 < REG_0b_REG3)/*(残り回数があればループ。)*/
		//	if (0 != REG_0b_REG3)/*(残り回数があればループ。)*/
			{
				goto loop;
			}
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
			#endif
		}
	}
}












/*-------------------------------------------------------
	s->center.x256	ナイフのx座標
	data->center.x256 ノードのx座標
	s->w/2		ナイフの横幅の半分
	ナイフのx座標 == ノードのx座標 + co_s1024((data->angle1024)) x length - ナイフの横幅の半分
	#define ryoute_knife_length256 t256(128)
	�兮ngle ≒ 任意/(2π×ryoute_knife_length/√2)
---------------------------------------------------------*/

//#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	咲夜 両手ナイフカード
	-------------------------------------------------------
	(r35)ナイフの絶対座標位置を一度で求める考え方から、
	相対座標(ナイフの移動ベクトル量、現在位置からの差分)を求める考え方に移行し、
	プログラムをシンプルに修正。
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。
	REG_09_REG1 	発弾用カウンタ。(左右で共用)
//	REG_0a_REG2 	aaa
//	REG_0b_REG3 	bbb

	-------------------------------------------------------
	REG_0e_REG6 	[発弾連絡レジスタ]
		0:	なし。
		1:	extern OBJ_CALL_FUNC(bullet_create_sakuya_ ryoute_knife); 咲夜 両手ナイフカード
		2:	extern OBJ_CALL_FUNC(bullet_create_sakuya_ kurukuru);	  咲夜 くるくるナイフカード
	-------------------------------------------------------
	ボス共通規格使用データー:
---------------------------------------------------------*/

/*---------------------------------------------------------
	両手ナイフ親(紫)
	-------------------------------------------------------
	length	teki-obj_player間の距離/√2
	r_or_l	0==右	1==左
	-------------------------------------------------------
	ToDo:
	弾システムに移行する為、廃止予定。
	カードスクリプトシステムが稼動する為には、発弾を一ヶ所に統合しないと
	速度的な面でこのままの形態では無理があるので、
	(ここの発弾処理のせいで他の弾が処理落ちする)
	ここの発弾処理を hatudan_system_regist_single に置きかえる必要がある。
---------------------------------------------------------*/


// バグ対策。 user_data02 良くワカンナイけど空けとく。()
	/*-r34旧仕様メモ*/ /* r(-1)==+128, l(+1)==-128 */	/* -1==l, +1==r */
	/*(==TAMA_SPEC_AKA_AO_KUNAI_BIT; tama_systemの HATSUDAN_04_tama_spec に移行予定)*/
	#define RYOUTE_KNIFE_DATA_l_or_r		user_data03


#define KNIFE_TYPE_RIGHT (0)

/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
---------------------------------------------------------*/

static OBJ_CALL_FUNC(sakuya_ryoute_knife_danmaku_01_callback)
{
	/*(ナイフが進行する時に曲がる角度[量])*/
	{
		int aaa;
		aaa = (1);
	//	if (KNIFE_TYPE_RIGHT!=src->RYOUTE_KNIFE_DATA_l_or_r)
		if (KNIFE_TYPE_RIGHT==src->RYOUTE_KNIFE_DATA_l_or_r)
		{
			aaa = -aaa;
		}
		/* 描画用角度(下が0度で左回り(反時計回り)) */
		src->rotationCCW1024 += (aaa);			/*fps_factor*/	/* R(0)== +2, L(1)== -2 */		/*(r35-)*/
	}
//不要	mask1024(src->rotationCCW1024);//(sincosの場合不要)
	mask1024(src->rotationCCW1024);/*必ず要る*/
	/*(ナイフの相対移動量。ベクトル。)*/
	#if (0)//
//	src->center.x256		+= ((si n1024((src->rotationCCW1024)))<<1);/*fps_factor*/	/* CCWの場合 */
//	src->center.y256		+= ((co s1024((src->rotationCCW1024)))<<1);/*fps_factor*/
	{
		int sin_value_t256; 	//	sin_value_t256 = 0;
		int cos_value_t256; 	//	cos_value_t256 = 0;
		int256_si ncos1024( (src->rotationCCW1024), &sin_value_t256, &cos_value_t256);
		/*(ナイフの進行移動量)*/
	//	src->center.x256 += ((sin_value_t256)<<1);/*fps_factor*/
	//	src->center.y256 += ((cos_value_t256)<<1);/*fps_factor*/
		src->center.x256 += ((sin_value_t256) );/*fps_factor*/
		src->center.y256 += ((cos_value_t256) );/*fps_factor*/
	}
	#else
	//------------------
	HATSUDAN_01_speed256	= t256(1.00);
	HATSUDAN_03_angle65536	= deg1024to65536((src->rotationCCW1024));
	sincos256();/*(破壊レジスタ多いので注意)*/
	src->center.x256 += (REG_03_DEST_Y);/*fps_factor*/
	src->center.y256 += (REG_02_DEST_X);/*fps_factor*/
	//------------------
	#endif
	/*(子ナイフの発弾間隔)*/
	/*(このカウンタは左右で共用なので2倍進む)*/
	REG_09_REG1++;
	if (((10)+(10)) < REG_09_REG1)
	{
		REG_09_REG1 = (0);
		/* 親ナイフ(紫)が、1回分の子ナイフ弾(青)を4弾、撃つ。 */
		/*(子ナイフ弾を4弾、撃つ)*/
		set_REG_DEST_XY(src);	/* 子ナイフ弾は親ナイフ(紫)弾の位置から発弾。 */
		//
		HATSUDAN_02_speed_offset		= t256(0);/*(テスト)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI;	/* [青ナイフ弾] */
		{
			int kk_speed256;
			kk_speed256 = (t256(1.6));
			unsigned int jj;
			for (jj=0; jj<4; jj++)/*(1回で4弾、撃つ)*/
			{
				kk_speed256 += (t256(0.2));
				HATSUDAN_01_speed256		= (kk_speed256);
				const /*int*/ /*u16*/s32/*(s16で可能)*/ jj_bbb[4] =
				{
					(int)((65536*6/12)-(65536*6/12)),	//	kk_speed256 = (t256(1.8));
					(int)((65536*6/12)-(65536*5/12)),	//	kk_speed256 = (t256(2.0));
					(int)((65536*6/12)-(65536*4/12)),	//	kk_speed256 = (t256(2.2));
					(int)((65536*6/12)-(65536*3/12)),	//	kk_speed256 = (t256(2.4));
				};
				#if (1)/*r35-*/
				{	int aaa 	= (((int)(jj_bbb[jj]) ));
					/*(差分は本来負数で持つべきだが、とりあえず正数で持っている。そこで左右の判断を逆にする)*/
					if (KNIFE_TYPE_RIGHT!=src->RYOUTE_KNIFE_DATA_l_or_r)	{	aaa = -aaa; 	}
				//	HATSUDAN_03_angle65536 = (aaa) + (deg1024to65536(src->rotationCCW1024)); /* 「1周が1024分割」から「1周が65536分割」へ変換する。 */
					HATSUDAN_03_angle65536 = (aaa) + (deg1024to65536(src->rotationCCW1024)); /* 「1周が1024分割」から「1周が65536分割」へ変換する。 */
				}
				#endif
				hatudan_system_regist_single();
			}
		}
	}
}


#if (1)
//#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	咲夜のくるくるナイフ弾(青→緑)
	咲夜
	プレイヤーの周りをくるくる回ってから、自分狙いのナイフ。
	青→緑に色が変わる。
	-------------------------------------------------------
	全方向ナイフ(青)
	狙いナイフ(緑)
	-------------------------------------------------------
	プレイヤーの周りに散らばり、時間が経つとプレイヤー狙い弾
	-------------------------------------------------------
	弾を全弾書き換え方式に移行するのでここは無くなる予定。
	-------------------------------------------------------
	REG_0e_REG6 	[発弾連絡レジスタ]
		0:	なし。
		1:	extern OBJ_CALL_FUNC(bullet_create_sakuya_ryoute_knife); 咲夜 両手ナイフカード
		2:	extern OBJ_CALL_FUNC(bullet_create_sakuya_kurukuru);	 咲夜 くるくるナイフカード
	-------------------------------------------------------
	-------------------------------------------------------
	ボス共通規格使用データー:
---------------------------------------------------------*/

#if 0
// バグ対策。 user_data02 良くワカンナイけど空けとく。()
#define sakuya_aaa_HATUDAN_TIME256	(256)
local OBJ_CALL_FUNC(bullet_move_kurukuru_knife)
{
	if (sakuya_aaa_HATUDAN_TIME256 > src->jyumyou)
	{
		;
	}
	else
	if (sakuya_aaa_HATUDAN_TIME256 == src->jyumyou)
	{
		/* 変身する。 */
		{
			src->obj_type_set			= BULLET_KNIFE20_07_MIDORI;/* (青→緑ナイフに変身) */
			reflect_sprite_spec(src, OBJ_BANK_SIZE_00_TAMA);	/* 弾グラと弾あたり判定を変更する。 */
			#if 1/* わざと狙わない */
			u32 ra_nd32 = ra_nd();
			OBJ *zzz_player;
			zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
		//	set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
			#endif
			REG_00_SRC_X	= (zzz_player->center.x256)+((ra_nd32<<8)&0x1fff)-t256(16);
			REG_01_SRC_Y	= (zzz_player->center.y256)+((ra_nd32  )&0x0fff)-t256(8);
			REG_02_DEST_X	= ((src->center.x256));
			REG_03_DEST_Y	= ((src->center.y256));
			tmp_angleCCW65536_src_nerai();
			src->tmp_angleCCW1024 = (deg65536to1024(HATSUDAN_03_angle65536));		/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
			mask1024(src->tmp_angleCCW1024);/* ねんのため */
			src->rotationCCW1024 = ((src->tmp_angleCCW1024));	/* 表示角度 */
		}
		//------------------
		HATSUDAN_01_speed256	= (t256(2));
		HATSUDAN_03_angle65536	= deg1024to65536((src->tmp_angleCCW1024));
		sincos256();/*(破壊レジスタ多いので注意)*/
		src->math_vector.x256 = REG_03_DEST_Y;//sin_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
		src->math_vector.y256 = REG_02_DEST_X;//cos_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
		//------------------
	}
	else
	{
		/* くるくるさせる */
		src->rotationCCW1024 += (64);
		mask1024(src->rotationCCW1024);/* ねんのため */
	}
	src->center.x256 += (src->math_vector.x256);	/*fps_factor*/
	src->center.y256 += (src->math_vector.y256);	/*fps_factor*/
}
#endif
local OBJ_CALL_FUNC(sakuya_kurukuru_knife_danmaku_02_callback)
{
	/* 0-64 カウントまで */ 	/* [0]カウント==発弾位置 */
	if ((HATUDAN_ITI_NO_JIKAN-64) < src->jyumyou)/* 発弾エフェクト後から[0-31]カウント経過した弾 */
	{
		/* 発弾後、くるくるさせる */
		src->rotationCCW1024 += (64);
		mask1024(src->rotationCCW1024);/* ねんのため */
	}
	else/* 発弾エフェクト後から[65]カウント経過した弾 */
	{
		/* 65 カウントなら、自機狙い弾に変身する。 */
		src->obj_type_set			= BULLET_KNIFE20_07_MIDORI;/* (青→緑ナイフに変身) */
		reflect_sprite_spec(src, OBJ_BANK_SIZE_00_TAMA);	/* 弾グラと弾あたり判定を変更する。 */
		#if 1/* わざと狙わない */
		u32 ra_nd32 = ra_nd();
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
	//	set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
		#endif
		REG_00_SRC_X	= (zzz_player->center.x256)+((ra_nd32<<8)&0x1fff)-t256(16);
		REG_01_SRC_Y	= (zzz_player->center.y256)+((ra_nd32  )&0x0fff)-t256(8);
		REG_02_DEST_X	= ((src->center.x256));
		REG_03_DEST_Y	= ((src->center.y256));
		tmp_angleCCW65536_src_nerai();
		src->tmp_angleCCW1024 = (deg65536to1024(HATSUDAN_03_angle65536));		/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
	//	mask1024(src->tmp_angleCCW1024);/* ねんのため */
		src->rotationCCW1024 = ((src->tmp_angleCCW1024));	/* 表示角度 */
		//------------------
		src->hatudan_register_speed65536	= (t256(2)<<8);
		//------------------
		/* (通常弾へ変身する) */
		src->hatudan_register_spec_data 	= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
	}
}
/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
#if (0)/*(弾幕システムに移行する場合)*/
local OBJ_CALL_FUNC(bullet_init_sakuya_ryoute_knife)
{
	card.danmaku_callback[1] = sakuya_ryoute_knife_danmaku_01_callback;/*(親ナイフ。)*/
//	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}
#endif
local OBJ_CALL_FUNC(spell_init_47_sakuya_festival_knife)
{
	//
	card.danmaku_callback[2] = sakuya_kurukuru_knife_danmaku_02_callback;/*( )*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}


/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(bullet_create_sakuya_ryoute_knife)
{
	REG_09_REG1 	= (0);/*(左右共用発弾カウンタ)*/
	//
	/* 自機狙い角を HATSUDAN_03_angle65536 に作成 */
//	REG_02_DEST_X	= (src->center.x256);/*(咲夜の現在位置座標)*/
//	REG_03_DEST_Y	= (src->center.y256);/*(咲夜の現在位置座標)*/
	set_REG_DEST_XY(src);	/* ナイフ(紫)弾は咲夜の位置から発弾。 */
	calculate_jikinerai();
	/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
	HATSUDAN_03_angle1024 = (deg65536to1024(HATSUDAN_03_angle65536));
	//
	#if (0)
	/*(弾幕システムに移行する場合)*/
//	set_REG_DEST_XY(src);	/* ナイフ(紫)弾は咲夜の位置から発弾。 */	/*(既に自機狙いの時に設定済み)*/
	HATSUDAN_01_speed256			= (0);				/* 弾速(0) */
	HATSUDAN_02_speed_offset		= t256(0);/*(てすと)*/
	HATSUDAN_05_bullet_obj_type 	= (BULLET_KNIFE20_06_YUKARI);	/* ナイフ親(紫) */
	#endif
	int r_or_l;
	for (r_or_l=0; r_or_l<2; r_or_l++)/*r35-*/
	{
		#if (1)
		OBJ *h;
		h								= obj_regist_tama();/*全方向ナイフ(青)*/
		if (NULL != h)
		#else
		/*(弾幕システムに移行する場合)*/
		#endif
		{
			#if (1)
			h->center.x256				= (src->center.x256);	/* ナイフ(紫)弾は咲夜の位置から発弾。 */
			h->center.y256				= (src->center.y256);	/* ナイフ(紫)弾は咲夜の位置から発弾。 */
		//	h->obj_type_set 			= BULLET_KNIFE20_04_AOI;/* ナイフ親(青) */
			h->obj_type_set 			= BULLET_KNIFE20_06_YUKARI;/* ナイフ親(紫) */
			reflect_sprite_spec(h, OBJ_BANK_SIZE_00_TAMA);	/* 弾グラと弾あたり判定を変更する。 */
			h->callback_mover			= sakuya_ryoute_knife_danmaku_01_callback;
			h->m_Hit256R				= TAMA_ATARI_KNIFE18_PNG;
			#else
			/*(弾幕システムに移行する場合)*/
			#endif
		//
			#if (1)/*(特別処理)*/
			/*(ナイフの左右別フラグを決定)*/
			h->RYOUTE_KNIFE_DATA_l_or_r 		= r_or_l;
			/*(ナイフの進行向きを決定)*/
			{
				int tmp1024;
				tmp1024 = (128);
				if (KNIFE_TYPE_RIGHT!=r_or_l)	{		tmp1024 = -tmp1024; 	}
				tmp1024 += (HATSUDAN_03_angle1024); 	/* 「1周が1024分割」 */
			//	mask1024(tmp1024);
				#if (1)
				h->rotationCCW1024	= (tmp1024);	/*(ナイフの進行向きは表示向きと同じなので、表示向きで保持する)*/
				#else
				/*(弾幕システムに移行する場合)*/
				HATSUDAN_03_angle65536			= (tmp1024);
				#endif
			}
			/*(ナイフの発弾座標を決定)*/
			#if (0)//
		//	h->center.x256	+= ((si n1024((h->rotationCCW1024)))<<7);/*fps_factor*/ /* CCWの場合 */
		//	h->center.y256	+= ((co s1024((h->rotationCCW1024)))<<7);/*fps_factor*/
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_si ncos1024( (h->rotationCCW1024), &sin_value_t256, &cos_value_t256);
		//		h->center.x256		+= ((sin_value_t256)<<7);/*fps_factor*/
		//		h->center.y256		+= ((cos_value_t256)<<7);/*fps_factor*/
				h->center.x256		+= ((sin_value_t256)<<3);/*fps_factor*/
				h->center.y256		+= ((cos_value_t256)<<3);/*fps_factor*/
			}
			#else
			//------------------
			HATSUDAN_01_speed256	= t256(8.00);
			HATSUDAN_03_angle65536	= deg1024to65536((h->rotationCCW1024));
			sincos256();/*(破壊レジスタ多いので注意)*/
			h->center.x256 += (REG_03_DEST_Y);/*fps_factor*/
			h->center.y256 += (REG_02_DEST_X);/*fps_factor*/
			//------------------
			#endif
			/*(ナイフの寿命を決定)*/
			h->jyumyou						= (128+(REG_0f_GAME_DIFFICULTY<<6));/*(r33)*/	/*意味無い(?)*/
			#endif
		}
		#if (0)
		/*(弾幕システムに移行する場合)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(r_or_l<<8)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		hatudan_system_regist_single();
		#endif
	}
}


/*
	-------------------------------------------------------
	ToDo:
	弾システムに移行する為、廃止予定。
	カードスクリプトシステムが稼動する為には、発弾を一ヶ所に統合しないと
	速度的な面でこのままの形態では無理があるので、
	(ここの発弾処理のせいで他の弾が処理落ちする)
	ここの発弾処理を hatudan_system_regist_single に置きかえる必要がある。
	-------------------------------------------------------
	REG_0e_REG6 	[発弾連絡レジスタ]
		0:	なし。
		1:	extern OBJ_CALL_FUNC(bullet_create_sakuya_ryoute_knife); 咲夜 両手ナイフカード
		2:	extern OBJ_CALL_FUNC(bullet_create_sakuya_kurukuru);	 咲夜 くるくるナイフカード
	-------------------------------------------------------
---------------------------------------------------------*/

static OBJ_CALL_FUNC(bullet_create_sakuya_kurukuru)
{
	OBJ *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
	/* ある程度遠くに出ないと認識しづらい */
	/* キャラ(コア)がでかいと避ける前に死ぬ。(ナイフが広がる前にあたる) */
	#if (1)
	/*(弾幕システムに移行する場合)*/
//	set_REG_DEST_XY(src);	/* ナイフ(紫)弾は咲夜の位置から発弾。 */	/*(既に自機狙いの時に設定済み)*/
//	HATSUDAN_01_speed256			= (0);				/* 弾速(0) */
	HATSUDAN_02_speed_offset		= t256(0);/*(てすと)*/
	HATSUDAN_05_bullet_obj_type 	= (BULLET_KNIFE20_04_AOI);	/* (青→緑ナイフに変身) */
	#endif
	const int tmp_step[4]	=
	{
		(65536/8),
		(65536/10),
		(65536/12),
		(65536/16),
	};
	int ii;
	for (ii=0; ii<65536; ii += tmp_step[(REG_0f_GAME_DIFFICULTY)] ) 	/* プレイヤーの周りに8弾配置する */
	{
		#if (0)
		OBJ *h;
		h					= ob j_add_A00_t ama_er ror();
		if (NULL != h)
		#else
		/*(弾幕システムに移行する場合)*/
		#endif
		{
		//	REG_0b_REG3 = REG_02_DEST_X;//[ボスx座標一時保存]
		//	REG_0c_REG4 = REG_03_DEST_Y;//[ボスy座標一時保存]
			//------------------
			HATSUDAN_01_speed256	= (t256(128));
			HATSUDAN_03_angle65536	= (ii);
			sincos256();/*(破壊レジスタ多いので注意)*/
			#if (0)
			h->center.x256 = (zzz_player->center.x256) + REG_03_DEST_Y;//sin_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
			h->center.y256 = (zzz_player->center.y256) + REG_02_DEST_X;//cos_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
			#else
			/*(弾幕システムに移行する場合)*/
			REG_03_DEST_Y += (zzz_player->center.x256);//sin_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
			REG_02_DEST_X += (zzz_player->center.y256);//cos_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
			#endif
			//------------------
		//	// 発射座標リストア。
		//	REG_02_DEST_X = REG_0b_REG3;//[ボスx座標復活]
		//	REG_03_DEST_Y = REG_0c_REG4;//[ボスy座標復活]
			HATSUDAN_01_speed256			= (0);				/* 弾速(0) */
			#if (0)
			h->obj_type_set 			= BULLET_KNIFE20_04_AOI;	/* (青→緑ナイフに変身) */
			reflect_sprite_spec(h, OBJ_BANK_SIZE_00_TAMA);	/* 弾グラと弾あたり判定を変更する。 */
			h->callback_mover			= bullet_move_kurukuru_knife;
			h->m_Hit256R				= TAMA_ATARI_KNIFE18_PNG;
			#else
			/*(弾幕システムに移行する場合)*/
			#endif
		//
			#if (0)
			h->jyumyou			= (sakuya_aaa_HATUDAN_TIME256+64);
			h->math_vector.x256 		= (0);
			h->math_vector.y256 		= (0);
			#else
			/*(弾幕システムに移行する場合)*/
			#endif
		//
			#if (1)
			/*(弾幕システムに移行する場合)*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_02)|(0)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			hatudan_system_regist_single();
			#endif
		}
	}
}

#endif
/*---------------------------------------------------------
	[咲夜] 幻想「フェスティバルナイフ」(?) むりやり変換 (まだ変換中)
	-------------------------------------------------------
	REG_0e_REG6 	[発弾連絡レジスタ]
		0:	なし。
		1:	extern OBJ_CALL_FUNC(bullet_create_sakuya_ryoute_knife); 咲夜 両手ナイフカード
		2:	extern OBJ_CALL_FUNC(bullet_create_sakuya_kurukuru);	 咲夜 くるくるナイフカード
	-------------------------------------------------------
---------------------------------------------------------*/
//tern OBJ_CALL_FUNC(bullet_create_sakuya_ryoute_knife); /* 咲夜 両手ナイフカード */
//tern OBJ_CALL_FUNC(bullet_create_sakuya_kurukuru);	/* 咲夜 くるくるナイフカード */
local OBJ_CALL_FUNC(spell_create_47_sakuya_festival_knife)
{
	/*[発弾連絡処理]*/
	if ((0)!=REG_0e_REG6)/*[発弾指示があれば]*/
	{
		if ((1)==REG_0e_REG6)/*[咲夜 両手ナイフカード]*/
		{
			bullet_create_sakuya_ryoute_knife(src);
		}
		if ((2)==REG_0e_REG6)/*[咲夜 くるくるナイフカード]*/
		{
			bullet_create_sakuya_kurukuru(src);
		}
		REG_0e_REG6=(0);/*[おわったよ]*/
	}
};

