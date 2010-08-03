
; gdiplus_structures_ver1.inc         additions: 03/05
; original version by xzazet
;
; 1) GUID conversion added
;
; 2)  EncoderParameter  STRUC     is:
;                        pGUID  GUID <?>
;                  {not  pGUID  DWORD ?}

;dougiem


;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

; E-Z GUID Converting from C++ to MASM, by NaN, 2002

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

MGUID macro a_c_guid
      LOCAL guidsize, guid1, guid2, guid3, guid4, guid5, guid6, guid7, guid8, guid9, guid10, guid11
      quidsize = @SizeStr(a_c_guid)
      if quidsize eq 36
         guid1 SUBSTR <a_c_guid>, 1, 8
         guid1 CATSTR <0>,guid1,<H>
         guid2 SUBSTR <a_c_guid>, 10, 4
         guid2 CATSTR <0>,guid2,<H>
         guid3 SUBSTR <a_c_guid>, 15, 4
         guid3 CATSTR <0>,guid3,<H>
         guid4 SUBSTR <a_c_guid>, 20, 2
         guid4 CATSTR <0>,guid4,<H>
         guid5 SUBSTR <a_c_guid>, 22, 2
         guid5 CATSTR <0>,guid5,<H>
         guid6 SUBSTR <a_c_guid>, 25, 2
         guid6 CATSTR <0>,guid6,<H>
         guid7 SUBSTR <a_c_guid>, 27, 2
         guid7 CATSTR <0>,guid7,<H>
         guid8 SUBSTR <a_c_guid>, 29, 2
         guid8 CATSTR <0>,guid8,<H>
         guid9 SUBSTR <a_c_guid>, 31, 2
         guid9 CATSTR <0>,guid9,<H>
         guid10 SUBSTR <a_c_guid>, 33, 2
         guid10 CATSTR <0>,guid10,<H>
         guid11 SUBSTR <a_c_guid>, 35, 2
         guid11 CATSTR <0>,guid11,<H>
      else
         echo
         echo FORMAT:  LABEL MGUID (0000000-0000-0000-0000-000000000000)
         echo Must be 36 bytes long, including dashes!
         echo
         .err <MGUID ERROR>
      endif
      exitm <TEXTEQU !<{guid1, guid2, guid3, {guid4, guid5, guid6, guid7, guid8, guid9, guid10, guid11}}!> >     
endm  


;---------------------------------------------------------------------------
; Decoder parameter sets
;---------------------------------------------------------------------------
     GUID_DecoderTransColor       MGUID (b7a98c8f-dce7-457d-bfa5-cea71bd14dd6)
     GUID_DecoderTransRange       MGUID (abeed189-d988-4d03-b425-571055c76ad1)
     GUID_DecoderOutputChannel    MGUID (2ff8f51e-724d-45fe-86ce-1777a056da60)
     GUID_DecoderIconRes          MGUID (5c656eec-e94f-45ba-a6f6-1062e85f4a7f)

;---------------------------------------------------------------------------
; Encoder parameter sets
;---------------------------------------------------------------------------
     GUID_EncoderCompression      MGUID (E09D739D-ccd4-44ee-8eba-3fbf8be4fc58)
     GUID_EncoderColorDepth       MGUID (66087055-ad66-4c7c-9a18-38a2310b8337)
     GUID_EncoderScanMethod       MGUID (3A4E2661-3109-4e56-8536-42c156e7dcfa)
     GUID_EncoderVersion          MGUID (24D18C76-814a-41a4-bf53-1c219cccf797)
     GUID_EncoderRenderMethod     MGUID (6D42C53A-229a-4825-8bb7-5c99e2b9a8b8)
     GUID_EncoderQuality          MGUID (1D5BE4B5-fa4a-452d-9cdd-5db35105e7eb)
     GUID_EncoderTransformation   MGUID (8D0EB2D1-a58e-4ea8-aa14-108074b7b6f9)
     GUID_EncoderLuminanceTable   MGUID (EDB33BCE-0266-4a77-b904-27216099e717)
     GUID_EncoderChrominanceTable MGUID (F2E455DC-09b3-4316-8260-676ada32481c)
     GUID_EncoderSaveFlag         MGUID (292266FC-ac40-47bf-8cfc-a85b89a655de)
     GUID_CodecIImageBytes        MGUID (025D1823-6c7d-447b-bbdb-a3cbc3dfa2fc)

;---------------------------------------------------------------------------
; Image format sets
;---------------------------------------------------------------------------
     GUID_ImageFormatUndefined    MGUID (B96B3CA9-0728-11D3-9D7B-0000F81EF32E)
     GUID_ImageFormatMemoryBMP    MGUID (B96B3CAA-0728-11D3-9D7B-0000F81EF32E)
     GUID_ImageFormatBMP          MGUID (B96B3CAB-0728-11D3-9D7B-0000F81EF32E)
     GUID_ImageFormatEMF          MGUID (B96B3CAC-0728-11D3-9D7B-0000F81EF32E)
     GUID_ImageFormatWMF          MGUID (B96B3CAD-0728-11D3-9D7B-0000F81EF32E)
     GUID_ImageFormatJPEG         MGUID (B96B3CAE-0728-11D3-9D7B-0000F81EF32E)
     GUID_ImageFormatPNG          MGUID (B96B3CAF-0728-11D3-9D7B-0000F81EF32E)
     GUID_ImageFormatGIF          MGUID (B96B3CB0-0728-11D3-9D7B-0000F81EF32E)
     GUID_ImageFormatTIFF         MGUID (B96B3CB1-0728-11D3-9D7B-0000F81EF32E)
     GUID_ImageFormatEXIF         MGUID (B96B3CB2-0728-11D3-9D7B-0000F81EF32E)
     GUID_ImageFormatIcon         MGUID (B96B3CB3-0728-11D3-9D7B-0000F81EF32E)



.data

     ImageFormatUndefined         GUID GUID_ImageFormatUndefined
     ImageFormatMemoryBMP         GUID GUID_ImageFormatMemoryBMP
     ImageFormatBMP               GUID GUID_ImageFormatBMP
     ImageFormatEMF               GUID GUID_ImageFormatEMF
     ImageFormatWMF               GUID GUID_ImageFormatWMF
     ImageFormatJPEG              GUID GUID_ImageFormatJPEG
     ImageFormatPNG               GUID GUID_ImageFormatPNG
     ImageFormatGIF               GUID GUID_ImageFormatGIF
     ImageFormatTIFF              GUID GUID_ImageFormatTIFF
     ImageFormatEXIF              GUID GUID_ImageFormatEXIF
     ImageFormatIcon              GUID GUID_ImageFormatIcon

     DecoderTransColor            GUID GUID_DecoderTransColor     
     DecoderTransRange            GUID GUID_DecoderTransRange    
     DecoderOutputChannel         GUID GUID_DecoderOutputChannel 
     DecoderIconRes               GUID GUID_DecoderIconRes  

     gEncoderCompression          GUID GUID_EncoderCompression       
     gEncoderColorDepth           GUID GUID_EncoderColorDepth    
     gEncoderScanMethod           GUID GUID_EncoderScanMethod    
     gEncoderVersion              GUID GUID_EncoderVersion           
     gEncoderRenderMethod         GUID GUID_EncoderRenderMethod 
     gEncoderQuality              GUID GUID_EncoderQuality    
     gEncoderTransformation       GUID GUID_EncoderTransformation 
     gEncoderLuminanceTable       GUID GUID_EncoderLuminanceTable 
     gEncoderChrominanceTable     GUID GUID_EncoderChrominanceTable  
     gEncoderSaveFlag             GUID GUID_EncoderSaveFlag    
     gCodecIImageBytes            GUID GUID_CodecIImageBytes  
