
#include<bits/stdc++.h>
#include<windows.h>
using namespace std;
const long long MINN=1e8;
const long long MAXN=4e8;
const int clean_ten=1;
const int digit_complex_2=2;
const int digit_complex_3=3;
const int digit_complex_4=4;
const int digit_complex_5=5;
const int digit_complex_6=6;
const int digit_complex_7=7;
const int digit_complex_8=8;
const int digit_complex_9=9;
const int repeat_4=10;
const int repeat_5=11;
const int repeat_6=12;
const int repeat_7=13;
const int repeat_8=14;
const int repeat_9=15;
const int prime_badge=16;
const int high_man=17;
const int basketball_champion=18;
const int icu_badge=19;
const int monitor_badge=20;
const int li_yin_yang=21;
const int duck_neck=22;
const int old_hill=23;
const int negro_badge=24;
const int big_plate_chicken=25;
const int chicken_dust=26;
const int kevin_divisible=27;
const int plus_seven=28;
const int blue_archive=29;
const int iron_ke=30;
const int chicken_neck=31;
const int mason_xu=32;
const int red_chicken=33;
const int znso4_badge=34;
const int even_steven=35;
const int odd_todd=36;
const int homo_3=37;
const int homo_4=38;
const int homo_5=39;
const int full_homo=40;
const int half_homo_b_side=41;
const int homo_b_side=42;
const int kevin_badge=43;
const int o_kevin=44;
const int almost_balanced=45;
const int consecutive_3=46;
const int consecutive_2=47;
const int consecutive_4=48;
const int consecutive_5=49;
const int consecutive_6=50;
const int consecutive_7=51;
const int consecutive_8=52;
const int consecutive_9=53;
const int contiguous_2=54;
const int contiguous_3=55;
const int contiguous_4=56;
const int contiguous_5=57;
const int contiguous_6=58;
const int contiguous_7=59;
const int contiguous_8=60;
const int alternator_badge=61;
const int six_seven=62;
const int six_seven_divisible=63;
const int two_pair=64;
const int two_contiguous_pair=65;
const int three_pair=66;
const int three_contiguous_pair=67;
const int four_pair=68;
const int four_contiguous_pair=69;
const int second_power=70;
const int third_power=71;
const int fourth_power=72;
const int high_power=73;
const int wide_badge=74;
const int slim_badge=75;
const int palindrome_badge=76;
const int clean_hundred=77;
const int clean_thousand=78;
const int clean_ten_thousand=79;
const int clean_hundred_thousand=80;
const int clean_million=81;
const int clean_ten_million=82;
const int clean_hundred_million=83;
const int binary_soul=84;
const int low_ball=85;
const int high_roller=86;
const int steps_badge=87;
const int slopes_badge=88;
const int mountain_badge=89;
const int mesa_badge=90;
const int valley_badge=91;
const int canyon_badge=92;
const int hills_badge=93;
const int dunes_badge=94;
const int divisible_by_3=95;
const int abyss_badge=96;
const int chemistry_0=97;
const int chemistry_1=98;
const int chemistry_2=99;
const int chemistry_3=100;
const int chemistry_4=101;
const int chemistry_5=102;
const int chemistry_6=103;
const int chemistry_7=104;
const int chemistry_8=105;
const int chemistry_9=106;
const int mini_echo=107;
const int medium_echo=108;
const int major_echo=109;
const int mini_palindrome_3=110;
const int mini_palindrome_4=111;
const int mini_palindrome_5=112;
const int mini_palindrome_6=113;
const int mini_palindrome_7=114;
const int mini_palindrome_8=115;
const int binary_clean=116;
const int double_binary_clean=117;
const int harshed_number=118;
const int beginner_feather=119;
const int beginner_heavy=120;
const int intermediate_feather=121;
const int intermediate_heavy=122;
const int advanced_feather=123;
const int advanced_heavy=124;
const int expert_feather=125;
const int expert_heavy=126;
const int grandmaster_feather=127;
const int grandmaster_heavy=128;
const int german_whisper=129;
const int turtle_badge=130;
const int full_house=131;
const int two_triplet=132;
const int echo_form=133;
const int echo_form_plus=134;
const int echo_form_echo_form=135;
const int consecutive_pairs=136;
const int airplane_badge=137;
const int zipper_badge=138;
const int three_triplet=139;
const int triple_airplane=140;
const int consecutive_pairs_plus=141;
const int neutrality_badge=142;
const int even_spacing_absolute=143;
const int almost_sorted=144;
const int skipping_badge=145;
const int six_six_six=146;
const int calender_badge=147;
const int emergency_badge=148;
const int not_found=149;
const int silence_in_sea=150;
const int hopscotch_badge=151;
const int hyper_hopscotch=152;
const int ultra_hopscotch=153;

const int type_special=0;
const int type_needab=1;
const int type_divide=2;
const int type_contain=3;
const int type_differ=4;

typedef unsigned long long ull;
ull seed;

ull xs64()
{
    seed^=seed>>11;
    seed^=seed<<45;
    seed^=seed>>14;
    return seed;
}

void askuser();

void setcolor(int x)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),x);
}

struct badge
{
    string name;
    string desc;
    long long poss=0;
    int testtype;
};

bool notp[400000005];
long long prm[50000005];
long long points[100005];
int prmn=0;

void initprime()
{
    for(long long i=2; i<=400000000; i++)
    {
        if(!notp[i])
        {
            prm[++prmn]=i;
        }
        for(int j=1; j<=prmn; j++)
        {
            if(i*prm[j]>400000000)
                break;
            notp[i*prm[j]]=1;
            if(i%prm[j]==0)
                break;
        }
    }
}

long long highpower[10005];
int highpowern=0;

void inithighpower()
{
    for(long long i=2; i<=100; i++)
    {
        long long x=i*i*i*i*i;
        while(x<=MAXN)
        {
            highpower[++highpowern]=x;
            x*=i;
        }
    }
    sort(highpower+1,highpower+highpowern+1);
}



const int BLACK=0;
const int BLUE=1;
const int GREEN=2;
const int CYAN=3;
const int RED=4;
const int PURPLE=5;
const int YELLOW=6;
const int WHITE=7;
const int GRAY=8;
const int LIGHTBLUE=9;
const int LIGHTGREEN=10;
const int LIGHTCYAN=11;
const int LIGHTRED=12;
const int LIGHTPURPLE=13;
const int LIGHTYELLOW=14;
const int LIGHTWHITE=15;

