program viewdmaports;

uses CRT;

procedure outportb( port : Integer; data : Byte );
assembler;
asm
   mov dx,port;
   mov al,data;
   out dx,al;
end;

function inportb( P:Integer ) : Byte;
var
   T: Byte;
begin
   asm
       mov dx,P;
       in ax,dx;
       mov T,al;
   end;
   inportb := T;
end;

var
   A : Integer;
   TP : Integer;
   TV : Byte;
   TP2 : Integer;
   TV2 : Byte;
begin
   randomize;
   clrscr;
   TextMode(CO80 or FONT8x8);
   while( not KeyPressed ) do
   begin
       TP := random($1F);
       TV := random(255);
{      outportb(TP,TV);}
       gotoxy(30,20); write( TP:4 , ':' , TV:4 );
       gotoxy(5,1); writeln('Port 00h( DMA ) - ',inportb($00));
       gotoxy(5,2); writeln('Port 01h( DMA ) - ',inportb($01));
       gotoxy(5,3); writeln('Port 02h( DMA ) - ',inportb($02));
       gotoxy(5,4); writeln('Port 03h( DMA ) - ',inportb($03));
       gotoxy(5,5); writeln('Port 04h( DMA ) - ',inportb($04));
       gotoxy(5,6); writeln('Port 05h( DMA ) - ',inportb($05));
       gotoxy(5,7); writeln('Port 06h( DMA ) - ',inportb($06));
       gotoxy(5,8); writeln('Port 07h( DMA ) - ',inportb($07));
       gotoxy(5,9); writeln('Port 08h( DMA ) - ',inportb($08));
       gotoxy(5,10); writeln('Port 09h( DMA ) - ',inportb($09));
       gotoxy(5,11); writeln('Port 0Ah( DMA ) - ',inportb($0A));
       gotoxy(5,12); writeln('Port 0Bh( DMA ) - ',inportb($0B));
       gotoxy(5,13); writeln('Port 0Ch( DMA ) - ',inportb($0C));
       gotoxy(5,14); writeln('Port 0Dh( DMA ) - ',inportb($0D));
       gotoxy(5,15); writeln('Port 0Eh( DMA ) - ',inportb($0E));
       gotoxy(5,16); writeln('Port 0Fh( DMA ) - ',inportb($0F));
       gotoxy(5,17); writeln('Port 10h( DMA ) - ',inportb($10));
       gotoxy(5,18); writeln('Port 11h( DMA ) - ',inportb($11));
       gotoxy(5,19); writeln('Port 12h( DMA ) - ',inportb($12));
       gotoxy(5,20); writeln('Port 13h( DMA ) - ',inportb($13));
       gotoxy(5,21); writeln('Port 14h( DMA ) - ',inportb($14));
       gotoxy(5,22); writeln('Port 15h( DMA ) - ',inportb($15));
       gotoxy(5,23); writeln('Port 16h( DMA ) - ',inportb($16));
       gotoxy(5,24); writeln('Port 17h( DMA ) - ',inportb($17));
       gotoxy(5,25); writeln('Port 18h( DMA ) - ',inportb($18));
       gotoxy(5,26); writeln('Port 19h( DMA ) - ',inportb($19));
       gotoxy(5,27); writeln('Port 1Ah( DMA ) - ',inportb($1A));
       gotoxy(5,28); writeln('Port 1Bh( DMA ) - ',inportb($1B));
       gotoxy(5,29); writeln('Port 1Ch( DMA ) - ',inportb($1C));
       gotoxy(5,30); writeln('Port 1Dh( DMA ) - ',inportb($1D));
       gotoxy(5,31); writeln('Port 1Eh( DMA ) - ',inportb($1E));
       gotoxy(5,32); writeln('Port 1Fh( DMA ) - ',inportb($1F));
       gotoxy(30,1); writeln('Port 80h( DMAP) - ',inportb($80));
       gotoxy(30,2); writeln('Port 81h( DMAP) - ',inportb($81));
       gotoxy(30,3); writeln('Port 82h( DMAP) - ',inportb($82));
       gotoxy(30,4); writeln('Port 83h( DMAP) - ',inportb($83));
       gotoxy(30,5); writeln('Port 84h( DMAP) - ',inportb($84));
       gotoxy(30,6); writeln('Port 85h( DMAP) - ',inportb($85));
       gotoxy(30,7); writeln('Port 86h( DMAP) - ',inportb($86));
       gotoxy(30,8); writeln('Port 87h( DMAP) - ',inportb($87));
       gotoxy(30,9); writeln('Port 88h( DMAP) - ',inportb($88));
       gotoxy(30,10); writeln('Port 89h( DMAP) - ',inportb($89));
       gotoxy(30,11); writeln('Port 8Ah( DMAP) - ',inportb($8A));
       gotoxy(30,12); writeln('Port 8Bh( DMAP) - ',inportb($8B));
       gotoxy(30,13); writeln('Port 8Ch( DMAP) - ',inportb($8C));
       gotoxy(30,14); writeln('Port 8Dh( DMAP) - ',inportb($8D));
       gotoxy(30,15); writeln('Port 8Eh( DMAP) - ',inportb($8E));
       gotoxy(30,16); writeln('Port 8Fh( DMAP) - ',inportb($8F));
       gotoxy(55,1); writeln('Port C0h( DMAC) - ',inportb($C0));
       gotoxy(55,2); writeln('Port C1h( DMAC) - ',inportb($C1));
       gotoxy(55,3); writeln('Port C2h( DMAC) - ',inportb($C2));
       gotoxy(55,4); writeln('Port C3h( DMAC) - ',inportb($C3));
       gotoxy(55,5); writeln('Port C4h( DMAC) - ',inportb($C4));
       gotoxy(55,6); writeln('Port C5h( DMAC) - ',inportb($C5));
       gotoxy(55,7); writeln('Port C6h( DMAC) - ',inportb($C6));
       gotoxy(55,8); writeln('Port C7h( DMAC) - ',inportb($C7));
       gotoxy(55,9); writeln('Port C8h( DMAC) - ',inportb($C8));
       gotoxy(55,10); writeln('Port C9h( DMAC) - ',inportb($C9));
       gotoxy(55,11); writeln('Port CAh( DMAC) - ',inportb($CA));
       gotoxy(55,12); writeln('Port CBh( DMAC) - ',inportb($CB));
       gotoxy(55,13); writeln('Port CCh( DMAC) - ',inportb($CC));
       gotoxy(55,14); writeln('Port CDh( DMAC) - ',inportb($CD));
       gotoxy(55,15); writeln('Port CEh( DMAC) - ',inportb($CE));
       gotoxy(55,16); writeln('Port CFh( DMAC) - ',inportb($CF));
       gotoxy(55,17); writeln('Port D0h( DMAC) - ',inportb($D0));
       gotoxy(55,18); writeln('Port D1h( DMAC) - ',inportb($D1));
       gotoxy(55,19); writeln('Port D2h( DMAC) - ',inportb($D2));
       gotoxy(55,20); writeln('Port D3h( DMAC) - ',inportb($D3));
       gotoxy(55,21); writeln('Port D4h( DMAC) - ',inportb($D4));
       gotoxy(55,22); writeln('Port D5h( DMAC) - ',inportb($D5));
       gotoxy(55,23); writeln('Port D6h( DMAC) - ',inportb($D6));
       gotoxy(55,24); writeln('Port D7h( DMAC) - ',inportb($D7));
       gotoxy(55,25); writeln('Port D8h( DMAC) - ',inportb($D8));
       gotoxy(55,26); writeln('Port D9h( DMAC) - ',inportb($D9));
       gotoxy(55,27); writeln('Port DAh( DMAC) - ',inportb($DA));
       gotoxy(55,28); writeln('Port DBh( DMAC) - ',inportb($DB));
       gotoxy(55,29); writeln('Port DCh( DMAC) - ',inportb($DC));
       gotoxy(55,30); writeln('Port DDh( DMAC) - ',inportb($DD));
       gotoxy(55,31); writeln('Port DEh( DMAC) - ',inportb($DE));
       gotoxy(55,32); writeln('Port DFh( DMAC) - ',inportb($DF));
   end;
end.