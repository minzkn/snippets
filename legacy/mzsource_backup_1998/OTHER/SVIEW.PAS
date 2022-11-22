program SectorViewer;

uses CRT;

var
   Ch : Char;
   LoopEvent : Boolean;
   HexString : String[16];
   Sectordata : array[1..512] of byte;  { 섹터 저장 장소 }
   SectorNum  : Integer;
   TrackNum   : Integer;
   HeadNum    : Integer;
   PageNum    : Integer;
procedure PutStatus;
var
   AbsSecNum : LongInt;
begin
   gotoxy(1,4);
   AbsSecNum := 9*TrackNum+SectorNum;
   write('Abs SectorNum :',AbsSecNum:7,'      Head:',HeadNum:4);
   write('     Track:',TrackNum:4,'     Sector:',SectorNum:4);
   write('         ',PageNum+1,'/2');
end;
procedure MainScreen;
begin
   { 그냥 폼내는 표시이다. }
   textcolor(15);
   textbackground(1);
   gotoxy(1,1);
   write('    Simple sectorviewer only A drive - Copyright (C) 1998 MINZ technologies     ');

   textbackground(0);
   textcolor(12);
   gotoxy(1,5);
   write('--------------------------------------------------------------------------------');
   gotoxy(1,22);
   write('--------------------------------------------------------------------------------');
   textcolor(15);
   gotoxy(1,23);
   write('Key - Page Up,Page Down    Esc - Exit');
   gotoxy(1,4);
end;
procedure GetSector( H:Byte ; T:Byte ; S:Byte ); { 섹터 내용 얻기 }
var
   d1,d2,d3 : Word;
begin
   d1 := (T shl 8) or S;
   d2 := (H shl 4) or $00;
   asm
      { Seg와 Ofs얻기 }
      mov ax,SEG Sectordata;
      mov bx,OFFSET Sectordata;
      mov es,ax;
      mov ax,0201h;
      mov cx,d1;
      mov dx,d2;
      int 13h;
   end;
end;
procedure ViewSector( PSwitch : Byte ); { 섹터 내용 출력 }
var
   { 카운트 전용 }
   Count : Word;
   C1,C2 : Byte;
begin
   C1 := 0;
   C2 := 0;
   PutStatus;
   for count:=1 to 256 do
   begin
      Inc(C1);
      if( C1 = 17 ) then
      begin
        Inc(C2);
        C1 := 1;
      end;
      gotoxy(3*C1+11, C2+6);
      { 16진수로 출력 }
      write( HexString[(Sectordata[256*PSwitch+count] div 16)+1] );
      write( HexString[(Sectordata[256*PSwitch+count] mod 16)+1] );
      { 아스키로 출력 소리 없이 찍기 위해 비디오 메모리에 직접 찍음 }
      mem[$b800:(160*(C2+5))+(126+2*C1)] := Sectordata[256*PSwitch+count];
      mem[$b800:(160*(C2+5))+(126+2*C1)+1] := 14;
   end;
end;

begin
   LoopEvent := TRUE;
   HexString := '0123456789ABCDEF'; { 16진수 출력 목적 문자열 초기화 }
   HeadNum   := 0;
   TrackNum  := 0;
   SectorNum := 1;
   PageNum   := 0;
   textcolor(7);
   textbackground(0);
   clrscr;
   MainScreen; { 주 메인 화면 출력 }
   GetSector( HeadNum , TrackNum , SectorNum );  { 섹터얻음 }
   ViewSector(0); { 섹터내용 출력 }
   while( LoopEvent ) do
   begin
      if( KeyPressed ) then
      begin
        Ch := ReadKey;
        if( Ch = Chr(0) ) then
        begin
          Ch := ReadKey;
          if( Ch = 'I' ) then
          begin
            Dec(PageNum);
            if( PageNum = -1 ) then
            begin
              PageNum := 1;
              Dec(SectorNum);
              if( SectorNum = 0 ) then
              begin
                 SectorNum := 1;
                 Dec(TrackNum);
                 if( TrackNum = -1 ) then TrackNum := 0;
              end;
              GetSector( HeadNum , TrackNum , SectorNum );  { 섹터얻음 }
            end else
              PageNum := 0;
            ViewSector(PageNum); { 섹터내용 출력 }
          end;
          if( Ch = 'Q' ) then
          begin
            Inc(PageNum);
            if( PageNum = 2 ) then
            begin
              PageNum := 0;
              Inc(SectorNum);
              if( SectorNum = 10 ) then
              begin
                 SectorNum := 1;
                 Inc(TrackNum);
              end;
              GetSector( HeadNum , TrackNum , SectorNum );  { 섹터얻음 }
            end else
              PageNum := 1;
            ViewSector(PageNum); { 섹터내용 출력 }
          end;
        end else
        begin
          if( Ch = Chr(27) ) then LoopEvent := FALSE;
        end;
      end;
   end;
   textcolor(7);
   textbackground(0);
   clrscr;
end.


