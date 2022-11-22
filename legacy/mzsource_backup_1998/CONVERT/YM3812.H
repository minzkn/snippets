/* YM3812.H */

#ifndef _YM3812_H_
#define _YM3812_H_

#ifndef _YM3812_C_

extern unsigned char   YM_instrument[][12];
extern unsigned int    YM_frequency[12];

extern void far pascal YM_register(unsigned int, unsigned int);
extern void far pascal YM_enablewave(void);
extern void far pascal YM_disablewave(void);
extern void far pascal YM_method1(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern void far pascal YM_method2(unsigned int, unsigned int, unsigned int);
extern void far pascal YM_method3(unsigned int, unsigned int, unsigned int);
extern void far pascal YM_method4(unsigned int, unsigned int, unsigned int);
extern void far pascal YM_method5(unsigned int, unsigned int, unsigned int);
extern void far pascal YM_method6(unsigned int, unsigned int, unsigned int);
extern void far pascal YM_selectwave(unsigned int, unsigned int);
extern void far pascal YM_frequency_on(unsigned int, unsigned int, unsigned int);
extern void far pascal YM_frequency_off(unsigned int, unsigned int, unsigned int);
extern unsigned int far pascal YM_iscard(void);

#endif _YM3812_C_

#endif _YM3812_H_

/* End of source */
