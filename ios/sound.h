#ifndef IOS_H
#define IOS_H

#endif // IOS_H
/******************************************************************************\
 * Copyright (c) 2004-2014
 *
 * Author(s):
 *  Volker Fischer
 *
 ******************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
\******************************************************************************/

#if !defined(_SOUND_H__9518A621345F78_363456876UZGSDF82CF549__INCLUDED_)
#define _SOUND_H__9518A621345F78_363456876UZGSDF82CF549__INCLUDED_

/*#include <CoreServices/CoreServices.h>*/
#include <MobileCoreServices/MobileCoreServices.h>
#include <AudioUnit/AudioUnit.h>
#include <AudioUnit/AudioComponent.h>
#include <AudioToolbox/AudioServices.h>

#define AudioDeviceID unsigned

/*#include <CoreAudio/AudioHardware.h>*/

#include <QMutex>
#include "util.h"
#include "soundbase.h"
#include "global.h"


/* Classes ********************************************************************/
class CSound : public CSoundBase {
public:
    CSound(void (*fpNewProcessCallback)(CVector<short> &psData, void *arg),
           void *arg,
           const int iCtrlMIDIChannel,
           const bool,
           const QString &);

    virtual ~CSound() {}


    virtual int Init(const int iNewPrefMonoBufferSize);

    virtual void Start();

    virtual void Stop();

    // these variables should be protected but cannot since we want
    // to access them from the callback function
    CVector<short> vecsTmpAudioSndCrdStereo;
    int iCoreAudioBufferSizeMono;
    int iCoreAudioBufferSizeStereo;

protected:
    virtual QString LoadAndInitializeDriver(int iIdx);

    //ComponentInstance == AudioComponentInstance??
    QString CheckDeviceCapabilities(AudioComponentInstance &NewAudioInputUnit,
                                    AudioComponentInstance &NewAudioOutputUnit);

    void CloseCoreAudio();

    UInt32 SetBufferSize(AudioDeviceID &audioDeviceID,
                         const bool bIsInput,
                         UInt32 iPrefBufferSize);

    void GetAudioDeviceInfos(const AudioDeviceID DeviceID,
                             QString &strDeviceName,
                             bool &bIsInput,
                             bool &bIsOutput);

    // callbacks
    static OSStatus deviceNotification(AudioDeviceID,
                                       UInt32,
                                       Boolean,
                                       AudioUnitPropertyID inPropertyID,
                                       void *inRefCon);

    static OSStatus processInput(void *inRefCon,
                                 AudioUnitRenderActionFlags *ioActionFlags,
                                 const AudioTimeStamp *inTimeStamp,
                                 UInt32 inBusNumber,
                                 UInt32 inNumberFrames,
                                 AudioBufferList *);

    static OSStatus processOutput(void *inRefCon,
                                  AudioUnitRenderActionFlags *,
                                  const AudioTimeStamp *,
                                  UInt32,
                                  UInt32,
                                  AudioBufferList *ioData);

    AudioStreamBasicDescription streamFormat;

    AURenderCallbackStruct inputCallbackStruct;
    AURenderCallbackStruct outputCallbackStruct;

    //ComponentInstance == AudioComponentInstance??
    AudioComponentInstance audioInputUnit;
    AudioDeviceID audioInputDevice[MAX_NUMBER_SOUND_CARDS];
    AudioComponentInstance audioOutputUnit;
    AudioDeviceID audioOutputDevice[MAX_NUMBER_SOUND_CARDS];

    AudioBufferList *pBufferList;

    QMutex Mutex;
};

#endif // !defined(_SOUND_H__9518A621345F78_363456876UZGSDF82CF549__INCLUDED_)