;dougiem



.data?
GdiplusStartupInput STRUCT
    GdiplusVersion           DWORD ?
    DebugEventCallback       DWORD ?
    SuppressBackgroundThread DWORD ?
    SuppressExternalCodecs   DWORD ?
GdiplusStartupInput ENDS

PointF STRUCT
    x         REAL4 ?
    y         REAL4 ?
PointF ENDS

RectF STRUCT
    Left      REAL4 ?
    Top       REAL4 ?
    Right     REAL4 ?
    Bottom    REAL4 ?
RectF ENDS

SizeF STRUCT
    sx        REAL4 ?
    sy        REAL4 ?
SizeF ENDS

; xzazet
;-----------------------------------------------
; GDI+ Constants
;-----------------------------------------------
LF_FACESIZE = 32
; Shift count and bit mask for A, R, G, B components
AlphaShift    equ  24
RedShift      equ  16
GreenShift    equ  8
BlueShift     equ  0
AlphaMask     equ  0FF000000h
RedMask       equ  000FF0000h
GreenMask     equ  00000FF00h
BlueMask      equ  0000000FFh

; Image property types
PropertyTagTypeByte               equ  1
PropertyTagTypeASCII              equ  2
PropertyTagTypeShort              equ  3
PropertyTagTypeLong               equ  4
PropertyTagTypeRational           equ  5
PropertyTagTypeUndefined          equ  7
PropertyTagTypeSLONG              equ  9
PropertyTagTypeSRational          equ  10

; Image property ID tags
PropertyTagExifIFD                equ  8769h
PropertyTagGpsIFD                 equ  8825h

PropertyTagNewSubfileType         equ  0FEh
PropertyTagSubfileType            equ  0FFh
PropertyTagImageWidth             equ  100h
PropertyTagImageHeight            equ  101h
PropertyTagBitsPerSample          equ  102h
PropertyTagCompression            equ  103h
PropertyTagPhotometricInterp      equ  106h
PropertyTagThreshHolding          equ  107h
PropertyTagCellWidth              equ  108h
PropertyTagCellHeight             equ  109h
PropertyTagFillOrder              equ  10Ah
PropertyTagDocumentName           equ  10Dh
PropertyTagImageDescription       equ  10Eh
PropertyTagEquipMake              equ  10Fh
PropertyTagEquipModel             equ  110h
PropertyTagStripOffsets           equ  111h
PropertyTagOrientation            equ  112h
PropertyTagSamplesPerPixel        equ  115h
PropertyTagRowsPerStrip           equ  116h
PropertyTagStripBytesCount        equ  117h
PropertyTagMinSampleValue         equ  118h
PropertyTagMaxSampleValue         equ  119h
PropertyTagXResolution            equ  11Ah      ; Image resolution in width direction
PropertyTagYResolution            equ  11Bh      ; Image resolution in height direction
PropertyTagPlanarConfig           equ  11Ch      ; Image data arrangement
PropertyTagPageName               equ  11Dh
PropertyTagXPosition              equ  11Eh
PropertyTagYPosition              equ  11Fh
PropertyTagFreeOffset             equ  120h
PropertyTagFreeByteCounts         equ  121h
PropertyTagGrayResponseUnit       equ  122h
PropertyTagGrayResponseCurve      equ  123h
PropertyTagT4Option               equ  124h
PropertyTagT6Option               equ  125h
PropertyTagResolutionUnit         equ  128h      ; Unit of X and Y resolution
PropertyTagPageNumber             equ  129h
PropertyTagTransferFuncition      equ  12Dh
PropertyTagSoftwareUsed           equ  131h
PropertyTagDateTime               equ  132h
PropertyTagArtist                 equ  13Bh
PropertyTagHostComputer           equ  13Ch
PropertyTagPredictor              equ  13Dh
PropertyTagWhitePoint             equ  13Eh
PropertyTagPrimaryChromaticities  equ  13Fh
PropertyTagColorMap               equ  140h
PropertyTagHalftoneHints          equ  141h
PropertyTagTileWidth              equ  142h
PropertyTagTileLength             equ  143h
PropertyTagTileOffset             equ  144h
PropertyTagTileByteCounts         equ  145h
PropertyTagInkSet                 equ  14Ch
PropertyTagInkNames               equ  14Dh
PropertyTagNumberOfInks           equ  14Eh
PropertyTagDotRange               equ  150h
PropertyTagTargetPrinter          equ  151h
PropertyTagExtraSamples           equ  152h
PropertyTagSampleFormat           equ  153h
PropertyTagSMinSampleValue        equ  154h
PropertyTagSMaxSampleValue        equ  155h
PropertyTagTransferRange          equ  156h

PropertyTagJPEGProc               equ  200h
PropertyTagJPEGInterFormat        equ  201h
PropertyTagJPEGInterLength        equ  202h
PropertyTagJPEGRestartInterval    equ  203h
PropertyTagJPEGLosslessPredictors equ  205h
PropertyTagJPEGPointTransforms    equ  206h
PropertyTagJPEGQTables            equ  207h
PropertyTagJPEGDCTables           equ  208h
PropertyTagJPEGACTables           equ  209h
PropertyTagYCbCrCoefficients      equ  211h
PropertyTagYCbCrSubsampling       equ  212h
PropertyTagYCbCrPositioning       equ  213h
PropertyTagREFBlackWhite          equ  214h
PropertyTagICCProfile             equ  8773h     ; This TAG is defined by ICC for embedded ICC in TIFF
PropertyTagGamma                  equ  301h
PropertyTagICCProfileDescriptor   equ  302h
PropertyTagSRGBRenderingIntent    equ  303h
PropertyTagImageTitle             equ  320h
PropertyTagCopyright              equ  8298h

; Extra TAGs (Like Adobe Image Information tags etc.)
PropertyTagResolutionXUnit             equ  5001h
PropertyTagResolutionYUnit             equ  5002h
PropertyTagResolutionXLengthUnit       equ  5003h
PropertyTagResolutionYLengthUnit       equ  5004h
PropertyTagPrintFlags                  equ  5005h
PropertyTagPrintFlagsVersion           equ  5006h
PropertyTagPrintFlagsCrop              equ  5007h
PropertyTagPrintFlagsBleedWidth        equ  5008h
PropertyTagPrintFlagsBleedWidthScale   equ  5009h
PropertyTagHalftoneLPI                 equ  500Ah
PropertyTagHalftoneLPIUnit             equ  500Bh
PropertyTagHalftoneDegree              equ  500Ch
PropertyTagHalftoneShape               equ  500Dh
PropertyTagHalftoneMisc                equ  500Eh
PropertyTagHalftoneScreen              equ  500Fh
PropertyTagJPEGQuality                 equ  5010h
PropertyTagGridSize                    equ  5011h
PropertyTagThumbnailFormat             equ  5012h     ; 1 = JPEG, 0 = RAW RGB
PropertyTagThumbnailWidth              equ  5013h
PropertyTagThumbnailHeight             equ  5014h
PropertyTagThumbnailColorDepth         equ  5015h
PropertyTagThumbnailPlanes             equ  5016h
PropertyTagThumbnailRawBytes           equ  5017h
PropertyTagThumbnailSize               equ  5018h
PropertyTagThumbnailCompressedSize     equ  5019h
PropertyTagColorTransferFunction       equ  501Ah
PropertyTagThumbnailData               equ  501Bh     ; RAW thumbnail bits in JPEG format or RGB format
                                       ; depends on PropertyTagThumbnailFormat

