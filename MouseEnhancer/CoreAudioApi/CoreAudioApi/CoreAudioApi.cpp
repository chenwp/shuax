// CoreAudioApi.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "CoreAudioApi.h"
#include "mmsystem.h"

bool GetVolumnVista_Inner(UINT& uVol)
{
     IMMDeviceEnumerator* m_pEnumerator = NULL;
     HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&m_pEnumerator);
     if (FAILED(hr)) return FALSE;
     IMMDevice* m_pDeviceOut = NULL;
     hr = m_pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &m_pDeviceOut);
     if (FAILED(hr)) return FALSE;
     IAudioEndpointVolume* m_AudioEndpointVolume = NULL;
     hr = m_pDeviceOut->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&m_AudioEndpointVolume);
     if (FAILED(hr)) return FALSE;
     IAudioClient* pAudioClient = NULL;
     hr = m_pDeviceOut->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);
     if (FAILED(hr)) return FALSE;

	 BOOL test;
	 m_AudioEndpointVolume->GetMute(&test);
	 if(test==true)
	 {
		uVol = 0;
		return TRUE;
	 }
     UINT uStep = 0;
     UINT uStepCount = 0;
     hr = m_AudioEndpointVolume->GetVolumeStepInfo(&uStep, &uStepCount);
     if (FAILED(hr)) return FALSE;
     uVol = (uStep * 100) / uStepCount;
     return TRUE;
}
void SetVolumnVista_Inner(UINT dwVolume)
{
    CoInitialize(NULL); 
	IMMDeviceEnumerator* m_pEnumerator = NULL;
     HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&m_pEnumerator);
     if (FAILED(hr)) return;
     IMMDevice* m_pDeviceOut = NULL;
     hr = m_pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &m_pDeviceOut);
     if (FAILED(hr)) return;
     IAudioEndpointVolume* m_AudioEndpointVolume = NULL;
     hr = m_pDeviceOut->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&m_AudioEndpointVolume);
     if (FAILED(hr)) return;
     IAudioClient* pAudioClient = NULL;
     hr = m_pDeviceOut->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);
     if (FAILED(hr)) return;
     UINT uStep = 0;
     UINT uStepCount = 0;
     hr = m_AudioEndpointVolume->GetVolumeStepInfo(&uStep, &uStepCount);
     if (FAILED(hr)) return;
     UINT uVol = (dwVolume * uStepCount) / 100;
     BOOL bUp = uVol < uStep;
     UINT uLess = bUp ? (uStep - uVol) : (uVol - uStep);
     for (UINT i = 0; i < uLess; i++)
     {
         if (!bUp)
         {
             hr = m_AudioEndpointVolume->VolumeStepUp(&GUID_NULL);
         }
         else
         {
             hr = m_AudioEndpointVolume->VolumeStepDown(&GUID_NULL);
         }
         if (FAILED(hr)) return;
     }
	 CoUninitialize();
}
COREAUDIOAPI_API bool GetVolumnVista(UINT& uVol)
{
	CoInitialize(NULL);
	bool result = GetVolumnVista_Inner(uVol);
	CoUninitialize();
	return result;
}
COREAUDIOAPI_API void SetVolumnVista(UINT dwVolume)
{
	CoInitialize(NULL);
	SetVolumnVista_Inner(dwVolume);
	CoUninitialize();
}
COREAUDIOAPI_API void SetXPVolume(UINT dwVolume )
{
    HMIXER m_hMixer = NULL;
    MIXERCAPS m_mxcaps;
    ::ZeroMemory(&m_mxcaps, sizeof(MIXERCAPS));
    if (::mixerGetNumDevs() != 0)
    {
        if (::mixerOpen(&m_hMixer,0,0,NULL,MIXER_OBJECTF_MIXER )
            != MMSYSERR_NOERROR)
            return ;
        if (::mixerGetDevCaps((UINT)m_hMixer, &m_mxcaps, sizeof(MIXERCAPS))
            != MMSYSERR_NOERROR)
            return ;
    }
    if (m_hMixer == NULL)    return ;
    MIXERLINE mxl;
    MIXERCONTROL mxc;
    MIXERLINECONTROLS mxlc;
    mxl.cbStruct = sizeof(MIXERLINE);
    mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
    if (::mixerGetLineInfo((HMIXEROBJ)m_hMixer,
        &mxl,
        MIXER_OBJECTF_HMIXER |
        MIXER_GETLINEINFOF_COMPONENTTYPE)
        != MMSYSERR_NOERROR)        return ;
    
    DWORD m_controlnum=mxl.cChannels; 
    mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
    mxlc.dwLineID = mxl.dwLineID;
    mxlc.dwControlType =MIXERCONTROL_CONTROLTYPE_VOLUME; 
    mxlc.cControls =1;
    mxlc.cbmxctrl = sizeof(MIXERCONTROL);
    mxlc.pamxctrl = &mxc;
    if (::mixerGetLineControls((HMIXEROBJ)m_hMixer,
        &mxlc,
        MIXER_OBJECTF_HMIXER |
        MIXER_GETLINECONTROLSF_ONEBYTYPE )
        != MMSYSERR_NOERROR)
        return;
    //PCHAR m_name=mxc.szName; 
    DWORD m_controlid = mxc.dwControlID;
    LONG m_min= mxc.Bounds.lMinimum;
    LONG m_max= mxc.Bounds.lMaximum;

    DWORD dVol = ( dwVolume * m_max ) / 100;

    MIXERCONTROLDETAILS_SIGNED mxcdVolume[2] = {dVol, dVol};
    MIXERCONTROLDETAILS mxcd;
    mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
    mxcd.dwControlID = m_controlid;
    mxcd.cChannels = m_controlnum;
    mxcd.cMultipleItems = 0;
    mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_SIGNED);
    mxcd.paDetails = &mxcdVolume;
    if (::mixerSetControlDetails((HMIXEROBJ)m_hMixer,
        &mxcd,
        MIXER_OBJECTF_HMIXER |
        MIXER_SETCONTROLDETAILSF_VALUE)
        != MMSYSERR_NOERROR)    return ;

    mixerClose( m_hMixer );
}


