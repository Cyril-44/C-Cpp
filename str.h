// A simple header file
// Includes some string operation
// (Based for C) (Based for char[])
// Free and open source
// Author : Cyril Zhang
// E-mail : outlook_038C02E5408563E4@outlook.com

// Data type mini function
// Declaration
bool isbig(char);
bool issmall(char);
bool isnum(char);
bool isalph(char);
char tosmall(char);
char tobig(char);
// Realization
inline bool isbig(char c){return c>='A'&&c<='Z';}
inline bool issmall(char c){return c>='a'&&c<='z';}
inline bool isnum(char c){return c>='0'&&c<='9';}
inline bool isalph(char c){return isbig(c)||issmall(c);}
inline char tosmall(char c){return isbig(c)?c:c-'a'+'A';}
inline char tobig(char c){return issmall(c)?c:c-'A'+'a';}
// String mini function
// Declaration
void swaps(char*,char*);
void strset(char*,char);
bool streql(char*,char*);
bool strneql(char*,char*,int);
// Please #include <stdio.h> first
// Or you can't use instr(char*) or inword(char*,char[])
#ifdef _INC_STDIO
    void instr(char*);
    int inword(char*,char[],char);
#endif
//Realization
inline void strset(char *s,char c){if(*s!='\0') *s=c,strset(s+1,c);}
void swaps(char *s1,char *s2){
    if(*s1=='\0'&&*s2=='\0') return;
    char t;t=*s1,*s1=*s2,*s2=t;
    swaps(s1+1,s2+1);
}
bool streql(char *s1,char *s2){
    if(*s1=='\0'||*s2=='\0') return 0;
    if(*s1==*s2) return streql(s1+1,s2+1);
    else return 0;
}
bool strneql(char *s1,char *s2,int len){
    if(!len) return 1;
    if(*s1=='\0'||*s2=='\0') return 0;
    if(*s1==*s2) return strneql(s1+1,s2+1,len-1);
    else return 0;
}
#ifdef _INC_STDIO
    inline void instr(char *s){*s=getchar();if(*s!='\n') instr(s+1);else *s='\0';}
    int inword(char *s,const char chk[]){
        *s=getchar();bool f=1;
        if(*s=='\n') {*s='\0';return EOF;}
        for(int i=0;f&&chk[i]!='\0';i++)
           if(*s==chk[i]) f=0;
        if(f) return inword(s+1,chk);
        else {*s='\0';return 1;}
    }
#endif
// When the code has already #include <string.h>
#ifdef _INC_STRING
    // When the include path is not from Windows
    #ifndef __WINNT
        // Declaration
        void strupr(char*);
        void strlwr(char*);
        //Realization
        inline void strupr(char *s){if(*s!='\0'){if(issmall(*s)) *s-='a'-'A';strupr(s+1);}}
        inline void strlwr(char *s){if(*s!='\0'){if(isbig(*s)) *s+='a'-'A';strlwr(s+1);}}
    #endif
#else
    int strlen(char*);
    void strupr(char*);
    void strlwr(char*);
    void strcpy(char*);
    //Realization
    #ifdef _INC_STDIO
        inline void instr(char *s){*s=getchar();if(*s!='\n') instr(s+1);else *s='\0';}
        int inword(char *s,const char chk[]){
            *s=getchar();bool f=1;
            if(*s=='\n') {*s='\0';return EOF;}
            for(int i=0;f&&chk[i]!='\0';i++)
               if(*s==chk[i]) f=0;
            if(f) return inword(s+1,chk);
            else {*s='\0';return 1;}
        }
    #endif
    inline int strlen(char *s){if(*s=='\0') return 0;else return 1+strlen(s+1);}
    inline void strupr(char *s){if(*s!='\0'){if(issmall(*s)) *s-='a'-'A';strupr(s+1);}}
    inline void strlwr(char *s){if(*s!='\0'){if(isbig(*s)) *s+='a'-'A';strlwr(s+1);}}
    inline void strcpy(char *src,char *dest){if(*src!='\0'||*dest!='\0') *dest=*src,strcpy(src+1,dest+1);}
#endif
