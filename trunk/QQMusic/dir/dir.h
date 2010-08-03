#ifndef _DIR_H_
#define _DIR_H_

#ifdef	__cplusplus
extern "C" {
#endif

#if BUILDING_DLL
# define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define DLLIMPORT __declspec (dllimport)
#endif /* Not BUILDING_DLL */


DLLIMPORT void dir(char *path,char *ppath,void(*oprt)(char*,char*));

#ifdef	__cplusplus
}
#endif

#endif /* _DIR_H_ */
