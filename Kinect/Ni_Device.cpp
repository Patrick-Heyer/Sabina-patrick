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
// --------------------------------
// Includes
// --------------------------------
#include "Ni_Device.h"
#include <iostream>
#include <math.h>
#include <XnLog.h>

// --------------------------------
// Defines
// --------------------------------
#define MAX_STRINGS 20


Ni_Device *Ni_Device::m_singleton=0;

// ----------------------------------------------
// getInstance() - get a pointer to the unique
// Ni_Device instance (the singleton)
// ----------------------------------------------
Ni_Device &Ni_Device::getInstance( void )
{

    if ( m_singleton == 0 )
        m_singleton = new Ni_Device;

    return (*m_singleton);
}

// ----------------------------------------------
// FreeInstance() - delete the unique Input_Singleton
// instance.
// ----------------------------------------------
void Ni_Device::FreeInstance( void )
{
    if ( m_singleton != 0 )
    {
        delete m_singleton;
        m_singleton = 0;
    }
}



void XN_CALLBACK_TYPE onErrorStateChanged(XnStatus errorState, void* pCookie)
{
    if (errorState != XN_STATUS_OK)
    {
        std::cout << (xnGetStatusString(errorState)) << std::endl;
    }
}
// ----------------------------------------------
// Initialize() - Set default state
// .
// ----------------------------------------------
void Ni_Device::Initialize(void )
{
    std::cout << "init" <<std::endl;
    xn::EnumerationErrors errors;

//     g_Context.InitFromXmlFile("../data/SamplesConfig.xml", g_scriptNode, &errors);

    g_pPrimary = NULL;
    // Primary Streams
    int nIndex = 0;

    g_PrimaryStream.pValues[nIndex++] = "Any";
    g_PrimaryStream.pValues[nIndex++] = xnProductionNodeTypeToString(XN_NODE_TYPE_DEPTH);
    g_PrimaryStream.pValues[nIndex++] = xnProductionNodeTypeToString(XN_NODE_TYPE_IMAGE);
    g_PrimaryStream.pValues[nIndex++] = xnProductionNodeTypeToString(XN_NODE_TYPE_IR);
    g_PrimaryStream.pValues[nIndex++] = xnProductionNodeTypeToString(XN_NODE_TYPE_AUDIO);

    g_PrimaryStream.nValuesCount = nIndex;

    // Registration
    nIndex = 0;

    g_Registration.pValues[nIndex++] = FALSE;
    g_Registration.pValueToName[FALSE] = "Off";

    g_Registration.pValues[nIndex++] = TRUE;
    g_Registration.pValueToName[TRUE] = "Depth -> Image";

    g_Registration.nValuesCount = nIndex;

    // Resolutions
    nIndex = 0;

    g_Resolution.pValues[nIndex++] = XN_RES_QVGA;
    g_Resolution.pValueToName[XN_RES_QVGA] = Resolution(XN_RES_QVGA).GetName();

    g_Resolution.pValues[nIndex++] = XN_RES_VGA;
    g_Resolution.pValueToName[XN_RES_VGA] = Resolution(XN_RES_VGA).GetName();

    g_Resolution.pValues[nIndex++] = XN_RES_SXGA;
    g_Resolution.pValueToName[XN_RES_SXGA] = Resolution(XN_RES_SXGA).GetName();

    g_Resolution.pValues[nIndex++] = XN_RES_UXGA;
    g_Resolution.pValueToName[XN_RES_UXGA] = Resolution(XN_RES_UXGA).GetName();

    g_Resolution.nValuesCount = nIndex;


    XnStatus nRetVal = XN_STATUS_OK;

    g_bIsDepthOn = false;
    g_bIsImageOn = false;
    g_bIsIROn = false;
    g_bIsAudioOn = false;
    g_bIsPlayerOn = false;



    std::cout << "init ended" <<std::endl;
}