int printrarity(long long x)
{
    if(x>3e7)
    {
        setcolor(GREEN);
        cout<<"Common";
        setcolor(7);
        return 1;
    }
    if(x>3e6)
    {
        setcolor(LIGHTYELLOW);
        cout<<"Unusual";
        setcolor(7);
        return 2;
    }
    if(x>3e5)
    {
        setcolor(BLUE);
        cout<<"Rare";
        setcolor(7);
        return 3;
    }
    if(x>3e4)
    {
        setcolor(LIGHTPURPLE);
        cout<<"Epic";
        setcolor(7);
        return 4;
    }
    if(x>3e3)
    {
        setcolor(RED);
        cout<<"Legendary";
        setcolor(7);
        return 5;
    }
    if(x>300)
    {
        setcolor(LIGHTCYAN);
        cout<<"Mythic";
        setcolor(7);
        return 6;
    }
    if(x>30)
    {
        setcolor(LIGHTRED);
        cout<<"Ultra";
        setcolor(7);
        return 7;
    }
    setcolor(LIGHTGREEN);
    cout<<"Super";
    setcolor(7);
    return 8;
}

long long pw10[20];

bool iscontain(long long x,long long y,int yn)
{
    for(int i=9-yn; i>=0; i--)
    {
        if(x/pw10[i]%pw10[yn]==y)
            return 1;
    }
    return 0;
}

badge badge[100005];
int badgen=0;
int badgeorder[100005];
pair<int,int> srttmp[100005];
int primequerycnt=0;

