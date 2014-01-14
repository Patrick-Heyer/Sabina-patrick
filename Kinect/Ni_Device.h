/****************************************************************************
 *                                                                           *
 *  OpenNI 1.x Alpha                                                         *
 *  Copyright (C) 2011 PrimeSense Ltd.                                       *
 *                                                                           *
 *  This file is part of OpenNI.                                             *
 *                                                                           *
 *  OpenNI is free software: you can redistribute it and/or modify           *
 *  it under the terms of the GNU Lesser General Public License as published *
 *  by the Free Software Foundation, either version 3 of the License, or     *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  OpenNI is distributed in the hope that it will be useful,                *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU Lesser General Public License *
 *  along with OpenNI. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                           *
 ****************************************************************************/
#ifndef __NI_DEVICE_H__
#define __NI_DEVICE_H__

#include <XnCppWrapper.h>
#include <XnTypes.h>
using namespace xn;

// --------------------------------
// Defines
// --------------------------------
#define MAX_STRINGS 20

// --------------------------------
// Types
// --------------------------------
typedef struct
{
    int nValuesCount;
    unsigned int pValues[MAX_STRINGS];
    const char* pValueToName[MAX_STRINGS];
} DeviceParameter;

typedef struct
{
    int nValuesCount;
    XnCodecID pValues[MAX_STRINGS];
    const char* pIndexToName[MAX_STRINGS];
} NodeCodec;

typedef struct
{
    int nValuesCount;
    const char* pValues[MAX_STRINGS];
} DeviceStringProperty;


class Ni_Device
{
protected:
    // constructor/destructor
    Ni_Device( void ) {
        
    }
    virtual ~Ni_Device( void ) { }
    
private:
        static Ni_Device    *m_singleton;

public:
    // singleton functions
    static Ni_Device       &getInstance( void );
    static void FreeInstance( void );
    void        Initialize ( void );
    


    DeviceStringProperty g_PrimaryStream;
    DeviceParameter g_Registration;
    DeviceParameter g_Resolution;

    // --------------------------------
    // Function Declarations
    // --------------------------------
    Device *getDevice();
    DepthGenerator *getDepthGenerator();
    ImageGenerator *getImageGenerator();
    IRGenerator *getIRGenerator();
    AudioGenerator *getAudioGenerator();
	Player *getPlayer();
	UserGenerator *getUserGenerator();

    DepthMetaData* getDepthMetaData();
    ImageMetaData* getImageMetaData();
    IRMetaData* getIRMetaData();
    AudioMetaData* getAudioMetaData();
	
    
    Context g_Context;
    ProductionNode g_pPrimary;
    ScriptNode g_scriptNode;

    Device g_Device;
    DepthGenerator g_Depth;
    ImageGenerator g_Image;
    IRGenerator g_IR;
    AudioGenerator g_Audio;
    Player g_Player;
	UserGenerator g_User;

    DepthMetaData g_DepthMD;
    ImageMetaData g_ImageMD;
    IRMetaData g_irMD;
    AudioMetaData g_AudioMD;

	

    void openCommon();
    XnStatus openDeviceFile(const char* csFile);
    XnStatus openDeviceFromXml(const char* csXmlFile, EnumerationErrors& errors);
    XnStatus openDeviceFromXmlWithChoice(const char* csXmlFile, EnumerationErrors& errors);
    void closeDevice();
    void readFrame();
    void changeRegistration(int nValue);
    void changePrimaryStream(int nValue);
    void toggleMirror(int);
    void seekFrame(int nDiff);
    void createStream(Generator& generator, XnProductionNodeType type);
    void toggleStream(Generator& generator, XnProductionNodeType type, bool* bIsOn);
    void toggleDepthState(int nDummy);
    void toggleImageState(int nDummy);
    void toggleIRState(int nDummy);
    void toggleAudioState(int nDummy);
    void getDepthFormats(const char** pNames, unsigned int* pValues, int* pCount);
    void getImageFormats(const char** pNames, unsigned int* pValues, int* pCount);
    void getAudioFormats(const char** pNames, unsigned int* pValues, int* pCount);
    void getPrimaryStreams(const char** pNames, unsigned int* pValues, int* pCount);
    bool isDepthOn();
    bool isImageOn();
    bool isIROn();
    bool isAudioOn();
    bool isPlayerOn();
    void setDepthResolution(int res);
    void setDepthFPS(int fps);
    void setImageResolution(int res);
    void setImageFPS(int fps);
    void setIRResolution(int res);
    void setIRFPS(int fps);
    void setStreamCropping(MapGenerator* pGenerator, XnCropping* pCropping);
    void setPlaybackSpeed(int ratioDiff);
    XnDouble getPlaybackSpeed();


	
private:


    bool g_bIsDepthOn;
    bool g_bIsImageOn;
    bool g_bIsIROn;
    bool g_bIsAudioOn;
    bool g_bIsPlayerOn;

};


#endif //__NI_DEVICE_H__
