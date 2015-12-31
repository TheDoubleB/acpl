#ifndef ACPL_DEVEL_H
#define ACPL_DEVEL_H

#define acplDbgStringify(N_ELEMENT) #N_ELEMENT
#define acplDbgStringifyIndrct(N_ELEMENT) acplDbgStringify(N_ELEMENT)

#define acplDbgSrcLoc __FILE__ ":" acplDbgStringifyIndrct(__LINE__)

#endif // ACPL_DEVEL_H
