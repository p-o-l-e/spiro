cmake_minimum_required(VERSION 3.25)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(SpiroVersion)

include(CPM)
include(SpiroMacOS)
include(JUCEDefaults)

set(PROJECT_NAME "Spiro")
set(PRODUCT_NAME "spiro")
set(COMPANY_NAME "POLE")

set(BUNDLE_ID "com.spiro.spiro")

# Valid choices: AAX Unity VST VST3 AU AUv3 Standalone
set(FORMATS Standalone)

# Force JUCE to work with sccache
# instruct MSVC to embed debug info (/Z7) instead of emitting a .pdb
set(CMAKE_POLICY_DEFAULT_CMP0141        NEW      CACHE STRING "" FORCE)
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT Embedded CACHE STRING "" FORCE)

project(${PROJECT_NAME} VERSION ${CURRENT_VERSION})

add_subdirectory(JUCE)

add_subdirectory(modules/clap-juce-extensions EXCLUDE_FROM_ALL)

# juce_add_module(modules/my_module)

add_subdirectory(modules/melatonin_inspector)

# See `docs/CMake API.md` in the JUCE repo for all config options
juce_add_plugin("${PROJECT_NAME}"

    ICON_BIG "${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.png"

    COMPANY_NAME "${COMPANY_NAME}"
    BUNDLE_ID "${BUNDLE_ID}"

    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Spro

    PLUGIN_CODE SPRO
    FORMATS "${FORMATS}"

    PRODUCT_NAME "${PRODUCT_NAME}")

add_library(SharedCode INTERFACE)

clap_juce_extensions_plugin(TARGET "${PROJECT_NAME}"
    CLAP_ID "${BUNDLE_ID}"
    CLAP_FEATURES audio-effect)

# Enable fast math, C++20 and a few other target defaults
include(SharedCodeDefaults)


target_include_directories(SharedCode INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/source"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/utility"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/modules"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/modules/interface"
    #"${CMAKE_CURRENT_SOURCE_DIR}/source/core/tools"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/setup"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/graphics"

)

file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/*.hpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/utility/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/utility/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/utility/*.cpp"

    #"${CMAKE_CURRENT_SOURCE_DIR}/source/core/tools/*.h"
    #"${CMAKE_CURRENT_SOURCE_DIR}/source/core/tools/*.hpp"
    #"${CMAKE_CURRENT_SOURCE_DIR}/source/core/tools/*.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/setup/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/setup/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/setup/*.cpp"
    
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/graphics/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/graphics/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/graphics/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/graphics/*.c"

    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/modules/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/modules/*.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/modules/interface/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/modules/interface/*.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/modules/interface/*.cpp"
)

target_sources(SharedCode INTERFACE ${SourceFiles})

include(Assets)
# MacOS only: Cleans up folder and target organization on Xcode.
include(XcodePrettify)

target_compile_definitions(SharedCode
    INTERFACE

    # JUCE_WEB_BROWSER and JUCE_USE_CURL off by default
    JUCE_WEB_BROWSER=0  # If you set this to 1, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you set this to 1, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0

    # lets the app known if we're Debug or Release
    CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
    VERSION="${CURRENT_VERSION}"

    PRODUCT_NAME_WITHOUT_VERSION="Spiro"
)

target_link_libraries(SharedCode
    INTERFACE
    Assets
    melatonin_inspector
    juce_audio_utils
    juce_audio_processors
    juce_dsp
    juce_gui_basics
    juce_gui_extra
    juce_opengl
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

target_link_libraries("${PROJECT_NAME}" PRIVATE SharedCode)

include(SpiroIPP)
include(Tests)
include(Benchmarks)
include(GitHubENV)
