/*
  Name: SUGA O/S
  File: KERNEL.CPP
  Copy: Copyright(c)1998-1999 by Cho JaeHyuk
  What: Kernel
*/

#define FILE_KERNEL_CPP                "KERNEL.CPP"

#include <KERNEL.H>
#include <KEY.H>

void far pascal _begin_main(t_boot_sector far *);
void far pascal _end_main(void);
void far pascal _exit(unsigned int);
void far pascal nop(void);
void interrupt inop(void);
unsigned int far pascal itop(unsigned int);
void far * far pascal farmalloc(unsigned int);
void far pascal farfree(void far *);
unsigned int far pascal inportb(unsigned int);
unsigned int far pascal inportw(unsigned int);
void far pascal outportb(unsigned int, unsigned int);
void far pascal outportw(unsigned int, unsigned int);
void far pascal insportb(void far *, unsigned int, unsigned int);
void far pascal insportw(void far *, unsigned int, unsigned int);
void far pascal outsportb(unsigned int, void far *, unsigned int);
void far pascal outsportw(unsigned int, void far *, unsigned int);
void far pascal memsetb(void far *, unsigned int, unsigned int);
void far pascal memsetw(void far *, unsigned int, unsigned int);
void far pascal memcpyb(void far *, void far *, unsigned int);
void far pascal memcpyw(void far *, void far *, unsigned int);
unsigned int far pascal memcmpb(void far *, void far *, unsigned int);
unsigned int far pascal strlen(void far *);
unsigned int far pascal strcpy(void far *, void far *);
unsigned int far pascal strcat(void far *, void far *);
unsigned int far pascal strcmp(void far *, void far *);
unsigned int far pascal isdigit(unsigned int);
unsigned int far pascal islower(unsigned int);
unsigned int far pascal isupper(unsigned int);
unsigned int far pascal isalpha(unsigned int);
unsigned int far pascal tolower(unsigned int);
unsigned int far pascal toupper(unsigned int);
unsigned int far pascal tonum(unsigned int);
void far * far pascal itoa(void far *, unsigned int, unsigned int, unsigned int);
void far * far pascal ltoa(void far *, unsigned long, unsigned int, unsigned int);
unsigned int far pascal atoi(void far *, unsigned int);
unsigned long far pascal atol(void far *, unsigned int);
void far pascal vsprintf(void far *, void far *, void far *);
void far cdecl sprintf(void far *, void far *, ...);
void far * far pascal strupr(void far *);
void far * far pascal strlwr(void far *);
void far pascal swap(void far *, void far *, unsigned int);
void far pascal putc(unsigned int);
void far pascal puts(void far *);
void far pascal nputs(unsigned int, void far *);
void far cdecl printf(void far *, ...);
void far cdecl nprintf(unsigned int, void far *, ...);
unsigned int far pascal bioskey(unsigned int);
unsigned int far pascal biosdisk(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, void far *);
void far pascal delay(unsigned int);
void far pascal sound(unsigned int);
void far pascal nosound(void);
void far pascal beep(unsigned int);
unsigned int far pascal rand(void);
void far * far pascal howcpu(void);
unsigned int far pascal inportx(unsigned int, unsigned int);
void far pascal outportx(unsigned int, unsigned int, unsigned int);
unsigned int far pascal peekb(unsigned int, unsigned int);
unsigned int far pascal peekw(unsigned int, unsigned int);
void far pascal pokeb(unsigned int, unsigned int, unsigned int);
void far pascal pokew(unsigned int, unsigned int, unsigned int);
void far pascal setrgb(unsigned int, unsigned int, unsigned int, unsigned int);
void far pascal getrgb(unsigned int, void far *, void far *, void far *);
unsigned long far pascal farcore(void);
unsigned int far pascal biosmemory(void);
unsigned long far pascal farcoreleft(void);

void far * operator new(unsigned int);
void operator delete(void far *);

void interrupt (**interrupt_vector)(void);
void interrupt (*interrupt_proc[256U])(void);
void interrupt (*bios_video_interrupt)(void);
void interrupt (*bios_disk_interrupt)(void);
void interrupt (*bios_key_interrupt)(void);

t_heap far                  *g_heap_head, far *g_heap_tail;
unsigned char far            g_numchar[36U]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
t_disk far *                 g_disk_head, far *g_disk_tail;
t_drive far *                g_drive_head, far *g_drive_tail;
t_text far *                 g_text;
unsigned int                 g_drive_name;
void far *                   g_cpu_name[]={"8086/8088 microprocessor", "Intel 286 microprocessor", "Intel 386 (TM) microprocessor", "Intel 486 (TM) DX microprocessor", "Pentium (TM) microprocessor", "This system contains a Genuine Intel Processor"};
unsigned char                g_line[4U][11U]={{0xb3, 0xc4, 0xda, 0xc2, 0xbf, 0xc3, 0xc5, 0xb4, 0xc0, 0xc1, 0xd9}, {0xba, 0xcd, 0xc9, 0xc8, 0xbb, 0xcc, 0xce, 0xb9, 0xc8, 0xca, 0xbc}, {0xb3, 0xcd, 0xd5, 0xd1, 0xb8, 0xc6, 0xd8, 0xb5, 0xd4, 0xcf, 0xbe}, {0xba, 0xc4, 0xd6, 0xd2, 0xb7, 0xc7, 0xd7, 0xb6, 0xd3, 0xd0, 0xbd}};t_boot_sector far *g_boot_sector;
e_boolean                    g_original_boot;

void far pascal _begin_main(t_boot_sector far *s_boot_sector)
{
 unsigned int s_count; t_drive far *s_drive;
           clear_cache(); g_heap_head=g_heap_tail=NULL;
           memcpyw(&interrupt_proc[0U], (void far *)0LU, 256U<<1U);
           bios_video_interrupt=interrupt_proc[0x10]; bios_disk_interrupt=interrupt_proc[0x13];
           bios_key_interrupt=interrupt_proc[0x16];
           interrupt_vector=MK_FP(0x0000, 0x0000);
           disable();
           g_key=new e_boolean far [128U];
           memsetb(g_key, 0x0000, 128U);
           interrupt_vector[0x09]=interrupt_key;
           enable();
           while(bioskey(1U))bioskey(0U);
           nosound();
           g_text=new tc_text(80U, 25U, 0U, 0U, 79U, 24U);
           g_text->tty_printf("\r\n$79 \r\n", 0xcd);
           g_boot_sector=s_boot_sector;
           if(g_boot_sector->OEM_name[0U]=='M'&&g_boot_sector->OEM_name[1U]=='I'&&g_boot_sector->OEM_name[2U]=='N'&&g_boot_sector->OEM_name[3U]=='Z')g_original_boot=True;
           else
           {
            g_original_boot=False;
            g_text->tty_printf("\xfe Warning: You run to be under of other O/S!!!\r\n");
           }
           g_text->tty_printf("þ Detect CPU type: \`$s\'\r\n", howcpu());
           g_drive_head=NULL;
           g_drive_name='C';
           g_text->tty_printf("þ Detecting disk drive\r\n");
           for(s_count=0x0000;s_count<0x0100;s_count++)
           {
            if(s_count<2U||s_count&0x0080)
            {
             g_disk_tail->next=new t_disk(s_count);
             if(g_disk_tail->next->access==False&&s_count&0x0080)
             {
              delete g_disk_tail->next;
              g_disk_tail->next=NULL;
              break;
             }
             else
             {
              g_disk_tail->next->prev=g_disk_tail; g_disk_tail=g_disk_tail->next;
              g_disk_tail->next=NULL; g_disk_tail->print();
             }
            }
           }
           g_text->tty_printf("  Reports\r\n");
           s_drive=g_drive_head;
           while(s_drive)
           {
            if(s_drive->disk->access==True)g_text->tty_printf("   $c: drive - Disk=0x$02x, Size=$luBytes\r\n", s_drive->drive_name, s_drive->disk->disk_num, s_drive->length*512LU);
            s_drive=s_drive->next;
           }
           g_text->tty_printf("$79 \r\n", 0xcd);
           g_text->tty_printf("þ Starting SUGA O/S . . .\r\n");
           delay(1000U);
}

void far pascal _end_main(void)
{
           g_text->tty_puts("þ Shutdown SUGA O/S.\r\n  Have good time and see you again~\r\n");
           while(bioskey(0x0001))bioskey(0x0000);
           disable(); memcpyw((void far *)0LU, &interrupt_proc[0U], 256U<<1U); delete(g_key); enable();
}