; Thumbnail related TAGs
PropertyTagThumbnailImageWidth         equ  5020h     ; Thumbnail width
PropertyTagThumbnailImageHeight        equ  5021h     ; Thumbnail height
PropertyTagThumbnailBitsPerSample      equ  5022h     ; Number of bits per component
PropertyTagThumbnailCompression        equ  5023h     ; Compression Scheme
PropertyTagThumbnailPhotometricInterp  equ  5024h     ; Pixel composition
PropertyTagThumbnailImageDescription   equ  5025h     ; Image Tile
PropertyTagThumbnailEquipMake          equ  5026h     ; Manufacturer of Image Input equipment
PropertyTagThumbnailEquipModel         equ  5027h     ; Model of Image input equipment
PropertyTagThumbnailStripOffsets       equ  5028h     ; Image data location
PropertyTagThumbnailOrientation        equ  5029h     ; Orientation of image
PropertyTagThumbnailSamplesPerPixel    equ  502Ah     ; Number of components
PropertyTagThumbnailRowsPerStrip       equ  502Bh     ; Number of rows per strip
PropertyTagThumbnailStripBytesCount    equ  502Ch     ; Bytes per compressed strip
PropertyTagThumbnailResolutionX        equ  502Dh     ; Resolution in width direction
PropertyTagThumbnailResolutionY        equ  502Eh     ; Resolution in height direction
PropertyTagThumbnailPlanarConfig       equ  502Fh     ; Image data arrangement
PropertyTagThumbnailResolutionUnit     equ  5030h     ; Unit of X and Y resolution
PropertyTagThumbnailTransferFunction   equ  5031h     ; Transfer function
PropertyTagThumbnailSoftwareUsed       equ  5032h     ; Software used
PropertyTagThumbnailDateTime           equ  5033h     ; File change date and time
PropertyTagThumbnailArtist             equ  5034h     ; Person who created the  image
PropertyTagThumbnailWhitePoint         equ  5035h     ; White point chromaticity
PropertyTagThumbnailPrimaryChromaticities   equ  5036h; Chromaticities of primaries
PropertyTagThumbnailYCbCrCoefficients  equ  5037h     ; Color space transformation coefficients
PropertyTagThumbnailYCbCrSubsampling   equ  5038h     ; Subsampling ratio of Y to C
PropertyTagThumbnailYCbCrPositioning   equ  5039h     ; Y and C position
PropertyTagThumbnailRefBlackWhite      equ  503Ah     ; Pair of black and white reference values
PropertyTagThumbnailCopyRight          equ  503Bh     ; CopyRight holder
PropertyTagLuminanceTable              equ  5090h
PropertyTagChrominanceTable            equ  5091h
PropertyTagFrameDelay                  equ  5100h
PropertyTagLoopCount                   equ  5101h
PropertyTagPixelUnit                   equ  5110h     ; Unit specifier for pixel/unit
PropertyTagPixelPerUnitX               equ  5111h     ; Pixels per unit in X
PropertyTagPixelPerUnitY               equ  5112h     ; Pixels per unit in Y
PropertyTagPaletteHistogram            equ  5113h     ; Palette histogram

; EXIF specific tag
PropertyTagExifExposureTime            equ  829Ah
PropertyTagExifFNumber                 equ  829Dh
PropertyTagExifExposureProg            equ  8822h
PropertyTagExifSpectralSense           equ  8824h
PropertyTagExifISOSpeed                equ  8827h
PropertyTagExifOECF                    equ  8828h
PropertyTagExifVer                     equ  9000h
PropertyTagExifDTOrig                  equ  9003h     ; Date & time of original
PropertyTagExifDTDigitized             equ  9004h     ; Date & time of digital data generation
PropertyTagExifCompConfig              equ  9101h
PropertyTagExifCompBPP                 equ  9102h
PropertyTagExifShutterSpeed            equ  9201h
PropertyTagExifAperture                equ  9202h
PropertyTagExifBrightness              equ  9203h
PropertyTagExifExposureBias            equ  9204h
PropertyTagExifMaxAperture             equ  9205h
PropertyTagExifSubjectDist             equ  9206h
PropertyTagExifMeteringMode            equ  9207h
PropertyTagExifLightSource             equ  9208h
PropertyTagExifFlash                   equ  9209h
PropertyTagExifFocalLength             equ  920Ah
PropertyTagExifMakerNote               equ  927Ch
PropertyTagExifUserComment             equ  9286h
PropertyTagExifDTSubsec                equ  9290h     ; Date & Time subseconds
PropertyTagExifDTOrigSS                equ  9291h     ; Date & Time original subseconds
PropertyTagExifDTDigSS                 equ  9292h     ; Date & TIme digitized subseconds
PropertyTagExifFPXVer                  equ  0A000h
PropertyTagExifColorSpace              equ  0A001h
PropertyTagExifPixXDim                 equ  0A002h
PropertyTagExifPixYDim                 equ  0A003h
PropertyTagExifRelatedWav              equ  0A004h    ; Related sound file
PropertyTagExifInterop                 equ  0A005h
PropertyTagExifFlashEnergy             equ  0A20Bh
PropertyTagExifSpatialFR               equ  0A20Ch    ; Spatial Frequency Response
PropertyTagExifFocalXRes               equ  0A20Eh    ; Focal Plane X Resolution
PropertyTagExifFocalYRes               equ  0A20Fh    ; Focal Plane Y Resolution
PropertyTagExifFocalResUnit            equ  0A210h    ; Focal Plane Resolution Unit
PropertyTagExifSubjectLoc              equ  0A214h
PropertyTagExifExposureIndex           equ  0A215h
PropertyTagExifSensingMethod           equ  0A217h
PropertyTagExifFileSource              equ  0A300h
PropertyTagExifSceneType               equ  0A301h
PropertyTagExifCfaPattern              equ  0A302h
PropertyTagGpsVer                      equ  0
PropertyTagGpsLatitudeRef              equ  1
PropertyTagGpsLatitude                 equ  2
PropertyTagGpsLongitudeRef             equ  3
PropertyTagGpsLongitude                equ  4
PropertyTagGpsAltitudeRef              equ  5
PropertyTagGpsAltitude                 equ  6
PropertyTagGpsGpsTime                  equ  7
PropertyTagGpsGpsSatellites            equ  8
PropertyTagGpsGpsStatus                equ  9
PropertyTagGpsGpsMeasureMode           equ  0Ah
PropertyTagGpsGpsDop                   equ  0Bh       ; Measurement precision
PropertyTagGpsSpeedRef                 equ  0Ch
PropertyTagGpsSpeed                    equ  0Dh
PropertyTagGpsTrackRef                 equ  0Eh
PropertyTagGpsTrack                    equ  0Fh
PropertyTagGpsImgDirRef                equ  10h
PropertyTagGpsImgDir                   equ  11h
PropertyTagGpsMapDatum                 equ  12h
PropertyTagGpsDestLatRef               equ  13h
PropertyTagGpsDestLat                  equ  14h
PropertyTagGpsDestLongRef              equ  15h
PropertyTagGpsDestLong                 equ  16h
PropertyTagGpsDestBearRef              equ  17h
PropertyTagGpsDestBear                 equ  18h
PropertyTagGpsDestDistRef              equ  19h
PropertyTagGpsDestDist                 equ  1Ah


 COLORBYTES  STRUC    ; OK   
  BlueByte   BYTE ?    
  GreenByte  BYTE ?   
  RedByte    BYTE ?   
  AlphaByte  BYTE ?
 COLORBYTES  ENDS

 COLORLONG  STRUC     ; OK
  longval   DWORD ?
 COLORLONG  ENDS 


