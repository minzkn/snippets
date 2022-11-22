/*
EAT.C ==>> Simple file compression program
      Written by Lee, Daeil April 1995  Nownuri, Hitel id cocohead
*/

#include <stdio.h>
#include <alloc.h>
#include <mem.h>

#if !defined(__COMPACT__)
#error  This program requires compact memory model.
#endif

typedef unsigned char   uchar;
typedef unsigned short  ushrt;
typedef unsigned long   ulong;

#define PBIT        15
#define DICSIZ      ((1<<PBIT)-1)
#define HASHSIZ     (1<<(PBIT-1))
#define MAXLEN      259
#define DEPTH       1024

#define PT          30
#define LT          29

#define NC          (256+LT+1)
#define NP          (PT+1)

#define BLSIZ       16384

#define getkey(r)   ((dic[r]^(dic[r+1]<<4)^(dic[r+2]<<8))&(HASHSIZ-1))
#define insnode(r)  (key=getkey(r),prev[r]=head[key],head[key]=r,hcnt[key]++)
#define delnode(s)  (key=getkey(s),hcnt[key]--,hcnt[key]&=-1)
#define initbits()  (outlen=16,inlen=bitbuf=0)
#define flushbits() putbits(0,15)
#define EncodeChar(t,c)     putbits(t[c].code, t[c].len)

uchar   *dic;
ushrt   *head, *prev, *buf;
short   *hcnt;

ushrt   matchpos, matchlen, key, bufp, blsiz, plen, ppos;

short   lbits[LT]
      = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};
short   lcode[LT]
      = {0,1,2,3,4,5,6,7,9,11,13,15,19,23,27,31,39,47,55,63,79,95,
           111,127,159,191,223,255,256};
short   dbits[PT]
      = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};
short   dcode[PT]
      = {0,1,2,3,5,7,11,15,23,31,47,63,95,127,191,255,383,511,767,1023,
           1535,2047,3071,4095,6143,8191,12287,16383,24575,32767};

short   ltbl[257], dtbl[512];

typedef struct {
        ushrt   freq;
        short   lson;
        short   rson;
} huftree;

typedef struct {
        ushrt   code;
        short   len;
} hufcode;

huftree ctree[NC*2], ptree[NP*2];
hufcode ccode[NC], pcode[NP];

short   croot, proot;

FILE    *infp, *outfp;
ulong   incount, outcount, filesize;
ushrt   outlen, inlen, bitbuf;


void putbits(ushrt a, short n)
{
    if(outlen <= n) {
        n -= outlen;
        bitbuf = (bitbuf << outlen) | (a >> n);
        putw(bitbuf, outfp); outcount += 2;
        outlen = 16 - n; bitbuf = a;
        return;
    }
    bitbuf = (bitbuf << n) | a;
    outlen -= n;
}

ushrt getbits(short n)
{
    ushrt a = 0;

    if(inlen > n) {
        a = bitbuf >> (16 - n);
        bitbuf <<= n; inlen -= n;
        return a;
    }
    n -= inlen;
    a = (bitbuf >> (16 - inlen)) << n;
    bitbuf = getw(infp); incount += 2;
    a = a | (bitbuf >> (16 - n));
    inlen = 16 - n; bitbuf <<= n;
    return a;
}

short maketree(huftree *tree, short size, short opt)
{
    short   i, min1, min2, n=size, r=2*size-1;

    tree[r].freq = 0xffff;

    while(n++) {
        min1 = min2 = r;
        for(i=0; i<n; i++)
        if(tree[i].freq != 0)
        if(tree[i].freq < tree[min1].freq) {
            min2 = min1; min1 = i;
        }
        else if(tree[i].freq < tree[min2].freq) min2 = i;
        if(min2 == r) break;
        if(opt) tree[n].freq = tree[min1].freq + tree[min2].freq;
        else tree[n].freq = tree[min1].freq + 1;
        tree[min1].freq = tree[min2].freq = 0;
        tree[n].lson = min1;
        tree[n].rson = min2;
    }
    return --n;
}

void makecode(tree, code, s, temp, bits, n)
huftree *tree;
hufcode *code;
short   s, bits, n;
ushrt   temp;
{
    if(n < s) {
        code[n].code = temp;
        code[n].len  = bits;
        return;
    }
    temp <<= 1; bits++;
    makecode(tree, code, s, temp, bits, tree[n].lson);
    makecode(tree, code, s, temp|1, bits, tree[n].rson);
}

