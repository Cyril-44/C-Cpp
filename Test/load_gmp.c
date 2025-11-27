#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <elf.h>
#include <langinfo.h>
#include <signal.h>

const char lgmp[] = "/usr/lib/x86_64-linux-gnu/libgmp.so.10.4.0";

/* ---------- Helpers: page alignment ---------- */
static inline size_t page_size(void) {
    static size_t ps = 0;
    if (!ps) ps = (size_t)sysconf(_SC_PAGESIZE);
    return ps;
}
static inline uintptr_t page_floor(uintptr_t x) { size_t ps = page_size(); return x & ~(ps - 1); }
static inline uintptr_t page_ceil(uintptr_t x)  { size_t ps = page_size(); return (x + ps - 1) & ~(ps - 1); }

/* ---------- Minimal libc resolver (externs) ---------- */
typedef struct { const char* name; void* addr; } sympair;

extern void* malloc(size_t);
extern void free(void*);
extern void* realloc(void*, size_t);
extern int raise(int);
extern void abort(void);
extern void exit(int);
extern int* __errno_location(void);
extern int posix_memalign(void**, size_t, size_t);
extern void* aligned_alloc(size_t, size_t);

extern void* memcpy(void*, const void*, size_t);
extern void* memmove(void*, const void*, size_t);
extern void* memset(void*, int, size_t);
extern int memcmp(const void*, const void*, size_t);
extern char* strcpy(char*, const char*);
extern char* strncpy(char*, const char*, size_t);
extern char* strcat(char*, const char*);
extern char* strncat(char*, const char*, size_t);
extern int strcmp(const char*, const char*);
extern int strncmp(const char*, const char*, size_t);
extern size_t strlen(const char*);
extern const char* strchr(const char*, int);
extern const char* strrchr(const char*, int);
extern const char* strstr(const char*, const char*);
extern char* strtok(char*, const char*);
extern size_t strspn(const char*, const char*);
extern size_t strcspn(const char*, const char*);
extern char* strpbrk(const char*, const char*);
extern char* strerror(int);
extern char* strdup(const char*);
extern char* stpcpy(char*, const char*);
extern size_t strnlen(const char*, size_t);
extern int strcoll(const char*, const char*);
extern size_t strxfrm(char*, const char*, size_t);
extern void* memchr(const void*, int, size_t);
extern char* strsep(char**, const char*);
extern char* strndup(const char*, size_t);

extern size_t fread(void*, size_t, size_t, FILE*);
extern size_t fwrite(const void*, size_t, size_t, FILE*);
extern FILE* fopen(const char*, const char*);
extern int fclose(FILE*);
extern int fflush(FILE*);
extern int fseek(FILE*, long, int);
extern long ftell(FILE*);
extern void rewind(FILE*);
extern int fgetc(FILE*);
extern int fputc(int, FILE*);
extern char* fgets(char*, int, FILE*);
extern int fputs(const char*, FILE*);
extern int getc(FILE*);
extern int putc(int, FILE*);
extern int getchar(void);
extern int putchar(int);
extern int ungetc(int, FILE*);
extern int feof(FILE*);
extern int ferror(FILE*);
extern void clearerr(FILE*);
extern int setvbuf(FILE*, char*, int, size_t);
extern void setbuf(FILE*, char*);
extern int fileno(FILE*);
extern int fscanf(FILE*, const char*, ...);
extern int fprintf(FILE*, const char*, ...);
extern int fgetpos(FILE*, fpos_t*);
extern int fsetpos(FILE*, const fpos_t*);
extern void perror(const char*);
extern int scanf(const char*, ...);
extern int sscanf(const char*, const char*, ...);
extern int vscanf(const char*, va_list);
extern int vfscanf(FILE*, const char*, va_list);
extern int vsscanf(const char*, const char*, va_list);
extern int printf(const char*, ...);
extern int sprintf(char*, const char*, ...);
extern int snprintf(char*, size_t, const char*, ...);
extern int vprintf(const char*, va_list);
extern int vfprintf(FILE*, const char*, va_list);
extern int vsprintf(char*, const char*, va_list);
extern int vsnprintf(char*, size_t, const char*, va_list);
extern int __isoc99_fscanf(FILE*, const char*, ...);
extern int __isoc99_sscanf(const char*, const char*, ...);
extern int __isoc99_scanf(const char*, ...);
extern char* gets(char*);
extern int puts(const char*);

