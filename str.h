// A simple header file
// Includes some string operation
// (Based for C) (Based for char[])
// Free and open source
// Author : Cyril Zhang
// E-mail : outlook_038C02E5408563E4@outlook.com

// Declaration
// Please #include <stdio.h>
// Or you can't use instr(char*)
bool isbig(char);
bool issmall(char);
bool isnum(char);
bool isalph(char);
int strlen(char*);
#ifdef _INC_STDIO
    void instr(char*);
#endif
void strupr(char*);
void strlwr(char*);
void strcpy(char*);
void swaps(char*,char*);
bool strcmp(char*,char*);
bool strncmp(char*,char*,int);

// Realization
#ifdef _INC_STDIO
    int __cdecl getchar(void);
    inline void instr(char *s){*s=getchar();if(*s!='\n') instr(s+1);}
#endif
inline bool isbig(char c){return c>='A'&&c<='Z';}
inline bool issmall(char c){return c>='a'&&c<='z';}
inline bool isnum(char c){return c>='0'&&c<='9';}
inline bool isalph(char c){return isbig(c)||issmall(c);}
inline int strlen(char *s){if(*s=='\0') return 0;else return 1+strlen(s+1);}
inline void strupr(char *s){if(*s!='\0'){if(issmall(*s)) *s-='a'-'A';strupr(s+1);}}
inline void strlwr(char *s){if(*s!='\0'){if(isbig(*s)) *s+='a'-'A';strlwr(s+1);}}
inline void strcpy(char *src,char *dest){if(*src!='\0'||*dest!='\0') *dest=*src,strcpy(src+1,dest+1);}
void swaps(char *s1,char *s2){
    if(*s1=='\0'&&*s2=='\0') return;
    char t;t=*s1,*s1=*s2,*s2=t;
    swaps(s1+1,s2+1);
}
bool strcmp(char *s1,char *s2){
    if(*s1=='\0'||*s2=='\0') return 0;
    if(*s1==*s2) return strcmp(s1+1,s2+1);
    else return 0;
}
bool strncmp(char *s1,char *s2,int len){
    if(!len) return 1;
    if(*s1=='\0'||*s2=='\0') return 0;
    if(*s1==*s2) return strncmp(s1+1,s2+1,len-1);
    else return 0;
}