void Ni_Device::openCommon()
{
    XnStatus nRetVal = XN_STATUS_OK;

    g_bIsDepthOn = false;
    g_bIsImageOn = false;
    g_bIsIROn = false;
    g_bIsAudioOn = false;
    g_bIsPlayerOn = false;

    NodeInfoList list;
    nRetVal = g_Context.EnumerateExistingNodes(list);
    if (nRetVal == XN_STATUS_OK)
    {
        for (NodeInfoList::Iterator it = list.Begin(); it != list.End(); ++it)
        {
            switch ((*it).GetDescription().Type)
            {
            case XN_NODE_TYPE_DEVICE:
                (*it).GetInstance(g_Device);
                break;
            case XN_NODE_TYPE_DEPTH:
                g_bIsDepthOn = true;
                (*it).GetInstance(g_Depth);
                break;
            case XN_NODE_TYPE_IMAGE:
                g_bIsImageOn = true;
                (*it).GetInstance(g_Image);
                break;
            case XN_NODE_TYPE_USER:

                g_bIsUserOn = true;
                (*it).GetInstance(g_User);
                break;
            case XN_NODE_TYPE_IR:
                g_bIsIROn = true;
                (*it).GetInstance(g_IR);
                break;
            case XN_NODE_TYPE_AUDIO:
                g_bIsAudioOn = true;
                (*it).GetInstance(g_Audio);
                break;
            case XN_NODE_TYPE_PLAYER:
                g_bIsPlayerOn = true;
                (*it).GetInstance(g_Player);
            }
        }
    }

    XnCallbackHandle hDummy;
    g_Context.RegisterToErrorStateChange(onErrorStateChanged, NULL, hDummy);


    readFrame();

}

XnStatus Ni_Device::openDeviceFile(const char* csFile)
{
    XnStatus nRetVal = g_Context.Init();
    XN_IS_STATUS_OK(nRetVal);
    nRetVal = g_Context.OpenFileRecording(csFile, g_Player);
    XN_IS_STATUS_OK(nRetVal);
    openCommon();

    return XN_STATUS_OK;
}

XnStatus Ni_Device::openDeviceFromXml(const char* csXmlFile, EnumerationErrors& errors)
{
    XnStatus nRetVal = XN_STATUS_OK;


    nRetVal = g_Context.InitFromXmlFile(csXmlFile,  &errors);
    XN_IS_STATUS_OK(nRetVal);

    openCommon();

    return (XN_STATUS_OK);
}

XnStatus Ni_Device::openDeviceFromXmlWithChoice(const char* csXmlFile, EnumerationErrors& errors)
{
    XnStatus nRetVal = XN_STATUS_OK;

    xnLogInitFromXmlFile(csXmlFile);

    nRetVal = g_Context.Init();
    XN_IS_STATUS_OK(nRetVal);

    // find devices
    NodeInfoList list;
    nRetVal = g_Context.EnumerateProductionTrees(XN_NODE_TYPE_DEVICE, NULL, list, &errors);
    XN_IS_STATUS_OK(nRetVal);

    std::cout << "The following devices were found:\n" << std::endl;
    int i = 1;
    for (NodeInfoList::Iterator it = list.Begin(); it != list.End(); ++it, ++i)
    {
        NodeInfo deviceNodeInfo = *it;

        Device deviceNode;
        deviceNodeInfo.GetInstance(deviceNode);
        XnBool bExists = deviceNode.IsValid();
        if (!bExists)
        {
            g_Context.CreateProductionTree(deviceNodeInfo, deviceNode);
            // this might fail.
        }

        if (deviceNode.IsValid() && deviceNode.IsCapabilitySupported(XN_CAPABILITY_DEVICE_IDENTIFICATION))
        {
            const XnUInt32 nStringBufferSize = 200;
            XnChar strDeviceName[nStringBufferSize];
            XnChar strSerialNumber[nStringBufferSize];

            XnUInt32 nLength = nStringBufferSize;
            deviceNode.GetIdentificationCap().GetDeviceName(strDeviceName, nLength);
            nLength = nStringBufferSize;
            deviceNode.GetIdentificationCap().GetSerialNumber(strSerialNumber, nLength);
            std::cout << "[" << i << "] " << strDeviceName << "(" << strSerialNumber << std::endl;
        }
        else
        {
            std::cout << "[" << i << "] " << deviceNodeInfo.GetCreationInfo() << std::endl;
        }

        // release the device if we created it
        if (!bExists && deviceNode.IsValid())
        {
            deviceNode.Release();
        }
    }
    std::cout << "\n" << std::endl;
    std::cout << "Choose device to open (1): " << std::endl;

    int chosen = 1;
    int nRetval = scanf("%d", &chosen);

    // create it
    NodeInfoList::Iterator it = list.Begin();
    for (i = 1; i < chosen; ++i)
    {
        it++;
    }

    NodeInfo deviceNode = *it;
    nRetVal = g_Context.CreateProductionTree(deviceNode, g_Device);
    XN_IS_STATUS_OK(nRetVal);

    // now run the rest of the XML
    nRetVal = g_Context.RunXmlScriptFromFile(csXmlFile, g_scriptNode, &errors);
    XN_IS_STATUS_OK(nRetVal);

    openCommon();

    return (XN_STATUS_OK);
}

