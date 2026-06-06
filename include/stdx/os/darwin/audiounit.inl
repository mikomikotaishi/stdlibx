#pragma once

/**
 * @namespace stdx::os::darwin::audiounit
 * @brief AudioUnit - <AudioUnit/AudioUnit.h>, AUComponent.h,
 *        AudioComponent.h, AudioOutputUnit.h, AUAudioUnit.h.
 *
 * Apple's plugin/render-graph API for audio processing units. Use for
 * low-latency output (kAudioUnitSubType_DefaultOutput / HALOutput) or
 * effects.
 */
export namespace stdx::os::darwin::audiounit {
    #if defined(__APPLE__) && defined(__BLOCKS__) && __has_include(<AudioUnit/AudioUnit.h>)
    // Component subsystem
    using AudioComponent = ::AudioComponent;
    using AudioComponentDescription = ::AudioComponentDescription;
    using AudioComponentInstance = ::AudioComponentInstance;
    using AudioComponentInstantiationOptions = ::AudioComponentInstantiationOptions;
    using AudioComponentMethod = ::AudioComponentMethod;
    using AudioComponentPlugInInterface = ::AudioComponentPlugInInterface;

    // AudioUnit handle (typedef of ComponentInstance)
    using AudioUnit = ::AudioUnit;
    using AudioUnitElement = ::AudioUnitElement;
    using AudioUnitScope = ::AudioUnitScope;
    using AudioUnitParameterId = ::AudioUnitParameterID;
    using AudioUnitParameterValue = ::AudioUnitParameterValue;
    using AudioUnitParameterEvent = ::AudioUnitParameterEvent;
    using AudioUnitPropertyId = ::AudioUnitPropertyID;
    using AudioUnitConnection = ::AudioUnitConnection;
    using AudioUnitRenderActionFlags = ::AudioUnitRenderActionFlags;
    using AudioUnitParameter = ::AudioUnitParameter;
    using AudioUnitParameterInfo = ::AudioUnitParameterInfo;

    // Callbacks
    using AURenderCallback = ::AURenderCallback;
    using AURenderCallbackStruct = ::AURenderCallbackStruct;
    using AudioUnitPropertyListenerProc = ::AudioUnitPropertyListenerProc;
    using AUInputSamplesInOutputCallback = ::AUInputSamplesInOutputCallback;

    // Component discovery / instantiation
    using ::AudioComponentFindNext;
    using ::AudioComponentCount;
    using ::AudioComponentInstanceNew;
    using ::AudioComponentInstanceDispose;
    using ::AudioComponentInstanceGetComponent;
    using ::AudioComponentGetDescription;
    using ::AudioComponentGetVersion;
    using ::AudioComponentCopyName;
    using ::AudioComponentInstantiate;

    // AudioUnit core lifecycle
    using ::AudioUnitInitialize;
    using ::AudioUnitUninitialize;
    using ::AudioUnitReset;
    using ::AudioUnitRender;
    using ::AudioUnitProcess;
    using ::AudioUnitProcessMultiple;
    using ::AudioUnitScheduleParameters;

    // Properties / parameters
    using ::AudioUnitGetPropertyInfo;
    using ::AudioUnitGetProperty;
    using ::AudioUnitSetProperty;
    using ::AudioUnitAddPropertyListener;
    using ::AudioUnitRemovePropertyListener;
    using ::AudioUnitRemovePropertyListenerWithUserData;
    using ::AudioUnitAddRenderNotify;
    using ::AudioUnitRemoveRenderNotify;
    using ::AudioUnitGetParameter;
    using ::AudioUnitSetParameter;

    // Output unit (audiotoolbox-style start/stop)
    using ::AudioOutputUnitStart;
    using ::AudioOutputUnitStop;
    #endif
}
