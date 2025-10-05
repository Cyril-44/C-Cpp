/* glibc bindings for target ABI version glibc 2.5 */
#if !defined (__LIBC_CUSTOM_BINDINGS_H__)

#  if !defined (__OBJC__) && !defined (__ASSEMBLER__)
#    if defined (__cplusplus)
extern "C" {
#    endif

/* Symbols redirected to earlier glibc versions */

/* Symbols introduced in newer glibc versions, which must not be used */
__asm__(".symver memcpy, memcpy@GLIBC_2.2.5");

#    if defined (__cplusplus)
}
#    endif
#  endif /* !defined (__OBJC__) && !defined (__ASSEMBLER__) */
#endif