int havebadge(long long x,int y)
{
    if(badge[y].testtype==type_special)
    {
        if(y==prime_badge)
        {
            primequerycnt++;
            if(primequerycnt>=10000 && !prmn)
                initprime();
            if(prmn)
                return !notp[x];
            for(long long i=2; i*i<=x; i++)
            {
                if(x%i==0)
                    return 0;
            }
            return 1;
        }
        if(y==second_power)
        {
            long long xx=sqrt(x)+0.5;
            return xx*xx==x;
        }
        if(y==third_power)
        {
            long long xx=pow(x,1/3.0)+0.5;
            return xx*xx*xx==x;
        }
        if(y==fourth_power)
        {
            long long xx=sqrt(sqrt(x))+0.5;
            return xx*xx*xx*xx==x;
        }
        if(y==high_power)
        {
            if(highpowern==0)
                inithighpower();
            int l=1,r=highpowern;
            while(l<=r)
            {
                int mid=(l+r)/2;
                if(highpower[mid]==x)
                    return 1;
                if(x>highpower[mid])
                    l=mid+1;
                else
                    r=mid-1;
            }
            return 0;
        }
        int a[10];
        long long tmp=x;
        for(int i=9; i>=1; i--)
        {
            a[i]=tmp%10;
            tmp/=10;
        }
        if(y>=hopscotch_badge && y<=ultra_hopscotch)
        {
            if(y==hopscotch_badge)
            {
                for(int i=1; i<=5; i++)
                    if(a[i]==a[i+2] && a[i+2]==a[i+4])
                        return 1;
                return 0;
            }
            if(y==hyper_hopscotch)
            {
                for(int i=1; i<=3; i++)
                    if(a[i]==a[i+2] && a[i+2]==a[i+4] && a[i+4]==a[i+6])
                        return 1;
                return 0;
            }
            if(y==ultra_hopscotch)
            {
                return a[1]==a[3] && a[3]==a[5] && a[5]==a[7] && a[7]==a[9];
            }
        }
        if(y==neutrality_badge || y==almost_sorted)
        {
            int cnt1=0,cnt2=0;
            for(int i=1; i<=8; i++)
                for(int j=i+1; j<=9; j++)
                    cnt1+=(a[i]>a[j]),cnt2+=(a[i]<a[j]);
            if(y==neutrality_badge)
                return cnt1==cnt2;
            if(y==almost_sorted)
                return cnt1==1 || cnt2==1;
        }
        if(y==zipper_badge)
        {
            return a[1]==a[3] && a[3]==a[5] && a[5]==a[7] && a[7]==a[9] && a[2]==a[4] && a[4]==a[6]
                   && a[6]==a[8];
        }
        if(y==echo_form)
        {
            for(int i=1; i<=4; i++)
                if(a[i]==a[i+2] && a[i+2]==a[i+4] && a[i+1]==a[i+3] && a[i+3]==a[i+5])
                    return 1;
            return 0;
        }
        if(y==echo_form_plus)
        {
            for(int i=1; i<=2; i++)
                if(a[i]==a[i+2] && a[i+2]==a[i+4] && a[i+4]==a[i+6] && a[i+1]==a[i+3] && a[i+3]==a[i+5]
                        && a[i+5]==a[i+7])
                    return 1;
            return 0;
        }
        if(y==echo_form_echo_form)
        {
            return a[1]==a[4] && a[4]==a[7] && a[2]==a[5] && a[5]==a[8] && a[3]==a[6] && a[6]==a[9];
        }
        if(y>=beginner_feather && y<=grandmaster_heavy)
        {
            int sum=a[1]+a[2]+a[3]+a[4]+a[5]+a[6]+a[7]+a[8]+a[9];
            if(y==beginner_feather)
                return sum<=25;
            if(y==beginner_heavy)
                return sum>=50;
            if(y==intermediate_feather)
                return sum<=20;
            if(y==intermediate_heavy)
                return sum>=55;
            if(y==advanced_feather)
                return sum<=15;
            if(y==advanced_heavy)
                return sum>=60;
            if(y==expert_feather)
                return sum<=10;
            if(y==expert_heavy)
                return sum>=65;
            if(y==grandmaster_feather)
                return sum<=5;
            if(y==grandmaster_heavy)
                return sum>=70;
        }
        if(y==harshed_number)
        {
            return x%(a[1]+a[2]+a[3]+a[4]+a[5]+a[6]+a[7]+a[8]+a[9])==0;
        }
        if(y>=mini_palindrome_3 && y<=mini_palindrome_8)
        {
            if(y==mini_palindrome_3)
            {
                for(int i=1; i<=7; i++)
                    if(a[i]==a[i+2])
                        return 1;
                return 0;
            }
            if(y==mini_palindrome_4)
            {
                for(int i=1; i<=6; i++)
                    if(a[i]==a[i+3] && a[i+1]==a[i+2])
                        return 1;
                return 0;
            }
            if(y==mini_palindrome_5)
            {
                for(int i=1; i<=5; i++)
                    if(a[i]==a[i+4] && a[i+1]==a[i+3])
                        return 1;
                return 0;
            }
            if(y==mini_palindrome_6)
            {
                for(int i=1; i<=4; i++)
                    if(a[i]==a[i+5] && a[i+1]==a[i+4] && a[i+2]==a[i+3])
                        return 1;
                return 0;
            }
            if(y==mini_palindrome_7)
            {
                for(int i=1; i<=3; i++)
                    if(a[i]==a[i+6] && a[i+1]==a[i+5] && a[i+2]==a[i+4])
                        return 1;
                return 0;
            }
            if(y==mini_palindrome_8)
            {
                for(int i=1; i<=2; i++)
                    if(a[i]==a[i+7] && a[i+1]==a[i+6] && a[i+2]==a[i+5] && a[i+3]==a[i+4])
                        return 1;
                return 0;
            }
        }
        if(y==mini_echo)
        {
            for(int i=1; i<=6; i++)
            {
                if(a[i]==a[i+2] && a[i+1]==a[i+3])
                    return 1;
            }
            return 0;
        }
        if(y==medium_echo)
        {
            for(int i=1; i<=4; i++)
            {
                if(a[i]==a[i+3] && a[i+1]==a[i+4] && a[i+2]==a[i+5])
                    return 1;
            }
            return 0;
        }
        if(y==major_echo)
        {
            for(int i=1; i<=2; i++)
            {
                if(a[i]==a[i+4] && a[i+1]==a[i+5] && a[i+2]==a[i+6] && a[i+3]==a[i+7])
                    return 1;
            }
            return 0;
        }
        if(y==wide_badge || y==slim_badge)
        {
            int maxx=-1,minn=10;
            for(int i=1; i<=9; i++)
            {
                maxx=max(maxx,a[i]);
                minn=min(minn,a[i]);
            }
            if(y==wide_badge)
                return maxx-minn==9;
            if(y==slim_badge)
                return maxx-minn<=3;
        }
        if(y==palindrome_badge)
        {
            return (a[1]==a[9] && a[2]==a[8] && a[3]==a[7] && a[4]==a[6]);
        }
    }
    if(badge[y].testtype==type_divide)
    {
        if(y==clean_ten)
            return x%10==0;
        if(y==high_man)
            return x%2==0;
        if(y==basketball_champion)
            return x%3==0;
        if(y==icu_badge)
            return x%5==0;
        if(y==monitor_badge)
            return x%7==0;
        if(y==li_yin_yang)
            return x%11==0;
        if(y==duck_neck)
            return x%13==0;
        if(y==old_hill)
            return x%15==0;
        if(y==negro_badge)
            return x%17==0;
        if(y==big_plate_chicken)
            return x%19==0;
        if(y==chicken_dust)
            return x%23==0;
        if(y==kevin_divisible)
            return x%27==0;
        if(y==plus_seven)
            return x%29==0;
        if(y==blue_archive)
            return x%31==0;
        if(y==iron_ke)
            return x%37==0;
        if(y==chicken_neck)
            return x%41==0;
        if(y==mason_xu)
            return x%43==0;
        if(y==red_chicken)
            return x%47==0;
        if(y==znso4_badge)
            return x%49==0;
        if(y==six_seven_divisible)
            return x%67==0;
        if(y==clean_hundred)
            return x%100==0;
        if(y==clean_thousand)
            return x%1000==0;
        if(y==clean_ten_thousand)
            return x%10000==0;
        if(y==clean_hundred_thousand)
            return x%100000==0;
        if(y==clean_million)
            return x%1000000==0;
        if(y==clean_ten_million)
            return x%10000000==0;
        if(y==clean_hundred_million)
            return x%100000000==0;
        if(y==binary_clean)
            return x%1024==0;
        if(y==double_binary_clean)
            return x%1048576==0;
        return 0;
    }
    if(badge[y].testtype==type_contain)
    {
        if(y==homo_3)
            return iscontain(x,114,3);
        if(y==homo_4)
            return iscontain(x,1145,4);
        if(y==homo_5)
            return iscontain(x,11451,5);
        if(y==full_homo)
            return iscontain(x,114514,6);
        if(y==half_homo_b_side)
            return iscontain(x,1919,4);
        if(y==homo_b_side)
            return iscontain(x,1919810,7);
        if(y==kevin_badge)
            return iscontain(x,27,2);
        if(y==o_kevin)
            return iscontain(x,27,2);
        if(y==six_seven)
            return iscontain(x,67,2);
        if(y==six_six_six)
            return iscontain(x,666,3);
        if(y==calender_badge)
            return iscontain(x,365,3);
        if(y==emergency_badge)
            return iscontain(x,911,3);
        if(y==not_found)
            return iscontain(x,404,3);
        if(y==silence_in_sea)
            return iscontain(x,543,3);
    }
    int a[10];
    long long tmp=x;
    for(int i=9; i>=1; i--)
    {
        a[i]=tmp%10;
        tmp/=10;
    }
    if(badge[y].testtype==type_needab)
    {
        int b[10]= {0,0,0,0,0,0,0,0,0,0};
        for(int i=1; i<=9; i++)
            b[a[i]]++;
        if(y>=chemistry_0 && y<=chemistry_9)
            return b[y-chemistry_0]==1;
        if(y==abyss_badge)
            return b[0]==0;
        if(y==binary_soul)
            return b[0]+b[1]==9;
        if(y==low_ball)
            return b[0]+b[1]+b[2]+b[3]+b[4]==9;
        if(y==high_roller)
            return b[5]+b[6]+b[7]+b[8]+b[9]==8;
        if(y==divisible_by_3)
            return b[0]+b[3]+b[6]+b[9]==9;
        int cnt=0,maxx=0;
        for(int i=0; i<=9; i++)
        {
            if(b[i])
                cnt++;
            maxx=max(maxx,b[i]);
        }
        if(y==skipping_badge)
        {
            for(int i=0; i<=8; i++)
                if(b[i] && b[i+1])
                    return 0;
            return 1;
        }
        if(y==consecutive_pairs)
        {
            for(int i=0; i<=7; i++)
                if(b[i]>=2 && b[i+1]>=2 && b[i+2]>=2)
                    return 1;
            return 0;
        }
        if(y==airplane_badge)
        {
            for(int i=0; i<=8; i++)
                if(b[i]>=3 && b[i+1]>=3)
                    return 1;
            return 0;
        }
        if(y==triple_airplane)
        {
            for(int i=0; i<=7; i++)
                if(b[i]>=3 && b[i+1]>=3 && b[i+2]>=3)
                    return 1;
            return 0;
        }
        if(y==consecutive_pairs_plus)
        {
            for(int i=0; i<=6; i++)
                if(b[i]>=2 && b[i+1]>=2 && b[i+2]>=2 && b[i+3]>=2)
                    return 1;
            return 0;
        }
        if(y>=digit_complex_2 && y<=repeat_9)
        {
            if(y==digit_complex_2)
                return cnt==2;
            if(y==digit_complex_3)
                return cnt==3;
            if(y==digit_complex_4)
                return cnt==4;
            if(y==digit_complex_5)
                return cnt==5;
            if(y==digit_complex_6)
                return cnt==6;
            if(y==digit_complex_7)
                return cnt==7;
            if(y==digit_complex_8)
                return cnt==8;
            if(y==digit_complex_9)
                return cnt==9;
            if(y==repeat_4)
                return maxx>=4;
            if(y==repeat_5)
                return maxx>=5;
            if(y==repeat_6)
                return maxx>=6;
            if(y==repeat_7)
                return maxx>=7;
            if(y==repeat_8)
                return maxx>=8;
            if(y==repeat_9)
                return maxx>=9;
        }
        if(y==odd_todd)
            return b[0]+b[2]+b[4]+b[6]+b[8]==0;
        if(y==even_steven)
            return b[1]+b[3]+b[5]+b[7]+b[9]==0;
        if(y==almost_balanced)
        {
            int cnt=b[0]+b[2]+b[4]+b[6]+b[8];
            return (cnt==4 || cnt==5);
        }
        int c[10];
        int buc[10]= {0,0,0,0,0,0,0,0,0,0};
        for(int i=0; i<=9; i++)
            buc[b[i]]++;
        int cid=0;
        for(int i=9; i>=0; i--)
            while(buc[i]--)
                c[cid++]=i;
        if(y==two_pair)
            return c[0]>=2 && c[1]>=2;
        if(y==three_pair)
            return c[0]>=2 && c[1]>=2 && c[2]>=2;
        if(y==four_pair)
            return c[0]>=2 && c[1]>=2 && c[2]>=2 && c[3]>=2;
        if(y==full_house)
            return c[0]>=3 && c[1]>=2;
        if(y==two_triplet)
            return c[0]>=3 && c[1]>=3;
        if(y==three_triplet)
            return c[0]>=3 && c[1]>=3 && c[2]>=3;
        return 0;
    }
    if(badge[y].testtype==type_differ)
    {
        int b[10];
        for(int i=1; i<=8; i++)
            b[i]=a[i+1]-a[i];
        if(y==even_spacing_absolute)
        {
            for(int i=1; i<=7; i++)
                if(b[i]*b[i]!=b[i+1]*b[i+1])
                    return 0;
            return 1;
        }
        if(y==turtle_badge)
        {
            for(int i=1; i<=8; i++)
                if(b[i]>1 || b[i]<-1)
                    return 0;
            return 1;
        }
        if(y==german_whisper)
        {
            for(int i=1; i<=8; i++)
                if(b[i]>-5 && b[i]<5)
                    return 0;
            return 1;
        }
        if(y==hills_badge)
        {
            for(int i=1; i<=7; i++)
                if(b[i]*b[i+1]>=0)
                    return 0;
            return 1;
        }
        if(y==dunes_badge)
        {
            for(int i=1; i<=7; i++)
                if(b[i]*b[i+1]>0)
                    return 0;
            return 1;
        }
        if(y>=mountain_badge && y<=canyon_badge)
        {
            int atl=1;
            if(y==mesa_badge || y==canyon_badge)
                atl=0;
            int mtp=1;
            if(y==valley_badge || y==canyon_badge)
                mtp=-1;
            int xl=0,xr=0;
            for(int i=1; i<=8; i++)
            {
                if(b[i]*mtp>=atl)
                    xl++;
                else
                    break;
            }
            for(int i=8; i>=1; i--)
            {
                if(b[i]*mtp<=-atl)
                    xr++;
                else
                    break;
            }
            return xl+xr>=8;
        }
        if(y>=consecutive_3 && y<=contiguous_8)
        {
            int ava1=1,ava2=-1;
            if(y>=contiguous_2)
                ava1=ava2=0;
            int cur=0,maxx=0;
            for(int j=1; j<=8; j++)
            {
                cur++;
                if(b[j]!=ava1 && b[j]!=ava2)
                    cur=0;
                if(j==8 || b[j]!=b[j+1])
                {
                    maxx=max(maxx,cur);
                    cur=0;
                }
            }
            if(y==consecutive_3 || y==contiguous_3)
                return maxx>=2;
            if(y==consecutive_2 || y==contiguous_2)
                return maxx>=1;
            if(y==consecutive_4 || y==contiguous_4)
                return maxx>=3;
            if(y==consecutive_5 || y==contiguous_5)
                return maxx>=4;
            if(y==consecutive_6 || y==contiguous_6)
                return maxx>=5;
            if(y==consecutive_7 || y==contiguous_7)
                return maxx>=6;
            if(y==consecutive_8 || y==contiguous_8)
                return maxx>=7;
            if(y==consecutive_9)
                return maxx>=8;
        }
        if(y==alternator_badge)
        {
            for(int i=1; i<=8; i++)
            {
                if((b[i]+20)%2==0)
                    return 0;
            }
            return 1;
        }
        if(y==two_contiguous_pair || y==three_contiguous_pair || y==four_contiguous_pair)
        {
            int cnt=0;
            for(int i=1; i<=8; i++)
                if(b[i]==0)
                    cnt++;
            if(y==two_contiguous_pair)
                return cnt>=2;
            if(y==three_contiguous_pair)
                return cnt>=3;
            if(y==four_contiguous_pair)
                return cnt>=4;
        }
        if(y==steps_badge)
        {
            for(int i=1; i<=8; i++)
                if(b[i]<0)
                    return 0;
            return 1;
        }
        if(y==slopes_badge)
        {
            for(int i=1; i<=8; i++)
                if(b[i]>0)
                    return 0;
            return 1;
        }
    }
    return 0;
}