void far pascal _exit(unsigned int s_return)
{
           _end_main();
           asm mov ah, 0x4c;
           asm mov al, byte ptr s_return;
           asm int 0x21;
           delay(1000U);
 l_shutdown:;
           puts("\xfe Press any key to reboot. #KEY#");
           asm xor ax, ax;
           asm int 0x16;
           puts("\r\n");
           asm mov dl, 0x80;
           asm int 0x19;
           asm jmp short l_shutdown;
}

void far pascal nop(void)
{
}

void interrupt inop(void)
{
}

unsigned int far pascal itop(unsigned int s_num)
{
	   if(s_num&0x000fU)s_num=(s_num&0xfff0U)+0x0010U;
           s_num>>=4U;
	   return(s_num);
}

void far * far pascal farmalloc(unsigned int s_length)
{
 t_heap far *s_node, far *s_temp;
 unsigned int s_locate;
	   if(g_heap_head==NULL)
	   {
	    s_node=(t_heap far *)MK_FP(g_segment_heap, 0x0000U);
	    s_node->address=(t_heap far *)MK_FP(g_segment_heap+1U, 0x0000U);
	    s_node->length=(unsigned int)(itop(s_length)+1U);
	    g_heap_head=s_node;
	    g_heap_head->next=g_heap_head->prev=NULL;
	    g_heap_tail=g_heap_head;
	    return(s_node->address);
	   }
	   s_node=g_heap_head;
	   while(s_node!=NULL)
	   {
	    s_temp=s_node->next;
	    s_locate=FP_SEG(s_node)+s_node->length;
	    if((FP_SEG(s_temp)-s_locate)>itop(s_length))break;
	    s_node=s_node->next;
	   }
	   if(s_node==NULL)
	   {
	    s_node=(t_heap far *)MK_FP(FP_SEG(g_heap_tail)+g_heap_tail->length, 0x0000U);
	    s_node->address=(t_heap far *)MK_FP(FP_SEG(g_heap_tail)+g_heap_tail->length+1U, 0x0000U);
	    s_node->length=itop(s_length)+1U;
	    g_heap_tail->next=s_node;
	    s_node->prev=g_heap_tail;
	    s_node->next=NULL;
	    g_heap_tail=s_node;
	   }
	   else
	   {
	    t_heap far *s_alloc;
	    s_alloc=(t_heap far *)MK_FP(s_locate, 0x0000U);
	    s_alloc->address=(t_heap far *)MK_FP(s_locate+1U, 0x0000U);
	    s_alloc->length=itop(s_length)+1U;
	    s_node->next=s_alloc;
	    s_alloc->prev=s_node;
	    s_alloc->next=s_temp;
	    s_temp->prev=s_alloc;
	    s_node=s_alloc;
	   }
	   return(s_node->address);
}

void far pascal farfree(void far *s_pointer)
{
 t_heap far *s_crnt, far *s_temp;
	   if(s_pointer==NULL)return;
	   s_crnt=g_heap_head;
	   while(s_crnt->next)
	   {
	    if(FP_SEG(s_pointer)==FP_SEG(s_crnt->address))break;
	    s_crnt=s_crnt->next;
	   }
	   if(s_crnt==NULL)return;
	   s_temp=s_crnt->prev;
	   if(s_temp)s_temp->next=s_crnt->next;
	   s_temp=s_crnt->next;
	   if(s_temp)s_temp->prev=s_crnt->prev;
           s_crnt=NULL;
}

unsigned int far pascal inportb(unsigned int s_port)
{
           asm mov dx, word ptr s_port;
           asm xor ax, ax;
           asm in al, dx;
           return(_AX);
}

unsigned int far pascal inportw(unsigned int s_port)
{
           asm mov dx, word ptr s_port;
           asm in ax, dx;
           return(_AX);
}

void far pascal outportb(unsigned int s_port, unsigned int s_byte)
{
           asm mov dx, word ptr s_port;
           asm mov al, byte ptr s_byte;
           asm out dx, al;
}

void far pascal outportw(unsigned int s_port, unsigned int s_word)
{
           asm mov dx, word ptr s_port;
           asm mov ax, word ptr s_word;
           asm out dx, ax;
}

void far pascal insportb(void far *s_target, unsigned int s_port, unsigned int s_length)
{
           asm_push_es;
           asm_push_di;
           asm mov cx, word ptr s_length;
           asm mov dx, word ptr s_port;
           asm les di, dword ptr s_target;
           asm cld;
           asm rep insb;
           asm_pop_di;
           asm_pop_es;
}

void far pascal insportw(void far *s_target, unsigned int s_port, unsigned int s_length)
{
           asm_push_es;
           asm_push_di;
           asm mov cx, word ptr s_length;
           asm mov dx, word ptr s_port;
           asm les di, dword ptr s_target;
           asm cld;
           asm rep insw;
           asm_pop_di;
           asm_pop_es;
}

void far pascal outsportb(unsigned int s_port, void far *s_source, unsigned int s_length)
{
           asm_push_ds;
           asm_push_si;
           asm mov cx, word ptr s_length;
           asm mov dx, word ptr s_port;
           asm lds si, dword ptr s_source;
           asm cld;
           asm rep outsb;
           asm_pop_si;
           asm_pop_ds;
}

void far pascal outsportw(unsigned int s_port, void far *s_source, unsigned int s_length)
{
           asm_push_ds;
           asm_push_si;
           asm mov cx, word ptr s_length;
           asm mov dx, word ptr s_port;
           asm lds si, dword ptr s_source;
           asm cld;
           asm rep outsw;
           asm_pop_si;
           asm_pop_ds;
}

void far pascal memsetb(void far *s_ptr, unsigned int s_byte, unsigned int s_length)
{
           asm_push_es;
           asm_push_di;
           asm mov cx, word ptr s_length;
           asm mov al, byte ptr s_byte;
           asm les di, dword ptr s_ptr;
           asm cld;
           asm rep stosb;
           asm_pop_di;
           asm_pop_es;
}

void far pascal memsetw(void far *s_ptr, unsigned int s_word, unsigned int s_length)
{
           asm_push_es;
           asm_push_di;
           asm mov cx, word ptr s_length;
           asm mov ax, word ptr s_word;
           asm les di, dword ptr s_ptr;
           asm cld;
           asm rep stosw;
           asm_pop_di;
           asm_pop_es;
}

void far pascal memcpyb(void far *s_target, void far *s_source, unsigned int s_length)
{
           asm_push_es;
           asm_push_di;
           asm_push_ds;
           asm_push_si;
           asm mov cx, word ptr s_length;
           asm les di, dword ptr s_target;
           asm lds si, dword ptr s_source;
           asm cld;
           asm rep movsb;
           asm_pop_si;
           asm_pop_ds;
           asm_pop_di;
           asm_pop_es;
}

void far pascal memcpyw(void far *s_target, void far *s_source, unsigned int s_length)
{
           asm_push_es;
           asm_push_di;
           asm_push_ds;
           asm_push_si;
           asm mov cx, word ptr s_length;
           asm les di, dword ptr s_target;
           asm lds si, dword ptr s_source;
           asm cld;
           asm rep movsw;
           asm_pop_si;
           asm_pop_ds;
           asm_pop_di;
           asm_pop_es;
}

unsigned int far pascal memcmpb(void far *s_target, void far *s_source, unsigned int s_length)
{
           asm_push_ds;
           asm_push_es;
           asm_push_si;
           asm_push_di;
           asm mov cx, word ptr s_length;
           asm les di, dword ptr s_target;
           asm lds si, dword ptr s_source;
           asm cld;
           asm rep cmpsb;
           asm jne short l_memcmp_0000;
           asm mov ax, 0x0001;
           asm jmp short l_memcmp_0001;
l_memcmp_0000:;
           asm xor ax, ax;
l_memcmp_0001:;
           asm_pop_di;
           asm_pop_si;
           asm_pop_es;
           asm_pop_ds;
           return(_AX);
}

unsigned int far pascal strlen(void far *s_string)
{
          asm_push_ds;
          asm_push_si;
          asm lds si, dword ptr s_string
          asm xor cx, cx;
          asm cld;
 l_strlen_0000:;
          asm lodsb;
          asm or al, al;
          asm jz short l_strlen_0001;
          asm inc cx;
          asm jmp short l_strlen_0000;
 l_strlen_0001:;
          asm_pop_si;
          asm_pop_ds;
          return(_CX);
}

