/*
  JACK's MENU.H
*/

extern signed char s_popmouseflag;

extern void far PopMain(void);
extern void far StorePal(void);
extern void far ReStorePal(void);
extern void far System(void);
extern void far BackGround(signed char n_sw, signed int n_select);
extern void far DeCompressData(signed char far *n_string);
extern signed int far RightButtonMenu(signed char far **n_string, signed int n_x =0, signed int n_y =0);
extern signed int far YesNo(signed char far *n_string, signed char n_default);
extern void far Check(signed char far *n_string);

/* End of source */