extern void* calloc(size_t, size_t);
extern int atexit(void (*)(void));
extern int system(const char*);
extern char* getenv(const char*);
extern int putenv(char*);
extern int setenv(const char*, const char*, int);
extern int unsetenv(const char*);
extern void* bsearch(const void*, const void*, size_t, size_t, int (*)(const void*, const void*));
extern void qsort(void*, size_t, size_t, int (*)(const void*, const void*));
extern int abs(int);
extern long labs(long);
extern long long llabs(long long);
extern div_t div(int, int);
extern ldiv_t ldiv(long, long);
extern lldiv_t lldiv(long long, long long);
extern int rand(void);
extern void srand(unsigned int);
extern double drand48(void);
extern double erand48(unsigned short[3]);
extern long lrand48(void);
extern long mrand48(void);
extern long jrand48(unsigned short[3]);
extern void srand48(long);
extern unsigned short* seed48(unsigned short[3]);
extern void lcong48(unsigned short[7]);
extern double atof(const char*);
extern int atoi(const char*);
extern long atol(const char*);
extern long long atoll(const char*);
extern double strtod(const char*, char**);
extern float strtof(const char*, char**);
extern long double strtold(const char*, char**);
extern long strtol(const char*, char**, int);
extern long long strtoll(const char*, char**, int);
extern unsigned long strtoul(const char*, char**, int);
extern unsigned long long strtoull(const char*, char**, int);
extern int mblen(const char*, size_t);
extern int mbtowc(wchar_t*, const char*, size_t);
extern int wctomb(char*, wchar_t);
extern size_t mbstowcs(wchar_t*, const char*, size_t);
extern size_t wcstombs(char*, const wchar_t*, size_t);
extern char* realpath(const char*, char*);
extern int mkstemp(char*);
extern char* mktemp(char*);
extern int getsubopt(char**, char* const*, char**);
extern int rand_r(unsigned int*);

extern void obstack_free(struct obstack*, void*);
extern void (*obstack_alloc_failed_handler)(void);
extern void _obstack_newchunk(struct obstack*, int);
extern int obstack_printf(struct obstack*, const char*, ...);
extern int obstack_vprintf(struct obstack*, const char*, va_list);

extern void  __stack_chk_fail(void);
extern void _obstack_newchunk(struct obstack*, int);
extern int __printf_chk(int, const char*, ...);
extern int __fprintf_chk(FILE*, int, const char*, ...);
extern int __sprintf_chk(char*, int, size_t, const char*, ...);
extern int __snprintf_chk(char*, size_t, int, size_t, const char*, ...);
extern int __vfprintf_chk(FILE*, int, const char*, va_list);
extern int __vsprintf_chk(char*, int, size_t, const char*, va_list);
extern int __vsnprintf_chk(char*, size_t, int, size_t, const char*, va_list);
// extern int __scanf_chk(const char*, ...);
// extern int __fscanf_chk(FILE*, const char*, ...);
// extern int __sscanf_chk(const char*, const char*, ...);
// extern int __vscanf_chk(const char*, va_list);
// extern int __vfscanf_chk(FILE*, const char*, va_list);
// extern int __vsscanf_chk(const char*, const char*, va_list);
extern void* __memcpy_chk(void*, const void*, size_t, size_t);
extern void* __memmove_chk(void*, const void*, size_t, size_t);
extern void* __memset_chk(void*, int, size_t, size_t);
extern char* __strcpy_chk(char*, const char*, size_t);
extern char* __stpcpy_chk(char*, const char*, size_t);
extern char* __strncpy_chk(char*, const char*, size_t, size_t);
extern char* __strcat_chk(char*, const char*, size_t);
extern char* __strncat_chk(char*, const char*, size_t, size_t);
extern const unsigned short int** __ctype_b_loc(void);
extern const int32_t** __ctype_tolower_loc(void);
extern const int32_t** __ctype_toupper_loc(void);
extern void __cxa_finalize(void*);
// extern void __gmon_start__(void);
// extern void _ITM_registerTMCloneTable(void);
// extern void _ITM_deregisterTMCloneTable(void);
void dummy(void) { }

