// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� COREAUDIOAPI_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// COREAUDIOAPI_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef COREAUDIOAPI_EXPORTS
#define COREAUDIOAPI_API __declspec(dllexport)
#else
#define COREAUDIOAPI_API __declspec(dllimport)
#endif


extern COREAUDIOAPI_API bool GetVolumnVista(UINT& uVol);
extern COREAUDIOAPI_API void SetVolumnVista(UINT dwVolume);
extern COREAUDIOAPI_API void SetXPVolume( UINT dwVolume );
extern COREAUDIOAPI_API bool GetXPVolume( UINT& uVol );