unsigned int far pascal strcpy(void far *s_target, void far *s_source)
{
           asm_push_es;
           asm_push_di;
           asm_push_ds;
           asm_push_si;
           asm les di, dword ptr s_target;
           asm lds si, dword ptr s_source;
           asm xor cx, cx;
           asm cld;
 l_strcpy_0000:;
           asm lodsb;
           asm stosb;
           asm or al, al;
           asm jz short l_strcpy_0001;
           asm inc cx;
           asm jmp short l_strcpy_0000;
 l_strcpy_0001:;
           asm dec cx;
           asm_pop_si;
           asm_pop_ds;
           asm_pop_di;
           asm_pop_es;
           return(_CX);
}

unsigned int far pascal strcat(void far *s_target, void far *s_source)
{
           return(strcpy(((unsigned char far *)s_target)+strlen(s_target), s_source));
}

unsigned int far pascal strcmp(void far *s_target, void far *s_source)
{
           if(strlen(s_target)!=strlen(s_source))return(0U);
           asm_push_es;
           asm_push_di;
           asm_push_ds;
           asm_push_si;
           asm mov cx, ax;
           asm les di, dword ptr s_target;
           asm lds si, dword ptr s_source;
           asm cld;
           asm rep cmpsb;
           asm jne short l_strcmp_0001;
           asm mov ax, 1;
           asm jmp short l_strcmp_0002;
 l_strcmp_0001:;
           asm xor ax, ax;
 l_strcmp_0002:;
           asm_pop_si;
           asm_pop_ds;
           asm_pop_di;
           asm_pop_es;
           return(_AX);
}

unsigned int far pascal isdigit(unsigned int s_char)
{
           return(s_char>='0'&&s_char<='9');
}

unsigned int far pascal islower(unsigned int s_char)
{
           return(s_char>='a'&&s_char<='z');
}

unsigned int far pascal isupper(unsigned int s_char)
{
           return(s_char>='A'&&s_char<='Z');
}

unsigned int far pascal isalpha(unsigned int s_char)
{
           return(islower(s_char)||isupper(s_char));
}

unsigned int far pascal tolower(unsigned int s_char)
{
           if(isupper(s_char))s_char+=0x20;
           return(s_char);
}

unsigned int far pascal toupper(unsigned int s_char)
{
           if(islower(s_char))s_char-=0x20;
           return(s_char);
}

unsigned int far pascal tonum(unsigned int s_char)
{
           if(isdigit(s_char))s_char-='0';
           else if(isalpha(s_char))
           {
            s_char=toupper(s_char);
            s_char-=('A'-0x0a);
           }
           return(s_char);
}

void far * far pascal itoa(void far *s_target, unsigned int s_integer, unsigned int s_radix, unsigned int s_sign)
{
 unsigned char s_reverse[16U+1U], far *s_string=(unsigned char far *)s_target;
 unsigned int s_reversepoint=0U;
           if(s_sign)
           {
            if(s_integer&0x8000)
            {
             asm neg word ptr s_integer;
             *(s_string++)='-';
            }
           }
           do
           {
            s_reverse[s_reversepoint++]=g_numchar[s_integer%s_radix];
            s_integer/=s_radix;
           }while(s_integer);
           while(s_reversepoint)*(s_string++)=s_reverse[--s_reversepoint];
           *(s_string)='\0';
           return(s_target);
}

void far * far pascal ltoa(void far *s_target, unsigned long s_long, unsigned int s_radix, unsigned int s_sign)
{
 unsigned char s_reverse[32U+1U], far *s_string=(unsigned char far *)s_target;
 unsigned int s_reversepoint=0U;
           if(s_sign)
           {
            if(s_long&0x80000000LU)
            {
             s_long=(~s_long)+1LU;
             *(s_string++)='-';
            }
           }
           do
           {
            s_reverse[s_reversepoint++]=g_numchar[(unsigned int)(s_long%s_radix)];
            s_long/=s_radix;
           }while(s_long);
           while(s_reversepoint)*(s_string++)=s_reverse[--s_reversepoint];
           *(s_string)='\0';
           return(s_target);
}

unsigned int far pascal atoi(void far *s_string, unsigned int s_radix)
{
 unsigned int s_return=0U, s_num=1U, s_len, s_minus=0U;
           if(*((unsigned char far *)s_string)=='-')
           {
            ((unsigned char far *)s_string)++;
            s_minus=1U;
           }
           s_len=strlen(s_string);
           if(!s_len)return(0U);
           while(s_len)
           {
            s_len--;
            s_return+=(s_num*tonum(*(((unsigned char far *)s_string)+s_len)));
            s_num*=s_radix;
           }
           if(s_minus)asm neg word ptr s_return;
           return(s_return);
}

unsigned long far pascal atol(void far *s_string, unsigned int s_radix)
{
 unsigned long s_return=0LU, s_num=1LU; unsigned int s_len, s_minus=0U;
           if(*((unsigned char far *)s_string)=='-')
           {
            ((unsigned char far *)s_string)++;
            s_minus=1U;
           }
           s_len=strlen(s_string);
           if(!s_len)return(0LU);
           while(s_len)
           {
            s_len--;
            s_return+=(s_num*(unsigned long)tonum(*(((unsigned char far *)s_string)+s_len)));
            s_num*=(unsigned long)s_radix;
           }
           if(s_minus)s_return=(~s_return)+1LU;
           return(s_return);
}

