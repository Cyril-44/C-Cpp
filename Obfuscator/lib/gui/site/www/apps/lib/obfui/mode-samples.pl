$VAR1 = {
          '_sys_base_production_mixcase' => {
                                              'comment' => 'Production mode with "mixcase" mangler for all symbols (using same name for symbols, but with
randomized case of each letter in the symbol). 
Comments are removed, whitespace is jammed, strings and integers are mangled too.',
                                              'input' => 'static int error_level = 4; /* error level to show alerts for, initialized by some accessor*/
void log_error(const char* whereStr,const char* msg)
{
    log_common(3,whereStr,msg); //call common function
}
void log_common(int lvl,const char* whereStr,const char* msg)
{
    char buf[2000];
    time_t currtime = time(NULL);
    snprintf(buf,sizeof(buf), "%s: level %d at" 
	" %s: %s",ctime(&currtime),lvl,whereStr,msg);
    if (lvl >= error_level)
	tracelog.write(buf);
    fulllog.write(buf);
}
',
                                              'comment_input' => 'This is a trivial piece of C++  with 1 global var, 2 functions that refer to it and
have local variables or parameters of the same name.',
                                              'output' => 'static int veRy_top_sECReT=(0x1287+4964-0x25e7);void StRictly_CONfIdenTiAl(const
 char*vEry_top_sECReT,const char*Strictly_CONfIdenTiAl){veRY_top_sECReT(
(0x13ba+4076-0x23a3),vEry_top_sECReT,Strictly_CONfIdenTiAl);}void 
veRY_top_sECReT(int StrIctly_CONfIdenTiAl,const char*vEry_top_sECReT,const char*
Strictly_CONfIdenTiAl){char vEry_TOP_SecReT[(0xced+8636-0x26d9)];
StRICTLY_coNfIdenTiAl veRy_tOP_SecReT=StRIcTLY_coNfIdenTiAl(vEry_ToP_SecReT);
StrICtLY_coNfIdenTiAl(vEry_TOP_SecReT,sizeof(vEry_TOP_SecReT),
"\\x25\\x73\\x3a\\x20\\x6c\\x65\\x76\\x65\\x6c\\x20\\x25\\x64\\x20\\x61\\x74"
"\\x20\\x25\\x73\\x3a\\x20\\x25\\x73",veRY_toP_SecReT(&veRy_tOP_SecReT),
StrIctly_CONfIdenTiAl,vEry_top_sECReT,Strictly_CONfIdenTiAl);if(
StrIctly_CONfIdenTiAl>=veRy_top_sECReT)StriCTlY_coNfIdenTiAl.vEry_tOp_SecReT(
vEry_TOP_SecReT);StRiCtlY_coNfIdenTiAl.vEry_tOp_SecReT(vEry_TOP_SecReT);}
'
                                            },
          '_sys_base_no_syms_obf_jam' => {
                                           'comment' => 'No symbol names are changed. Comments are removed, whitespace is jammed. String and integer
constants are obfuscated.This can be used when you don\'t have time to test your code afer
obfuscation at all; the code after obfuscation should work exactly as before obfuscation.',
                                           'input' => 'static int error_level = 4; /* error level to show alerts for, initialized by some accessor*/
void log_error(const char* whereStr,const char* msg)
{
    log_common(3,whereStr,msg); //call common function
}
void log_common(int lvl,const char* whereStr,const char* msg)
{
    char buf[2000];
    time_t currtime = time(NULL);
    snprintf(buf,sizeof(buf), "%s: level %d at" 
	" %s: %s",ctime(&currtime),lvl,whereStr,msg);
    if (lvl >= error_level)
	tracelog.write(buf);
    fulllog.write(buf);
}
',
                                           'comment_input' => 'This is a trivial piece of C++  with 1 global var, 2 functions that refer to it and
have local variables or parameters of the same name.',
                                           'output' => 'static int error_level=(0x2235+176-0x22e1);void log_error(const char*whereStr,
const char*msg){log_common((0xb68+1365-0x10ba),whereStr,msg);}void log_common(
int lvl,const char*whereStr,const char*msg){char buf[(0x2339+2481-0x251a)];
time_t currtime=time(NULL);snprintf(buf,sizeof(buf),
"\\x25\\x73\\x3a\\x20\\x6c\\x65\\x76\\x65\\x6c\\x20\\x25\\x64\\x20\\x61\\x74"
"\\x20\\x25\\x73\\x3a\\x20\\x25\\x73",ctime(&currtime),lvl,whereStr,msg);if(lvl>=
error_level)tracelog.write(buf);fulllog.write(buf);}
'
                                         },
          '_sys_base_noobf_nojam' => {
                                       'comment' => 'Only comments are removed, nothing else is changed.',
                                       'input' => 'static int error_level = 4; /* error level to show alerts for, initialized by some accessor*/
void log_error(const char* whereStr,const char* msg)
{
    log_common(3,whereStr,msg); //call common function
}
void log_common(int lvl,const char* whereStr,const char* msg)
{
    char buf[2000];
    time_t currtime = time(NULL);
    snprintf(buf,sizeof(buf), "%s: level %d at" 
	" %s: %s",ctime(&currtime),lvl,whereStr,msg);
    if (lvl >= error_level)
	tracelog.write(buf);
    fulllog.write(buf);
}
',
                                       'comment_input' => 'This is a trivial piece of C++  with 1 global var, 2 functions that refer to it and
have local variables or parameters of the same name.',
                                       'output' => 'static int error_level = 4; 
void log_error(const char* whereStr,const char* msg)
{
    log_common(3,whereStr,msg); }
void log_common(int lvl,const char* whereStr,const char* msg)
{
    char buf[2000];
    time_t currtime = time(NULL);
    snprintf(buf,sizeof(buf), "%s: level %d at" 
	" %s: %s",ctime(&currtime),lvl,whereStr,msg);
    if (lvl >= error_level)
	tracelog.write(buf);
    fulllog.write(buf);
}

'
                                     },
          '_sys_base_production_shortest_combs' => {
                                                     'comment' => 'Production mode with "combs" mangler for all symbols (turning each symbol into combination
of characters). Comments are removed, whitespace is jammed, strings and integers are mangled too.',
                                                     'input' => 'static int error_level = 4; /* error level to show alerts for, initialized by some accessor*/