ImageCodecInfo  STRUC
  ClassID            GUID  <>  
  FormatID           GUID  <>
  CodecName          DWORD ?                     ;  UNICODE  
  DllName            DWORD ?                     ;  UNICODE 
  FormatDescription  DWORD ?                     ;  UNICODE 
  FilenameExtension  DWORD ?                     ;  UNICODE 
  MimeType           DWORD ?                     ;  UNICODE 
  flags              DWORD ?   
  Version            DWORD ?   
  SigCount           DWORD ?
  SigSize            DWORD ?
  SigPattern         DWORD ?    
  SigMask            DWORD ?   
ImageCodecInfo  ENDS

 ;  Encoder Parameter
 ; ------------------------------------
                                       
 EncoderParameter  STRUC        ; OK             ; NOTE: pGUID is a GUID STRUCTURE
 pGUID             GUID <?>                      ; GUID 
 NumberOfValues    DWORD ?  
 vType             DWORD ?                       ; EncoderParameterValueType 
 value             DWORD ? 
 EncoderParameter  ENDS

 ; Encoder Parameters
 ; -------------------------------------

 EncoderParameters   STRUC      ; OK
  Count              DWORD ?    
  Parameter          EncoderParameter <>                                
 EncoderParameters   ENDS 

 ColorPalette  STRUC            ; OK 
  flags        DWORD ?  
  Count        DWORD ?       
  Entries      DWORD ?                
 ColorPalette  ENDS     

 ColorMatrix  STRUC             ; OK             
  m11         REAL4 ?    
  m12         REAL4 ?     
  m13         REAL4 ?
  m14         REAL4 ?
  m15         REAL4 ?
  m21         REAL4 ?
  m22         REAL4 ?
  m23         REAL4 ?
  m24         REAL4 ?
  m25         REAL4 ?
  m31         REAL4 ?
  m32         REAL4 ?
  m33         REAL4 ?
  m34         REAL4 ?
  m35         REAL4 ?
  m41         REAL4 ?
  m42         REAL4 ?
  m43         REAL4 ?
  m44         REAL4 ?
  m45         REAL4 ?
  m51         REAL4 ?
  m52         REAL4 ?
  m53         REAL4 ?
  m54         REAL4 ?
  m55         REAL4 ?
 ColorMatrix  ENDS 




 BitmapData    STRUC          ; OK                            
  nWIDTH       DWORD ?   
  nHeight      DWORD ?   
  stride       DWORD ?   
  PixelFormat  DWORD ?   
  scan0        DWORD ?   
  Reserved     DWORD ?
 BitmapData    ENDS

 PathData      STRUC          ; OK
  Count        DWORD ?   
  pPoint       DWORD ?            
  types        DWORD ?          
 PathData      ENDS

 PropertyItem  STRUC          ; OK 
  propId       DWORD ?   
  _length      DWORD ?    
  nType        WORD  ?          
  value        DWORD ?   
 PropertyItem  ENDS

 LOGFONTA           STRUC     ; OK
  lfHeight          DWORD ?   
  lfWidth           DWORD ?   
  lfEscapement      DWORD ?   
  lfOrientation     DWORD ?   
  lfWeight          DWORD ?   
  lfItalic          BYTE  ?   
  lfUnderline       BYTE  ?   
  lfStrikeOut       BYTE  ?   
  lfCharSet         BYTE  ?   
  lfOutPrecision    BYTE  ?   
  lfClipPrecision   BYTE  ?   
  lfQuality         BYTE  ?   
  lfPitchAndFamily  BYTE  ?   
  lfFaceName        BYTE  LF_FACESIZE dup(?)
 LOGFONTA           ENDS

 LOGFONTW           STRUC     ; OK
  lfHeight          DWORD ?   
  lfWidth           DWORD ?   
  lfEscapement      DWORD ?   
  lfOrientation     DWORD ?   
  lfWeight          DWORD ?   
  lfItalic          BYTE  ?   
  lfUnderline       BYTE  ?   
  lfStrikeOut       BYTE  ?   
  lfCharSet         BYTE  ?   
  lfOutPrecision    BYTE  ?   
  lfClipPrecision   BYTE  ?
  lfQuality         BYTE  ? 
  lfPitchAndFamily  BYTE  ?   
  lfFaceName        WORD  LF_FACESIZE dup(?)     ; TODO: Spruce this up a bit ???
 LOGFONTW           ENDS

 CharacterRange   STRUC            ; OK
  First           DWORD ?
  _length         DWORD ?
 CharacterRange   ENDS

 PWMFRect16   STRUC                ; OK
  nLEFT       WORD ? 
  nTop        WORD ?
  nRIGHT      WORD ?  
  nBottom     WORD ?
 PWMFRect16   ENDS

 WmfPlaceableFileHeader  STRUC     ; OK
  Key                    DWORD ?                 ; GDIP_WMF_PLACEABLEKEY   
  Hmf                    WORD  ? 
  boundingBox            PWMFRect16 <> 
  Inch                   WORD  ? 
  Reserved               DWORD ?
  Checksum               WORD  ? 
 WmfPlaceableFileHeader  ENDS

 ENHMETAHEADER3   STRUC            ; OK
  itype           DWORD ?                        ; EMR_HEADER   
  nSize           DWORD ?                        ; ENHMETAHEADER
  rclBounds       RECTL <> 
  rclFrame        RECTL <> 
  dSignature      DWORD ?                        ; ENHMETA_SIGNATURE.   
  nVersion        DWORD ?      
  nBytes          DWORD ?   
  nRecords        DWORD ?   
  nHandles        WORD  ?    
  sReserved       WORD  ?    
  nDescription    DWORD ?  
  offDescription  DWORD ?  
  nPalEntries     DWORD ?  
  szlDevice       SIZEL <>  
  szlMillimeters  SIZEL <>        
 ENHMETAHEADER3   ENDS

 MetafileHeader      STRUC         ; OK   
  mType              DWORD ?      
  msize              DWORD ?           
  Version            DWORD ?        
  EmfPlusFlags       DWORD ?   
  DpiX               REAL4 ?
  DpiY               REAL4 ?   
  x                  DWORD ?  
  y                  DWORD ?   
  nWIDTH             DWORD ?   
  Height             DWORD ?
  EmfHeader          ENHMETAHEADER3 <> 
  EmfPlusHeaderSize  DWORD ?       
  LogicalDpiX        DWORD ?     
  LogicalDpiY        DWORD ?  
 MetafileHeader      ENDS


; xzazet
;-----------------------------------------------
; GDI+ Enums
;-----------------------------------------------
UnitWorld          equ  0    ; World coordinate (non-physical unit)
UnitDisplay        equ  1    ; Variable -- for PageTransform only
UnitPixel          equ  2    ; Each unit is one device pixel.
UnitPoint          equ  3    ; Each unit is a printer's point, or 1/72 inch.
UnitInch           equ  4    ; Each unit is 1 inch.
UnitDocument       equ  5    ; Each unit is 1/300 inch.
UnitMillimeter     equ  6    ; Each unit is 1 millimeter.