void far pascal vsprintf(void far *s_target, void far *s_format, void far *s_argument)
{
 union
 {
  struct
  {
   unsigned int minus    :  2U;
   unsigned int long_data:  1U;
   unsigned int sign     :  1U;
   unsigned int radix    :  5U;
   unsigned int reserved :  7U;
  }flag;
  unsigned int dump;
 }s_flag;
 unsigned int s_length, s_default_char;
           while(*((unsigned char far *)s_format))
           {
            if(*((unsigned char far *)s_format)=='\$')
            {
             ((unsigned char far *)s_format)++;
             s_flag.dump=0U;
             s_length=0U;
             s_default_char=' ';
             switch(*((unsigned char far *)s_format))
             {
              case '_': s_flag.flag.minus=3U; break;
              case '-': s_flag.flag.minus=2U; break;
              case '+': s_flag.flag.minus=1U; break;
              default:  break;
             }
             if(s_flag.flag.minus)((unsigned char far *)s_format)++;
             else s_flag.flag.minus=1U;
             if(isdigit(*((unsigned char far *)s_format)))
             {
              unsigned char s_buffer[5U];
              if(*((unsigned char far *)s_format)=='0')
              {
               s_default_char='0';
               ((unsigned char far *)s_format)++;
              }
              while(isdigit(*((unsigned char far *)s_format)))
              {
               s_buffer[s_length++]=*((unsigned char far *)s_format);
               ((unsigned char far *)s_format)++;
               if(s_length>=4)break;
              }
              s_buffer[s_length]='\0';
              s_length=atoi(&s_buffer[0U], 10U);
             }
             if(toupper(*((unsigned char far *)s_format))=='L')
             {
              ((unsigned char far *)s_format)++;
              s_flag.flag.long_data=1U;
             }
             switch(toupper(*((unsigned char far *)s_format)))
             {
              unsigned int s_len, s_count;
              unsigned char far *s_global_point;
              void far *s_global_buffer;
              case 'B': s_flag.flag.radix=2U;            goto l_vsprintf_0000;
              case 'O': s_flag.flag.radix=8U;            goto l_vsprintf_0000;
              case 'D': case 'I': s_flag.flag.sign=1U;
              case 'U': s_flag.flag.radix=10U; goto l_vsprintf_0000;
              case 'X': case 'H': s_flag.flag.radix=16U;
                   l_vsprintf_0000:;
                   s_global_buffer=farmalloc(32U+1U);
                   s_global_point=(unsigned char far *)s_global_buffer;
                   if(s_flag.flag.long_data)ltoa(s_global_buffer, *(((unsigned long far *)s_argument)++), s_flag.flag.radix, s_flag.flag.sign);
                   else itoa(s_global_buffer, *(((unsigned int far *)s_argument)++), s_flag.flag.radix, s_flag.flag.sign);
                   if(islower(*((unsigned char far *)s_format)))strlwr(s_global_buffer);
                   s_len=strlen(s_global_buffer);
                   if(s_length>s_len)
                   {
                    switch(s_flag.flag.minus)
                    {
                     case 0x03: s_count=(s_length-s_len)>>1U; break;
                     case 0x02: s_count=0U; break;
                     default: s_count=s_length-s_len; break;
                    }
                    while(s_count--)*(((unsigned char far *)s_target)++)=s_default_char;
                   }
                   while(*(s_global_point))*(((unsigned char far *)s_target)++)=*(s_global_point++);
                   if(s_length>s_len)
                   {
                    switch(s_flag.flag.minus)
                    {
                     case 0x03: s_count=(s_length-s_len)-((s_length-s_len)>>1U); break;
                     case 0x02: s_count=s_length-s_len; break;
                     default: s_count=0U; break;
                    }
                    while(s_count--)*(((unsigned char far *)s_target)++)=s_default_char;
                   }
                   ((unsigned char far *)s_format)++;
                   farfree(s_global_buffer);
                   break;
              case 'S': case 'A':
                   s_global_point=(unsigned char far *)(*(((unsigned long far *)s_argument)++));
                   s_len=strlen(s_global_point);
                   if(s_length>s_len)
                   {
                    switch(s_flag.flag.minus)
                    {
                     case 0x03: s_count=(s_length-s_len)>>1U; break;
                     case 0x02: s_count=0U; break;
                     default: s_count=s_length-s_len; break;
                    }
                    while(s_count--)*(((unsigned char far *)s_target)++)=s_default_char;
                   }
                   while(*(s_global_point))*(((unsigned char far *)s_target)++)=*(s_global_point++);
                   if(s_length>s_len)
                   {
                    switch(s_flag.flag.minus)
                    {
                     case 0x03: s_count=(s_length-s_len)-((s_length-s_len)>>1U); break;
                     case 0x02: s_count=s_length-s_len; break;
                     default: s_count=0U; break;
                    }
                    while(s_count--)*(((unsigned char far *)s_target)++)=s_default_char;
                   }
                   ((unsigned char far *)s_format)++;
                   break;
              case 'C':
                   if(s_length>s_len)
                   {
                    switch(s_flag.flag.minus)
                    {
                     case 0x03: s_count=(s_length-1U)>>1U; break;
                     case 0x02: s_count=0U; break;
                     default: s_count=s_length-1U; break;
                    }
                    while(s_count--)*(((unsigned char far *)s_target)++)=s_default_char;
                   }
                   *(((unsigned char far *)s_target)++)=(unsigned char)(*(((unsigned int far *)s_argument)++));
                   if(s_length>s_len)
                   {
                    switch(s_flag.flag.minus)
                    {
                     case 0x03: s_count=(s_length-1U)-((s_length-1U)>>1U); break;
                     case 0x02: s_count=s_length-1U; break;
                     default: s_count=0U; break;
                    }
                    while(s_count--)*(((unsigned char far *)s_target)++)=s_default_char;
                   }
                   ((unsigned char far *)s_format)++;
                   break;
              case ' ':
                   if(!s_length)s_length++;
                   while(s_length--)*(((unsigned char far *)s_target)++)=(unsigned char)(*((unsigned int far *)s_argument));
                   ((unsigned int far *)s_argument)++;
                   ((unsigned char far *)s_format)++;
                   break;
              case '\0': break;
              default: *(((unsigned char far *)s_target)++)=*(((unsigned char far *)s_format)++); break;
             }
            }
            else *(((unsigned char far *)s_target)++)=*(((unsigned char far *)s_format)++);
           }
           *((unsigned char far *)s_target)='\0';
}

void far cdecl sprintf(void far *s_target, void far *s_format, ...)
{
           vsprintf(s_target, s_format, ...);
}

void far * far pascal strupr(void far *s_string)
{
 unsigned char far *s_copy=(unsigned char far *)s_string;
           while(*(s_copy))
           {
            *(s_copy)=toupper(*(s_copy));
            s_copy++;
           }
           return(s_string);
}

void far * far pascal strlwr(void far *s_string)
{
 unsigned char far *s_copy=(unsigned char far *)s_string;
           while(*(s_copy))
           {
            *(s_copy)=tolower(*(s_copy));
            s_copy++;
           }
           return(s_string);
}

void far pascal swap(void far *s_source1, void far *s_source0, unsigned int s_length)
{
 unsigned char s_byte;
           while(s_length--)
           {
            s_byte=*((unsigned char far *)s_source0);
            *((unsigned char far *)s_source0)=*((unsigned char far *)s_source1);
            *((unsigned char far *)s_source1)=s_byte;
            ((unsigned char far *)s_source0)++;
            ((unsigned char far *)s_source1)++;
           }
}

void far pascal putc(unsigned int s_char)
{
           asm mov ah, 0x0e;
           asm mov al, byte ptr s_char;
           asm mov bx, 0x0007;
           bios_video_interrupt();
}

void far pascal puts(void far *s_string)
{
           while(*((unsigned char far *)s_string))putc(*(((unsigned char far *)s_string)++));
}

void far pascal nputs(unsigned int s_length, void far *s_string)
{
           while(s_length--)putc(*(((unsigned char far *)s_string)++));
}

void far cdecl printf(void far *s_format, ...)
{
 void far *s_string=farmalloc(512U); vsprintf(s_string, s_format, ...); puts(s_string); farfree(s_string);
}

void far cdecl nprintf(unsigned int s_length, void far *s_format, ...)
{
 void far *s_string=farmalloc(512U); vsprintf(s_string, s_format, ...); nputs(s_length, s_string); farfree(s_string);
}

unsigned int far pascal bioskey(unsigned int s_command)
{
           asm xor al, al;
           asm mov ah, byte ptr s_command;
           bios_key_interrupt();
           asm jz short l_nokey;
           asm test byte ptr s_command, 0x01;
           asm jz short l_keydone;
           asm or ax, ax;
           asm jnz short l_keydone;
           asm mov ax, 0xffff;
           asm jmp short l_keydone;
 l_nokey:;
           asm test byte ptr s_command, 0x01;
           asm jz short l_keydone;
           asm xor ax, ax;
 l_keydone:;
           return(_AX);
}

unsigned int far pascal biosdisk(unsigned int s_command, unsigned int s_drive, unsigned int s_head, unsigned int s_track, unsigned int s_sector, unsigned int s_nsects, void far *s_buffer)
{
 unsigned int s_recount;
           asm_push_es;
 l_retry_error:;
           s_recount=4U;
 l_retry:;
           asm mov ah, byte ptr s_command;
           asm mov al, byte ptr s_nsects;
           asm or ah, ah;
           asm jz short l_reset;
           asm mov cx, word ptr s_track;
           asm shl ch, 0x06;
           asm or ch, byte ptr s_sector;
           asm xchg ch, cl;
           asm mov dl, byte ptr s_drive;
           asm mov dh, byte ptr s_head;
           asm les bx, dword ptr s_buffer;
 l_reset:;
           asm clc;
           bios_disk_interrupt();
           asm jnb short l_success;
           asm xor ax, ax;
           asm mov dl, byte ptr s_drive;
           bios_disk_interrupt();
           delay(3000U);
           s_recount--;
           if(!s_recount)goto l_retry;
           asm xor ax, ax;
           asm int 0x24;
           asm cmp ax, 0x0001;
           asm je l_retry_error;
 l_success:;
           asm mov al, ah;
           asm xor ah, ah;
           asm_pop_es;
           return(_AX);
}

void far pascal delay(unsigned int s_freq)
{
           asm mov cx, word ptr s_freq;
           asm in al, 0x40;
           asm mov ah, al;
 l_delay_0000:;
           asm push cx;
           asm mov cx, 182 shl 2;
 l_delay_0001:;
           asm in al, 0x40;
           asm cmp ah, al;
           asm je short l_delay_0001;
           asm mov ah, al;
           asm loop short l_delay_0001;
           asm pop cx;
           asm loop short l_delay_0000;
}

void far pascal sound(unsigned int s_herz)
{
           asm mov bx, word ptr s_herz;
           asm mov ax, 0x34dd;
           asm mov dx, 0x0012;
           asm cmp dx, bx;
           asm jnb short l_sound_0001;
           asm div bx;
           asm mov bx, ax;
           asm in al, 0x61;
           asm test al, 0x03;
           asm jne short l_sound_0000;
           asm or al, 3;
           asm out 0x61, al;
           asm mov al, 0xb6;
           asm out 0x43, al;
l_sound_0000:;
           asm mov al, bl;
           asm out 0x42, al;
           asm mov al, bh;
           asm out 0x42, al;
l_sound_0001:;
}