short DecodeChar(huftree *tree, short s, short r)
{
    short n = r;
    do {
        if(getbits(1)) n = tree[n].rson;
        else n = tree[n].lson;
    } while(n > s);
    return n;
}

void putheader(tree, code, size, root)
huftree *tree;
hufcode *code;
short   size, root;
{
    short   i;

    root = maketree(tree, size, 1);
    makecode(tree, code, size, 0, 0, root);
    for(i=0; i<size; i++)
        tree[i].freq = code[i].len ? 16 - code[i].len : 0 ;
    for(i=0; i<size; i++) putbits(tree[i].freq, 4);
    root = maketree(tree, size, 0);
    makecode(tree, code, size, 0, 0, root);
}

short getheader(huftree *tree, short size)
{
    short   i;

    for(i=0; i<size; i++) tree[i].freq = getbits(4);
    return maketree(tree, size, 0);
}

void flush_block()
{
    ushrt   i, c, n;

    putbits(blsiz, 16);
    putheader(ctree, ccode, NC, croot);
    putheader(ptree, pcode, NP, proot);

    for(i=0; i<bufp;) {
        if((c=buf[i++]) < 256) {
            EncodeChar(ccode, c);
            continue;
        }
        if(c==0xffff) {
            EncodeChar(ccode, 256+LT);
            continue;
        }
        c -= 256;
        n =  ltbl[c];
        EncodeChar(ccode, 256 + n);
        putbits(lcode[n]-c, lbits[n]);
        c = buf[i++];
        if(c == 0xffff) {
            EncodeChar(pcode, PT);
            continue;
        }
        n = c < 256 ? dtbl[c] : dtbl[256+(c>>7)];
        EncodeChar(pcode, n);
        putbits(dcode[n]-c, dbits[n]);
    }
    bufp = blsiz = 0;
}

void output(short c, short p)
{
    if(p == -1) {
        buf[bufp++] = c;
        ctree[c].freq++;
    } else {
        if(c==plen && p==ppos) {
            buf[bufp++] = 0xffff;
            ctree[256+LT].freq++;
        } else {
            buf[bufp++] = 256 + c;
            ctree[256+ltbl[c]].freq++;
            if(p==ppos) {
                buf[bufp++] = 0xffff;
                ptree[PT].freq++;
            } else {
                buf[bufp++] = p;
                ptree[(p<256) ? dtbl[p] : dtbl[256+(p>>7)]].freq++;
            }
            plen = c;
            ppos = p;
        }
    }
    if(bufp >= BLSIZ-2) flush_block();
}

short longmatch(ushrt r)
{
    uchar   *s, *t, *p, *l;
    short   len, pos, n, ml=2;

    p = dic+(pos=r); l=p+MAXLEN-1;
    if((n=hcnt[key]-1) >= DEPTH) n = DEPTH;

    while(n--) {
        pos = prev[pos]; s = p - 1; t = dic + pos - 1;
        if(*(ushrt*)(s+ml) != *(ushrt*)(t+ml)) continue;
        while(*(ushrt*)(s+=2) == *(ushrt*)(t+=2) && s < l) ;
        if(*s == *t) s++;
        if((len = s-p) <= ml) continue;
        matchpos = pos; ml = len;
        if(len < MAXLEN) continue;
        ml = MAXLEN;
        break;
    }
    return ml;
}