int last_not_tested=0;

void test(int x)
{
    if(x==0)
        x=last_not_tested;
    if(x>badgen)
    {
        setcolor(LIGHTRED);
        cout<<"All badges have been tested!\n";
        setcolor(LIGHTGREEN);
        cout<<"Now automaticly show badges:\n\n";
        setcolor(7);
        askuser();
        return;
    }
    long long tgt=1e8+3e6;
    int fac=1;
    long long cnt=0;
    for(long long i=MINN; i<=MAXN; i++)
    {
        if(havebadge(i,x))
            cnt++;
        if(i==tgt)
        {
            printf("Calculating...  %d%%\n",fac);
            tgt+=3e6;
            fac+=1;
        }
    }
    cout<<x<<"-\""<<badge[x].name<<"\"\n";
    cout<<cnt<<"\n";
    printf("(%.7f%%)",cnt*100.0/(MAXN-MINN+1));
}

int maxcount=153;

void init()
{
    badge[1]= {"Clean Ten","Ends with 0.",30000001,type_divide};
    badge[2]= {"Digit Complex 2","Use exactly 2 different digits.",6886,type_needab};
    badge[3]= {"Digit Complex 3","Use exactly 3 different digits.",653400,type_needab};
    badge[4]= {"Digit Complex 4","Use exactly 4 different digits.",11748240,type_needab};
    badge[5]= {"Digit Complex 5","Use exactly 5 different digits.",63059472,type_needab};
    badge[6]= {"Digit Complex 6","Use exactly 6 different digits.",120022560,type_needab};
    badge[7]= {"Digit Complex 7","Use exactly 7 different digits.",83825280,type_needab};
    badge[8]= {"Digit Complex 8","Use exactly 8 different digits.",19595520,type_needab};
    badge[9]= {"Digit Complex 9","Use exactly 9 different digits.",1088640,type_needab};
    badge[10]= {"Repeat 4","A same digit exists 4 times.",24921841,type_needab};
    badge[11]= {"Repeat 5","A same digit exists 5 times.",2672761,type_needab};
    badge[12]= {"Repeat 6","A same digit exists 6 times.",192703,type_needab};
    badge[13]= {"Repeat 7","A same digit exists 7 times.",8995,type_needab};
    badge[14]= {"Repeat 8","A same digit exists 8 times.",247,type_needab};
    badge[15]= {"Repeat 9","A same digit exists 9 times.",3,type_needab};
    badge[16]= {"Prime","Divisible only by 1 and itself.",15574871,type_special};
    badge[17]= {"High Man","Divisible by 2.",150000001,type_divide};
    badge[18]= {"Basketball Champion","Divisible by 3.",100000000,type_divide};
    badge[19]= {"ICU","Divisible by 5.",60000001,type_divide};
    badge[20]= {"Monitor","Divisible by 7.",42857143,type_divide};
    badge[21]= {"Li Yin Yang","Divisible by 11.",27272727,type_divide};
    badge[22]= {"Duck Neck","Divisible by 13.",23076923,type_divide};
    badge[23]= {"Old Hill","Divisible by 15.",20000000,type_divide};
    badge[24]= {"Negro","Divisible by 17.",17647059,type_divide};
    badge[25]= {"Big Plate Chicken","Divisible by 19.",15789474,type_divide};
    badge[26]= {"Chicken Dust","Divisible by 23.",13043478,type_divide};
    badge[27]= {"Kevin (Divisible)","Divisible by 27.",11111111,type_divide};
    badge[28]= {"Plus Seven","Divisible by 29.",10344828,type_divide};
    badge[29]= {"Blue Archive","Divisible by 31.",9677419,type_divide};
    badge[30]= {"Iron Ke","Divisible by 37.",8108108,type_divide};
    badge[31]= {"Chicken Neck","Divisible by 41.",7317073,type_divide};
    badge[32]= {"Mason Xu","Divisible by 43.",6976744,type_divide};
    badge[33]= {"Red Chicken","Divisible by 47.",6382979,type_divide};
    badge[34]= {"ZnSO4","Divisible by 49.",6122449,type_divide};
    badge[35]= {"Even Steven","All digits are even.",390626,type_needab};
    badge[36]= {"Odd Todd","All digits are odd.",781250,type_needab};
    badge[37]= {"Homo 3","Contains 114.",2794201,type_contain};
    badge[38]= {"Homo 4","Contains 1145.",249977,type_contain};
    badge[39]= {"Homo 5","Contains 11451.",21999,type_contain};
    badge[40]= {"Full Homo","Contains 114514.",1900,type_contain};
    badge[41]= {"Half Homo B-Side","Contains 1919.",248090,type_contain};
    badge[42]= {"Homo B-Side","Contains 1919810.",160,type_contain};
    badge[43]= {"Kevin","Contains 27.",29962957,type_contain};
    badge[44]= {"O Kevin","Contains 15.",29962957,type_contain};
    badge[45]= {"Almost Balanced","The number of even and odd digits differs 1.",147656250,type_needab};
    badge[46]= {"Consecutive 3","Contains 3 adjust consecutive digits in order.",30406795,type_differ};
    badge[47]= {"Consecutive 2","Contains 2 adjust consecutive digits in order.",239251563,type_differ};
    badge[48]= {"Consecutive 4","Contains 4 adjust consecutive digits in order.",2323318,type_differ};
    badge[49]= {"Consecutive 5","Contains 5 adjust consecutive digits in order.",161997,type_differ};
    badge[50]= {"Consecutive 6","Contains 6 adjust consecutive digits in order.",11220,type_differ};
    badge[51]= {"Consecutive 7","Contains 7 adjust consecutive digits in order.",742,type_differ};
    badge[52]= {"Consecutive 8","Contains 8 adjust consecutive digits in order.",37,type_differ};
    badge[53]= {"Consecutive 9","Contains 9 adjust consecutive digits in order.",1,type_differ};
    badge[54]= {"Contiguous 2","Contains 2 adjust same digits.",170859838,type_differ};
    badge[55]= {"Contiguous 3","Contains 3 adjust same digits.",18946444,type_differ};
    badge[56]= {"Contiguous 4","Contains 4 adjust same digits.",1649218,type_differ};
    badge[57]= {"Contiguous 5","Contains 5 adjust same digits.",138001,type_differ};
    badge[58]= {"Contiguous 6","Contains 6 adjust same digits.",11101,type_differ};
    badge[59]= {"Contiguous 7","Contains 7 adjust same digits.",841,type_differ};
    badge[60]= {"Contiguous 8","Contains 8 adjust same digits.",58,type_differ};
    badge[61]= {"Alternator","Digits strictly alternate between even add odd.",1171875,type_differ};
    badge[62]= {"Six Seven","Contains 67.",20552997,type_contain};
    badge[63]= {"Six Seven (Divisible)","Divisible by 67.",4477612,type_divide};
    badge[64]= {"Two Pair","Contains two different pairs.",257081418,type_needab};
    badge[65]= {"Two Contiguous Pair","Contains two contiguous different pairs.",56068582,type_differ};
    badge[66]= {"Three Pair","Contains three different pairs.",109006128,type_needab};
    badge[67]= {"Three Contiguous Pair","Contains three contiguous different pairs.",11427538,type_differ};
    badge[68]= {"Four Pair","Contains four different pairs.",10478160,type_needab};
    badge[69]= {"Four Contiguous Pair","Contains four contiguous different pairs.",1507306,type_differ};
    badge[70]= {"Second Power","A perfect square.",10001,type_special};
    badge[71]= {"Third Power","A perfect cube.",272,type_special};
    badge[72]= {"Fourth Power","A perfect fourth power.",42,type_special};
    badge[73]= {"High Power","A perfect fifth or more power.",27,type_special};
    badge[74]= {"Wide","Contains both 0 and 9.",92051322,type_special};
    badge[75]= {"Slim","Max and min digits differ not greater than 3.",550458,type_special};
    badge[76]= {"Palindrome","Reads the same forwards and backwards.",30000,type_special};
    badge[77]= {"Clean Hundred","Ends in 00.",3000001,type_divide};
    badge[78]= {"Clean Thousand","Ends in 000.",300001,type_divide};
    badge[79]= {"Clean Ten Thousand","Ends in 0000.",30001,type_divide};
    badge[80]= {"Clean Hundred Thousand","Ends in 00000.",3001,type_divide};
    badge[81]= {"Clean Million","Ends in 000000.",301,type_divide};
    badge[82]= {"Clean Ten Million","Ends in 0000000.",31,type_divide};
    badge[83]= {"Clean Hundred Million","Ends in 00000000.",4,type_divide};
    badge[84]= {"Binary Soul","Contains only 0s and 1s.",256,type_needab};
    badge[85]= {"Low Ball","Contains only digits from 0 to 4.",1171876,type_needab};
    badge[86]= {"High Roller","Contains only digits from 5 to 9 expect for the first digit.",1171875,type_needab};
    badge[87]= {"Steps","Digits never decrease.",22308,type_differ};
    badge[88]= {"Slopes","Digits never increase.",220,type_differ};
    badge[89]= {"Mountain","Digits ascend to a peak and then descend.",28662,type_differ};
    badge[90]= {"Mesa","Digits rise to a peak, then fall (flat stretches allowed).",1377302,type_differ};
    badge[91]= {"Valley","Digits descend to a trough and then ascend.",532,type_differ};
    badge[92]= {"Canyon","Digits fall to a floor, then rise (flat stretches allowed).",161292,type_differ};
    badge[93]= {"Hills","Digits strictly alternate between rising and falling.",8461713,type_differ};
    badge[94]= {"Dunes","Rises and falls keep alternating (flat stretches allowed).",40436845,type_differ};
    badge[95]= {"Divisible by 3","All digits are divisible by 3.",65536,type_needab};
    badge[96]= {"Abyss","Contains no 0s.",129140163,type_needab};
    badge[97]= {"Void","Contains exactly 1 \"0\".",114791256,type_needab};
    badge[98]= {"Hydrogen","Contains exactly 1 \"1\".",119574225,type_needab};
    badge[99]= {"Helium","Contains exactly 1 \"2\".",119574225,type_needab};
    badge[100]= {"Lithium","Contains exactly 1 \"3\".",119574225,type_needab};
    badge[101]= {"Beryllium","Contains exactly 1 \"4\".",114791257,type_needab};
    badge[102]= {"Boron","Contains exactly 1 \"5\".",114791256,type_needab};
    badge[103]= {"Carbon","Contains exactly 1 \"6\".",114791256,type_needab};
    badge[104]= {"Nitrogen","Contains exactly 1 \"7\".",114791256,type_needab};
    badge[105]= {"Oxygen","Contains exactly 1 \"8\".",114791256,type_needab};
    badge[106]= {"Fluorine","Contains exactly 1 \"9\".",114791256,type_needab};
    badge[107]= {"Mini Echo","Contains an adjacent 2-digit repeat.",16346101,type_special};
    badge[108]= {"Medium Echo","Contains an adjacent 3-digit repeat.",1110001,type_special};
    badge[109]= {"Major Echo","Contains an adjacent 4-digit repeat.",57001,type_special};
    badge[110]= {"Mini Palindrome 3","Contains a palindrome of 3 digits.",156510931,type_special};
    badge[111]= {"Mini Palindrome 4","Contains a palindrome of 4 digits.",17565718,type_special};
    badge[112]= {"Mini Palindrome 5","Contains a palindrome of 5 digits.",14710441,type_special};
    badge[113]= {"Mini Palindrome 6","Contains a palindrome of 6 digits.",1198264,type_special};
    badge[114]= {"Mini Palindrome 7","Contains a palindrome of 7 digits.",899131,type_special};
    badge[115]= {"Mini Palindrome 8","Contains a palindrome of 8 digits.",59998,type_special};
    badge[116]= {"Binary Clean","Divisible by 1024.",292969,type_divide};
    badge[117]= {"Double Binary Clean","Divisible by 1048576.",286,type_divide};
    badge[118]= {"Harshed Number","Divisible by the sum of its own digits.",19647577,type_special};
    badge[119]= {"Beginner Feather","The sum of its digits is not greater than 25.",19085851,type_special};
    badge[120]= {"Beginner Heavy","The sum of its digits is not less than 50.",24237105,type_special};
    badge[121]= {"Intermediate Feather","The sum of its digits is not greater than 20.",4515006,type_special};
    badge[122]= {"Intermediate Heavy","The sum of its digits is not less than 55.",6242979,type_special};
    badge[123]= {"Advanced Feather","The sum of its digits is not greater than 15.",643591,type_special};
    badge[124]= {"Advanced Heavy","The sum of its digits is not less than 60.",997998,type_special};
    badge[125]= {"Expert Feather","The sum of its digits is not greater than 10.",43616,type_special};
    badge[126]= {"Expert Heavy","The sum of its digits is not less than 65.",80930,type_special};
    badge[127]= {"Grandmaster Feather","The sum of its digits is not greater than 5.",706,type_special};
    badge[128]= {"Grandmaster Heavy","The sum of its digits is not less than 70.",1947,type_special};
    badge[129]= {"German Whisper","All consecutive digits differ by at least 5.",64763,type_differ};
    badge[130]= {"Turtle","All consecutive digits differ by at most 1.",15104,type_differ};
    badge[131]= {"Full House","Contains a triplet and a pair.",122770458,type_needab};
    badge[132]= {"Two Triplet","Contains two different triplet.",13851054,type_needab};
    badge[133]= {"Echo Form","Contains an adjust 2-digit repeat for 3 times.",111001,type_special};
    badge[134]= {"Echo Form+","Contains an adjust 2-digit repeat for 4 times.",571,type_special};
    badge[135]= {"Echo Form Echo Form","Contains an adjust 3-digit repeat for 3 times.",300,type_special};
    badge[136]= {"Consecutive Pairs","Contains 3 pairs that are consecutive in value.",9601368,type_needab};
    badge[137]= {"Airplane","Contains 2 triplets that are consecutive in value.",2978906,type_needab};
    badge[138]= {"Zipper","Two digits alternating perfectly.",30,type_special};
    badge[139]= {"Three Triplet","Contains three different triplet.",60480,type_needab};
    badge[140]= {"Triple Airplane","Contains 3 triplets that are consecutive in value.",4480,type_needab};
    badge[141]= {"Consecutive Pairs+","Contains 4 pairs that are consecutive in value.",370440,type_needab};
    badge[142]= {"Neutrality","The number of reverse pairs equal to order pairs.",12979991,type_special};
    badge[143]= {"Even Spacing (Absolute)","All digits have the same absolute spacing.",848,type_differ};
    badge[144]= {"Almost Sorted","The number of reverse pairs or order pairs is exactly 1.",94943,type_special};
    badge[145]= {"Skipping","No two digits differ by 1.",3182417,type_needab};
    badge[146]= {"Six Six Six","Contains 666.",1649217,type_contain};
    badge[147]= {"Calender","Contains 365.",2794201,type_contain};
    badge[148]= {"Emergency","Contains 911.",1798200,type_contain};
    badge[149]= {"Not Found","Contains 404.",1786266,type_contain};
    badge[150]= {"Slience in Sea","Contains 543.",1798200,type_contain};
    badge[151]= {"Hopscotch","A digit appears at every other position (3 times).",13940401,type_special};
    badge[152]= {"Hyper Hopscotch","A digit appears at every other position (4 times).",869431,type_special};
    badge[153]= {"Ultra Hopscotch","A digit appears at every other position (5 times).",30000,type_special};

    badgen=0;
    while(badge[badgen+1].poss)
        badgen++;
    last_not_tested=1;
    while(badge[last_not_tested].poss!=-1 && last_not_tested<=badgen)
        last_not_tested++;
    for(int i=1; i<=badgen; i++)
        srttmp[i]=make_pair(badge[i].poss,i);
    sort(srttmp+1,srttmp+badgen+1);
    for(int i=1; i<=badgen; i++)
        badgeorder[i]=srttmp[i].second;
    pw10[0]=1;
    for(int i=1; i<=18; i++)
        pw10[i]=pw10[i-1]*10;
    for(int i=1; i<=badgen; i++)
        points[i]=100000/sqrt(badge[i].poss);
    seed=time(0);
    system("title Submission Idle");
}
char printstring[15]="%.?f%%";