void far pascal nosound(void)
{
           asm in al, 0x61;
           asm and al, 0xfc;
           asm out 0x61, al;
}

void far pascal beep(unsigned int s_delay)
{
           sound(2000U); delay(s_delay); nosound();
}

unsigned int far pascal rand(void)
{
 static unsigned long s_seed=(unsigned long)inportb(0x40);
        s_seed=0x015a4e35LU*s_seed+1LU; return((unsigned int)(s_seed>>16U)&0x7fff);
}

void far * far pascal howcpu(void)
{
 void far *s_return;
           switch(_howcpu())
           {
            case 0x8086: s_return=g_cpu_name[0U]; break;
            case 0x0286: s_return=g_cpu_name[1U]; break;
            case 0x0386: s_return=g_cpu_name[2U]; break;
            case 0x0486: s_return=g_cpu_name[3U]; break;
            case 0x0586: s_return=g_cpu_name[4U]; break;
            default:     s_return=g_cpu_name[5U]; break;
           }
           return(s_return);
}

unsigned int far pascal inportx(unsigned int s_port, unsigned int s_index)
{
           asm mov dx, word ptr s_port;
           asm mov al, byte ptr s_index;
           asm out dx, al;
           asm inc dx;
           asm in al, dx;
           asm xor ah, ah;
           return(_AX);
}

void far pascal outportx(unsigned int s_port, unsigned int s_index, unsigned int s_data)
{
           asm mov dx, word ptr s_port;
           asm mov al, byte ptr s_index;
           asm mov ah, byte ptr s_data;
           asm out dx, ax;
}

unsigned int far pascal peekb(unsigned int s_segment, unsigned int s_offset)
{
           return(*((unsigned char far *)(((((unsigned long)s_segment)<<16U)&0xffff0000LU)|(((unsigned long)s_offset)&0x0000ffffLU))));
}

unsigned int far pascal peekw(unsigned int s_segment, unsigned int s_offset)
{
           return(*((unsigned int far *)(((((unsigned long)s_segment)<<16U)&0xffff0000LU)|(((unsigned long)s_offset)&0x0000ffffLU))));
}

void far pascal pokeb(unsigned int s_segment, unsigned int s_offset, unsigned int s_byte)
{
           *((unsigned char far *)(((((unsigned long)s_segment)<<16U)&0xffff0000LU)|(((unsigned long)s_offset)&0x0000ffffLU)))=(unsigned char)s_byte;
}

void far pascal pokew(unsigned int s_segment, unsigned int s_offset, unsigned int s_word)
{
           *((unsigned int far *)(((((unsigned long)s_segment)<<16U)&0xffff0000LU)|(((unsigned long)s_offset)&0x0000ffffLU)))=s_word;
}

void far pascal setrgb(unsigned int s_color_num, unsigned int s_red, unsigned int s_green, unsigned int s_blue)
{
           asm mov dx, 0x03c8;
           asm mov al, byte ptr s_color_num;
           asm out dx, al;
           asm inc dx;
           asm mov al, byte ptr s_red;
           asm out dx, al;
           asm mov al, byte ptr s_green;
           asm out dx, al;
           asm mov al, byte ptr s_blue;
           asm out dx, al;
}

void far pascal getrgb(unsigned int s_color_num, void far *s_red, void far *s_green, void far *s_blue)
{
           outportb(0x03c7, s_color_num);
           *((unsigned int far *)s_red)=inportb(0x03c9);
           *((unsigned int far *)s_green)=inportb(0x03c9);
           *((unsigned int far *)s_blue)=inportb(0x03c9);
}

unsigned long far pascal farcore(void)
{
 void far *s_ptr=farmalloc(1U);
 unsigned int s_heap;
           s_heap=FP_SEG(s_ptr);
           farfree(s_ptr);
           return((((unsigned long)s_heap)<<4)-(((unsigned long)g_segment_text)<<4U)+(512LU*5LU));
}

unsigned int far pascal biosmemory(void)
{
           interrupt_proc[0x12](); return(_AX);
}

unsigned long far pascal farcoreleft(void)
{
           return(((((unsigned long)biosmemory())&0x0000ffffLU)<<10U)-farcore());
}

t_drive far * far pascal found_drive(unsigned int s_drive_name)
{
 t_drive far *s_drive=g_drive_head;
           while(s_drive)
           {
            if(toupper(s_drive->drive_name)==toupper(s_drive_name))break;
            s_drive=s_drive->next;
           }
           return(s_drive);
}

void far * operator new(unsigned int s_num)
{
           return(farmalloc(s_num));
}

void operator delete(void far *s_pointer)
{
           farfree(s_pointer);
}

void far pascal tc_text::window(unsigned int s_minx, unsigned int s_miny, unsigned int s_maxx, unsigned int s_maxy)
{
           tc_text::minx=s_minx; tc_text::miny=s_miny; tc_text::maxx=s_maxx; tc_text::maxy=s_maxy;
}

void far pascal tc_text::lputc(unsigned int s_cc, unsigned int s_x, unsigned int s_y)
{
           *((unsigned int far *)tc_text::video_memory+(s_y*tc_text::hres)+s_x)=s_cc;
}

unsigned int far pascal tc_text::lgetc(unsigned int s_x, unsigned int s_y)
{
           return(*((unsigned int far *)tc_text::video_memory+(s_y*tc_text::hres)+s_x));
}

void far pascal tc_text::putc(unsigned int s_color, unsigned int s_char, unsigned int s_x, unsigned int s_y)
{
           tc_text::lputc((s_color<<8U)|s_char, s_x, s_y);
}

void far pascal tc_text::puts(unsigned int s_color, void far *s_string, unsigned int s_x, unsigned int s_y)
{
           while(*((unsigned char far *)s_string))tc_text::putc(s_color, *(((unsigned char far *)s_string)++), s_x++, s_y);
}

void far pascal tc_text::nputs(unsigned int s_color, unsigned int s_length, void far *s_string, unsigned int s_x, unsigned int s_y)
{
           while(s_length--)tc_text::putc(s_color, *(((unsigned char far *)s_string)++), s_x++, s_y);
}

void far cdecl tc_text::printf(unsigned int s_color, unsigned int s_x, unsigned int s_y, void far *s_format, ...)
{
 unsigned char far *s_string=new unsigned char far [256U]; vsprintf(s_string, s_format, ...); tc_text::puts(s_color, s_string, s_x, s_y); delete(s_string);
}

void far cdecl tc_text::nprintf(unsigned int s_color, unsigned int s_length, unsigned int s_x, unsigned int s_y, void far *s_format, ...)
{
 unsigned char far *s_string=new unsigned char far [256U]; vsprintf(s_string, s_format, ...); tc_text::nputs(s_color, s_length, s_string, s_x, s_y); delete(s_string);
}

void far pascal tc_text::hline(unsigned int s_color, unsigned int s_char, unsigned int s_x1, unsigned int s_x2, unsigned int s_y)
{
           while(s_x1<=s_x2)tc_text::putc(s_color, s_char, s_x1++, s_y);
}

void far pascal tc_text::vline(unsigned int s_color, unsigned int s_char, unsigned int s_x, unsigned int s_y1, unsigned int s_y2)
{
           while(s_y1<=s_y2)tc_text::putc(s_color, s_char, s_x, s_y1++);
}

void far pascal tc_text::fillbox(unsigned int s_color, unsigned int s_char, unsigned int s_x1, unsigned int s_y1, unsigned int s_x2, unsigned int s_y2)
{
           while(s_y1<=s_y2)tc_text::hline(s_color, s_char, s_x1, s_x2, s_y1++);
}

void far pascal tc_text::box(unsigned int s_color, unsigned int s_type, unsigned int s_x1, unsigned int s_y1, unsigned int s_x2, unsigned int s_y2)
{
           tc_text::hline(s_color, g_line[s_type][1U], s_x1+1U, s_x2-1U, s_y1);
           tc_text::vline(s_color, g_line[s_type][0U], s_x1, s_y1+1U, s_y2-1U);
           tc_text::vline(s_color, g_line[s_type][0U], s_x2, s_y1+1U, s_y2-1U);
           tc_text::hline(s_color, g_line[s_type][1U], s_x1+1U, s_x2-1U, s_y2);
           tc_text::putc(s_color, g_line[s_type][2U], s_x1, s_y1);
           tc_text::putc(s_color, g_line[s_type][4U], s_x2, s_y1);
           tc_text::putc(s_color, g_line[s_type][8U], s_x1, s_y2);
           tc_text::putc(s_color, g_line[s_type][10U], s_x2, s_y2);
}