void Ni_Device::closeDevice()
{
    g_Player.Release();
    g_Device.Release();
    g_Depth.Release();
    g_Image.Release();
    g_IR.Release();
    g_Audio.Release();
    g_scriptNode.Release();
    g_Context.Release();
}

void Ni_Device::readFrame()
{
    XnStatus rc = XN_STATUS_OK;

    do
    {

        rc = g_Context.WaitAndUpdateAll();


        if (rc != XN_STATUS_OK)
        {
            std::cout << "Error: " << xnGetStatusString(rc) << std::endl;
            usleep(500);
        }



        if (g_Depth.IsValid())
        {
            g_Depth.GetMetaData(g_DepthMD);
        }

        if (g_Image.IsValid())
        {
            g_Image.GetMetaData(g_ImageMD);
        }

    } while(rc != XN_STATUS_OK);

}

void Ni_Device::changeRegistration(int nValue)
{
    if (!g_Depth.IsValid() || !g_Depth.IsCapabilitySupported(XN_CAPABILITY_ALTERNATIVE_VIEW_POINT))
    {
        return;
    }

    if (!nValue)
    {
        g_Depth.GetAlternativeViewPointCap().ResetViewPoint();
    }
    else if (g_Image.IsValid())
    {
        g_Depth.GetAlternativeViewPointCap().SetViewPoint(g_Image);
    }
}

void Ni_Device::changePrimaryStream(int nIndex)
{
    XnStatus nRetVal = XN_STATUS_OK;

    if (nIndex == 0)
    {
        g_pPrimary = NULL;
        return;
    }

    XnProductionNodeType Type;
    xnProductionNodeTypeFromString(g_PrimaryStream.pValues[nIndex], &Type);

    switch (Type)
    {
    case XN_NODE_TYPE_DEPTH:
        g_pPrimary = *getDepthGenerator();
        break;
    case XN_NODE_TYPE_IMAGE:
        g_pPrimary = *getImageGenerator();
        break;
    case XN_NODE_TYPE_IR:
        g_pPrimary = *getIRGenerator();
        break;
    case XN_NODE_TYPE_AUDIO:
        g_pPrimary = *getAudioGenerator();
        break;
    case XN_NODE_TYPE_USER:
        g_pPrimary = *getUserGenerator();
        break;
    }
}

void Ni_Device::createStream(Generator& generator, XnProductionNodeType type)
{
    EnumerationErrors errors;
    XnStatus nRetVal = g_Context.CreateAnyProductionTree(type, NULL, generator, &errors);
    if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
    {
        XnChar strError[1024];
        errors.ToString(strError, 1024);
        std::cout <<"Error" << strError << std::endl;
        return;
    }
    else if (nRetVal != XN_STATUS_OK)
    {
        std::cout <<"Open failed: " << xnGetStatusString(nRetVal) << std::endl;;
        return;
    }
}

void Ni_Device::toggleStream(Generator& generator, XnProductionNodeType type, bool* bIsOn)
{
    XnStatus nRetVal = XN_STATUS_OK;

    if (!generator.IsValid())
    {
        createStream(generator, type);
    }

    if (!generator.IsValid())
    {
        // failed creating the stream
        return;
    }

    if (generator.IsGenerating())
    {
        generator.StopGenerating();
    }
    else
    {
        nRetVal = generator.StartGenerating();
        if (nRetVal != XN_STATUS_OK)
        {
            std::cout <<"Failed to turn on " << generator.GetInfo().GetInstanceName() << ":" << xnGetStatusString(nRetVal) << std::endl;
            return;
        }
    }

    *bIsOn = (generator.IsGenerating() == TRUE);
}

