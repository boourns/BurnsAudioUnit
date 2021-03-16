# BurnsAudioUnit
Core utilities for all Audio Units from Burns Audio

Reusable, isolated classes for solving specific problems you encounter when building Audio Units.  Most of these can be used independently of one another, so pick and choose or just read the code to see how I solve these issues.

# Objective-C Helpers

[AudioBuffers](https://github.com/boourns/BurnsAudioUnit/blob/master/BurnsAudioUnit/AudioBuffers.mm)

Initializes `inputBusses` and `outputBusses`.

[HostTransport](https://github.com/boourns/BurnsAudioUnit/blob/master/BurnsAudioUnit/HostTransport.mm)

Reads the host transport state from within the AudioUnit internalRenderBlock.

[StateManager](https://github.com/boourns/BurnsAudioUnit/blob/master/BurnsAudioUnit/StateManager.mm)

Handles state management callbacks for the Audio Unit, and hard-coded presets.  Does not yet support saved-file AUv3 presets.

# C++ Helpers

[LFOKernel](https://github.com/boourns/BurnsAudioUnit/blob/master/BurnsAudioUnit/LFOKernel.hpp)

Free-running, or tempo-synced LFO.  Originally based on open source code by Mutable Instruments for the Peaks module.  Also supports sending the LFO waveform back to the UI layer.

[MidiProcessor](https://github.com/boourns/BurnsAudioUnit/blob/master/BurnsAudioUnit/MIDIProcessor.hpp)

Assigns a fixed number DSP voices to notes based on held notes and note priority.  Supports MPE.
Also can read MIDI CC messages and update AU parameters based on an AU parameter map.

[ModulationEngine](https://github.com/boourns/BurnsAudioUnit/blob/master/BurnsAudioUnit/ModulationEngine.hpp)

Implements a synthesizer modulation matrix that can either be exposed as AUParameters, or used internally to calculate modulation values.

# License
MIT