void far pascal tc_text::recbox(unsigned int s_color, unsigned int s_type, unsigned int s_x1, unsigned int s_y1, unsigned int s_x2, unsigned int s_y2)
{
           tc_text::fillbox(s_color, ' ', s_x1+1U, s_y1+1U, s_x2-1U, s_y2-1U);
           tc_text::box(s_color, s_type, s_x1, s_y1, s_x2, s_y2);
}

void far pascal tc_text::clear(unsigned int s_color, unsigned int s_char)
{
           tc_text::fillbox(s_color, s_char, tc_text::minx, tc_text::miny, tc_text::maxx, tc_text::maxy);
}

void far pascal tc_text::gettext(void far *s_buffer, unsigned int s_x1, unsigned int s_y1, unsigned int s_x2, unsigned int s_y2)
{
 unsigned int s_cx;
           while(s_y1<=s_y2)
           {
            for(s_cx=s_x1;s_cx<=s_x2;s_cx++)*(((unsigned int far *)s_buffer)++)=tc_text::lgetc(s_cx, s_y1);
            s_y1++;
           }
}

void far pascal tc_text::puttext(void far *s_buffer, unsigned int s_x1, unsigned int s_y1, unsigned int s_x2, unsigned int s_y2)
{
 unsigned int s_cx;
           while(s_y1<=s_y2)
           {
            for(s_cx=s_x1;s_cx<=s_x2;s_cx++)
            {
             tc_text::lputc(*(((unsigned int far *)s_buffer)++), s_cx, s_y1);
            }
            s_y1++;
           }
}

void far pascal tc_text::setcursor(unsigned int s_start, unsigned int s_end)
{
           asm mov dx, 0x03d4;
           asm mov al, 0x0a;
           asm out dx, al;
           asm inc dx;
           asm mov al, byte ptr s_start;
           asm out dx, al;
           asm dec dx;
           asm mov al, 0x0b;
           asm out dx, al;
           asm inc dx;
           asm mov al, byte ptr s_end;
           asm out dx, al;
}

void far pascal tc_text::setblank(e_boolean s_sw)
{
           asm mov dx, 0x03d4 + 0x0006;
           asm in al, dx;
           asm mov dx, 0x03c0;
           asm mov al, 0x10;
           asm out dx, al;
           asm inc dx;
           asm in al, dx;
           asm cmp word ptr s_sw, True;
           asm je short l_true;
           asm and al, 0xf7;
           asm jmp short l_false;
 l_true:;
           asm or al, 0x08;
 l_false:;
           asm mov dx, 0x03c0;
           asm out dx, al;
           asm mov dx, 0x03d4 + 0x0006;
           asm in al, dx;
           asm mov dx, 0x03c0;
           asm mov al, 0x20;
           asm out dx, al;
}

void far pascal tc_text::setfont(void far *s_font)
{
 unsigned int s_count, s_cc;
           outportb(0x03c5, inportx(0x03c4, 0x04)|0x04);
           outportb(0x03cf, inportx(0x03ce, 0x05)&0xef);
           outportx(0x03c4, 0x02, 0x04);
           outportx(0x03ce, 0x04, 0x02);
           outportx(0x03ce, 0x06, 0x00);
           for(s_count=0U;s_count<256U;s_count++)
           {
            for(s_cc=0U;s_cc<16U;s_cc++)
            pokeb(0xb000, (s_count<<5)+s_cc, *(((unsigned char far *)s_font)+(s_count<<4)+s_cc));
           }
           outportx(0x03c4, 0x04, 0x03);
           outportx(0x03ce, 0x05, 0x10);
           outportx(0x03c4, 0x02, 0x03);
           outportx(0x03ce, 0x04, 0x00);
           outportx(0x03ce, 0x06, 0x0e);
}

unsigned int far pascal tc_text::wherex(void)
{
           tc_text::tty_x=peekb(0x0040, 0x0050)&0x00ff;
           return(tc_text::tty_x);
}

unsigned int far pascal tc_text::wherey(void)
{
           tc_text::tty_y=peekb(0x0040, 0x0051)&0x00ff;
           return(tc_text::tty_y);
}

void far pascal tc_text::gotoxy(unsigned int s_x, unsigned int s_y)
{
 unsigned int s_data;
           if(s_x<tc_text::minx)s_x=tc_text::minx;
           if(s_y<tc_text::miny)s_y=tc_text::miny;
           if(s_x>tc_text::maxx)s_x=tc_text::maxx;
           if(s_y>tc_text::maxy)s_y=tc_text::maxy;
	   tc_text::tty_x=s_x;
	   tc_text::tty_y=s_y;
	   pokeb(0x0040, 0x0050, s_x);
           pokeb(0x0040, 0x0051, s_y);
	   s_data=(s_y*tc_text::hres)+s_x;
           outportx(0x03d4, 0x0e, (s_data>>8U)&0x00ff);
           outportx(0x03d4, 0x0f, s_data&0x00ff);
}

void far pascal tc_text::linefeed(e_boolean s_sw)
{
           tc_text::tty_y++;
           if(tc_text::tty_y>=tc_text::maxy)
           {
            unsigned char far *s_store=new unsigned char far [8000U];
            tc_text::tty_y=tc_text::maxy;
            tc_text::gettext(s_store, tc_text::minx, tc_text::miny+1U, tc_text::maxx, tc_text::maxy);
            tc_text::puttext(s_store, tc_text::minx, tc_text::miny, tc_text::maxx, tc_text::maxy-1U);
            tc_text::hline(tc_text::tty_color, ' ', tc_text::minx, tc_text::maxx, tc_text::maxy);
            delete(s_store);
           }
           if(s_sw==True)tc_text::tty_x=0U;
}

void far pascal tc_text::tty_putc(unsigned char s_char)
{
           switch(s_char)
           {
            case 0x0d: tc_text::tty_x=0U; break;
            case 0x0a: tc_text::linefeed(False); break;
            case 0x08:
                 if(tc_text::tty_x)tc_text::tty_x--;
                 else if(tc_text::tty_y)
                 {
                  tc_text::tty_x=tc_text::maxx;
                  tc_text::tty_y--;
                 }
                 else beep(10U);
                 break;
            case 0x09:
                 tc_text::tty_x+=tc_text::tab;
                 if(tc_text::tty_x>tc_text::maxx)
                 {
                  unsigned int s_x=tc_text::tty_x-tc_text::maxx;
                  tc_text::linefeed(True);
                  tc_text::tty_x+=s_x;
                 }
                 break;
            default:
                 tc_text::lputc((tc_text::tty_color<<8)|s_char, tc_text::tty_x++, tc_text::tty_y);
                 if(tc_text::tty_x>tc_text::maxx)tc_text::linefeed(True);
                 break;
           }
           tc_text::gotoxy(tc_text::tty_x, tc_text::tty_y);
}

void far pascal tc_text::tty_puts(void far *s_string)
{
           while(*((unsigned char far *)s_string))tc_text::tty_putc(*(((unsigned char far *)s_string)++));
}

void far pascal tc_text::tty_nputs(unsigned int s_length, void far *s_string)
{
           while(s_length--)tc_text::tty_putc(*(((unsigned char far *)s_string)++));
}

void far cdecl tc_text::tty_printf(void far *s_format, ...)
{
 void far *s_string=farmalloc(512U);
           vsprintf(s_string, s_format, ...);
           tc_text::tty_puts(s_string);
           farfree(s_string);
}

void far cdecl tc_text::tty_nprintf(unsigned int s_length, void far *s_format, ...)
{
 void far *s_string=farmalloc(512U);
           vsprintf(s_string, s_format, ...);
           tc_text::tty_nputs(s_length, s_string);
           farfree(s_string);
}

void far pascal tc_disk::long2item(void far *s_head, void far *s_cylinder, void far *s_sector, unsigned long s_lsector)
{
           *((unsigned int far *)s_head)    =(unsigned int)((unsigned long)(s_lsector/(unsigned long)tc_disk::sectors_per_track)%(unsigned long)tc_disk::heads);
           *((unsigned int far *)s_cylinder)=(unsigned int)((unsigned long)(s_lsector/(unsigned long)tc_disk::sectors_per_track)/(unsigned long)tc_disk::heads);
           *((unsigned int far *)s_sector)  =(unsigned int)((unsigned long)(s_lsector%(unsigned long)tc_disk::sectors_per_track)+1LU);
}

