  /***************************************
   *         Copyright(c)1998-1998       *
   *       MC Compiler source (STANDARD) *
   *         Code by Cho JaeHyuk         *
   *          MINZ (02)822-2744          *
   ***************************************/

/* STANDARD.C */

#pragma inline

#define _STANDARD_C_

#include "standard.h"

extern void __emit__();

extern const unsigned char far pascal _num[16];
extern unsigned long far pascal _vector[256];
extern unsigned int pascal _heapseg;
extern unsigned int pascal _argc;
extern unsigned char far ** pascal _argv;

extern void far pascal nowork(void);
extern void interrupt nointwork(void);
extern void interrupt zerovector(void);
extern void far pascal exit(unsigned int);
extern void far pascal abort(void);
extern unsigned int far pascal putc(unsigned int);
extern void far * far pascal puts(void far *);
extern void far * far pascal memset(void far *, unsigned int, unsigned int);
extern void far * far pascal memcpy(void far *, void far *, unsigned int);
extern void far * far pascal memchr(void far *, unsigned int, unsigned int);
extern unsigned int far pascal memcmp(void far *, void far *, unsigned int);
extern void far pascal swap(void far *, void far *);
extern unsigned int far pascal strclen(void far *, unsigned int);
extern unsigned int far pascal strlen(void far *);
extern void far * far pascal strncpy(void far *, void far *, unsigned int);
extern void far * far pascal strcpy(void far *, void far *);
extern void far * far pascal strncat(void far *, void far *, unsigned int);
extern void far * far pascal strcat(void far *, void far *);
extern void far * far pascal strnchr(void far *, unsigned int, unsigned int);
extern void far * far pascal strchr(void far *, unsigned int);
extern unsigned int far pascal strcmp(void far *, void far *);
extern unsigned int far pascal toupper(unsigned int);
extern unsigned int far pascal tolower(unsigned int);
extern unsigned int far pascal isdigit(unsigned int);
extern unsigned int far pascal isalpha(unsigned int);
extern void far * far pascal strupr(void far *);
extern void far * far pascal strlwr(void far *);
extern unsigned int far pascal tonum(unsigned int);
extern unsigned int far pascal peekb(unsigned int, unsigned int);
extern unsigned int far pascal peekw(unsigned int, unsigned int);
extern unsigned int far pascal pokeb(unsigned int, unsigned int, unsigned int);
extern unsigned int far pascal pokew(unsigned int, unsigned int, unsigned int);
extern void far * far pascal getvect(unsigned int);
extern unsigned int far pascal setvect(unsigned int, void far *);
extern unsigned int far pascal inportb(unsigned int);
extern unsigned int far pascal inportw(unsigned int);
extern unsigned int far pascal outportb(unsigned int, unsigned int);
extern unsigned int far pascal outportw(unsigned int, unsigned int);
extern unsigned int far pascal rand(void);
extern void far pascal delay(unsigned int);
extern void far pascal sound(void);
extern void far pascal nosound(void);
extern void far pascal beep(unsigned int);
extern unsigned int far pascal inx(unsigned int, unsigned int);
extern void far pascal outx(unsigned int, unsigned int, unsigned int);
extern unsigned int far pascal kbhit(void);
extern unsigned int far pascal getch(void);
extern void far pascal coldboot(void);
extern void far pascal warmboot(void);
extern void far pascal biosreset(void);
extern void far pascal geninterrupt(unsigned int);
extern void far pascal setvideomode(unsigned int);
extern void far pascal setdelaykey(unsigned int);
extern unsigned int far pascal inx_test(unsigned int, unsigned int, unsigned int);
extern void far pascal outx_test(unsigned int, unsigned int, unsigned int, unsigned int);
extern void far pascal _putpixel12h(unsigned int, unsigned int, unsigned int);
extern void far pascal farjump(unsigned int, unsigned int);
extern void far pascal farcall(unsigned int, unsigned int);
extern unsigned int far pascal disk_reset(unsigned int);
extern unsigned int far pascal disk_read(unsigned int, unsigned int, unsigned int, unsigned int, void far *);
extern unsigned int far pascal disk_write(unsigned int, unsigned int, unsigned int, unsigned int, void far *);
extern void far pascal enable(void);
extern void far pascal disable(void);