long long quicktest(long long x)
{
    long long sum=0;
    for(int i=1; i<=maxcount; i++)
    {
        if(havebadge(x,i))
            sum+=points[i];
    }
    return sum;
}

int ranks[1050]=
{
    0,
    65,66,67,69,71,72,72,73,74,74,75,75,75,75,76,76,77,77,78,79,
    79,80,80,80,81,81,81,81,82,82,82,82,82,83,83,83,83,83,83,84,
    84,84,84,85,85,85,85,85,85,86,86,87,87,87,87,87,88,88,88,88,
    89,89,89,89,89,90,90,90,90,90,90,90,90,91,91,91,91,91,91,92,
    92,92,92,92,92,92,93,93,93,93,93,93,93,93,94,94,94,94,94,95,
    95,95,95,95,95,95,96,96,96,96,96,97,97,97,97,97,97,97,98,98,
    98,98,98,98,98,99,99,99,99,99,99,99,100,100,100,100,100,100,100,100,
    100,100,101,101,101,101,101,101,101,102,102,102,102,102,102,102,102,103,103,103,
    103,103,103,103,103,104,104,104,104,104,104,104,105,105,105,105,105,105,105,105,
    106,106,106,106,106,106,106,107,107,107,107,107,107,107,107,107,107,108,108,108,
    108,108,108,108,108,109,109,109,109,109,109,109,109,109,110,110,110,110,110,110,
    110,110,110,110,111,111,111,111,111,111,111,111,111,112,112,112,112,112,112,112,
    112,112,113,113,113,113,113,113,113,113,113,114,114,114,114,114,114,114,114,115,
    115,115,115,115,115,115,115,115,115,116,116,116,116,116,116,116,116,116,117,117,
    117,117,117,117,117,117,117,117,117,118,118,118,118,118,118,118,118,118,119,119,
    119,119,119,119,119,119,119,120,120,120,120,120,120,120,120,120,120,121,121,121,
    121,121,121,121,121,121,122,122,122,122,122,122,122,122,122,123,123,123,123,123,
    123,123,123,123,124,124,124,124,124,124,124,124,124,124,125,125,125,125,125,125,
    125,125,125,126,126,126,126,126,126,126,126,126,127,127,127,127,127,127,127,127,
    127,127,128,128,128,128,128,128,128,128,128,129,129,129,129,129,129,129,129,129,
    130,130,130,130,130,130,130,130,130,131,131,131,131,131,131,131,131,131,132,132,
    132,132,132,132,132,132,133,133,133,133,133,133,133,133,133,134,134,134,134,134,
    134,134,134,134,135,135,135,135,135,135,135,135,135,136,136,136,136,136,136,136,
    136,136,137,137,137,137,137,137,137,137,137,138,138,138,138,138,138,138,138,139,
    139,139,139,139,139,139,139,140,140,140,140,140,140,140,140,141,141,141,141,141,
    141,141,141,141,142,142,142,142,142,142,142,142,143,143,143,143,143,143,143,143,
    144,144,144,144,144,144,144,144,145,145,145,145,145,145,145,145,146,146,146,146,
    146,146,146,146,147,147,147,147,147,147,147,147,148,148,148,148,148,148,148,149,
    149,149,149,149,149,149,149,150,150,150,150,150,150,150,150,151,151,151,151,151,
    151,151,152,152,152,152,152,152,152,153,153,153,153,153,153,153,154,154,154,154,
    154,154,154,155,155,155,155,155,155,155,156,156,156,156,156,156,156,157,157,157,
    157,157,157,157,158,158,158,158,158,158,159,159,159,159,159,159,159,160,160,160,
    160,160,160,161,161,161,161,161,161,162,162,162,162,162,162,163,163,163,163,163,
    163,164,164,164,164,164,164,165,165,165,165,165,165,166,166,166,166,166,166,167,
    167,167,167,167,167,168,168,168,168,168,168,169,169,169,169,169,170,170,170,170,
    170,170,171,171,171,171,171,172,172,172,172,172,173,173,173,173,173,173,174,174,
    174,174,174,175,175,175,175,175,176,176,176,176,176,177,177,177,177,177,178,178,
    178,178,179,179,179,179,179,180,180,180,180,180,181,181,181,181,182,182,182,182,
    183,183,183,183,183,184,184,184,184,185,185,185,185,186,186,186,186,187,187,187,
    187,188,188,188,188,189,189,189,189,190,190,190,190,191,191,191,192,192,192,192,
    193,193,193,193,194,194,194,195,195,195,196,196,196,196,197,197,197,198,198,198,
    199,199,199,200,200,200,200,201,201,201,202,202,202,203,203,203,204,204,204,205,
    205,206,206,206,207,207,207,208,208,208,209,209,210,210,210,211,211,212,212,212,
    213,213,214,214,215,215,215,216,216,217,217,218,218,219,219,220,220,220,221,222,
    222,223,223,224,224,225,225,226,226,227,227,228,228,229,230,230,231,231,232,233,
    233,234,235,235,236,237,237,238,239,239,240,241,242,242,243,244,245,246,246,247,
    248,249,250,251,252,253,254,255,255,256,258,259,260,261,262,263,264,265,267,268,
    269,270,272,273,275,276,278,279,281,283,284,286,288,290,292,294,296,298,300,303,
    305,307,310,313,315,318,321,324,328,331,335,339,343,347,352,357,361,367,372,378,
    384,391,398,406,414,423,432,443,455,469,484,501,523,549,583,631,705,814,1011,
};

