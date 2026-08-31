#include <cstdio>
int main(int argc, char **argv) {
    FILE *in = fopen(argv[1], "r"), *ans = fopen(argv[2], "r");
    int tid, T;
    fscanf(in, "%d%d", &tid, &T);
    while (T--) {
        int n, m;
        fscanf(in, "%d%d", &n, &m);
        char res[5];
        fscanf(ans, "%s", res);
        if (1ll * n * m > 500'0000 && res[0] == 'Y')
            { puts("Failed!"); return 0; }
    }
    return 0;
}