static void* resolve_libc_symbol(const char* name) {
    if (!name) return NULL;
    struct { const char* n; void* p; } t[] = {
        {"malloc", (void*)malloc},
        {"free", (void*)free},
        {"realloc", (void*)realloc},
        {"raise", (void*)raise},
        {"abort", (void*)abort},
        {"exit", (void*)exit},
        {"__errno_location", (void*)__errno_location},
        {"posix_memalign", (void*)posix_memalign},
        {"aligned_alloc", (void*)aligned_alloc}, // 若无可去掉

        /* ==== string.h functions ==== */
        {"memcpy", (void*)memcpy},
        {"memmove", (void*)memmove},
        {"memset", (void*)memset},
        {"memcmp", (void*)memcmp},
        {"strcpy", (void*)strcpy},
        {"strncpy", (void*)strncpy},
        {"strcat", (void*)strcat},
        {"strncat", (void*)strncat},
        {"strcmp", (void*)strcmp},
        {"strncmp", (void*)strncmp},
        {"strlen", (void*)strlen},
        {"strchr", (void*)strchr},
        {"strrchr", (void*)strrchr},
        {"strstr", (void*)strstr},
        {"strtok", (void*)strtok},
        {"strspn", (void*)strspn},
        {"strcspn", (void*)strcspn},
        {"strpbrk", (void*)strpbrk},
        {"strerror", (void*)strerror},
        {"strdup", (void*)strdup},
        {"stpcpy", (void*)stpcpy},
        {"strnlen", (void*)strnlen},
        {"strcoll", (void*)strcoll},
        {"strxfrm", (void*)strxfrm},
        {"memchr", (void*)memchr},
        {"strsep", (void*)strsep},
        {"strndup", (void*)strndup},

        /* ==== stdio.h file operations ==== */
        {"fread", (void*)fread},
        {"fwrite", (void*)fwrite},
        {"fopen", (void*)fopen},
        {"fclose", (void*)fclose},
        {"fflush", (void*)fflush},
        {"fseek", (void*)fseek},
        {"ftell", (void*)ftell},
        {"rewind", (void*)rewind},
        {"fgetc", (void*)fgetc},
        {"fputc", (void*)fputc},
        {"fgets", (void*)fgets},
        {"fputs", (void*)fputs},
        {"getc", (void*)getc},
        {"putc", (void*)putc},
        {"getchar", (void*)getchar},
        {"putchar", (void*)putchar},
        {"ungetc", (void*)ungetc},
        {"feof", (void*)feof},
        {"ferror", (void*)ferror},
        {"clearerr", (void*)clearerr},
        {"setvbuf", (void*)setvbuf},
        {"setbuf", (void*)setbuf},
        {"fileno", (void*)fileno},
        {"fscanf", (void*)fscanf},
        {"fprintf", (void*)fprintf},
        {"fgetpos", (void*)fgetpos},
        {"fsetpos", (void*)fsetpos},
        {"perror", (void*)perror},
        {"scanf", (void*)scanf},
        {"fscanf", (void*)fscanf},
        {"sscanf", (void*)sscanf},
        {"vscanf", (void*)vscanf},
        {"vfscanf", (void*)vfscanf},
        {"vsscanf", (void*)vsscanf},
        {"printf", (void*)printf},
        {"fprintf", (void*)fprintf},
        {"sprintf", (void*)sprintf},
        {"snprintf", (void*)snprintf},
        {"vprintf", (void*)vprintf},
        {"vfprintf", (void*)vfprintf},
        {"vsprintf", (void*)vsprintf},
        {"vsnprintf", (void*)vsnprintf},
        {"__isoc99_fscanf", (void*)fscanf},
        {"__isoc99_sscanf", (void*)sscanf},
        {"__isoc99_scanf", (void*)scanf},
        {"gets", (void*)gets},
        {"puts", (void*)puts},

        /* ==== stdlib.h function mappings ==== */
        {"malloc", (void*)malloc},
        {"calloc", (void*)calloc},
        {"realloc", (void*)realloc},
        {"free", (void*)free},
        {"abort", (void*)abort},
        {"exit", (void*)exit},
        {"atexit", (void*)atexit},
        {"system", (void*)system},
        {"getenv", (void*)getenv},
        {"putenv", (void*)putenv},
        {"setenv", (void*)setenv},
        {"unsetenv", (void*)unsetenv},
        {"bsearch", (void*)bsearch},
        {"qsort", (void*)qsort},
        {"abs", (void*)abs},
        {"labs", (void*)labs},
        {"llabs", (void*)llabs},
        {"div", (void*)div},
        {"ldiv", (void*)ldiv},
        {"lldiv", (void*)lldiv},
        {"rand", (void*)rand},
        {"srand", (void*)srand},
        {"drand48", (void*)drand48},
        {"erand48", (void*)erand48},
        {"lrand48", (void*)lrand48},
        {"nrand48", (void*)nrand48},
        {"mrand48", (void*)mrand48},
        {"jrand48", (void*)jrand48},
        {"srand48", (void*)srand48},
        {"seed48", (void*)seed48},
        {"lcong48", (void*)lcong48},
        {"atof", (void*)atof},
        {"atoi", (void*)atoi},
        {"atol", (void*)atol},
        {"atoll", (void*)atoll},
        {"strtod", (void*)strtod},
        {"strtof", (void*)strtof},
        {"strtold", (void*)strtold},
        {"strtol", (void*)strtol},
        {"strtoll", (void*)strtoll},
        {"strtoul", (void*)strtoul},
        {"strtoull", (void*)strtoull},
        {"mblen", (void*)mblen},
        {"mbtowc", (void*)mbtowc},
        {"wctomb", (void*)wctomb},
        {"mbstowcs", (void*)mbstowcs},
        {"wcstombs", (void*)wcstombs},
        {"aligned_alloc", (void*)aligned_alloc},
        {"posix_memalign", (void*)posix_memalign},
        {"realpath", (void*)realpath},
        {"mkstemp", (void*)mkstemp},
        {"mktemp", (void*)mktemp},
        {"getsubopt", (void*)getsubopt},
        {"rand_r", (void*)rand_r},

        /* ==== obstack function mappings ==== */
        // {"obstack_init", (void*)obstack_init},
        // {"obstack_begin", (void*)obstack_begin},
        // {"obstack_begin_1", (void*)obstack_begin_1},
        {"obstack_free", (void*)obstack_free},
        {"obstack_alloc_failed_handler", (void*)obstack_alloc_failed_handler},
        // {"obstack_memory_used", (void*)obstack_memory_used},
        // {"obstack_chunk_alloc", (void*)obstack_chunk_alloc},
        // {"obstack_chunk_free", (void*)obstack_chunk_free},
        {"obstack_newchunk", (void*)_obstack_newchunk},
        // {"obstack_base", (void*)obstack_base},
        // {"obstack_next_free", (void*)obstack_next_free},
        // {"obstack_object_size", (void*)obstack_object_size},
        // {"obstack_finish", (void*)obstack_finish},
        // {"obstack_blank", (void*)obstack_blank},
        // {"obstack_blank_fast", (void*)obstack_blank_fast},
        // {"obstack_grow", (void*)obstack_grow},
        // {"obstack_grow0", (void*)obstack_grow0},
        // {"obstack_1grow", (void*)obstack_1grow},
        // {"obstack_ptr_grow", (void*)obstack_ptr_grow},
        // {"obstack_ptr_grow0", (void*)obstack_ptr_grow0},
        // {"obstack_int_grow", (void*)obstack_int_grow},
        // {"obstack_int_grow0", (void*)obstack_int_grow0},
        // {"obstack_copy", (void*)obstack_copy},
        // {"obstack_copy0", (void*)obstack_copy0},
        {"obstack_printf", (void*)obstack_printf},
        {"obstack_vprintf", (void*)obstack_vprintf},

        {"nl_langinfo", (void*)nl_langinfo},
        {"_obstack_newchunk", (void*)_obstack_newchunk},
        {"stderr", (void*)&stderr},
        {"stdout", (void*)&stdout},
        {"stdin",  (void*)&stdin},

        // GNU内部符号
        {"__stack_chk_fail", (void*)__stack_chk_fail},
        {"__printf_chk", (void*)__printf_chk},
        {"__fprintf_chk", (void*)__fprintf_chk},
        {"__sprintf_chk", (void*)__sprintf_chk},
        {"__snprintf_chk", (void*)__snprintf_chk},
        {"__vfprintf_chk", (void*)__vfprintf_chk},
        {"__vsprintf_chk", (void*)__vsprintf_chk},
        {"__vsnprintf_chk", (void*)__vsnprintf_chk},
        // {"__scanf_chk", (void*)__scanf_chk},
        // {"__fscanf_chk", (void*)__fscanf_chk},
        // {"__sscanf_chk", (void*)__sscanf_chk},
        // {"__vscanf_chk", (void*)__vscanf_chk},
        // {"__vfscanf_chk", (void*)__vfscanf_chk},
        // {"__vsscanf_chk", (void*)__vsscanf_chk},
        {"__memcpy_chk", (void*)__memcpy_chk},
        {"__memmove_chk", (void*)__memmove_chk},
        {"__memset_chk", (void*)__memset_chk},
        {"__strcpy_chk", (void*)__strcpy_chk},
        {"__stpcpy_chk", (void*)__stpcpy_chk},
        {"__strncpy_chk", (void*)__strncpy_chk},
        {"__strcat_chk", (void*)__strcat_chk},
        {"__strncat_chk", (void*)__strncat_chk},
        {"__ctype_b_loc", (void*)__ctype_b_loc},
        {"__ctype_tolower_loc", (void*)__ctype_tolower_loc},
        {"__ctype_toupper_loc", (void*)__ctype_toupper_loc},

        // 无法解析
        {"_ITM_registerTMCloneTable", (void*)dummy},
        {"_ITM_deregisterTMCloneTable", (void*)dummy},
        {"__gmon_start__", (void*)dummy},
        {"__cxa_finalize", (void*)__cxa_finalize},
    };
    for (size_t i = 0; i < sizeof(t)/sizeof(t[0]); ++i)
        if (strcmp(name, t[i].n) == 0) return t[i].p;
    return NULL;
}