void showbadgelist()
{
    cout<<"-- BADGE LIST --\n";
    for(int j=1; j<=badgen; j++)
    {
        int i=badgeorder[j];
        cout<<"[";
        printstring[2]=printrarity(badge[i].poss)+'0';
        cout<<"]  \t";
        printf(printstring,badge[i].poss*100.0/(MAXN-MINN+1));
        int x=printstring[2]-'0';
        for(int ii=max(x,2); ii<=9; ii++)
            cout<<" ";
        cout<<" "<<badge[i].name<<"\n";
    }
    cout<<"\n";
}

void askuser()
{
    cout<<"Input your submission ID(1e8~4e8):\n";
    cout<<"Luogu/Codeforces/Atcoder+1e8\n";
    cout<<"0 = Random\t1 = Show badge list\n";
    long long x;
    cin>>x;
    if(x!=0 && x!=1 && (x<MINN || x>MAXN))
    {
        setcolor(RED);
        cout<<"Error: Out of range ("<<MINN<<"~"<<MAXN<<")\n";
        setcolor(7);
        return;
    }
    if(x==0)
    {
        x=xs64()%(MAXN-MINN+1)+MINN;
        int a[10];
        int tmp=x;
        for(int i=9; i>=1; i--)
        {
            a[i]=tmp%10;
            tmp/=10;
        }
        for(int i=0; i<=8; i++)
        {
            for(int j=1; j<=i+10; j++)
            {
                system("cls");
                setcolor(WHITE);
                for(int ii=1; ii<=i; ii++)
                    cout<<a[ii];
                setcolor(GRAY);
                for(int ii=i+1; ii<=9; ii++)
                    cout<<xs64()%10;
                Sleep(100);
            }
        }
        setcolor(WHITE);
        system("cls");
        cout<<x;
        Sleep(500);
    }
    if(x==1)
    {
        system("cls");
        showbadgelist();
        askuser();
        return;
    }
    setcolor(7);
    cout<<"\n\n";
    cout<<"\n----- BADGES -----\n\n";
    int cnt=0;
    int sleeptime=500;
    long long sum=0,rsum=0;
    for(int j=badgen; j>=1; j--)
    {
        int i=badgeorder[j];
        if(havebadge(x,i))
        {
            cnt++;
            sleeptime=200*sqrt(9+cnt);
            Sleep(sleeptime);
            cout<<badge[i].name<<"\n";
            cout<<"[";
            printstring[2]=printrarity(badge[i].poss)+'0';
            cout<<"]\n";
            cout<<badge[i].desc<<"\n";
            cout<<"Possiblity: ";
            printf(printstring,badge[i].poss*100.0/(MAXN-MINN+1));
            cout<<"\n";
            cout<<"+ ";
            if(i<=maxcount)
            {
                setcolor(LIGHTWHITE);
                cout<<points[i]<<" ";
            }
            else
            {
                setcolor(LIGHTWHITE);
                cout<<0;
                setcolor(GRAY);
                cout<<"("<<points[i]<<") ";
            }
            setcolor(7);
            cout<<"pt\n\n";
            rsum+=points[i];
            if(i<=maxcount)
                sum+=points[i];
        }
    }
    Sleep(sleeptime*2);
    cout<<"Earned ";
    setcolor(LIGHTCYAN);
    cout<<cnt;
    setcolor(7);
    cout<<" badges\n";
    cout<<"TOTAL POINTS: ";
    setcolor(LIGHTWHITE);
    cout<<sum;
    if(sum!=rsum)
    {
        setcolor(GRAY);
        cout<<"("<<rsum<<")";
    }
    setcolor(7);
    cout<<" pt\n";
    int rk=0;
    for(int i=1; i<=999; i++)
        if(sum>=ranks[i])
            rk=i;
    int col=7;
    string tit="---";
    if(rk<1)
    {
        col=LIGHTYELLOW;
        tit="WORST TRASH";
    }
    else if(rk<10)
    {
        col=YELLOW;
        tit="TRASH";
    }
    else if(rk<500)
    {
        col=GREEN;
        tit="COMMON";
    }
    else if(rk<750)
    {
        col=LIGHTYELLOW;
        tit="UNUSUAL";
    }
    else if(rk<900)
    {
        col=BLUE;
        tit="RARE";
    }
    else if(rk<950)
    {
        col=PURPLE;
        tit="PURPLE";
    }
    else if(rk<975)
    {
        col=RED;
        tit="LEGENDARY";
    }
    else if(rk<990)
    {
        col=LIGHTCYAN;
        tit="MYTHIC";
    }
    else if(rk<995)
    {
        col=LIGHTRED;
        tit="ULTRA";
    }
    else
    {
        col=LIGHTGREEN;
        tit="SUPER";
    }
    cout<<"[";
    setcolor(col);
    cout<<x;
    setcolor(7);
    cout<<"]\n";
    setcolor(col);
    cout<<tit;
    setcolor(7);
    cout<<" - ";
    if(rk>=500)
    {
        setcolor(LIGHTWHITE);
        cout<<"TOP ";
        setcolor(LIGHTGREEN);
        printf("%.1f",(1000-rk)/10.0);
        setcolor(LIGHTWHITE);
        cout<<"%\n";
        setcolor(7);
    }
    else
    {
        setcolor(GRAY);
        cout<<"BOTTOM ";
        setcolor(LIGHTRED);
        printf("%.1f",(rk+1)/10.0);
        setcolor(GRAY);
        cout<<"%\n";
        setcolor(7);
    }
}
int res[100000005];

