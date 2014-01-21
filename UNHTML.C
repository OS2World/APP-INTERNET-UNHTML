#define INCL_NOPMAPI

#include <ctype.h>
#include <sys/types.h>
#include <types.h>
#include <netinet\in.h>
#include <sys\socket.h>
#include <sys\select.h>
#include <netdb.h>
#include <stdio.h>
#include <conio.h>

#include <nerrno.h>
#include <ctype.h>



FILE *out,*in;

int main(int argc, char **argv)
{
  unsigned char buf[4096],buf1[255],buf2[255],buf3[255],tmp[4096],filename[255],outfile[255],*I,*O,*P;
  int x,y,z,flag,check;

  if ( argc < 2 )
    {
        printf("UNHTML: Usage: UNHTML file.html [file.txt]  []'s optional\r\n");
        return;
    }

 if ( argc == 2)
    {
     _splitpath(argv[1],buf,buf2,buf2,buf3);
      sprintf(outfile,"%s.txt",buf2);
    }

 if ( argc == 3 )  sprintf(outfile,"%s",argv[2]);

 if ( argc > 3 )
    {
        printf("UNHTML: Usage: UNHTML file.html [file.txt]  []'s optional\r\n");
        return;
    }

  sprintf(filename,"%s",argv[1]);

  if ( strstr(outfile,filename) ) sprintf(outfile,"%s1",outfile);


  printf("UNHTML: Writing to %s\r\n",outfile);
  out = fopen(outfile,"w");

  in = fopen(filename,"r");

  while ( !feof(in) )
  {
  fgets(buf,2048,in);

  x = strlen(buf);
  flag =1;

  while(flag)
  {
    I = strstr(buf,"<");
    if (I)
    {
     O = strstr(buf,">");
     if ( !O )
       {
        y=strlen(I);
        memset(I,0,y);
        *I='\n';
        flag=0;
       }
     else
        {
        *O=0;
        y=strlen(I);
        memset(I,0,y);
        O++;
        sprintf(tmp,"%s%s",buf,O);
        sprintf(buf,"%s",tmp);
        }
     }
    else
        {
         I = strstr(buf,">");
         if (I)
            {
               *I=0;
               y=strlen(buf);
               memset(buf,0,y);
               I++;
               if ( *I != 0) sprintf(buf,"%s",I);
            }
         else flag =0;
        }
    }

  I = strstr(buf,"&");
  if ( I )
    {
     check = 1;
     while(check)
        {
          O=strstr(buf,"&");
          if(!O) { check = 0; break; }
          P=O;
          P++;
          switch(*P)
            {
             case 'l':
             case 'L': if ( (*(P+1)=='T') || (*(P+1)=='t') )
                            {
                             memset(O,0,2);
                             P+=2;
                             if (*P ==59) { *P=0; P++;}
                             sprintf(tmp,"%s<%s",buf,P);
                             sprintf(buf,"%s",tmp);
                             break;
                            }

             case 'g':
             case 'G': if ( (*(P+1)=='t') || (*(P+1)=='T') )
                            {
                             memset(O,0,3);
                             P+=2;
                             if (*P ==59) { *P=0; P++;}
                             sprintf(tmp,"%s>%s",buf,P);
                             sprintf(buf,"%s",tmp);
                             break;
                            }

             case 'a':
             case 'A': if ( ((*(P+1)=='M') || (*(P+1)=='m')) && ((*(P+2)=='P') || (*(P+2)=='p')) )
                            {
                             memset(O,0,3);
                             P+=3;
                             if (*P ==59) { *P=0; P++;}
                             sprintf(tmp,"%s&%s",buf,P);
                             sprintf(buf,"%s",tmp);
                             break;
                            }

             case 'q':
             case 'Q': if ( ((*(P+1)=='U') || (*(P+1)=='u')) && ((*(P+2)=='O') || (*(P+2)=='o')) && ((*(P+3)=='T') || (*(P+3)=='t')) )
                            {
                             memset(O,0,4);
                             P+=4;
                             if (*P ==59) { *P=0; P++;}
                             sprintf(tmp,"%s\"%s",buf,P);
                             sprintf(buf,"%s",tmp);
                             break;
                            }


             default:  *O=255;  break;
            }

        }

     }
  for (x=0;x<strlen(buf);x++) if ( buf[x]==255) buf[x]='&';
  fputs(buf,out);

  }
  fclose(out);
  fclose(in);

}