unsigned int far pascal tc_disk::build_drive(unsigned long s_start)
{
 unsigned int s_count, s_return=False;
 t_sector far *s_partition;
 t_drive far *s_drive;
           if(!(tc_disk::disk_num&0x0080))
           {
            s_drive=new t_drive(this, 0x00, tc_disk::disk_num+'A', 0LU,
                     tc_disk::heads*tc_disk::cylinders*tc_disk::sectors_per_track,
                     tc_disk::heads*tc_disk::cylinders*tc_disk::sectors_per_track,
                     False
                    );
            tc_disk::add_drive(s_drive);
            return(True);
           }
           s_partition=new t_sector;
           if(tc_disk::absread(&s_partition->sector[0U], s_start, 1U))return(False);
           if(s_partition->partition.ispartition==0xaa55)
           {
            for(s_count=0U;s_count<4U;s_count++)
            {
             switch(s_partition->partition.item[s_count].system)
             {
              case 0x00: break; // Unknown system
              case 0x02: break; // XENIX
              case 0x05:  // Extended MS-DOS
                   tc_disk::build_drive(s_start+s_partition->partition.item[s_count].first_position);
                   break;
              case 0x01:  // MS-DOS 12bit FAT & 16bit sector number
              case 0x06:  // MS-DOS 32bit sector number
                   s_drive=new t_drive(
                            this,
                            s_partition->partition.item[s_count].system,
                            g_drive_name++,
                            s_partition->partition.item[s_count].first_position+s_start,
                            s_partition->partition.item[s_count].first_position+s_start+s_partition->partition.item[s_count].sectors,
                            s_partition->partition.item[s_count].sectors,
                            True
                           );
                   tc_disk::add_drive(s_drive);
                   s_return=True;
                   break;
              case 0x51: break; // Ontrack extended
              case 0x64: break; // Nobell
              case 0x75: break; // PCIX
              case 0x80: break; // Minix
              case 0x81: break; // Linux/Minix
              case 0x82: break; // Linux swap
              case 0x83: break; // Linux native
              case 0xdb: break; // CP/M
              case 0xdd: break; // BBT
              default: break;
             }
            }
            delete(s_partition);
           }
           return(s_return);
}

unsigned int far pascal tc_disk::reset(void)
{
 unsigned int s_return, s_recount=tc_disk::recount;
 unsigned int s_heads=0U, s_cylinders=0U, s_sectors_per_track=0U;
           while(s_recount--)
           {
            s_return=biosdisk(D_BIOSDISK_RESET, tc_disk::disk_num, 0U, 0U, 1U, 1U, MK_FP(g_segment_stack, 0x0000));
            if(!s_return)break;
           }
           if(!s_recount)return(0xffff);
           s_recount=tc_disk::recount;
           s_return=tc_disk::disk_num;
 l_get_parameter_0:;
           asm mov ax, 0x0801;
           asm mov dl, byte ptr s_return;
           bios_disk_interrupt();
           asm jnb short l_get_parameter_1;
           asm dec word ptr s_recount;
           if(s_recount==0x0000)return(0xffff);
           asm jmp short l_get_parameter_0;
 l_get_parameter_1:;
           asm mov byte ptr s_cylinders, ch;
           asm mov byte ptr s_sectors_per_track, cl;
           asm mov byte ptr s_heads, dh;
           tc_disk::cylinders=(s_cylinders|((s_sectors_per_track<<2)&0xff00))+1U;
           tc_disk::sectors_per_track=s_sectors_per_track&0x3f;
           tc_disk::heads=s_heads+1U;
           if(!tc_disk::cylinders||!tc_disk::sectors_per_track||!tc_disk::heads)return(0xffff);
           else return(0U);
}

unsigned int far pascal tc_disk::status(void)
{
 unsigned int s_return, s_recount=tc_disk::recount;
           while(s_recount--)
           {
            s_return=biosdisk(D_BIOSDISK_STATUS, tc_disk::disk_num, 0U, 0U, 1U, 1U, NULL);
            if(!s_return)break;
           }
           return(s_return);
}

unsigned int far pascal tc_disk::read(unsigned int s_head, unsigned int s_cylinder, unsigned int s_sector, unsigned int s_nsector, void far *s_buffer)
{
 unsigned int s_return, s_recount=tc_disk::recount;
           while(s_recount--)
           {
            s_return=biosdisk(D_BIOSDISK_READ, tc_disk::disk_num, s_head, s_cylinder, s_sector, s_nsector, s_buffer);
            if(!s_return)break;
           }
           return(s_return);
}

unsigned int far pascal tc_disk::write(unsigned int s_head, unsigned int s_cylinder, unsigned int s_sector, unsigned int s_nsector, void far *s_buffer)
{
 unsigned int s_return, s_recount=tc_disk::recount;
           while(s_recount--)
           {
            s_return=biosdisk(D_BIOSDISK_WRITE, tc_disk::disk_num, s_head, s_cylinder, s_sector, s_nsector, s_buffer);
            if(!s_return)break;
           }
           return(s_return);
}

unsigned int far pascal tc_disk::verify(unsigned int s_head, unsigned int s_cylinder, unsigned int s_sector, unsigned int s_nsector, void far *s_buffer)
{
 unsigned int s_return, s_recount=tc_disk::recount;
           while(s_recount--)
           {
            s_return=biosdisk(D_BIOSDISK_VERIFY, tc_disk::disk_num, s_head, s_cylinder, s_sector, s_nsector, s_buffer);
            if(!s_return)break;
           }
           return(s_return);
}

unsigned int far pascal tc_disk::format(unsigned int s_head, unsigned int s_cylinder, unsigned int s_sector, unsigned int s_interleave, void far *s_buffer)
{
 unsigned int s_return, s_recount=tc_disk::recount;
           while(s_recount--)
           {
            s_return=biosdisk(D_BIOSDISK_FORMAT, tc_disk::disk_num, s_head, s_cylinder, s_sector, s_interleave, s_buffer);
            if(!s_return)break;
           }
           return(s_return);
}

unsigned int far pascal tc_disk::absread(void far *s_buffer, unsigned long s_lsector, unsigned int s_count)
{
 unsigned int s_head, s_cylinder, s_sector, s_return;
           while(s_count--)
           {
            tc_disk::long2item(&s_head, &s_cylinder, &s_sector, s_lsector++);
            s_return=tc_disk::read(s_head, s_cylinder, s_sector, 1U, s_buffer);
            if(s_return)return(s_return);
            ((unsigned char far *)s_buffer)+=D_BYTES_PER_SECTOR;
           }
           return(s_return);
}

unsigned int far pascal tc_disk::abswrite(void far *s_buffer, unsigned long s_lsector, unsigned int s_count)
{
 unsigned int s_head, s_cylinder, s_sector, s_return;
           while(s_count--)
           {
            tc_disk::long2item(&s_head, &s_cylinder, &s_sector, s_lsector++);
            s_return=tc_disk::write(s_head, s_cylinder, s_sector, 1U, s_buffer);
            if(s_return)break;
            ((unsigned char far *)s_buffer)+=D_BYTES_PER_SECTOR;
           }
           return(s_return);
}

void far pascal tc_disk::print(void)
{
           if(tc_disk::access==False)return;
           g_text->tty_printf("  - Number of disk = 0x$02X \(Head: $u, Track: $u, Sector: $u\)\r\n", tc_disk::disk_num, tc_disk::heads, tc_disk::cylinders, tc_disk::sectors_per_track);
}

unsigned int far pascal tc_disk::add_drive(tc_drive far *s_drive)
{
           strcpy(&s_drive->path[0U], "C:/");
           if(g_drive_head==NULL)
           {
            s_drive->prev=s_drive->next=NULL;
            g_drive_tail=g_drive_head=s_drive;
           }
           else
           {
            s_drive->prev=g_drive_tail;
            s_drive->next=NULL;
            g_drive_tail->next=s_drive;
            g_drive_tail=s_drive;
           }
           return(True);
}

unsigned int far pascal tc_drive::absread(void far *s_buffer, unsigned long s_lsector, unsigned int s_length)
{
           if(s_lsector>tc_drive::end)return(0xffff);
           return(tc_drive::disk->absread(s_buffer, s_lsector+tc_drive::begin, s_length));
}