; Common color constants
;[CHECKME]
; NOTE: Original enum was unnamed
ColorsAliceBlue              equ  0FFF0F8FFh
ColorsAntiqueWhite           equ  0FFFAEBD7h
ColorsAqua                   equ  0FF00FFFFh
ColorsAquamarine             equ  0FF7FFFD4h
ColorsAzure                  equ  0FFF0FFFFh
ColorsBeige                  equ  0FFF5F5DCh
ColorsBisque                 equ  0FFFFE4C4h
ColorsBlack                  equ  0FF000000h
ColorsBlanchedAlmond         equ  0FFFFEBCDh
ColorsBlue                   equ  0FF0000FFh
ColorsBlueViolet             equ  0FF8A2BE2h
ColorsBrown                  equ  0FFA52A2Ah
ColorsBurlyWood              equ  0FFDEB887h
ColorsCadetBlue              equ  0FF5F9EA0h
ColorsChartreuse             equ  0FF7FFF00h
ColorsChocolate              equ  0FFD2691Eh
ColorsCoral                  equ  0FFFF7F50h
ColorsCornflowerBlue         equ  0FF6495EDh
ColorsCornsilk               equ  0FFFFF8DCh
ColorsCrimson                equ  0FFDC143Ch
ColorsCyan                   equ  0FF00FFFFh
ColorsDarkBlue               equ  0FF00008Bh
ColorsDarkCyan               equ  0FF008B8Bh
ColorsDarkGoldenrod          equ  0FFB8860Bh
ColorsDarkGray               equ  0FFA9A9A9h
ColorsDarkGreen              equ  0FF006400h
ColorsDarkKhaki              equ  0FFBDB76Bh
ColorsDarkMagenta            equ  0FF8B008Bh
ColorsDarkOliveGreen         equ  0FF556B2Fh
ColorsDarkOrange             equ  0FFFF8C00h
ColorsDarkOrchid             equ  0FF9932CCh
ColorsDarkRed                equ  0FF8B0000h
ColorsDarkSalmon             equ  0FFE9967Ah
ColorsDarkSeaGreen           equ  0FF8FBC8Bh
ColorsDarkSlateBlue          equ  0FF483D8Bh
ColorsDarkSlateGray          equ  0FF2F4F4Fh
ColorsDarkTurquoise          equ  0FF00CED1h
ColorsDarkViolet             equ  0FF9400D3h
ColorsDeepPink               equ  0FFFF1493h
ColorsDeepSkyBlue            equ  0FF00BFFFh
ColorsDimGray                equ  0FF696969h
ColorsDodgerBlue             equ  0FF1E90FFh
ColorsFirebrick              equ  0FFB22222h
ColorsFloralWhite            equ  0FFFFFAF0h
ColorsForestGreen            equ  0FF228B22h
ColorsFuchsia                equ  0FFFF00FFh
ColorsGainsboro              equ  0FFDCDCDCh
ColorsGhostWhite             equ  0FFF8F8FFh
ColorsGold                   equ  0FFFFD700h
ColorsGoldenrod              equ  0FFDAA520h
ColorsGray                   equ  0FF808080h
ColorsGreen                  equ  0FF008000h
ColorsGreenYellow            equ  0FFADFF2Fh
ColorsHoneydew               equ  0FFF0FFF0h
ColorsHotPink                equ  0FFFF69B4h
ColorsIndianRed              equ  0FFCD5C5Ch
ColorsIndigo                 equ  0FF4B0082h
ColorsIvory                  equ  0FFFFFFF0h
ColorsKhaki                  equ  0FFF0E68Ch
ColorsLavender               equ  0FFE6E6FAh
ColorsLavenderBlush          equ  0FFFFF0F5h
ColorsLawnGreen              equ  0FF7CFC00h
ColorsLemonChiffon           equ  0FFFFFACDh
ColorsLightBlue              equ  0FFADD8E6h
ColorsLightCoral             equ  0FFF08080h
ColorsLightCyan              equ  0FFE0FFFFh
ColorsLightGoldenrodYellow   equ  0FFFAFAD2h
ColorsLightGray              equ  0FFD3D3D3h
ColorsLightGreen             equ  0FF90EE90h
ColorsLightPink              equ  0FFFFB6C1h
ColorsLightSalmon            equ  0FFFFA07Ah
ColorsLightSeaGreen          equ  0FF20B2AAh
ColorsLightSkyBlue           equ  0FF87CEFAh
ColorsLightSlateGray         equ  0FF778899h
ColorsLightSteelBlue         equ  0FFB0C4DEh
ColorsLightYellow            equ  0FFFFFFE0h
ColorsLime                   equ  0FF00FF00h
ColorsLimeGreen              equ  0FF32CD32h
ColorsLinen                  equ  0FFFAF0E6h
ColorsMagenta                equ  0FFFF00FFh
ColorsMaroon                 equ  0FF800000h
ColorsMediumAquamarine       equ  0FF66CDAAh
ColorsMediumBlue             equ  0FF0000CDh
ColorsMediumOrchid           equ  0FFBA55D3h
ColorsMediumPurple           equ  0FF9370DBh
ColorsMediumSeaGreen         equ  0FF3CB371h
ColorsMediumSlateBlue        equ  0FF7B68EEh
ColorsMediumSpringGreen      equ  0FF00FA9Ah
ColorsMediumTurquoise        equ  0FF48D1CCh
ColorsMediumVioletRed        equ  0FFC71585h
ColorsMidnightBlue           equ  0FF191970h
ColorsMintCream              equ  0FFF5FFFAh
ColorsMistyRose              equ  0FFFFE4E1h
ColorsMoccasin               equ  0FFFFE4B5h
ColorsNavajoWhite            equ  0FFFFDEADh
ColorsNavy                   equ  0FF000080h
ColorsOldLace                equ  0FFFDF5E6h
ColorsOlive                  equ  0FF808000h
ColorsOliveDrab              equ  0FF6B8E23h
ColorsOrange                 equ  0FFFFA500h
ColorsOrangeRed              equ  0FFFF4500h
ColorsOrchid                 equ  0FFDA70D6h
ColorsPaleGoldenrod          equ  0FFEEE8AAh
ColorsPaleGreen              equ  0FF98FB98h
ColorsPaleTurquoise          equ  0FFAFEEEEh
ColorsPaleVioletRed          equ  0FFDB7093h
ColorsPapayaWhip             equ  0FFFFEFD5h
ColorsPeachPuff              equ  0FFFFDAB9h
ColorsPeru                   equ  0FFCD853Fh
ColorsPink                   equ  0FFFFC0CBh
ColorsPlum                   equ  0FFDDA0DDh
ColorsPowderBlue             equ  0FFB0E0E6h
ColorsPurple                 equ  0FF800080h
ColorsRed                    equ  0FFFF0000h
ColorsRosyBrown              equ  0FFBC8F8Fh
ColorsRoyalBlue              equ  0FF4169E1h
ColorsSaddleBrown            equ  0FF8B4513h
ColorsSalmon                 equ  0FFFA8072h
ColorsSandyBrown             equ  0FFF4A460h
ColorsSeaGreen               equ  0FF2E8B57h
ColorsSeaShell               equ  0FFFFF5EEh
ColorsSienna                 equ  0FFA0522Dh
ColorsSilver                 equ  0FFC0C0C0h
ColorsSkyBlue                equ  0FF87CEEBh
ColorsSlateBlue              equ  0FF6A5ACDh
ColorsSlateGray              equ  0FF708090h
ColorsSnow                   equ  0FFFFFAFAh
ColorsSpringGreen            equ  0FF00FF7Fh
ColorsSteelBlue              equ  0FF4682B4h
ColorsTan                    equ  0FFD2B48Ch
ColorsTeal                   equ  0FF008080h
ColorsThistle                equ  0FFD8BFD8h
ColorsTomato                 equ  0FFFF6347h
ColorsTransparent            equ  0FFFFFFh
ColorsTurquoise              equ  0FF40E0D0h
ColorsViolet                 equ  0FFEE82EEh
ColorsWheat                  equ  0FFF5DEB3h
ColorsWhite                  equ  0FFFFFFFFh
ColorsWhiteSmoke             equ  0FFF5F5F5h
ColorsYellow                 equ  0FFFFFF00h
ColorsYellowGreen            equ  0FF9ACD32h