/* ---------- Symbol lookup in the same DSO ---------- */
typedef struct {
    void*          base;        // mapped base address
    size_t total_len;
    Elf64_Addr     min_vaddr;   // lowest p_vaddr among PT_LOAD
    Elf64_Dyn*     dynamic;     // PT_DYNAMIC
    Elf64_Xword    dyn_cnt;
    Elf64_Sym*     dynsym;
    const char*    dynstr;
    size_t         dynsym_cnt;
    Elf64_Rela*    rela_dyn;
    size_t         rela_dyn_cnt;
    Elf64_Rela*    rela_plt;
    size_t         rela_plt_cnt;
    void         (**init_array)(void);
    size_t         init_array_cnt;
    void         (*init_func)(void);
} so_image;

/* Extract dynamic entries and pointers we need */
static int parse_dynamic(so_image* img, Elf64_Ehdr* eh, const void* file_map, size_t file_size) {
    (void)file_map; (void)file_size;
    // Locate PT_DYNAMIC from program headers
    Elf64_Phdr* ph = (Elf64_Phdr*)((char*)file_map + eh->e_phoff);
    Elf64_Addr dyn_vaddr = 0, dyn_memsz = 0;
    for (int i = 0; i < eh->e_phnum; ++i) {
        if (ph[i].p_type == PT_DYNAMIC) {
            dyn_vaddr = ph[i].p_vaddr;
            dyn_memsz = ph[i].p_memsz;
            break;
        }
    }
    if (!dyn_vaddr || !dyn_memsz) return -1;
    Elf64_Dyn* dyn = (Elf64_Dyn*)((uintptr_t)img->base + (dyn_vaddr - img->min_vaddr));
    img->dynamic = dyn;

    // Walk dynamics
    Elf64_Addr symtab = 0, strtab = 0;
    Elf64_Xword syment = 0;
    Elf64_Addr rela = 0, jmprel = 0;
    Elf64_Xword relasz = 0, relaent = 0, pltrelsz = 0;
    Elf64_Addr init_array = 0;
    Elf64_Xword init_arraysz = 0;
    Elf64_Addr init_func = 0;
    Elf64_Sxword pltrel = 0; // DT_PLTREL says RELA or REL

    for (Elf64_Dyn* d = dyn; d->d_tag != DT_NULL; ++d) {
        switch (d->d_tag) {
            case DT_SYMTAB:   symtab = d->d_un.d_ptr; break;
            case DT_STRTAB:   strtab = d->d_un.d_ptr; break;
            case DT_SYMENT:   syment = d->d_un.d_val; break;
            case DT_RELA:     rela = d->d_un.d_ptr; break;
            case DT_RELASZ:   relasz = d->d_un.d_val; break;
            case DT_RELAENT:  relaent = d->d_un.d_val; break;
            case DT_JMPREL:   jmprel = d->d_un.d_ptr; break;
            case DT_PLTRELSZ: pltrelsz = d->d_un.d_val; break;
            case DT_INIT_ARRAY:   init_array = d->d_un.d_ptr; break;
            case DT_INIT_ARRAYSZ: init_arraysz = d->d_un.d_val; break;
            case DT_INIT:         init_func = d->d_un.d_ptr; break;
            case DT_PLTREL: pltrel = d->d_un.d_val; break;
            default: break;
        }
    }
    if (!symtab || !strtab || !syment) return -2;
    img->dynsym = (Elf64_Sym*)((uintptr_t)img->base + (symtab - img->min_vaddr));
    img->dynstr = (const char*)((uintptr_t)img->base + (strtab - img->min_vaddr));
    img->dynsym_cnt = 0; // we can infer count from hash tables, but we will guard via section sizes during reloc

    if (rela && relasz) {
        img->rela_dyn = (Elf64_Rela*)((uintptr_t)img->base + (rela - img->min_vaddr));
        img->rela_dyn_cnt = relaent ? (size_t)(relasz / relaent) : 0;
    }
    if (jmprel && pltrelsz) {
        if (pltrel == DT_RELA) {
            img->rela_plt = (Elf64_Rela*)((uintptr_t)img->base + (jmprel - img->min_vaddr));
            img->rela_plt_cnt = pltrelsz / sizeof(Elf64_Rela);
        } else {
            fprintf(stderr, "DT_PLTREL != DT_RELA not supported\n");
            return -3;
        }
    }
    if (init_array && init_arraysz) {
        img->init_array = (void(**)(void))((uintptr_t)img->base + (init_array - img->min_vaddr));
        img->init_array_cnt = init_arraysz / sizeof(void(*)(void));
    }
    img->init_func = init_func ? (void(*)(void))((uintptr_t)img->base + (init_func - img->min_vaddr)) : NULL;
    return 0;
}

