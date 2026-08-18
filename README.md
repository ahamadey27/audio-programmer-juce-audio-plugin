# Audio Programmer JUCE Audio Plug-in

A Projucer-free JUCE audio-effects template using CMake and Visual Studio Code.
It starts as a simple, smoothed output-gain effect so that the audio path,
parameter automation, state restoration, and editor attachment are all ready to
test before you replace the DSP with your own effect.

## Requirements (macOS)

- CMake 3.22 or newer
- Xcode and the Xcode command-line tools
- Visual Studio Code
- The workspace's recommended extensions: CMake Tools, C/C++, and CodeLLDB

JUCE 8.0.15 is downloaded automatically by CMake on the first configure. No
global JUCE installation and no Projucer project are required.

## Open the project

Open `AudioProgrammerPlugin.code-workspace` in Visual Studio Code. When VS Code
offers to install the recommended extensions, accept the prompt.

From Terminal, the equivalent command is:

```sh
open AudioProgrammerPlugin.code-workspace
```

## Build and test with F5

1. Select **Build + debug Standalone effect** in VS Code's Run and Debug panel.
2. Press **F5**.
3. The pre-launch task configures CMake, builds the AU, VST3, and Standalone
   targets, copies AU/VST3 into your user plug-in folders, validates the Audio
   Unit with `auval`, and launches the Standalone effect under the debugger.
4. In the Standalone window, open **Options** and choose your audio input and
   output device. Allow microphone/audio-input access when macOS asks.
5. Send a quiet test signal through the selected input and turn the Gain knob.
   Double-clicking the knob returns it to 0 dB.

Start with your monitors or headphones at a low level. A microphone routed to
speakers can create loud acoustic feedback.

The built plug-ins are copied to:

- `~/Library/Audio/Plug-Ins/VST3/Audio Programmer Plugin.vst3`
- `~/Library/Audio/Plug-Ins/Components/Audio Programmer Plugin.component`

You can load either format in a DAW for host automation, session restore, mono,
stereo, and different buffer-size/sample-rate tests. Rescan plug-ins or restart
the DAW after the first build if it does not appear immediately.

## Command-line build

```sh
cmake --preset macos-debug
cmake --build --preset debug-all
```

Build products remain under `build/AudioProgrammerPlugin_artefacts/Debug/`.

## Project layout

- `CMakeLists.txt` - JUCE dependency, plug-in formats, modules, and build settings
- `CMakePresets.json` - reproducible Debug configuration
- `Source/PluginProcessor.*` - real-time DSP, parameters, and state
- `Source/PluginEditor.*` - editor and parameter attachment
- `.vscode/` - F5 build/debug workflow

## Suggested first changes

Keep the parameter/state and editor wiring intact while replacing the gain stage
inside `processBlock`. Prepare DSP objects in `prepareToPlay`; do not allocate,
lock, access files, or perform unbounded work on the audio thread.