;' NOTE: Enums evaluate to a Long
GpStatusOk                        equ  0
GpStatusGenericError              equ  1
GpStatusInvalidParameter          equ  2
GpStatusOutOfMemory               equ  3
GpStatusObjectBusy                equ  4
GpStatusInsufficientBuffer        equ  5
GpStatusNotImplemented            equ  6
GpStatusWin32Error                equ  7
GpStatusWrongState                equ  8
GpStatusAborted                   equ  9
GpStatusFileNotFound              equ  10
GpStatusValueOverflow             equ  11
GpStatusAccessDenied              equ  12
GpStatusUnknownImageFormat        equ  13
GpStatusFontFamilyNotFound        equ  14
GpStatusFontStyleNotFound         equ  15
GpStatusNotTrueTypeFont           equ  16
GpStatusUnsupportedGdiplusVersion equ  17
GpStatusGdiplusNotInitialized     equ  18
GpStatusPropertyNotFound          equ  19
GpStatusPropertyNotSupported      equ  20

; Quality mode constants
QualityModeInvalid                equ  -1
QualityModeDefault                equ  0
QualityModeLow                    equ  1    ; Best performance
QualityModeHigh                   equ  2    ; Best rendering quality

; Alpha Compositing mode constants
CompositingModeSourceOver         equ  0
CompositingModeSourceCopy         equ  1

; Alpha Compositing quality constants
CompositingQualityInvalid         equ  QualityModeInvalid
CompositingQualityDefault         equ  QualityModeDefault
CompositingQualityHighSpeed       equ  QualityModeLow
CompositingQualityHighQuality     equ  QualityModeHigh
CompositingQualityGammaCorrected  equ  3
CompositingQualityAssumeLinear    equ  4

; Generic font families
GenericFontFamilySerif            equ  0
GenericFontFamilySansSerif        equ  1
GenericFontFamilyMonospace        equ  2

; FontStyle: face types and common styles
FontStyleRegular                  equ  0
FontStyleBold                     equ  1
FontStyleItalic                   equ  2
FontStyleBoldItalic               equ  3
FontStyleUnderline                equ  4
FontStyleStrikeout                equ  8
SmoothingModeInvalid              equ  QualityModeInvalid
SmoothingModeDefault              equ  QualityModeDefault
SmoothingModeHighSpeed            equ  QualityModeLow
SmoothingModeHighQuality          equ  QualityModeHigh
SmoothingModeNone                 equ  3
SmoothingModeAntiAlias            equ  4
FillModeAlternate                 equ  0
FillModeWinding                   equ  1
InterpolationModeInvalid          equ  QualityModeInvalid
InterpolationModeDefault          equ  QualityModeDefault
InterpolationModeLowQuality       equ  QualityModeLow
InterpolationModeHighQuality      equ  QualityModeHigh
InterpolationModeBilinear         equ  3
InterpolationModeBicubic          equ  4
InterpolationModeNearestNeighbor  equ  5
InterpolationModeHighQualityBilinear   equ  6
InterpolationModeHighQualityBicubic    equ  7

; Various wrap modes for brushes
WrapModeTile                      equ  0
WrapModeTileFlipX                 equ  1
WrapModeTileFlipY                 equ  2
WrapModeTileFlipXY                equ  3
WrapModeClamp                     equ  4
LinearGradientModeHorizontal      equ  0
LinearGradientModeVertical        equ  1
LinearGradientModeForwardDiagonal equ  2
LinearGradientModeBackwardDiagonal equ 3
ImageTypeUnknown                  equ  0
ImageTypeBitmap                   equ  1
ImageTypeMetafile                 equ  2


