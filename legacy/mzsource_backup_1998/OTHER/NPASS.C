#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys\stat.h>
#include<io.h>

#if defined __386__
 #include<i86.h>
#endif

#define StdOut            ((FILE *)stdout)
#define BUFFERSIZE        65000
#define PASSCODE          'n'

char                      filename[256] , password[257] , option=0 , passwordcmp[257];
char  far                 buffer[BUFFERSIZE];
int                       bp;
FILE                      *fr,*fw;
long                      passwordsize=0;
char far                  passdata[]="TwoN Group & SensSoft";

void far SoundB(void);
void far FileLock(void);
void far FileUnLock(void);
void far InputPassword(void);
long far GetFileSize(char far *);

void main(char argc,char *argv[])
{
 int  argvcount;
	   fprintf(StdOut,"\nTwoN Group's File-lock utillity version0.70\n");
	   fprintf(StdOut,"Made by JaeHyuk.CHO \(Pager:012-846-6420[In korea]\)\n\n");
	   if(argc<=1){
		       fprintf(StdOut,"USAGE: NPASS  -Option  <filename>       .ENTER.\n");
		       fprintf(StdOut,"\n***[ Option ]***");
		       fprintf(StdOut,"\n-P or -p or -L or -l      : File lock");
		       fprintf(StdOut,"\n-U or -u or -X or -x      : File unlock");
		       fprintf(StdOut,"\n\n");
		       exit(-1);
		       }
	   for(argvcount=0;argvcount<256;argvcount++)filename[argvcount]=password[argvcount]=0;
	   password[256]=0;
	   for(argvcount=1;argvcount<argc;argvcount++){
						       if(argv[argvcount][0]=='-'||argv[argvcount][0]=='/'||argv[argvcount][0]=='\\'){
																      if(argv[argvcount][2]!=0||argv[argvcount][1]==0){
																						       fprintf(StdOut,"ERROR: UnKnown option -> \"%s\"\n",argv[argvcount]);
																						       SoundB();
																						       exit(-1);
																						       }
																      else{
																	   option=argv[argvcount][1];
																	   }
																      }
						       else strcpy(filename,argv[argvcount]);
						       }
	   if(option==0){
			 fprintf(StdOut,"ERROR: Found not option!\n");
			 SoundB();
			 exit(-1);
			 }
	   if(filename[0]==0){
			      fprintf(StdOut,"ERROR: Found not filename!\n");
			      SoundB();
			      exit(-1);
			      }
	   if(option=='P'||option=='p'||option=='L'||option=='l')FileLock();
	   else if(option=='U'||option=='u'||option=='X'||option=='x')FileUnLock();
	   else{
		fprintf(StdOut,"ERROR: UnKnown option -> \"\-%c\"\n",option);
		SoundB();
		exit(-1);
		}
}