void Ni_Device::toggleDepthState(int nDummy)
{
    toggleStream(g_Depth, XN_NODE_TYPE_DEPTH, &g_bIsDepthOn);
}

void Ni_Device::toggleImageState(int nDummy)
{
    toggleStream(g_Image, XN_NODE_TYPE_IMAGE, &g_bIsImageOn);
}

void Ni_Device::toggleIRState(int nDummy)
{
    toggleStream(g_IR, XN_NODE_TYPE_IR, &g_bIsIROn);
}

void Ni_Device::toggleAudioState(int nDummy)
{
    toggleStream(g_Audio, XN_NODE_TYPE_AUDIO, &g_bIsAudioOn);
}

void Ni_Device::toggleMirror(int nDummy)
{
    XnStatus nRetVal = g_Context.SetGlobalMirror(!g_Context.GetGlobalMirror());
    if (nRetVal != XN_STATUS_OK)
    {
        std::cout <<"Failed to set global mirror: " << xnGetStatusString(nRetVal) << std::endl;;
    }
}

void Ni_Device::seekFrame(int nDiff)
{
    XnStatus nRetVal = XN_STATUS_OK;
    if (isPlayerOn())
    {
        const XnChar* strNodeName = NULL;
        if (&g_pPrimary != NULL)
        {
            strNodeName = g_pPrimary.GetName();
        }
        if (g_Depth.IsValid())
        {
            strNodeName = g_Depth.GetName();
        }
        if (g_Image.IsValid())
        {
            strNodeName = g_Image.GetName();
        }
        if (g_IR.IsValid())
        {
            strNodeName = g_IR.GetName();
        }
        if (g_Audio.IsValid())
        {
            strNodeName = g_Audio.GetName();
        }

        nRetVal = g_Player.SeekToFrame(strNodeName, nDiff, XN_PLAYER_SEEK_CUR);
        if (nRetVal != XN_STATUS_OK)
        {

            std::cout <<"Failed to seek: "  << xnGetStatusString(nRetVal) << std::endl;;
            return;
        }

        XnUInt32 nFrame = 0;
        XnUInt32 nNumFrames = 0;
        nRetVal = g_Player.TellFrame(strNodeName, nFrame);
        if (nRetVal != XN_STATUS_OK)
        {
            std::cout <<"Failed to tell frame: " << xnGetStatusString(nRetVal) << std::endl;;
            return;
        }

        nRetVal = g_Player.GetNumFrames(strNodeName, nNumFrames);
        if (nRetVal != XN_STATUS_OK)
        {
            std::cout <<"Failed to get number of frames: " << xnGetStatusString(nRetVal) << std::endl;;
            return;
        }

        std::cout << "Seeked " << strNodeName << " to frame " << nFrame <<" " << nNumFrames << std::endl;
    }
}

bool Ni_Device::isDepthOn()
{
    return (Ni_Device::g_bIsDepthOn);
}

bool Ni_Device::isImageOn()
{
    return (Ni_Device::g_bIsImageOn);
}

bool Ni_Device::isIROn()
{
    return (Ni_Device::g_bIsIROn);
}

bool Ni_Device::isAudioOn()
{
    return (Ni_Device::g_bIsAudioOn);
}

bool Ni_Device::isPlayerOn()
{
    return (Ni_Device::g_bIsPlayerOn);
}

void setResolution(MapGenerator* pGenerator, int res)
{
    if (pGenerator == NULL)
    {
        std::cout <<"Stream does not exist!" << std::endl;
        return;
    }

    XnMapOutputMode Mode;
    pGenerator->GetMapOutputMode(Mode);
    Mode.nXRes = Resolution((XnResolution)res).GetXResolution();
    Mode.nYRes = Resolution((XnResolution)res).GetYResolution();
    XnStatus nRetVal = pGenerator->SetMapOutputMode(Mode);
    if (nRetVal != XN_STATUS_OK)
    {
        std::cout <<"Failed to set resolution: " << xnGetStatusString(nRetVal) << std::endl;;
    }
}