unsigned char far *prevdir="..";
unsigned char far *wild_all="*.*";
unsigned char far *rootdir="\\";
t_heap far *HEAP_head;
t_heap far *HEAP_tail;
unsigned int minx=0U, miny=0U, maxx=640U, maxy=480U, maxcolor=16U;
unsigned int scanning_level;

unsigned int far pascal (far pascal *chdir)(void far *);
unsigned int far pascal (far pascal *findfirst)(void far *, void far *, unsigned int);
unsigned int far pascal (far pascal *findnext)(void far *);
unsigned int far pascal (far pascal *getcurdir)(unsigned int, void far *);
unsigned int far pascal (far pascal *getdisk)(void);
unsigned int far pascal (far pascal *mkdir)(void far *);
unsigned int far pascal (far pascal *rmdir)(void far *);
unsigned int far pascal (far pascal *setdisk)(unsigned int);
void far * far pascal (far pascal *getcwd)(void far *, unsigned int);
unsigned int far pascal (far pascal *creat)(void far *, unsigned int);
unsigned int far pascal (far pascal *open)(void far *, unsigned int);
unsigned int far pascal (far pascal *close)(unsigned int);
unsigned int far pascal (far pascal *unlink)(void far *);
unsigned int far pascal (far pascal *read)(unsigned int, void far *, unsigned int);
unsigned int far pascal (far pascal *write)(unsigned int, void far *, unsigned int);
unsigned int far pascal (far pascal *lseek)(unsigned int, unsigned long, unsigned int);
unsigned int far pascal (far pascal *getfileattr)(void far *, unsigned int far *);
unsigned int far pascal (far pascal *setfileattr)(void far *, unsigned int);
unsigned int far pascal (far pascal *scanning_func)(void far *)=(void far *)nowork;