void far FileLock(void)
{
 long a_loop;
 long avail=BUFFERSIZE;
 long filesize;
 long counter;
	   InputPassword();
	   if(password[0]==0){
			      fprintf(StdOut,"\nSTOP: User cacel!!!\n");
			      SoundB();
			      exit(-1);
			      }
	   filesize=GetFileSize(filename);
	   if((fr=fopen(filename,"rb"))==0){
					    fprintf(StdOut,"\nERROR: Not exist %s\n",filename);
					    SoundB();
					    exit(-1);
					    }
	   if((fw=fopen("__npass_.swp","wb"))==0){
					    fprintf(StdOut,"\nERROR: Don't lock!\n");
					    SoundB();
					    fclose(fr);
					    exit(-1);
					    }
	   fclose(fw);
	   unlink("__npass_.swp");
	   if((fw=fopen("__npass_.swp","wb"))==0){
						  fprintf(StdOut,"\nERROR: Don't lock!\n");
						  SoundB();
						  fclose(fr);
						  exit(-1);
						  }
	   fputc('n',fw);
	   fputc('p',fw);
	   fputc('a',fw);
	   a_loop=(long)filesize/(long)BUFFERSIZE;
	   avail=(long)filesize%(long)BUFFERSIZE;
	   fprintf(StdOut,"\nLock");
	   for(;a_loop>0;a_loop--){
				    fread((void far *)buffer,BUFFERSIZE,1,fr);
				    for(counter=0;counter<BUFFERSIZE/2;counter++)buffer[counter]^=buffer[(BUFFERSIZE-counter)-1L]^PASSCODE;
				    for(counter=0;counter<BUFFERSIZE/2;counter++)buffer[(BUFFERSIZE-counter)-1L]^=buffer[counter]^passdata[counter%sizeof(passdata)];
				    fwrite((void far *)buffer,BUFFERSIZE,1,fw);
				    fprintf(StdOut,".");
				    }
	   for(counter=strlen(password)+1;counter<sizeof(password);counter++)password[counter]=random(256);
	   for(counter=0;counter<sizeof(password);counter++)password[counter]^=PASSCODE;
	   fwrite(password,sizeof(password),1,fw);

	   fread((void far *)buffer,avail,1,fr);
	   for(counter=0;counter<avail/2;counter++)buffer[counter]^=buffer[(avail-counter)-1L]^PASSCODE;
	   for(counter=0;counter<avail/2;counter++)buffer[(avail-counter)-1L]^=buffer[counter]^passdata[counter%sizeof(passdata)];
	   fwrite((void far *)buffer,avail,1,fw);
	   fclose(fr);
	   fclose(fw);
	   unlink(filename);
	   rename("__npass_.swp",filename);
	   fprintf(StdOut,"OK\n\nLocking complete...\n");
}

void far FileUnLock(void)
{
 long a_loop;
 long avail=BUFFERSIZE;
 long filesize;
 long counter;
	   InputPassword();
	   if(password[0]==0){
			      fprintf(StdOut,"\nSTOP: User cacel!!!\n");
			      SoundB();
			      exit(-1);
			      }
	   filesize=GetFileSize(filename);
	   if((fr=fopen(filename,"rb"))==0){
					    fprintf(StdOut,"\nERROR: Not exist %s\n",filename);
					    SoundB();
					    exit(-1);
					    }
	   if((fw=fopen("__npass_.swp","wb"))==0){
					    fprintf(StdOut,"\nERROR: Don't lock!\n");
					    SoundB();
					    fclose(fr);
					    exit(-1);
					    }
	   fclose(fw);
	   unlink("__npass_.swp");
	   if((fw=fopen("__npass_.swp","wb"))==0){
						  fprintf(StdOut,"\nERROR: Don't lock!\n");
						  SoundB();
						  fclose(fr);
						  exit(-1);
						  }
	   if(fgetc(fr)!='n'){
			      fprintf(StdOut,"\nMESSEGE: Don't Unlock!!!\n");
			      SoundB();
			      fclose(fr);
			      fclose(fw);
			      exit(-1);
			      }
	   if(fgetc(fr)!='p'){
			      fprintf(StdOut,"\nMESSEGE: Don't Unlock!!!\n");
			      SoundB();
			      fclose(fr);
			      fclose(fw);
			      exit(-1);
			      }
	   if(fgetc(fr)!='a'){
			      fprintf(StdOut,"\nMESSEGE: Don't Unlock!!!\n");
			      SoundB();
			      fclose(fr);
			      fclose(fw);
			      exit(-1);
			      }
	   a_loop=(long)(filesize-260L)/(long)BUFFERSIZE;
	   avail=(long)(filesize-260L)%(long)BUFFERSIZE;
	   fprintf(StdOut,"\nUnLock");
	   for(;a_loop>0;a_loop--){
				    fread((void far *)buffer,BUFFERSIZE,1,fr);
				    for(counter=0;counter<BUFFERSIZE/2;counter++)buffer[(BUFFERSIZE-counter)-1L]^=buffer[counter]^passdata[counter%sizeof(passdata)];
				    for(counter=0;counter<BUFFERSIZE/2;counter++)buffer[counter]^=buffer[(BUFFERSIZE-counter)-1L]^PASSCODE;
				    fwrite((void far *)buffer,BUFFERSIZE,1,fw);
				    fprintf(StdOut,".");
				    }
	   fread(passwordcmp,sizeof(passwordcmp),1,fr);
	   for(counter=0;counter<sizeof(passwordcmp);counter++)passwordcmp[counter]^=PASSCODE;
	   if((strcmp(password,passwordcmp)!=0)||(strlen(password)!=strlen(passwordcmp))){
						  fprintf(StdOut,"\nIncorrect password!!!\n");
						  fclose(fr);
						  fclose(fw);
						  unlink("__npass_.swp");
						  SoundB();
						  exit(-1);
						  }

	   fread((void far *)buffer,avail,1,fr);
	   for(counter=0;counter<avail/2;counter++)buffer[(avail-counter)-1L]^=buffer[counter]^passdata[counter%sizeof(passdata)];
	   for(counter=0;counter<avail/2;counter++)buffer[counter]^=buffer[(avail-counter)-1L]^PASSCODE;
	   fwrite((void far *)buffer,avail,1,fw);
	   fclose(fr);
	   fclose(fw);
	   unlink(filename);
	   rename("__npass_.swp",filename);
	   fprintf(StdOut,"OK\n\nUnLocking complete...\n");
}