unsigned int far pascal tc_drive::abswrite(void far *s_buffer, unsigned long s_lsector, unsigned int s_length)
{
           if(s_lsector>tc_drive::end)return(0xffff);
           return(tc_drive::disk->abswrite(s_buffer, s_lsector+tc_drive::begin, s_length));
}

unsigned int far pascal tc_drive::fat1read(void far *s_buffer, unsigned long s_lsector, unsigned int s_length)
{
           return(tc_drive::absread(s_buffer, s_lsector+tc_drive::fat1, s_length));
}

unsigned int far pascal tc_drive::fat1write(void far *s_buffer, unsigned long s_lsector, unsigned int s_length)
{
           return(tc_drive::abswrite(s_buffer, s_lsector+tc_drive::fat1, s_length));
}

unsigned int far pascal tc_drive::fat2read(void far *s_buffer, unsigned long s_lsector, unsigned int s_length)
{
           return(tc_drive::absread(s_buffer, s_lsector+tc_drive::fat2, s_length));
}

unsigned int far pascal tc_drive::fat2write(void far *s_buffer, unsigned long s_lsector, unsigned int s_length)
{
           return(tc_drive::abswrite(s_buffer, s_lsector+tc_drive::fat2, s_length));
}

unsigned int far pascal tc_drive::rootread(void far *s_buffer, unsigned long s_lsector, unsigned int s_length)
{
           return(tc_drive::absread(s_buffer, s_lsector+tc_drive::root, s_length));
}

unsigned int far pascal tc_drive::rootwrite(void far *s_buffer, unsigned long s_lsector, unsigned int s_length)
{
           return(tc_drive::abswrite(s_buffer, s_lsector+tc_drive::root, s_length));
}

unsigned int far pascal tc_drive::clusterread(void far *s_buffer, unsigned long s_lsector, unsigned int s_length)
{
           return(tc_drive::absread(s_buffer, s_lsector+tc_drive::cluster, s_length));
}

unsigned int far pascal tc_drive::clusterwrite(void far *s_buffer, unsigned long s_lsector, unsigned int s_length)
{
           return(tc_drive::abswrite(s_buffer, s_lsector+tc_drive::cluster, s_length));
}

e_boolean far pascal tc_drive::rootname_to_filename(void far *s_filename, void far *s_rootname)
{
 unsigned char s_file[D_MAXFILE], s_ext[D_MAXEXT];
 unsigned int s_count;
           if(s_rootname==NULL)
           {
            *((unsigned char far *)s_filename)='\0';
            return(False);
           }
           memcpyb(&s_file[0U], s_rootname, D_MAXFILE-1U);
           memcpyb(&s_ext[0U], (((unsigned char far *)s_rootname)+(D_MAXFILE-1U)), D_MAXEXT-1U);
           s_file[D_MAXFILE-1U]=s_ext[D_MAXEXT-1U]='\0';
           for(s_count=0U;s_count<D_MAXFILE;s_count++)if(s_file[s_count]==' ')s_file[s_count]='\0';
           for(s_count=0U;s_count<D_MAXEXT;s_count++)if(s_ext[s_count]==' ')s_ext[s_count]='\0';
           strcpy(s_filename, &s_file[0U]);
           if(s_ext[0U]!='\0')
           {
            strcat(s_filename, ".");
            strcat(s_filename, &s_ext[0U]);
           }
           return(True);
}

e_boolean far pascal tc_drive::filename_to_rootname(void far *s_rootname, void far *s_filename)
{
 unsigned int s_count;
           memsetb(s_rootname, ' ', (D_MAXFILE-1U)+(D_MAXEXT-1U));
           if(strlen(s_filename)==0U)return(False);
           for(s_count=0U;s_count<D_MAXFILE;s_count++)
           {
            if(*((unsigned char far *)s_filename)=='.'||*((unsigned char far *)s_filename)=='\0')break;
            *(((unsigned char far *)s_rootname)+s_count)=*(((unsigned char far *)s_filename)++);
           }
           if(*((unsigned char far *)s_filename)=='.')
           {
            ((unsigned char far *)s_filename)++;
            for(s_count=(D_MAXFILE-1U);s_count<=(D_MAXFILE-1U)+(D_MAXEXT-1U);s_count++)
            {
             if(*((unsigned char far *)s_filename)=='\0')break;
             *(((unsigned char far *)s_rootname)+s_count)=*(((unsigned char far *)s_filename)++);
            }
           }
           return(True);
}

unsigned int far pascal tc_drive::found_name_in_root(t_root far *s_root, void far *s_name)
{
 t_sector far *s_sector;
 unsigned long s_count;
 unsigned int s_count_entry;
 unsigned char s_rootname[(D_MAXFILE-1U)+(D_MAXEXT-1U)];
           if(tc_drive::lower==False)strupr(s_name);
           tc_drive::filename_to_rootname(&s_rootname[0U], s_name);
           s_sector=new t_sector;
           for(s_count=0LU;s_count<tc_drive::number_of_root;s_count++)
           {
            tc_drive::rootread(s_sector, s_count);
            for(s_count_entry=0U;s_count_entry<(512U>>5U);s_count_entry++)
            {
             if(tc_drive::lower==False)strupr(&s_sector->root[s_count_entry].name[0U]);
             if(memcmpb(&s_rootname[0U], &s_sector->root[s_count_entry].name[0U], (D_MAXFILE-1U)+(D_MAXEXT-1U)))
             {
              memcpyb(s_root, &s_sector->root[s_count_entry], sizeof(t_root));
              return(True);
             }
            }
           }
           delete(s_sector);
           memsetb(s_root, 0U, sizeof(t_root));
           return(False);
}

unsigned int far pascal tc_drive::cluster_rw(void far *s_buffer, unsigned long s_cluster_number, unsigned int s_offset, unsigned int s_length, unsigned int s_mode)
{
           if(s_cluster_number<2LU)return(0xffff);
           if(s_length==0xffff)s_length=tc_drive::boot.boot.sectors_per_cluster;
           if(s_mode==D_READ)
           {
            return(tc_drive::clusterread(s_buffer, ((s_cluster_number-2LU)*tc_drive::boot.boot.sectors_per_cluster)+(unsigned long)s_offset, s_length));
           }
           return(tc_drive::clusterwrite(s_buffer, (s_cluster_number*tc_drive::boot.boot.sectors_per_cluster)+(unsigned long)s_offset, s_length));
}

t_fullname far * far pascal tc_drive::open_name(void far *s_filename)
{
 unsigned char s_name[D_MAXPATH];
 unsigned int s_nameptr=0U;
 t_fullname far *s_return=NULL, far *s_return_maker;
           if(s_filename==NULL)return(s_return);
           do
           {
            if(*((unsigned char far *)s_filename)=='\\'||*((unsigned char far *)s_filename)=='/'||*((unsigned char far *)s_filename)=='\0')
            {
             if(s_nameptr&&s_nameptr<((D_MAXFILE-1U)+(D_MAXEXT-1U)+1U+1U))
             {
              s_name[s_nameptr]='\0';
              if(s_return==NULL)s_return_maker=s_return=new t_fullname;
              else
              {
               s_return_maker->next=new t_fullname;
               s_return_maker=s_return_maker->next;
              }
              strcpy(&s_return_maker->name[0U], &s_name[0U]);
              s_return_maker->next=NULL;
             }
             if(*((unsigned char far *)s_filename)=='\0')break;
             ((unsigned char far *)s_filename)++;
             s_nameptr=0U;
            }
            else s_name[s_nameptr++]=*(((unsigned char far *)s_filename)++);
           }while(True);
           return(s_return);
}

void far pascal tc_drive::close_name(t_fullname far *s_filename)
{
 t_fullname far *s_closename;
           while(s_filename)
           {
            s_closename=s_filename->next;
            delete(s_filename);
            s_filename=s_closename;
           }
}

unsigned long far pascal tc_drive::next_cluster(unsigned long s_cluster)
{
 unsigned long s_nowfat1sector, s_return=0xffffffffLU;
           s_nowfat1sector=s_cluster/((unsigned long)D_FAT(16U));
           if(s_nowfat1sector!=tc_drive::nowfat1sector)
           {
            tc_drive::nowfat1sector=s_nowfat1sector;
            tc_drive::fat1read(&tc_drive::nowfat1.sector[0U], tc_drive::nowfat1sector, 1U);
           }
           s_nowfat1sector=s_cluster%((unsigned long)D_FAT(16U));
           s_return=(unsigned long)tc_drive::nowfat1.wsector[(unsigned int)s_nowfat1sector];
           return(s_return);
}

/* End of source */