;----------------------------

 HatchStyleHorizontal                  EQU  0
 HatchStyleVertical                    EQU  1
 HatchStyleForwardDiagonal             EQU  2
 HatchStyleBackwardDiagonal            EQU  3
 HatchStyleCross                       EQU  4
 HatchStyleDiagonalCross               EQU  5
 HatchStyle05Percent                   EQU  6
 HatchStyle10Percent                   EQU  7
 HatchStyle20Percent                   EQU  8
 HatchStyle25Percent                   EQU  9
 HatchStyle30Percent                   EQU  10
 HatchStyle40Percent                   EQU  11
 HatchStyle50Percent                   EQU  12
 HatchStyle60Percent                   EQU  13
 HatchStyle70Percent                   EQU  14
 HatchStyle75Percent                   EQU  15
 HatchStyle80Percent                   EQU  16
 HatchStyle90Percent                   EQU  17
 HatchStyleLightDownwardDiagonal       EQU  18
 HatchStyleLightUpwardDiagonal         EQU  19
 HatchStyleDarkDownwardDiagonal        EQU  20
 HatchStyleDarkUpwardDiagonal          EQU  21
 HatchStyleWideDownwardDiagonal        EQU  22
 HatchStyleWideUpwardDiagonal          EQU  23
 HatchStyleLightVertical               EQU  24
 HatchStyleLightHorizontal             EQU  25
 HatchStyleNarrowVertical              EQU  26
 HatchStyleNarrowHorizontal            EQU  27
 HatchStyleDarkVertical                EQU  28
 HatchStyleDarkHorizontal              EQU  29
 HatchStyleDashedDownwardDiagonal      EQU  30
 HatchStyleDashedUpwardDiagonal        EQU  31
 HatchStyleDashedHorizontal            EQU  32
 HatchStyleDashedVertical              EQU  33
 HatchStyleSmallConfetti               EQU  34
 HatchStyleLargeConfetti               EQU  35
 HatchStyleZigZag                      EQU  36
 HatchStyleWave                        EQU  37
 HatchStyleDiagonalBrick               EQU  38
 HatchStyleHorizontalBrick             EQU  39
 HatchStyleWeave                       EQU  40
 HatchStylePlaid                       EQU  41
 HatchStyleDivot                       EQU  42 
 HatchStyleDottedGrid                  EQU  43
 HatchStyleDottedDiamond               EQU  44
 HatchStyleShingle                     EQU  45
 HatchStyleTrellis                     EQU  46 
 HatchStyleSphere                      EQU  47
 HatchStyleSmallGrid                   EQU  48
 HatchStyleSmallCheckerBoard           EQU  49
 HatchStyleLargeCheckerBoard           EQU  50
 HatchStyleOutlinedDiamond             EQU  51
 HatchStyleSolidDiamond                EQU  52
 HatchStyleTotal                       EQU  53
 HatchStyleLargeGrid                   EQU  HatchStyleCross
 HatchStyleMin                         EQU  HatchStyleHorizontal
 HatchStyleMax                         EQU  HatchStyleTotal - 1

 ; Matrix
 ;------------------

 MatrixOrderPrepend                    EQU  0
 MatrixOrderAppend                     EQU  1

 ; Color
 ; -----------------

 ColorAdjustTypeDefault                EQU  0
 ColorAdjustTypeBitmap                 EQU  1
 ColorAdjustTypeBrush                  EQU  2
 ColorAdjustTypePen                    EQU  3
 ColorAdjustTypeText                   EQU  4
 ColorAdjustTypeCount                  EQU  5
 ColorAdjustTypeAny                    EQU  6  ; Reserved

 ColorChannelFlagsC                    EQU  0 
 ColorChannelFlagsM                    EQU  1 
 ColorChannelFlagsY                    EQU  2
 ColorChannelFlagsK                    EQU  3
 ColorChannelFlagsLast                 EQU  4

 ColorMatrixFlagsDefault               EQU  0
 ColorMatrixFlagsSkipGrays             EQU  1
 ColorMatrixFlagsAltGray               EQU  2

 ; Pen 
 ; --------------

 PenAlignmentCenter                    EQU  0
 PenAlignmentInset                     EQU  1

 ; Brush 
 ; ----------------

 BrushTypeSolidColor                   EQU  0
 BrushTypeHatchFill                    EQU  1
 BrushTypeTextureFill                  EQU  2
 BrushTypePathGradient                 EQU  3
 BrushTypeLinearGradient               EQU  4

 ; Dash
 ; --------------

 DashStyleSolid                        EQU  0
 DashStyleDash                         EQU  1
 DashStyleDot                          EQU  2
 DashStyleDashDot                      EQU  3
 DashStyleDashDotDot                   EQU  4
 DashStyleCustom                       EQU  5  
 
 ; Dash cap constants
 ;--------------------
 ; Line cap

 DashCapFlat                           EQU  0
 DashCapRound                          EQU  2
 DashCapTriangle                       EQU  3

 LineCapFlat                           EQU  0
 LineCapSquare                         EQU  1
 LineCapRound                          EQU  2
 LineCapTriangle                       EQU  3
 LineCapNoAnchor                       EQU  010h     ; corresponds to flat cap
 LineCapSquareAnchor                   EQU  011h     ; corresponds to square cap
 LineCapRoundAnchor                    EQU  012h     ; corresponds to round cap
 LineCapDiamondAnchor                  EQU  013h     ; corresponds to triangle cap
 LineCapArrowAnchor                    EQU  014h     ; no correspondence
 LineCapCustom                         EQU  0FFh     ; custom cap
 LineCapAnchorMask                     EQU  0F0h     ; mask to check for anchor or not.

 ; Custom Line cap type
 ;--------------------------------

 CustomLineCapTypeDefault              EQU  0
 CustomLineCapTypeAdjustableArrow      EQU  1        

 ; Line join
 ; --------------------

 LineJoinMiter                         EQU  0
 LineJoinBevel                         EQU  1
 LineJoinRound                         EQU  2
 LineJoinMiterClipped                  EQU  3

 ; Pen's Fill
 ; --------------------

 PenTypeSolidColor                     EQU  BrushTypeSolidColor
 PenTypeHatchFill                      EQU  BrushTypeHatchFill
 PenTypeTextureFill                    EQU  BrushTypeTextureFill
 PenTypePathGradient                   EQU  BrushTypePathGradient
 PenTypeLinearGradient                 EQU  BrushTypeLinearGradient
 PenTypeUnknown                        EQU  -1

 ; Warp Mode
 ; -----------

 WarpModePerspective                   EQU  0
 WarpModeBilinear                      EQU  1

 ; Region 
 ; -----------------------------

 CombineModeReplace                    EQU  0
 CombineModeIntersect                  EQU  1
 CombineModeUnion                      EQU  2
 CombineModeXor                        EQU  3
 CombineModeExclude                    EQU  4
 CombineModeComplement                 EQU  5 

 RotateNoneFlipNone                    EQU  0
 Rotate90FlipNone                      EQU  1
 Rotate180FlipNone                     EQU  2
 Rotate270FlipNone                     EQU  3
 RotateNoneFlipX                       EQU  4
 Rotate90FlipX                         EQU  5
 Rotate180FlipX                        EQU  6
 Rotate270FlipX                        EQU  7
 RotateNoneFlipY                       EQU  Rotate180FlipX
 Rotate90FlipY                         EQU  Rotate270FlipX
 Rotate180FlipY                        EQU  RotateNoneFlipX
 Rotate270FlipY                        EQU  Rotate90FlipX
 RotateNoneFlipXY                      EQU  Rotate180FlipNone
 Rotate90FlipXY                        EQU  Rotate270FlipNone
 Rotate180FlipXY                       EQU  RotateNoneFlipNone
 Rotate270FlipXY                       EQU  Rotate90FlipNone



; DirectionRightToLeft - the base embedding level by the Unicode
;                        bidirectional engine.

; DirectionVertical    - 

