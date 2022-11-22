/*signed int far ShellButton(WindowData far *n_wd, signed char far *n_default)
{
 signed int n_buttony[]={30, 70, 110};
 signed int n_key;
 signed int n_count;
            MC(N_False);
            drwfillbox(XOR, 11, n_wd->x1+345, n_wd->y1+n_buttony[*n_default]-5, n_wd->x1+435, n_wd->y1+n_buttony[*n_default]+35);
            drwfillbox(XOR, 11, n_wd->x1+350, n_wd->y1+n_buttony[*n_default], n_wd->x1+430, n_wd->y1+n_buttony[*n_default]+30);
            MC(N_True);
            shell_loop:;
            n_key=0; MouseProcess(); TimeProcess();
            if(s_mb==N_LEFTBUTTON)
            {
             for(n_count=0;n_count<3;n_count++)
             {
              if(MouseCheck(n_wd->x1+350, n_wd->y1+n_buttony[n_count], n_wd->x1+430, n_wd->y1+n_buttony[n_count]+30, N_LEFTBUTTON))
              {
               MC(N_False);
               PushRec(n_wd->x1+350, n_wd->y1+n_buttony[n_count], n_wd->x1+430, n_wd->y1+n_buttony[n_count]+30);
               if((*n_default)!=n_count)
               {
                drwfillbox(XOR, 11, n_wd->x1+345, n_wd->y1+n_buttony[*n_default]-5, n_wd->x1+435, n_wd->y1+n_buttony[*n_default]+35);
                drwfillbox(XOR, 11, n_wd->x1+350, n_wd->y1+n_buttony[*n_default], n_wd->x1+430, n_wd->y1+n_buttony[*n_default]+30);
                drwfillbox(XOR, 11, n_wd->x1+345, n_wd->y1+n_buttony[n_count]-5, n_wd->x1+435, n_wd->y1+n_buttony[n_count]+35);
                drwfillbox(XOR, 11, n_wd->x1+350, n_wd->y1+n_buttony[n_count], n_wd->x1+430, n_wd->y1+n_buttony[n_count]+30);
                *(n_default)=n_count;
               }
               MC(N_True);
               while(s_mb){MouseProcess();TimeProcess();}
               MC(N_False);
               PopRec(n_wd->x1+350, n_wd->y1+n_buttony[n_count], n_wd->x1+430, n_wd->y1+n_buttony[n_count]+30);
               MC(N_True);
               if(MouseCheck(n_wd->x1+350, n_wd->y1+n_buttony[n_count], n_wd->x1+430, n_wd->y1+n_buttony[n_count]+30, N_NOBUTTON))
               {
                switch(n_count)
                {
                 case 0x00: n_key=0x0d; break;
                 case 0x01: n_key=0x1b; break;
                 case 0x02: break;
                 default: break;
                }
                goto end_shell_loop;
               }
              }
             }
             if(n_count==3)goto end_shell_loop;
            }
            if(s_mb==N_RIGHTBUTTON)goto end_shell_loop;
            if(kbhit())
            {
             n_key=getch();
             if(n_key==0||kbhit())
             {
              n_key=getch(); MC(N_False);
              drwfillbox(XOR, 11, n_wd->x1+345, n_wd->y1+n_buttony[*n_default]-5, n_wd->x1+435, n_wd->y1+n_buttony[*n_default]+35);
              drwfillbox(XOR, 11, n_wd->x1+350, n_wd->y1+n_buttony[*n_default], n_wd->x1+430, n_wd->y1+n_buttony[*n_default]+30);
              MC(N_True);
              switch(n_key)
              {
               case 0x48:
                    (*n_default)--;
                    if((*n_default)<0)(*n_default)=2;
                    break;
               case 0x50:
                    (*n_default)++;
                    if((*n_default)>2)(*n_default)=0;
                    break;
               default: break;
              }
              MC(N_False);
              drwfillbox(XOR, 11, n_wd->x1+345, n_wd->y1+n_buttony[*n_default]-5, n_wd->x1+435, n_wd->y1+n_buttony[*n_default]+35);
              drwfillbox(XOR, 11, n_wd->x1+350, n_wd->y1+n_buttony[*n_default], n_wd->x1+430, n_wd->y1+n_buttony[*n_default]+30);
              MC(N_True);
              if(n_key==0x0f)n_key=0x09;
              else n_key=0;
             }
             switch(n_key)
             {
              case 0x09:
                   MC(N_False);
                   if(s_scankey[0x2a]||s_scankey[0x36])
                   {
                    if((*n_default)<=0)goto end_shell_loop;
                    drwfillbox(XOR, 11, n_wd->x1+345, n_wd->y1+n_buttony[*n_default]-5, n_wd->x1+435, n_wd->y1+n_buttony[*n_default]+35);
                    drwfillbox(XOR, 11, n_wd->x1+350, n_wd->y1+n_buttony[*n_default], n_wd->x1+430, n_wd->y1+n_buttony[*n_default]+30);
                    (*n_default)--;
                   }
                   else
                   {
                    if((*n_default)>=2)goto end_shell_loop;
                    drwfillbox(XOR, 11, n_wd->x1+345, n_wd->y1+n_buttony[*n_default]-5, n_wd->x1+435, n_wd->y1+n_buttony[*n_default]+35);
                    drwfillbox(XOR, 11, n_wd->x1+350, n_wd->y1+n_buttony[*n_default], n_wd->x1+430, n_wd->y1+n_buttony[*n_default]+30);
                    (*n_default)++;
                   }
                   drwfillbox(XOR, 11, n_wd->x1+345, n_wd->y1+n_buttony[*n_default]-5, n_wd->x1+435, n_wd->y1+n_buttony[*n_default]+35);
                   drwfillbox(XOR, 11, n_wd->x1+350, n_wd->y1+n_buttony[*n_default], n_wd->x1+430, n_wd->y1+n_buttony[*n_default]+30);
                   MC(N_True);
                   break;
              case 0x1b:
              case 0x0d:
                   goto end_shell_loop;
              default: break;
             }
             while(kbhit())getch();
            }
            goto shell_loop;
            end_shell_loop:;
            MC(N_False);
            drwfillbox(XOR, 11, n_wd->x1+345, n_wd->y1+n_buttony[*n_default]-5, n_wd->x1+435, n_wd->y1+n_buttony[*n_default]+35);
            drwfillbox(XOR, 11, n_wd->x1+350, n_wd->y1+n_buttony[*n_default], n_wd->x1+430, n_wd->y1+n_buttony[*n_default]+30);
            MC(N_True);
            if(s_mb==N_LEFTBUTTON)n_key=0;
            return(n_key);
}*/




                      //PutImageDisk(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, "jack07.jim", 0);
           /*drwbox(SET, BLACK, 450, 261, 470, 370);
           drwline(SET, BLACK, 451, 316, 469, 316);
           PopRec(451, 262, 469, 315);
           PopRec(451, 317, 469, 369);
           EngPutc(457, 281, 0x1e, WHITE, N_Default);
           EngPutc(456, 280, 0x1e, DARKGRAY, N_Default);
           EngPutc(457, 336, 0x1f, WHITE, N_Default);
           EngPutc(456, 335, 0x1f, DARKGRAY, N_Default);
           PutImageDisk(450, 130, 530, 240, "data.swp", 16);
           PushBox(110, 130, 530, 150, CYAN);*/
           //GetImageDisk(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, "jack07.jim", 0);





           //signed char far *n_bustring[]={"ÑÂ  ·¥", "Âá  ­¡", "®É‹¡“w"};
           //signed int n_count;
           //signed int n_cc;
           /*PutImageDisk(102, 102, 538, 378, "jackdata\\jack07.jim", 16);
           drwbox(SET, BLACK, 100, 100, 540, 380);
           PopRec(101, 101, 539, 379);
           drwfillbox(SET, BLUE, 104, 103, 536, 122);
           NonPutImageDisk(105, 97, 136, 128, "jackswap\\jack04.jim", 32, 32);
           PushBox(110, 130, 440, 150, CYAN);
           PushBox(110, 160, 440, 370, DARKGRAY);
           PushBox(450, 260, 530, 370, DARKGRAY);
           for(n_count=130, n_cc=0;n_count<=210;n_count+=40, n_cc++)
           {
            drwbox(SET, BLACK, 450, n_count, 530, n_count+30);
            PopRec(451, n_count+1, 529, n_count+29);
            Puts(467, n_count+9, n_bustring[n_cc], WHITE, N_Default);
            Puts(466, n_count+8, n_bustring[n_cc], BLACK, N_Default);
           }*/


           /*LoadPCX(0, 0, "mar.pcx");
           PutImageDisk(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, "jackdata\\jack08.jim", 16);
           drwbox(SET, BLACK, n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2);
           PopRec(n_wd.x1+1, n_wd.y1+1, n_wd.x2-1, n_wd.y2-1);
           drwfillbox(SET, BLUE, n_wd.x1+4, n_wd.y1+3, n_wd.x2-4, n_wd.y1+22);
           NonPutImageDisk(n_wd.x1+5, n_wd.y1-3, n_wd.x1+36, n_wd.y1+28, "jackswap\\jack04.jim", 32, 32);
           Puts(n_wd.x1+42, n_wd.y1+5, n_string, WHITE, N_Default);
           PopBox(n_wd.x2-20, n_wd.y1+6, n_wd.x2-7, n_wd.y1+19, LIGHTGRAY);
           drwline(SET, BLACK, n_wd.x2-17, n_wd.y1+9, n_wd.x2-10, n_wd.y1+16);
           drwline(SET, BLACK, n_wd.x2-10, n_wd.y1+9, n_wd.x2-17, n_wd.y1+16);
           drwline(SET, BLACK, n_wd.x2-16, n_wd.y1+9, n_wd.x2-11, n_wd.y1+16);
           drwline(SET, BLACK, n_wd.x2-11, n_wd.y1+9, n_wd.x2-16, n_wd.y1+16);*/



           /*signed int far GetImageXMS(signed int x1, signed int y1, signed int x2, signed int y2, unsigned int n_handle)
{
 unsigned long n_offset;
 signed int n_boffset;
 signed int cx, cy, cc;
 unsigned char far *n_buffer=new(unsigned char far [N_MAXBUFFER]);
           n_offset=sizeof(n_offset);
           n_boffset=0;
           for(cc=0;cc<5;cc++)
           {
            for(cy=y1+cc;cy<=y2;cy+=5)
            {
             for(cx=x1;cx<=x2;cx++)
             {
              *(n_buffer+n_boffset)=(unsigned char)getpoint(cx, cy);
              n_boffset++;
              if(n_boffset>=N_MAXBUFFER)
              {
               himemMEM2XMS(n_buffer, n_handle, n_offset, (unsigned long)N_MAXBUFFER);
               n_offset+=(unsigned long)N_MAXBUFFER;
               n_boffset=0;
              }
             }
            }
           }
           himemMEM2XMS(n_buffer, n_handle, n_offset, (unsigned long)n_boffset);
           n_offset+=(n_boffset+4);
           himemMEM2XMS(&n_offset, n_handle, 0L, (unsigned long)sizeof(n_offset));
           delete(n_buffer);
           return(N_True);
}

signed int far PutImageXMS(signed int x1, signed int y1, signed int x2, signed int y2, unsigned int n_handle)
{
 unsigned long n_offset;
 signed int n_boffset;
 signed int cx, cy, cc;
 unsigned char far *n_buffer=new(unsigned char far [N_MAXBUFFER]);
 unsigned long n_size;
           himemXMS2MEM(n_handle, &n_size, 0L, (unsigned long)sizeof(n_size));
           n_boffset=N_MAXBUFFER;
           n_offset=sizeof(n_size);
           for(cc=0;cc<5;cc++)
           {
            for(cy=y1+cc;cy<=y2;cy+=5)
            {
             for(cx=x1;cx<=x2;cx++)
             {
              if(n_boffset>=N_MAXBUFFER)
              {
               if((n_size/(unsigned long)N_MAXBUFFER)<(n_offset/(unsigned long)N_MAXBUFFER))n_boffset=(signed int)n_size%N_MAXBUFFER;
               else n_boffset=N_MAXBUFFER;
               himemXMS2MEM(n_handle, n_buffer, (unsigned long)n_offset, (unsigned long)n_boffset);
               n_offset+=(unsigned long)n_boffset;
               n_boffset=0;
              }
              drwpoint(SET, *(n_buffer+n_boffset), cx, cy);
              n_boffset++;
             }
            }
           }
           delete(n_buffer);
           return(N_True);
}
*/




           /*while(kbhit())getch();
           n_counter=0;
           drwfillbox(SET, BLACK, 0, 0, s_MaxX-1, s_MaxY-1);
           drwline(SET, WHITE, 0, s_MaxY>>1, s_MaxX-1, s_MaxY>>1);
           drwline(SET, WHITE, 0, (s_MaxY>>1)+1, s_MaxX-1, (s_MaxY>>1)+1);
           StartTimer(0, ClockTimer);
           while(!kbhit())
           {
            if(s_clock==N_True)
            {
             drwline(SET, BLACK, 0, s_MaxY>>1, n_counter, s_MaxY>>1);
             drwline(SET, BLACK, 0, (s_MaxY>>1)+1, n_counter, (s_MaxY>>1)+1);
             drwline(SET, BLACK, s_MaxX-n_counter-1, s_MaxY>>1, s_MaxX-1, s_MaxY>>1);
             drwline(SET, BLACK, s_MaxX-n_counter-1, (s_MaxY>>1)+1, s_MaxX-1, (s_MaxY>>1)+1);
             s_clock=N_False;
             n_counter+=50;
             if(n_counter>=(s_MaxX>>1))break;
            }
           }
           CloseTimer();
           while(kbhit())getch();*/