void randomtest(int n)
{
    int tgt=n/100,pct=1;
    for(int i=1; i<=n; i++)
    {
        long long x=xs64()%(MAXN-MINN+1)+MINN;
        res[i]=quicktest(x);
        if(i==tgt)
        {
            cout<<pct<<"%\n";
            pct++;
            tgt+=n/100;
        }
    }
    sort(res+1,res+n+1);
    setcolor(YELLOW);
    cout<<"BOTTOM 1%: ";
    setcolor(LIGHTWHITE);
    cout<<res[(int)(0.01*n)];
    setcolor(7);
    cout<<" pt\n";

    setcolor(LIGHTGREEN);
    cout<<"TOP 50%: ";
    setcolor(LIGHTWHITE);
    cout<<res[(int)(0.5*n)];
    setcolor(7);
    cout<<" pt\n";

    setcolor(BLUE);
    cout<<"TOP 25%: ";
    setcolor(LIGHTWHITE);
    cout<<res[(int)(0.75*n)];
    setcolor(7);
    cout<<" pt\n";

    setcolor(PURPLE);
    cout<<"TOP 10%: ";
    setcolor(LIGHTWHITE);
    cout<<res[(int)(0.9*n)];
    setcolor(7);
    cout<<" pt\n";

    setcolor(LIGHTYELLOW);
    cout<<"TOP 5%: ";
    setcolor(LIGHTWHITE);
    cout<<res[(int)(0.95*n)];
    setcolor(7);
    cout<<" pt\n";

    setcolor(RED);
    cout<<"TOP 1%: ";
    setcolor(LIGHTWHITE);
    cout<<res[(int)(0.99*n)];
    setcolor(7);
    cout<<" pt\n";

    setcolor(LIGHTCYAN);
    cout<<"TOP 1: ";
    setcolor(LIGHTWHITE);
    cout<<res[n];
    setcolor(7);
    cout<<" pt\n\n";

    for(long long i=1; i<=999; i++)
    {
        cout<<res[n*i/1000]<<",";
        if(i%20==0)
            cout<<"\n";
    }
}

int main()
{
    init();
    askuser();
    return 0;
}
/*
351461503 153071803 180573857 285037537 306852083 308173571 284085203 185085317 206308463 208620641
186048047 260268031 306150371 140317057 248038253 150758147 157425071 186046031 240258041 260482463
280720381 302406473 138253051 268051681 186042061 308208473 170468407 208642081 257380537 153147307
283047203 351730741 185180357
*/