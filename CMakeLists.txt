cmake_minimum_required(VERSION 3.22)

project(AudioProgrammerPlugin VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)

# Pinned to the latest JUCE 8 release so the project remains compatible with
# the tutorial books while avoiding machine-specific JUCE installation paths.
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.15
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(JUCE)

if(APPLE)
    set(PLUGIN_FORMATS AU VST3 Standalone)
else()
    set(PLUGIN_FORMATS VST3 Standalone)
endif()

juce_add_plugin(AudioProgrammerPlugin
    COMPANY_NAME "Audio Programmer"
    COMPANY_COPYRIGHT "Copyright (c) 2026 Alex"
    BUNDLE_ID "com.alex.audioprogrammerplugin"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    MICROPHONE_PERMISSION_ENABLED TRUE
    MICROPHONE_PERMISSION_TEXT "Audio Programmer Plugin needs audio input so you can test the effect."
    PLUGIN_MANUFACTURER_CODE Alxp
    PLUGIN_CODE Apfx
    FORMATS ${PLUGIN_FORMATS}
    PRODUCT_NAME "Audio Programmer Plugin"
)

target_sources(AudioProgrammerPlugin
    PRIVATE
        Source/PluginEditor.cpp
        Source/PluginEditor.h
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
)

target_compile_features(AudioProgrammerPlugin PRIVATE cxx_std_20)

target_compile_definitions(AudioProgrammerPlugin
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(AudioProgrammerPlugin
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

