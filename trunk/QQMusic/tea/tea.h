#ifndef _TEA_H_
#define _TEA_H_

#ifdef	__cplusplus
extern "C" {
#endif

#if BUILDING_DLL
# define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define DLLIMPORT __declspec (dllimport)
#endif /* Not BUILDING_DLL */

#define DWORD unsigned long

DLLIMPORT int decrypt(const char *file,char *path);

#ifdef	__cplusplus
}
#endif

#endif /* _TEA_H_ */
