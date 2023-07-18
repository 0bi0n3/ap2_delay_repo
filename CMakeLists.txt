cmake_minimum_required(VERSION 3.14.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(vst3sdk_SOURCE_DIR "/Users/OberonDW/Desktop/UWL/AP2_desktop/VST_SDK/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(delay_one
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "delay_one VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(delay_one
    source/version.h
    source/cids.h
    source/processor.h
    source/processor.cpp
    source/controller.h
    source/controller.cpp
    source/entry.cpp
)

#- VSTGUI Wanted ----
if(SMTG_ADD_VSTGUI)
    target_sources(delay_one
        PRIVATE
            resource/editor.uidesc
    )
    target_link_libraries(delay_one
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(delay_one
        RESOURCES
            "resource/editor.uidesc"
    )
endif(SMTG_ADD_VSTGUI)
# -------------------

smtg_target_add_plugin_snapshots (delay_one
    RESOURCES
        resource/E4964DE3799A58178174458AEEF2A6F9_snapshot.png
        resource/E4964DE3799A58178174458AEEF2A6F9_snapshot_2.0x.png
)

target_link_libraries(delay_one
    PRIVATE
        sdk
)

smtg_target_configure_version_file(delay_one)

if(SMTG_MAC)
    smtg_target_set_bundle(delay_one
        BUNDLE_IDENTIFIER com.oberon.daywest
        COMPANY_NAME "Oberon Day-West"
    )
    smtg_target_set_debug_executable(delay_one
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(delay_one PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT delay_one)

        smtg_target_set_debug_executable(delay_one
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
