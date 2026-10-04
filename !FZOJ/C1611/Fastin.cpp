struct fastin{
	char ch[1<<21],*bg=0,*ed=0;
	char getc(){
		if(bg==ed)ed=(bg=ch)+fread(ch,1,sizeof(ch),stdin);
		return *(bg++);
	}
	fastin& operator>>(int& x){
		x=0;
		char c=getc();
		while(c<'0'||c>'9')c=getc();
		while(c>='0'&&c<='9')x=(x<<3)+(x<<1)+(c^'0'),c=getc();
		return *this;
	}
	fastin& operator>>(long long& x){
		x=0;
		char c=getc();
		while(c<'0'||c>'9')c=getc();
		while(c>='0'&&c<='9')x=(x<<3)+(x<<1)+(c^'0'),c=getc();
		return *this;
	}
}fin;
#define cin fin
