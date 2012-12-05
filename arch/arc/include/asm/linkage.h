/*Sameer: */
#ifndef __ASM_LINKAGE_H
#define __ASM_LINKAGE_H

#define SYMBOL_NAME_STR(X) #X
#define SYMBOL_NAME(X) X

/* FIXME :: does not work with the ARC gcc */
#ifdef CONFIG_ARCH
#define SYMBOL_NAME_LABEL(X) X:
#else
#define SYMBOL_NAME_LABEL(X) X##:
#endif  /* CONFIG_ARCH */


#endif