void log_error(const char* whereStr,const char* msg)
{
    log_common(3,whereStr,msg); //call common function
}
void log_common(int lvl,const char* whereStr,const char* msg)
{
    char buf[2000];
    time_t currtime = time(NULL);
    snprintf(buf,sizeof(buf), "%s: level %d at" 
	" %s: %s",ctime(&currtime),lvl,whereStr,msg);
    if (lvl >= error_level)
	tracelog.write(buf);
    fulllog.write(buf);
}
',
                                                     'comment_input' => 'This is a trivial piece of C++  with 1 global var, 2 functions that refer to it and
have local variables or parameters of the same name.',
                                                     'output' => 'static int _Dnm7=(0xb07+6813-0x25a0);void vOLTY(const char*FQFFs,const char*
cUb_e){ey7uq((0x1a62+2267-0x233a),FQFFs,cUb_e);}void ey7uq(int OaHpI,const char*
FQFFs,const char*cUb_e){char ZMusA[(0x9a0+4497-0x1361)];s1PpD uxqVt=GQYDB(EiAvD)
;tABx3(ZMusA,sizeof(ZMusA),
"\\x25\\x73\\x3a\\x20\\x6c\\x65\\x76\\x65\\x6c\\x20\\x25\\x64\\x20\\x61\\x74"
"\\x20\\x25\\x73\\x3a\\x20\\x25\\x73",YRxMw(&uxqVt),OaHpI,FQFFs,cUb_e);if(OaHpI>=_Dnm7)
HK9Hr.K1RLX(ZMusA);wmf6D.K1RLX(ZMusA);}
'
                                                   },
          '_sys_base_debugging' => {
                                     'comment' => 'Whitespaces are not jammed, so line numbers should be same as in original code.
Symbols that will be renamed get <tt>ReplacementFor_</tt> prefix.
Best mode for preparation your code for protection and testing protected output.

',
                                     'input' => 'static int error_level = 4; /* error level to show alerts for, initialized by some accessor*/
void log_error(const char* whereStr,const char* msg)
{
    log_common(3,whereStr,msg); //call common function
}
void log_common(int lvl,const char* whereStr,const char* msg)
{
    char buf[2000];
    time_t currtime = time(NULL);
    snprintf(buf,sizeof(buf), "%s: level %d at" 
	" %s: %s",ctime(&currtime),lvl,whereStr,msg);
    if (lvl >= error_level)
	tracelog.write(buf);
    fulllog.write(buf);
}
',
                                     'comment_input' => 'This is a trivial piece of C++  with 1 global var, 2 functions that refer to it and
have local variables or parameters of the same name.',
                                     'output' => 'static int ReplacementFor_error_level = 4;  
void ReplacementFor_log_error(const char* ReplacementFor_whereStr,const char* ReplacementFor_msg)
{
    ReplacementFor_log_common(3,ReplacementFor_whereStr,ReplacementFor_msg); 
}
void ReplacementFor_log_common(int ReplacementFor_lvl,const char* ReplacementFor_whereStr,const char* ReplacementFor_msg)
{
    char ReplacementFor_buf[2000];
    ReplacementFor_time_t ReplacementFor_currtime = ReplacementFor_time(ReplacementFor_NULL);
    ReplacementFor_snprintf(ReplacementFor_buf,sizeof(ReplacementFor_buf), "%s: level %d at" 
	" %s: %s",ReplacementFor_ctime(&ReplacementFor_currtime),ReplacementFor_lvl,ReplacementFor_whereStr,ReplacementFor_msg);
    if (ReplacementFor_lvl >= ReplacementFor_error_level)
	ReplacementFor_tracelog.ReplacementFor_write(ReplacementFor_buf);
    ReplacementFor_fulllog.ReplacementFor_write(ReplacementFor_buf);
}

'
                                   },
          '_sys_base_production' => {
                                      'comment' => 'Production mode with md5 mangler for all symbols. Comments are removed, whitespace is
jammed, strings and integers are mangled too.',
                                      'input' => 'static int error_level = 4; /* error level to show alerts for, initialized by some accessor*/
void log_error(const char* whereStr,const char* msg)
{
    log_common(3,whereStr,msg); //call common function
}
void log_common(int lvl,const char* whereStr,const char* msg)
{
    char buf[2000];
    time_t currtime = time(NULL);
    snprintf(buf,sizeof(buf), "%s: level %d at" 
	" %s: %s",ctime(&currtime),lvl,whereStr,msg);
    if (lvl >= error_level)
	tracelog.write(buf);
    fulllog.write(buf);
}
',
                                      'comment_input' => 'This is a trivial piece of C++  with 1 global var, 2 functions that refer to it and
have local variables or parameters of the same name.',
                                      'output' => 'static int za6a4d01e87=(0x1506+871-0x1869);void z9fe5637c80(const char*
zea4f2a49a5,const char*z4796679a82){z2ddfb2b116((0x570+7693-0x237a),zea4f2a49a5,
z4796679a82);}void z2ddfb2b116(int z072f095beb,const char*zea4f2a49a5,const char
*z4796679a82){char z0aee77a0de[(0x108a+3588-0x16be)];zb125c3488c zf020607702=
z8e4d239361(z3f6bbee370);zffa19ce93a(z0aee77a0de,sizeof(z0aee77a0de),
"\\x25\\x73\\x3a\\x20\\x6c\\x65\\x76\\x65\\x6c\\x20\\x25\\x64\\x20\\x61\\x74"
"\\x20\\x25\\x73\\x3a\\x20\\x25\\x73",z716ec554aa(&zf020607702),z072f095beb,zea4f2a49a5
,z4796679a82);if(z072f095beb>=za6a4d01e87)zb5837460d1.z8ff0c54161(z0aee77a0de);
zcfb1131784.z8ff0c54161(z0aee77a0de);}
'
                                    },
          '_sys_base_production_shortest' => {
                                               'comment' => 'All symbols get shortest name possible (though without considering their scope).
Comments are removed, whitespace is jammed, integer and string constants are left as is.
',
                                               'input' => 'static int error_level = 4; /* error level to show alerts for, initialized by some accessor*/
void log_error(const char* whereStr,const char* msg)
{
    log_common(3,whereStr,msg); //call common function
}
void log_common(int lvl,const char* whereStr,const char* msg)
{
    char buf[2000];
    time_t currtime = time(NULL);
    snprintf(buf,sizeof(buf), "%s: level %d at" 
	" %s: %s",ctime(&currtime),lvl,whereStr,msg);
    if (lvl >= error_level)
	tracelog.write(buf);
    fulllog.write(buf);
}
',
                                               'comment_input' => 'This is a trivial piece of C++  with 1 global var, 2 functions that refer to it and
have local variables or parameters of the same name.',
                                               'output' => 'static int a=4;void e(const char*U,const char*M){b(3,U,M);}void b(int N,const 
char*U,const char*M){char D[2000];E d=h(o);n(D,sizeof(D),"%s: level %d at"
" %s: %s",k(&d),N,U,M);if(N>=a)g.V(D);w.V(D);}
'
                                             },
          '_sys_base_debugging_jam' => {
                                         'comment' => 'Whitespaces are jammed. Symbols that will be renamed get <tt>ReplacementFor_</tt> prefix.
Prepare your code for protection using this mode too after testing it protected with
previous mode  to check whether removal of line breaks will
affect your code (it should not!! but it would be easier to detect the problem caused
by newline removal using this preset rather than production presets).',
                                         'input' => 'static int error_level = 4; /* error level to show alerts for, initialized by some accessor*/
void log_error(const char* whereStr,const char* msg)
{
    log_common(3,whereStr,msg); //call common function
}
void log_common(int lvl,const char* whereStr,const char* msg)
{
    char buf[2000];
    time_t currtime = time(NULL);
    snprintf(buf,sizeof(buf), "%s: level %d at" 
	" %s: %s",ctime(&currtime),lvl,whereStr,msg);
    if (lvl >= error_level)
	tracelog.write(buf);
    fulllog.write(buf);
}
',
                                         'comment_input' => 'This is a trivial piece of C++  with 1 global var, 2 functions that refer to it and
have local variables or parameters of the same name.',
                                         'output' => 'static int ReplacementFor_error_level=4;void ReplacementFor_log_error(const char
*ReplacementFor_whereStr,const char*ReplacementFor_msg){
ReplacementFor_log_common(3,ReplacementFor_whereStr,ReplacementFor_msg);}void 
ReplacementFor_log_common(int ReplacementFor_lvl,const char*
ReplacementFor_whereStr,const char*ReplacementFor_msg){char ReplacementFor_buf[
2000];ReplacementFor_time_t ReplacementFor_currtime=ReplacementFor_time(
ReplacementFor_NULL);ReplacementFor_snprintf(ReplacementFor_buf,sizeof(
ReplacementFor_buf),"%s: level %d at"" %s: %s",ReplacementFor_ctime(&
ReplacementFor_currtime),ReplacementFor_lvl,ReplacementFor_whereStr,
ReplacementFor_msg);if(ReplacementFor_lvl>=ReplacementFor_error_level)
ReplacementFor_tracelog.ReplacementFor_write(ReplacementFor_buf);
ReplacementFor_fulllog.ReplacementFor_write(ReplacementFor_buf);}
'
                                       },
          '_sys_base_production_lI' => {
                                         'comment' => 'Production mode with mangler that turns all symbols into names like <tt>IIlIllI</tt>.
Comments are removed, whitespace is jammed, strings and integers are mangled too.',
                                         'input' => 'static int error_level = 4; /* error level to show alerts for, initialized by some accessor*/
void log_error(const char* whereStr,const char* msg)
{
    log_common(3,whereStr,msg); //call common function
}
void log_common(int lvl,const char* whereStr,const char* msg)
{
    char buf[2000];
    time_t currtime = time(NULL);
    snprintf(buf,sizeof(buf), "%s: level %d at" 
	" %s: %s",ctime(&currtime),lvl,whereStr,msg);
    if (lvl >= error_level)
	tracelog.write(buf);
    fulllog.write(buf);
}
',
                                         'comment_input' => 'This is a trivial piece of C++  with 1 global var, 2 functions that refer to it and
have local variables or parameters of the same name.',
                                         'output' => 'static int IllII=(0x571+109-0x5da);void llIII(const char*IlIlI,const char*lIIlI)
{lIlII((0xbe4+4407-0x1d18),IlIlI,lIIlI);}void lIlII(int IIIlI,const char*IlIlI,
const char*lIIlI){char llIlI[(0xc49+4026-0x1433)];IIlll IIlII=Illll(lIlIl);lIlll
(llIlI,sizeof(llIlI),
"\\x25\\x73\\x3a\\x20\\x6c\\x65\\x76\\x65\\x6c\\x20\\x25\\x64\\x20\\x61\\x74"
"\\x20\\x25\\x73\\x3a\\x20\\x25\\x73",IlIII(&IIlII),IIIlI,IlIlI,lIIlI);if(IIIlI>=IllII)
lllll.lllII(llIlI);llIll.lllII(llIlI);}
'
                                       },
          'aux' => {}
        };