/* Look up symbol addr within the same DSO or fallback to libc resolver */
static void* resolve_symbol_in_image(so_image* img, size_t sym_index) {
    Elf64_Sym* s = &img->dynsym[sym_index];
    if (s->st_shndx != SHN_UNDEF) {
        // Defined in this DSO
        return (void*)((uintptr_t)img->base + (s->st_value - img->min_vaddr));
    } else {
        const char* name = img->dynstr + s->st_name;
        return resolve_libc_symbol(name);
    }
}

// Add helper to get symbol name
/* Apply RELA relocations */
static const char* dynsym_name(so_image* img, uint32_t symidx) {
    if (!img->dynsym || !img->dynstr) return NULL;
    return img->dynstr + img->dynsym[symidx].st_name;
}

static int apply_relocations(so_image* img) {
#if defined(__x86_64__)
    // .rela.dyn
    for (size_t i = 0; i < img->rela_dyn_cnt; ++i) {
        Elf64_Rela* r = &img->rela_dyn[i];
        uint32_t type = ELF64_R_TYPE(r->r_info);
        uint32_t sym  = ELF64_R_SYM(r->r_info);
        uintptr_t* where = (uintptr_t*)((uintptr_t)img->base + (r->r_offset - img->min_vaddr));

        switch (type) {
            case R_X86_64_RELATIVE:
                // 正确规则：*where = base + addend
                *where = (uintptr_t)img->base + (uintptr_t)r->r_addend;
                break;

            case R_X86_64_GLOB_DAT:
            case R_X86_64_64: {
                void* addr = resolve_symbol_in_image(img, sym);
                if (!addr) {
                    const char* nm = dynsym_name(img, sym);
                    fprintf(stderr, "Unresolved symbol (GLOB_DAT/64): %s\n", nm ? nm : "<noname>");
                    return -10;
                }
                *where = (uintptr_t)addr + (uintptr_t)r->r_addend;
                break;
            }

            case R_X86_64_DTPMOD64:
            case R_X86_64_DTPOFF64:
            case R_X86_64_TPOFF64:
                fprintf(stderr, "Unsupported TLS relocation type: %u\n", type);
                return -11;

            case R_X86_64_COPY:
                fprintf(stderr, "Unsupported R_X86_64_COPY in DSO context\n");
                return -12;

            default:
                fprintf(stderr, "Unsupported relocation type (rela.dyn): %u\n", type);
                return -13;
        }
    }

    // .rela.plt
    for (size_t i = 0; i < img->rela_plt_cnt; ++i) {
        Elf64_Rela* r = &img->rela_plt[i];
        uint32_t type = ELF64_R_TYPE(r->r_info);
        uint32_t sym  = ELF64_R_SYM(r->r_info);
        uintptr_t* where = (uintptr_t*)((uintptr_t)img->base + (r->r_offset - img->min_vaddr));

        switch (type) {
            case R_X86_64_JUMP_SLOT: {
                void* addr = resolve_symbol_in_image(img, sym);
                if (!addr) {
                    const char* nm = dynsym_name(img, sym);
                    fprintf(stderr, "Unresolved symbol (JUMP_SLOT): %s\n", nm ? nm : "<noname>");
                    return -20;
                }
                *where = (uintptr_t)addr;
                break;
            }
            default:
                fprintf(stderr, "Unsupported relocation type (rela.plt): %u\n", type);
                return -21;
        }
    }
    return 0;
#else
    (void)img;
    return -100;
#endif
}