void setFPS(MapGenerator* pGenerator, int fps)
{
    if (pGenerator == NULL)
    {
        std::cout <<"Stream does not exist!" << std::endl;
        return;
    }

    XnMapOutputMode Mode;
    pGenerator->GetMapOutputMode(Mode);
    Mode.nFPS = fps;
    XnStatus nRetVal = pGenerator->SetMapOutputMode(Mode);
    if (nRetVal != XN_STATUS_OK)
    {
        std::cout <<"Failed to set FPS: " << xnGetStatusString(nRetVal) << std::endl;;
    }
}

void Ni_Device::setDepthResolution(int res)
{
    setResolution(getDepthGenerator(), res);
}

void Ni_Device::setDepthFPS(int fps)
{
    setFPS(getDepthGenerator(), fps);
}

void Ni_Device::setImageResolution(int res)
{
    setResolution(getImageGenerator(), res);
}

void Ni_Device::setImageFPS(int fps)
{
    setFPS(getImageGenerator(), fps);
}

void Ni_Device::setIRResolution(int res)
{
    setResolution(getIRGenerator(), res);
}

void Ni_Device::setIRFPS(int fps)
{
    setFPS(getIRGenerator(), fps);
}

void setStreamCropping(MapGenerator* pGenerator, XnCropping* pCropping)
{
    if (pGenerator == NULL)
    {
        std::cout <<"Stream does not exist!" << std::endl;
        return;
    }

    if (!pGenerator->IsCapabilitySupported(XN_CAPABILITY_CROPPING))
    {
        std::cout <<"Stream does not support cropping!" << std::endl;
        return;
    }

    XnStatus nRetVal = pGenerator->GetCroppingCap().SetCropping(*pCropping);
    if (nRetVal != XN_STATUS_OK)
    {
        std::cout <<"Failed to set cropping: " << xnGetStatusString(nRetVal) << std::endl;;
        return;
    }
}

void Ni_Device::setPlaybackSpeed(int ratioDiff)
{
    if (g_Player.IsValid())
    {
        XnDouble dNewSpeed = g_Player.GetPlaybackSpeed() * pow(2.0, (XnDouble)ratioDiff);
        XnStatus nRetVal = g_Player.SetPlaybackSpeed(dNewSpeed);
        if (nRetVal != XN_STATUS_OK)
        {
            std::cout <<"Failed to set playback speed: " << xnGetStatusString(nRetVal) << std::endl;;
        }
    }
    else
    {
        std::cout <<"Can't set playback speed - input is not a recording!" << std::endl;
    }
}

XnDouble Ni_Device::getPlaybackSpeed()
{
    if (g_Player.IsValid())
    {
        return g_Player.GetPlaybackSpeed();
    }
    else
    {
        return 1.0;
    }
}

Device *Ni_Device::getDevice()
{
    return g_Device.IsValid() ? &g_Device : NULL;
}
DepthGenerator *Ni_Device::getDepthGenerator()
{
    return g_Depth.IsValid() ? &g_Depth : NULL;
}
ImageGenerator *Ni_Device::getImageGenerator()
{
    return g_Image.IsValid() ? &g_Image : NULL;
}

UserGenerator* Ni_Device::getUserGenerator()
{
    return g_User.IsValid() ? &g_User : NULL;
}



IRGenerator *Ni_Device::getIRGenerator()
{
    return g_IR.IsValid() ? &g_IR : NULL;
}
AudioGenerator *Ni_Device::getAudioGenerator()
{
    return g_Audio.IsValid() ? &g_Audio : NULL;
}

DepthMetaData* Ni_Device::getDepthMetaData()
{
    return g_Depth.IsValid() ? &g_DepthMD : NULL;
}
ImageMetaData* Ni_Device::getImageMetaData()
{
    return g_Image.IsValid() ? &g_ImageMD : NULL;
}
IRMetaData* Ni_Device::getIRMetaData()
{
    return g_IR.IsValid() ? &g_irMD : NULL;
}

AudioMetaData* Ni_Device::getAudioMetaData()
{
    return g_Audio.IsValid() ? &g_AudioMD : NULL;
}
