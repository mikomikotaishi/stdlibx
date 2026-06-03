#pragma once

/**
 * @namespace stdx::os::darwin::audiounit
 * @brief AudioUnit — <AudioUnit/AudioUnit.h>, AUComponent.h,
 *        AudioComponent.h, AudioOutputUnit.h, AUAudioUnit.h.
 *
 * Apple's plugin/render-graph API for audio processing units. Use for
 * low-latency output (kAudioUnitSubType_DefaultOutput / HALOutput) or
 * effects.
 */
export namespace stdx::os::darwin::audiounit {
    #ifdef __APPLE__
    #if __has_include(<AudioUnit/AudioUnit.h>)
    // Component subsystem
    using Component = ::AudioComponent;
    using ComponentDescription = ::AudioComponentDescription;
    using ComponentInstance = ::AudioComponentInstance;
    using ComponentInstantiationOptions = ::AudioComponentInstantiationOptions;
    using ComponentMethod = ::AudioComponentMethod;
    using ComponentPlugInInterface = ::AudioComponentPlugInInterface;

    // AudioUnit handle (typedef of ComponentInstance)
    using Unit = ::AudioUnit;
    using UnitElement = ::AudioUnitElement;
    using UnitScope = ::AudioUnitScope;
    using UnitParameterId = ::AudioUnitParameterID;
    using UnitParameterValue = ::AudioUnitParameterValue;
    using UnitParameterEvent = ::AudioUnitParameterEvent;
    using UnitPropertyId = ::AudioUnitPropertyID;
    using UnitConnection = ::AudioUnitConnection;
    using UnitRenderActionFlags = ::AudioUnitRenderActionFlags;
    using UnitParameter = ::AudioUnitParameter;
    using UnitParameterInfo = ::AudioUnitParameterInfo;

    // Callbacks
    using RenderCallback = ::AURenderCallback;
    using RenderCallbackStruct = ::AURenderCallbackStruct;
    using PropertyListener = ::AudioUnitPropertyListenerProc;
    using InputSamplesInOutputCallback = ::AUInputSamplesInOutputCallback;

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
    #endif
}