void EncodeLZ()
{
    ushrt   r=0, s=MAXLEN, remaincode, size;
    short   full=0, i, dist, n;

    memset(hcnt, 0, HASHSIZ * sizeof(short));
    for(i=0, n=0; i<LT;) if(lcode[i] >= n) ltbl[n++] = i; else i++;
    for(i=0, n=0; i<16;) if(dcode[i] >= n) dtbl[n++] = i; else i++;
    for(n=0; i<PT;) if(dcode[i]>>7 >= n) dtbl[256+n++]=i; else i++;
    for(i=0; i<NC; i++) ctree[i].freq = 0;
    for(i=0; i<NP; i++) ptree[i].freq = 0;

    initbits();
    incount = remaincode = fread(&dic[DICSIZ], 1, DICSIZ, infp);
    memcpy(dic, &dic[DICSIZ], MAXLEN);
    bufp = blsiz = plen = ppos = 0;

    insnode(r);
    while(remaincode) {
        matchlen = longmatch(r);
        if((dist=r-matchpos-1) < 0) dist += DICSIZ;
        if(matchlen<3 || (matchlen == 3 && dist > 0x1000)) matchlen=1;
        if(matchlen > remaincode) matchlen = remaincode;

        blsiz += matchlen;
        if(matchlen < 3) output(dic[r], -1);
        else output(matchlen-3, dist);

        remaincode -= matchlen;
        for(i=0; i<matchlen; i++) {
            if(full) delnode(s);
            dic[s] = dic[s + DICSIZ];
            r = (r + 1) % DICSIZ;
            if(++s == DICSIZ) {
                full = 1; s = 0;
                size = fread(&dic[DICSIZ], 1, DICSIZ, infp);
                remaincode += size; incount += size;
                printf("(%3d%%)\r",incount * 100 / filesize);
            }
            insnode(r);
        }
    }
    flush_block();
    putbits(0, 16);
    flushbits();
}

void DecodeLZ()
{
    ushrt   r, i, n;
    short   c, pos, len;

    initbits();
    r = outcount = blsiz = plen = ppos = 0;

    while(outcount < filesize) {
        if(blsiz == 0) {
            blsiz = getbits(16);
            if(blsiz == 0) break;
            croot = getheader(ctree, NC);
            proot = getheader(ptree, NP);
        }

        c = DecodeChar(ctree, NC, croot);

        if(c < 256) {
            dic[r] = c;
            putc(c, outfp); outcount++;
            blsiz--;
            r = (r + 1) % DICSIZ;
            continue;
        }

        if(c==256+LT) {
            len = plen;
            pos = ppos;
        } else {
            c -= 256;
            len = lcode[c] - getbits(lbits[c]) + 3;
            c = DecodeChar(ptree, NP, proot);
            if(c==PT) pos = ppos;
            else pos = dcode[c] - getbits(dbits[c]);
            plen = len;
            ppos = pos;
        }

        if((pos = r - pos - 1) < 0) pos += DICSIZ;

        for(i=0; i<len; i++) {
            putc(dic[pos], outfp);
            dic[r] = dic[pos];
            pos = (pos + 1) % DICSIZ;
            r   = (r   + 1) % DICSIZ;
        }
        outcount += len;
        blsiz -= len;
    }
}

void main(int argc, char **argv)
{
    int o;

    if(argc < 4) {
        printf("\a\nUsage : EAT e|d source target\n");
        printf("\ne : encoding (compress)\nd : decoding (uncompress)\n");
        return;
    }
    if((infp = fopen(argv[2], "rb")) == NULL) {
        printf("\nfile not found %s\n", argv[2]);
        return;
    }

    outfp = fopen(argv[3], "wb");
    o     = *argv[1];

    dic = (uchar *) malloc(DICSIZ * 2);
    if(dic == NULL) {
        printf( "Memory error\n" );
        return;
    }

    if(o == 'E' || o == 'e') {
        head = (ushrt *) malloc(HASHSIZ * sizeof( ushrt));
        hcnt = (short *) malloc(HASHSIZ * sizeof( short));
        prev = (ushrt *) malloc(DICSIZ  * sizeof( ushrt));
        buf  = (ushrt *) malloc(BLSIZ   * sizeof( ushrt));
        if(head == NULL || hcnt == NULL || prev == NULL || buf == NULL) {
            printf( "Memory error\n" );
            return;
        }
        fseek(infp, 0L, 2);
        filesize = ftell(infp);
        fwrite(&filesize, sizeof filesize, 1, outfp);
        rewind(infp);
        outcount = sizeof filesize;

        EncodeLZ();

        free(head); free(hcnt); free(prev); free(buf);
        printf("In %lu, Out %lu  ==> (%3d%%)\n",
                incount, outcount, outcount*100/incount);
    } else {
        fread(&filesize, sizeof filesize, 1, infp);

        DecodeLZ();

        printf("Size %8lu, %8lu\n", filesize, outcount);
    }

    free(dic);

    fclose(infp);
    fclose(outfp);
}