; NoFitBlackBox        - Allows parts of glyphs to overhang the
;                        bounding rectangle.
;                        By default glyphs are first aligned
;                        inside the margines, then any glyphs which
;                        still overhang the bounding box are
;                        repositioned to avoid any overhang.
;                        For example when an italic
;                        lower case letter f in a font such as
;                        Garamond is aligned at the far left of a
;                        rectangle, the lower part of the f will
;                        reach slightly further left than the left
;                        edge of the rectangle. Setting this flag
;                        will ensure the character aligns visually
;                        with the lines above and below, but may
;                        cause some pixels outside the formatting
;                        rectangle to be clipped or painted.
;
; DisplayFormatControl - Causes control characters such as the
;                        left-to-right mark to be shown in the
;                        output with a representative glyph.
;
; NoFontFallback       - Disables fallback to alternate fonts for
;                        characters not supported in the requested
;                        font. Any missing characters will be
;                        be displayed with the fonts missing glyph,
;                        usually an open square.
;
; NoWrap               - Disables wrapping of text between lines
;                        when formatting within a rectangle.
;                        NoWrap is implied when a point is passed
;                        instead of a rectangle, or when the
;                        specified rectangle has a zero line length.
;
; NoClip               - By default text is clipped to the
;                        formatting rectangle. Setting NoClip
;                        allows overhanging pixels to affect the
;                        device outside the formatting rectangle.
;                        Pixels at the end of the line may be
;                        affected if the glyphs overhang their
;                        cells, and either the NoFitBlackBox flag
;                        has been set, or the glyph extends to far
;                        to be fitted.
;                        if the glyphs extend beyond their cell
;                        ascent / descent. This can occur rarely
;                        with unusual diacritic mark combinations.

 StringFormatFlagsDirectionRightToLeft   EQU  01h
 StringFormatFlagsDirectionVertical      EQU  02h
 StringFormatFlagsNoFitBlackBox          EQU  04h
 StringFormatFlagsDisplayFormatControl   EQU  020h
 StringFormatFlagsNoFontFallback         EQU  0400h
 StringFormatFlagsMeasureTrailingSpaces  EQU  0800h
 StringFormatFlagsNoWrap                 EQU  01000h
 StringFormatFlagsLineLimit              EQU  02000h

 StringFormatFlagsNoClip                 EQU  04000h

 StringTrimmingNone                      EQU  0
 StringTrimmingCharacter                 EQU  1
 StringTrimmingWord                      EQU  2
 StringTrimmingEllipsisCharacter         EQU  3
 StringTrimmingEllipsisWord              EQU  4
 StringTrimmingEllipsisPath              EQU  5

 ; National language digit substitution
 ; -------------------------------------

 StringDigitSubstituteUser               EQU  0 
 StringDigitSubstituteNone               EQU  1
 StringDigitSubstituteNational           EQU  2
 StringDigitSubstituteTraditional        EQU  3

 ; Hotkey prefix interpretation
 ; ------------------------------

 HotkeyPrefixNone                        EQU  0
 HotkeyPrefixShow                        EQU  1
 HotkeyPrefixHide                        EQU  2

 ; Left edge for left-to-right text,
 ; right for right-to-left text,
 ; and top for vertical

 StringAlignmentNear                     EQU  0
 StringAlignmentCenter                   EQU  1
 StringAlignmentFar                      EQU  2

 FlushIntentionFlush                     EQU  0     ; Flush all batched rendering operations
 FlushIntentionSync                      EQU  1     ; Flush all batched rendering operations
                                                    ; and wait for them to complete
                                                    ; Image encoder parameter related types

 EncoderParameterValueTypeByte           EQU  1     ; 8-bit int
 EncoderParameterValueTypeASCII          EQU  2     ; 8-bit byte  7-bit ASCII
                                                    ;  NULL .

 EncoderParameterValueTypeShort          EQU  3     ; 16-bit  int
 EncoderParameterValueTypeLong           EQU  4     ; 32-bit  int
 EncoderParameterValueTypeRational       EQU  5     ; Long

 EncoderParameterValueTypeLongRange      EQU  6  


 EncoderParameterValueTypeUndefined      EQU  7     ; 8-bit byte that can take any value
                                                    ; depending on field definition

 EncoderParameterValueTypeRationalRange  EQU  8     ; Two Rationals. The first Rational
                                                    ; specifies the lower end and the second
                                                    ; specifies the higher end. All values
                                                    ; are inclusive at both ends
 ; Image encoder value types
 ; ----------------------------

 EncoderValueColorTypeCMYK               EQU  0
 EncoderValueColorTypeYCCK               EQU  1
 EncoderValueCompressionLZW              EQU  2
 EncoderValueCompressionCCITT3           EQU  3
 EncoderValueCompressionCCITT4           EQU  4
 EncoderValueCompressionRle              EQU  5
 EncoderValueCompressionNone             EQU  6
 EncoderValueScanMethodInterlaced        EQU  7
 EncoderValueScanMethodNonInterlaced     EQU  8
 EncoderValueVersionGif87                EQU  9
 EncoderValueVersionGif89                EQU  10
 EncoderValueRenderProgressive           EQU  11
 EncoderValueRenderNonProgressive        EQU  12
 EncoderValueTransformRotate90           EQU  13
 EncoderValueTransformRotate180          EQU  14
 EncoderValueTransformRotate270          EQU  15
 EncoderValueTransformFlipHorizontal     EQU  16
 EncoderValueTransformFlipVertical       EQU  17
 EncoderValueMultiFrame                  EQU  18
 EncoderValueLastFrame                   EQU  19
 EncoderValueFlush                       EQU  20
 EncoderValueFrameDimensionTime          EQU  21
 EncoderValueFrameDimensionResolution    EQU  22
 EncoderValueFrameDimensionPage          EQU  23


 EncoderCompression                      EQU  0
 EncoderColorDepth                       EQU  1
 EncoderScanMethod                       EQU  2
 EncoderVersion                          EQU  3
 EncoderRenderMethod                     EQU  4
 EncoderQuality                          EQU  5
 EncoderTransformation                   EQU  6
 EncoderLuminanceTable                   EQU  7
 EncoderChrominanceTable                 EQU  8
 EncoderSaveFlag                         EQU  9

 PixelOffsetModeInvalid                  EQU  QualityModeInvalid
 PixelOffsetModeDefault                  EQU  QualityModeDefault
 PixelOffsetModeHighSpeed                EQU  QualityModeLow
 PixelOffsetModeHighQuality              EQU  QualityModeHigh
 PixelOffsetModeNone                     EQU  3
 PixelOffsetModeHalf                     EQU  4 

 TextRenderingHintSystemDefault             EQU  0     ; Glyph with system default rendering hint
 TextRenderingHintSingleBitPerPixelGridFit  EQU  1     ; Glyph bitmap with hinting
 TextRenderingHintSingleBitPerPixel         EQU  2     ; Glyph bitmap without hinting
 TextRenderingHintAntiAliasGridFit          EQU  3     ; Glyph anti-alias bitmap with hinting
 TextRenderingHintAntiAlias                 EQU  4     ; Glyph anti-alias bitmap without hinting
 TextRenderingHintClearTypeGridFit          EQU  5     ; Glyph CT bitmap with hinting

 MetafileTypeInvalid                        EQU  0 
 MetafileTypeWmf                            EQU  1     ; WMF
 MetafileTypeWmfPlaceable                   EQU  2     ; Placeable WMF
 MetafileTypeEmf                            EQU  3     ; EMF
 MetafileTypeEmfPlusOnly                    EQU  4     ; EMF+
 MetafileTypeEmfPlusDual                    EQU  5     ; EMF+

 ; Specifies the type of EMF to record
 ; -------------------------------------

 EmfTypeEmfOnly                             EQU  MetafileTypeEmf  
 EmfTypeEmfPlusOnly                         EQU  MetafileTypeEmfPlusOnly 
 EmfTypeEmfPlusDual                         EQU  MetafileTypeEmfPlusDual 

 ; EMF+ Persistent object types
 ; --------------------------------------
 
 ObjectTypeInvalid                          EQU  0
 ObjectTypeBrush                            EQU  1
 ObjectTypePen                              EQU  2
 ObjectTypePath                             EQU  3
 ObjectTypeRegion                           EQU  4
 ObjectTypeImage                            EQU  5
 ObjectTypeFont                             EQU  6
 ObjectTypeStringFormat                     EQU  7
 ObjectTypeImageAttributes                  EQU  8
 ObjectTypeCustomLineCap                    EQU  9

 ObjectTypeMax                              EQU  ObjectTypeCustomLineCap
 ObjectTypeMin                              EQU  ObjectTypeBrush

 ; frameRect
 ; There is an extra frame unit value (MetafileFrameUnitGdi) so
 ; that units can be supplied in the same units that GDI expects for
 ; frame rects -- these units are in .01 (1/100ths) millimeter units
 ; as defined by GDI.
 
 MetafileFrameUnitPixel                     EQU  UnitPixel
 MetafileFrameUnitPoint                     EQU  UnitPoint
 MetafileFrameUnitInch                      EQU  UnitInch
 MetafileFrameUnitDocument                  EQU  UnitDocument
 MetafileFrameUnitMillimeter                EQU  UnitMillimeter
 MetafileFrameUnitGdi                       EQU  7 

 ; Coordinate space identifiers
 ; -----------------------------

 CoordinateSpaceWorld                       EQU  0
 CoordinateSpacePage                        EQU  1
 CoordinateSpaceDevice                      EQU  2