void far InputPassword(void)
{
 int key,passwordoffset=0;
	   for(key=0;key<257;key++)passwordcmp[key]=0;
	   fprintf(StdOut,"Password(ESC:Cancel)? ");
	   PASS_LOOP:;
	   key=0;
	   if(kbhit()){
		       key=getch();
		       if(kbhit()||key==0){
					   key=getch();
					   key=0;
					   }
		       if(key==0x1b){
				     password[0]=0;
				     fprintf(StdOut,"\n");
				     return;
				     }
		       else if(key==0x0d){
					  fprintf(StdOut,"\n");
					  goto PASS_LOOP_EXIT;
					  }
		       else if(key==0x08){
					  if(passwordoffset>0){
							       password[--passwordoffset]=0;
							       fprintf(StdOut,"\b \b");
							       }
					  }
		       else{
			    if(passwordoffset<256){
						   password[passwordoffset++]=(char)key;
						   fprintf(StdOut,"*");
						   }
			    else{
				 fprintf(StdOut," 256Word!!!\n");
				 SoundB();
				 goto PASS_LOOP_EXIT;
				 }
			    }
		       }
	   goto PASS_LOOP;
	   PASS_LOOP_EXIT:;
	   passwordoffset=0;
	   fprintf(StdOut,"ReType(ESC:Cancel)? ");
	   PASS_LOOP_AGAIN:;
	   key=0;
	   if(kbhit()){
		       key=getch();
		       if(kbhit()||key==0){
					   key=getch();
					   key=0;
					   }
		       if(key==0x1b){
				     password[0]=0;
				     fprintf(StdOut,"\n");
				     return;
				     }
		       else if(key==0x0d){
					  if(strcmp(password,passwordcmp)!=NULL){
										 password[0]=0;
										 fprintf(StdOut,"\nERROR: ReType ! Password");
										 }
					  fprintf(StdOut,"\n");
					  passwordsize=strlen(password);
					  return;
					  }
		       else if(key==0x08){
					  if(passwordoffset>0){
							       passwordcmp[--passwordoffset]=0;
							       fprintf(StdOut,"\b \b");
							       }
					  }
		       else{
			    if(passwordoffset<256){
						   passwordcmp[passwordoffset++]=(char)key;
						   fprintf(StdOut,"*");
						   }
			    else{
				 fprintf(StdOut," 256Word!!!\n");
				 return;
				 }
			    }
		       }
	   goto PASS_LOOP_AGAIN;
}

void far SoundB(void)
{
	   sound(50);
	   delay(10);
	   nosound();
}

long far GetFileSize(char far *filename)
{
 int handle;
 long size;
	   handle=open(filename,O_RDONLY|O_BINARY);
	   size=filelength(handle);
	   close(handle);
	   return size;
}