/* Run DT_INIT then DT_INIT_ARRAY */
static void run_initializers(so_image* img) {
    if (img->init_func) img->init_func();
    for (size_t i = 0; i < img->init_array_cnt; ++i) {
        if (img->init_array[i]) img->init_array[i]();
    }
}

/* Map all PT_LOAD segments with proper protections and compute base/bias */
static int map_segments(int fd, Elf64_Ehdr* eh, const void* file_map, size_t file_size, so_image* img) {
    (void)file_map; (void)file_size;
    Elf64_Phdr* ph = (Elf64_Phdr*)((char*)file_map + eh->e_phoff);

    Elf64_Addr min_vaddr = ~0ULL, max_vaddr = 0;
    for (int i = 0; i < eh->e_phnum; ++i) {
        if (ph[i].p_type != PT_LOAD) continue;
        if (ph[i].p_vaddr < min_vaddr) min_vaddr = ph[i].p_vaddr;
        if (ph[i].p_vaddr + ph[i].p_memsz > max_vaddr) max_vaddr = ph[i].p_vaddr + ph[i].p_memsz;
    }
    if (min_vaddr == ~0ULL) return -1;

    size_t total = (size_t)(max_vaddr - min_vaddr);
    size_t total_aligned = page_ceil(total);

    // Reserve address space
    void* base = mmap(NULL, total_aligned, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (base == MAP_FAILED) return -2;

    // Map each PT_LOAD into the reserved region
    for (int i = 0; i < eh->e_phnum; ++i) {
        if (ph[i].p_type != PT_LOAD) continue;

        uintptr_t seg_off_file = (uintptr_t)ph[i].p_offset;
        uintptr_t seg_vaddr    = (uintptr_t)ph[i].p_vaddr;
        uintptr_t seg_filesz   = (uintptr_t)ph[i].p_filesz;
        uintptr_t seg_memsz    = (uintptr_t)ph[i].p_memsz;

        // Aligned addresses
        uintptr_t map_start_v = page_floor(seg_vaddr);
        uintptr_t map_end_v   = page_ceil(seg_vaddr + seg_filesz);
        size_t    map_len     = map_end_v - map_start_v;

        uintptr_t file_off_al = page_floor(seg_off_file);

        int prot = 0;
        if (ph[i].p_flags & PF_R) prot |= PROT_READ;
        if (ph[i].p_flags & PF_W) prot |= PROT_WRITE;
        if (ph[i].p_flags & PF_X) prot |= PROT_EXEC;

        void* seg_addr = (void*)((uintptr_t)base + (map_start_v - (uintptr_t)min_vaddr));

        if (map_len) {
            void* r = mmap(seg_addr, map_len, prot, MAP_PRIVATE | MAP_FIXED, fd, (off_t)file_off_al);
            if (r == MAP_FAILED) {
                munmap(base, total_aligned);
                return -3;
            }
        }

        // Zero the BSS tail
        uintptr_t file_tail = seg_vaddr + seg_filesz;
        if (seg_memsz > seg_filesz) {
            uintptr_t bss_start = (uintptr_t)base + (file_tail - (uintptr_t)min_vaddr);
            size_t bss_len = (size_t)(seg_memsz - seg_filesz);
            memset((void*)bss_start, 0, bss_len);
        }
    }

    img->base = base;
    img->min_vaddr = min_vaddr;
    img->total_len = total_aligned;
    return 0;
}

/* ---------- Public API ---------- */
void* loadSymbolExecutable(const char* soPath, const char* symbolName) {
    int fd = open(soPath, O_RDONLY);
    if (fd < 0) { perror("open"); return NULL; }

    struct stat st;
    if (fstat(fd, &st) < 0) { perror("fstat"); close(fd); return NULL; }

    void* fileMap = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (fileMap == MAP_FAILED) { perror("mmap file"); close(fd); return NULL; }

    Elf64_Ehdr* eh = (Elf64_Ehdr*)fileMap;
    if (memcmp(eh->e_ident, ELFMAG, SELFMAG) != 0 ||
        eh->e_ident[EI_CLASS] != ELFCLASS64 ||
        eh->e_type != ET_DYN) {
        fprintf(stderr, "Unsupported ELF (need ELF64 ET_DYN)\n");
        munmap(fileMap, st.st_size); close(fd); return NULL;
    }

    so_image img = {0};
    if (map_segments(fd, eh, fileMap, st.st_size, &img) != 0) {
        fprintf(stderr, "Failed to map PT_LOAD segments\n");
        munmap(fileMap, st.st_size); close(fd); return NULL;
    }

    // Now file-backed segments are mapped; close fd and metadata map can be kept until done parsing
    // We still need fileMap for dynamic parsing math, but dynamic parsing uses runtime image.
    // We can close fd now.
    close(fd);

    if (parse_dynamic(&img, eh, fileMap, st.st_size) != 0) {
        fprintf(stderr, "Failed to parse PT_DYNAMIC\n");
        munmap(fileMap, st.st_size);
        // Don't unmap img.base; caller may rely on already-mapped memory if we were going to return symbol.
        munmap(img.base, page_ceil((size_t)((uintptr_t)0 + ((uintptr_t)0)))); // no easy length here; skip
        return NULL;
    }

    // Apply relocations
    int rc = apply_relocations(&img);
    if (rc != 0) {
        fprintf(stderr, "Relocation failed: %d\n", rc);
        munmap(fileMap, st.st_size);
        // As above, leaving mapping would leak; for demo we won't try to recover.
        return NULL;
    }

    // Run constructors
    run_initializers(&img);

    // Lookup requested symbol in dynsym by name
    // We don't know dynsym count from DT_HASH/GNU_HASH; we can scan until we hit a NULL name
    // Safer: derive from section headers (from fileMap)
    Elf64_Shdr* sh = (Elf64_Shdr*)((char*)fileMap + eh->e_shoff);
    Elf64_Shdr* dynsym_sh = NULL;
    Elf64_Shdr* dynstr_sh = NULL;
    const char* shstr = (const char*)fileMap + sh[eh->e_shstrndx].sh_offset;
    for (int i = 0; i < eh->e_shnum; ++i) {
        const char* nm = shstr + sh[i].sh_name;
        if (strcmp(nm, ".dynsym") == 0) dynsym_sh = &sh[i];
        else if (strcmp(nm, ".dynstr") == 0) dynstr_sh = &sh[i];
    }

    void* result = NULL;
    if (dynsym_sh && dynstr_sh) {
        Elf64_Sym* syms_file = (Elf64_Sym*)((char*)fileMap + dynsym_sh->sh_offset);
        const char* strtab_file = (const char*)fileMap + dynstr_sh->sh_offset;
        size_t cnt = dynsym_sh->sh_size / sizeof(Elf64_Sym);
        for (size_t i = 0; i < cnt; ++i) {
            const char* nm = strtab_file + syms_file[i].st_name;
            if (nm && strcmp(nm, symbolName) == 0) {
                // Find same index in runtime dynsym to get relocated address
                // Dynsym order is identical; use img.dynsym[i]
                Elf64_Sym* rs = &img.dynsym[i];
                if (ELF64_ST_TYPE(rs->st_info) != STT_FUNC && rs->st_value == 0 && rs->st_shndx == SHN_UNDEF) {
                    // Undefined; try extern resolve
                    result = resolve_libc_symbol(symbolName);
                } else {
                    result = (void*)((uintptr_t)img.base + (rs->st_value - img.min_vaddr));
                }
                break;
            }
        }
    }

    munmap(fileMap, st.st_size);
    if (!result) {
        fprintf(stderr, "Symbol '%s' not found\n", symbolName);
        // Caller could still keep img.base via a handle; here we just leak mapping for simplicity
        return NULL;
    }
    return result;
}

typedef struct {
    int _mp_alloc;
    int _mp_size;
    unsigned long *_mp_d;
} __mpz_struct;
typedef __mpz_struct mpz_t[1];
int main() {
    // 获取 GMP 函数指针
    void (*mpz_init2)(mpz_t, unsigned long) = loadSymbolExecutable(lgmp, "__gmpz_init2");
    void (*mpz_init)(mpz_t) = loadSymbolExecutable(lgmp, "__gmpz_init");
    int  (*mpz_set_str)(mpz_t, const char *, int) = loadSymbolExecutable(lgmp, "__gmpz_set_str");
    void (*mpz_add)(mpz_t, const mpz_t, const mpz_t) = loadSymbolExecutable(lgmp, "__gmpz_add");
    char *(*mpz_get_str)(char *, int, const mpz_t) = loadSymbolExecutable(lgmp, "__gmpz_get_str");
    void (*mpz_clear)(mpz_t) = loadSymbolExecutable(lgmp, "__gmpz_clear");

    if (!mpz_init)      return fputs("Error loading mpz_init\n", stderr), 1;
    if (!mpz_init2)     return fputs("Error loading mpz_init2\n", stderr), 1;
    if (!mpz_set_str)   return fputs("Error loading mpz_set_str\n", stderr), 1;
    if (!mpz_add)       return fputs("Error loading mpz_add\n", stderr), 1;
    if (!mpz_get_str)   return fputs("Error loading mpz_get_str\n", stderr), 1;
    if (!mpz_clear)     return fputs("Error loading mpz_clear\n", stderr), 1;

    if (!mpz_init2 || !mpz_set_str || !mpz_add || !mpz_get_str || !mpz_clear) {
        return 1;
    }
    mpz_t a, b, res;
    mpz_init2(a, 2000005);
    mpz_init2(a, 2000005);
    mpz_init(res);
    int T;
    static char sa[2000005], sb[2000005], sr[4000005];
    scanf("%d", &T);
    while (T--) {
        scanf(" %s %s", sa, sb);
        mpz_set_str(a, sa, 10);
        mpz_set_str(b, sb, 10);
        mpz_add(res, a, b);
        mpz_get_str(sr, 10, res);
        puts(sr);
    }
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(res);
    return 0;
}