COREAUDIOAPI_API bool GetXPVolume(UINT& uVol)
{
    HMIXER m_hMixer = NULL;
    MIXERCAPS m_mxcaps;
    ::ZeroMemory(&m_mxcaps, sizeof(MIXERCAPS));
    if (::mixerGetNumDevs() != 0)
    {
        if (::mixerOpen(&m_hMixer,0,0,NULL,MIXER_OBJECTF_MIXER )
            != MMSYSERR_NOERROR)
            return FALSE;
        if (::mixerGetDevCaps((UINT)m_hMixer, &m_mxcaps, sizeof(MIXERCAPS))
            != MMSYSERR_NOERROR)
            return FALSE;
    }
    if (m_hMixer == NULL)    return FALSE;
    MIXERLINE mxl;
    MIXERCONTROL mxc;
    MIXERLINECONTROLS mxlc;
    mxl.cbStruct = sizeof(MIXERLINE);
    mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
    if (::mixerGetLineInfo((HMIXEROBJ)m_hMixer,
        &mxl,
        MIXER_OBJECTF_HMIXER |
        MIXER_GETLINEINFOF_COMPONENTTYPE)
        != MMSYSERR_NOERROR)        return FALSE;
    
    DWORD m_controlnum=mxl.cChannels; 
    mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
    mxlc.dwLineID = mxl.dwLineID;
    mxlc.dwControlType =MIXERCONTROL_CONTROLTYPE_VOLUME; 
    mxlc.cControls =1;
    mxlc.cbmxctrl = sizeof(MIXERCONTROL);
    mxlc.pamxctrl = &mxc;
    if (::mixerGetLineControls((HMIXEROBJ)m_hMixer,
        &mxlc,
        MIXER_OBJECTF_HMIXER |
        MIXER_GETLINECONTROLSF_ONEBYTYPE )
        != MMSYSERR_NOERROR)
        return FALSE;
   // PCHAR m_name=mxc.szName; 
    DWORD m_controlid = mxc.dwControlID;
    LONG m_min= mxc.Bounds.lMinimum;
    LONG m_max= mxc.Bounds.lMaximum;
    
    DWORD dVol = ( uVol * m_max ) / 100;
    
    MIXERCONTROLDETAILS_SIGNED mxcdVolume[2] = {dVol, dVol};
    MIXERCONTROLDETAILS mxcd;
    mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
    mxcd.dwControlID = m_controlid;
    mxcd.cChannels = m_controlnum;
    mxcd.cMultipleItems = 0;
    mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_SIGNED);
    mxcd.paDetails = &mxcdVolume;
    if (::mixerGetControlDetails((HMIXEROBJ)m_hMixer,
        &mxcd,
        MIXER_OBJECTF_HMIXER |
        MIXER_SETCONTROLDETAILSF_VALUE)
        != MMSYSERR_NOERROR)    return FALSE;
    
    mixerClose( m_hMixer );

    uVol = ( mxcdVolume[0].lValue * 100 ) / m_max;

    return TRUE;
}