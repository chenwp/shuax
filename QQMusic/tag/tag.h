#ifndef _TAG_H_
#define _TAG_H_

#include <windows.h>
#include <stdio.h>

#ifdef	__cplusplus
extern "C" {
#endif

#if BUILDING_DLL
# define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define DLLIMPORT __declspec (dllimport)
#endif /* Not BUILDING_DLL */

DLLIMPORT int GetWmaTag(const char *filename,wchar_t *f_name);

#ifdef	__cplusplus
}
#endif

#endif /* _TAG_H_ */