void far * far pascal farmalloc(unsigned int);
void far pascal farfree(void far *);
void far * far pascal itoa(void far *, unsigned int, unsigned int, unsigned int);
unsigned int far pascal atoi(void far *, unsigned int);
void far * far pascal vsprintf(void far *, void far *, void far *);
void far * far cdecl printf(void far *, ...);
void far * far cdecl sprintf(void far *, void far *, ...);
void far * far pascal insert(void far *, unsigned int);
void far * far pascal delete(void far *);
void far pascal eputc(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void far pascal eputs(unsigned int, unsigned int, void far *, unsigned int, unsigned int);
void far pascal gotoxy(unsigned int, unsigned int);
void far pascal fnsplit(void far *, void far *, void far *, void far *, void far *);
void far pascal fnmerge(void far *, void far *, void far *, void far *, void far *);
unsigned int far pascal scanning(void far *);

void far pascal _initmain(void);

void far * far pascal farmalloc(unsigned int s_size)
{
 t_heap far *s_node; t_heap far *s_temp; unsigned int s_locate, s_itop=s_size;
	   if(s_itop&0x000fU)s_itop=(s_itop&0xfff0U)+0x0010U;
	   s_itop>>=4;
	   if(HEAP_head==0LU)
	   {
	    s_node=(t_heap far *)MZ_MK_FP(_heapseg, 0x0000U);
	    s_node->crnt=(t_heap far *)MZ_MK_FP(_heapseg+1U, 0x0000U);
	    s_node->size=(unsigned int)(s_itop+1U);
	    HEAP_head=s_node; HEAP_head->next=HEAP_head->prev=0LU;
	    HEAP_tail=HEAP_head; return(s_node->crnt);
	   }
	   s_node=HEAP_head;
	   while(s_node!=0LU)
	   {
	    s_temp=s_node->next; s_locate=MZ_FP_SEG(s_node)+s_node->size;
	    if((MZ_FP_SEG(s_temp)-s_locate)>s_itop)break;
	    s_node=s_node->next;
	   }
	   if(s_node==0LU)
	   {
	    s_node=(t_heap far *)MZ_MK_FP(MZ_FP_SEG(HEAP_tail)+HEAP_tail->size, 0x0000U);
	    s_node->crnt=(t_heap far *)MZ_MK_FP(MZ_FP_SEG(HEAP_tail)+HEAP_tail->size+1U, 0x0000U);
	    s_node->size=s_itop+1U; HEAP_tail->next=s_node;
	    s_node->prev=HEAP_tail; s_node->next=0LU; HEAP_tail=s_node;
	   }
	   else
	   { /* Re alloc */
	    t_heap far *s_alloc;
	    s_alloc=(t_heap far *)MZ_MK_FP(s_locate, 0x0000U);
	    s_alloc->crnt=(t_heap far *)MZ_MK_FP(s_locate+1U, 0x0000U);
	    s_alloc->size=s_itop+1U;
	    s_node->next=s_alloc; s_alloc->prev=s_node;
	    s_alloc->next=s_temp; s_temp->prev=s_alloc; s_node=s_alloc;
	   }
	   return(s_node->crnt);
}

void far pascal farfree(void far *s_ptr)
{
 t_heap far *s_crnt; t_heap far *s_temp;
	   if(s_ptr==0LU)return;
	   s_crnt=HEAP_head;
	   while(s_crnt->next)
	   {
	    if(MZ_FP_SEG(s_ptr)==MZ_FP_SEG(s_crnt->crnt))break;
	    s_crnt=s_crnt->next;
	   }
	   if(s_crnt==0LU)return;
	   s_temp=s_crnt->prev; if(s_temp)s_temp->next=s_crnt->next;
	   s_temp=s_crnt->next; if(s_temp)s_temp->prev=s_crnt->prev;
}

void far * far pascal itoa(void far *s_ptr, unsigned int s_num, unsigned int s_radix, unsigned int s_sign)
{
 unsigned char far *s_sptr=s_ptr; unsigned char s_str[20]; unsigned int s_count=0U;
	   if(s_sign){if(s_num&0x8000){*(s_sptr++)='-'; s_num=(~s_num)+1U;}}
	   do{s_str[s_count++]=_num[s_num%s_radix]; s_num/=s_radix;}while(s_num);
	   while(s_count)*(s_sptr++)=s_str[--s_count];
	   *(s_sptr)='\0'; return(s_ptr);
}

unsigned int far pascal atoi(void far *s_str, unsigned int s_radix)
{
 unsigned char far *s_string=s_str; unsigned int s_ret=0U, s_len, s_num=1U;
	   if(*(s_string)=='-')s_string++;
	   s_len=strlen(s_string); if(s_len==0U)return(0U);
	   while(s_len){s_len--; s_ret+=s_num*tonum(*(s_string+s_len)); s_num*=s_radix;}
	   if(*((unsigned char far *)s_str)=='-')s_ret=(~s_ret)+1U;
	   return(s_ret);
}

void far * far pascal vsprintf(void far *s_dst, void far *s_fmt, void far *s_varg)
{
 unsigned char far *s_str=(unsigned char far *)s_dst;
 unsigned char s_numbuff[20], s_fmtchar, s_minus, s_long, s_fillchar;
 unsigned int s_fillnum, s_radix, s_sign, s_len; unsigned long s_data;
	   while(*((unsigned char far *)s_fmt))
	   {
	    s_fmtchar=*(((unsigned char far *)s_fmt)++);
	    if(s_fmtchar=='\$'||s_fmtchar=='\%')
	    {
	     s_fmtchar=*(((unsigned char far *)s_fmt)++);
	     if(s_fmtchar=='\_'||s_fmtchar=='\-'||s_fmtchar=='\+'||s_fmtchar=='\*'||s_fmtchar=='\/')
	     {
	      if(s_fmtchar=='\*')s_minus=3;
	      else if(s_fmtchar=='\_'||s_fmtchar=='\/')s_minus=2;
	      else if(s_fmtchar=='\+')s_minus=0;
	      else s_minus=1;
	      s_fmtchar=*(((unsigned char far *)s_fmt)++);
	     }
	     else s_minus=0;
	     switch(s_fmtchar)
	     {
	      case '0': case '\*': case '\#':
		   s_fillchar=s_fmtchar; s_fmtchar=*(((unsigned char far *)s_fmt)++);
		   break;
	      default: s_fillchar=' '; break;
	     }
	     if(isdigit(s_fmtchar))
	     {
	      s_fillnum=0U; s_numbuff[s_fillnum++]=s_fmtchar;
	      while(isdigit(*((unsigned char far *)s_fmt)))
	      {
	       s_numbuff[s_fillnum++]=*(((unsigned char far *)s_fmt)++);
	       if(s_fillnum>=7)break;
	      }
	      s_numbuff[s_fillnum]='\0'; s_fillnum=atoi(&s_numbuff[0], 10);
	      s_fmtchar=*(((unsigned char far *)s_fmt)++);
	     }
	     else s_fillnum=1U;
	     if(toupper(s_fmtchar)=='L'){s_long=1; s_fmtchar=*(((unsigned char far *)s_fmt)++);}
	     else s_long=0;
	     if(s_fmtchar)
	     {
	      s_fmtchar=toupper(s_fmtchar);
	      switch(s_fmtchar)
	      {
	       case 'D': case 'U': case 'I': case 'X': case 'O': case 'B':
		    if(s_long)(unsigned long)*(((unsigned long far *)s_varg)++);
		    else s_data=(unsigned long)*(((unsigned int far *)s_varg)++);
		    if(s_fmtchar=='D'||s_fmtchar=='I'){s_radix=10; s_sign=1;}
		    else if(s_fmtchar=='X'){s_radix=16; s_sign=0;}
		    else if(s_fmtchar=='O'){s_radix=8; s_sign=0;}
		    else if(s_fmtchar=='B'){s_radix=2; s_sign=0;}
		    else {s_radix=10; s_sign=0;}
		    if(s_long);
		    else itoa(&s_numbuff[0], (unsigned int)s_data, s_radix, s_sign);
		    if(s_minus==0||s_minus==2||s_minus==3)
		    {
		     if(s_fillnum>strlen(&s_numbuff[0]))
		     {
		      s_len=s_fillnum-strlen(&s_numbuff[0]);
		      if(s_minus==2)s_len>>=1;
		      while(s_len--)*(s_str++)=s_fillchar;
		     }
		    }
		    strcpy(s_str, &s_numbuff[0]); s_str+=strlen(&s_numbuff[0]);
		    if(s_minus)
		    {
		     if(s_fillnum>strlen(&s_numbuff[0]))
		     {
		      s_len=s_fillnum-strlen(&s_numbuff[0]);
		      if(s_minus==2)s_len=s_len-(s_len>>1);
		      while(s_len--)*(s_str++)=s_fillchar;
		     }
		    }
		    break;
	       case 'S': case 'A':
		    s_data=(unsigned long)*(((unsigned long far *)s_varg)++);
		    if(s_minus==0||s_minus==2||s_minus==3)
		    {
		     if(s_fillnum>strlen((void far *)s_data))
		     {
		      s_len=s_fillnum-strlen((void far *)s_data);
		      if(s_minus==2)s_len>>=1;
		      while(s_len--)*(s_str++)=s_fillchar;
		     }
		    }
		    strcpy(s_str, (void far *)s_data); s_str+=strlen((void far *)s_data);
		    if(s_minus)
		    {
		     if(s_fillnum>strlen((void far *)s_data))
		     {
		      s_len=s_fillnum-strlen((void far *)s_data);
		      if(s_minus==2)s_len=s_len-(s_len>>1);
		      while(s_len--)*(s_str++)=s_fillchar;
		     }
		    }
		    break;
	       case 'C':
		    s_data=(unsigned long)*(((unsigned int far *)s_varg)++);
		    if(s_data)while(s_fillnum--)*(s_str++)=(unsigned char)s_data;
		    break;
	       default: *(s_str++)=s_fmtchar; break;
	      }
	     }
	    }
	    else *(s_str++)=s_fmtchar;
	   }
	   *(s_str)='\0'; return(s_dst);
}

void far * far cdecl printf(void far *s_fmt, ...)
{
 void far *s_str=(void far *)farmalloc(256);
	   vsprintf(s_str, s_fmt, ...); puts(s_str); farfree(s_str); return(s_fmt);
}

void far * far cdecl sprintf(void far *s_str, void far *s_fmt, ...)
{
	   vsprintf(s_str, s_fmt, ...); return(s_str);
}

void far * far pascal insert(void far *s_ptr, unsigned int s_char)
{
 unsigned char far *s_str=(unsigned char far *)s_ptr;
 unsigned int s_len=strlen(s_ptr);
	   while(s_len--)*(s_str+s_len+1)=*(s_str+s_len);
	   *(s_str)=(unsigned char)s_char; return(s_ptr);
}

void far * far pascal delete(void far *s_ptr)
{
 unsigned char far *s_str=(unsigned char far *)s_ptr;
	   while(*(s_str)){*(s_str)=*(s_str+1); s_str++;} return(s_ptr);
}

void far pascal gotoxy(unsigned int s_x, unsigned int s_y)
{
 unsigned int s_offset=(s_y*80U)+s_x; outx(0x03d4, 0x0e, s_offset>>8);
	   outx(0x03d4, 0x0f, s_offset); pokew(0x0000, 0x0450, s_x|(s_y<<8));
}

void far pascal fnsplit(void far *s_path, void far *s_drive, void far *s_dir, void far *s_name, void far *s_ext)
{
 unsigned int s_len=strlen(s_path), s_ptr, s_ptr2, s_strt=0U, s_last=0U;
           *((unsigned char far *)s_drive)='\0'; *((unsigned char far *)s_dir)='\0';
           *((unsigned char far *)s_name)='\0'; *((unsigned char far *)s_ext)='\0';
	   for(s_ptr=0U;s_ptr<s_len;s_ptr++)
	   {
	    if(*(((unsigned char far *)s_path)+s_ptr)=='\\'||*(((unsigned char far *)s_path)+s_ptr)=='\/')
	    {
	     if(s_strt==0U)s_strt=s_last=s_ptr; s_last=s_ptr+1U;
	    }
	   }
	   if(*(((unsigned char far *)s_path)+1)=='\:')
	   {
	    *(((unsigned char far *)s_drive)+0)=*(((unsigned char far *)s_path));
	    *(((unsigned char far *)s_drive)+1)=*(((unsigned char far *)s_path)+1);
	    *(((unsigned char far *)s_drive)+2)=0;
	    if(s_strt==0U)s_strt=s_last=2;
	   }
	   for(s_ptr=s_strt, s_ptr2=0U;s_ptr<s_last;s_ptr++)if(s_ptr2<MAXDIR-1)*(((unsigned char far *)s_dir)+s_ptr2++)=*(((unsigned char far *)s_path)+s_ptr);
	   *(((unsigned char far *)s_dir)+s_ptr2)=0;
	   for(s_ptr=s_last, s_ptr2=0U;s_ptr<s_len;s_ptr++)
	   {
	    if(*(((unsigned char far *)s_path)+s_ptr)=='\.')break;
	    if(s_ptr2<MAXNAME-1)*(((unsigned char far *)s_name)+s_ptr2++)=*(((unsigned char far *)s_path)+s_ptr);
	   }
	   *(((unsigned char far *)s_name)+s_ptr2)=0;
	   for(s_ptr2=0U;s_ptr<s_len;s_ptr++)if(s_ptr2<MAXEXT-1)*(((unsigned char far *)s_ext)+s_ptr2++)=*(((unsigned char far *)s_path)+s_ptr);
	   *(((unsigned char far *)s_ext)+s_ptr2)=0;
}

void far pascal fnmerge(void far *s_path, void far *s_drive, void far *s_dir, void far *s_name, void far *s_ext)
{
	   strcpy(s_path, s_drive); strcat(s_path, s_dir); strcat(s_path, s_name); strcat(s_path, s_ext);
}

unsigned int far pascal scanning(void far *s_path)
{
 t_ffblk far *s_ffblk; unsigned int s_done;
	   chdir(s_path);
	   if(!scanning_func(s_path))return(0U);
	   s_ffblk=(t_ffblk far *)farmalloc(sizeof(t_ffblk));
	   s_done=findfirst(wild_all, s_ffblk, FA_DIREC|FA_RDONLY|FA_HIDDEN|FA_ARCH|FA_SYSTEM);
	   while(!s_done)
	   {
	    if(s_ffblk->ff_name[0]!=prevdir[0]&&MZ_BIT(s_ffblk->ff_attrib, FA_DIREC))
	    {
	     scanning_level++;
	     if(!scanning(s_ffblk->ff_name)){farfree(s_ffblk); return(0U);}
	     chdir(prevdir); scanning_level--;
	    }
	    s_done=findnext(s_ffblk);
	   }
	   farfree(s_ffblk); return(1U);
}

/* Initialize main */

void far pascal _initmain(void)
{
	   HEAP_head=HEAP_tail=0LU; _argc=0U; _argv=0LU;
	   setvect(0x00, (void far *)zerovector);
}

/* End of source */