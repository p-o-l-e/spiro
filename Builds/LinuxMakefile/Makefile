# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# build with "V=1" for verbose builds
ifeq ($(V), 1)
V_AT =
else
V_AT = @
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef PKG_CONFIG
  PKG_CONFIG=pkg-config
endif

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Debug
endif

JUCE_ARCH_LABEL := $(shell uname -m)

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DDEBUG=1" "-D_DEBUG=1" "-DJUCE_DISPLAY_SPLASH_SCREEN=0" "-DJUCE_USE_DARK_SPLASH_SCREEN=1" "-DJUCE_PROJUCER_VERSION=0x70005" "-DJUCE_MODULE_AVAILABLE_juce_audio_basics=1" "-DJUCE_MODULE_AVAILABLE_juce_audio_devices=1" "-DJUCE_MODULE_AVAILABLE_juce_audio_formats=1" "-DJUCE_MODULE_AVAILABLE_juce_audio_plugin_client=1" "-DJUCE_MODULE_AVAILABLE_juce_audio_processors=1" "-DJUCE_MODULE_AVAILABLE_juce_audio_utils=1" "-DJUCE_MODULE_AVAILABLE_juce_core=1" "-DJUCE_MODULE_AVAILABLE_juce_data_structures=1" "-DJUCE_MODULE_AVAILABLE_juce_events=1" "-DJUCE_MODULE_AVAILABLE_juce_graphics=1" "-DJUCE_MODULE_AVAILABLE_juce_gui_basics=1" "-DJUCE_MODULE_AVAILABLE_juce_gui_extra=1" "-DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1" "-DJUCE_USE_OGGVORBIS=1" "-DJUCE_USE_MP3AUDIOFORMAT=0" "-DJUCE_USE_LAME_AUDIO_FORMAT=0" "-DJUCE_USE_WINDOWS_MEDIA_FORMAT=0" "-DJUCE_VST3_CAN_REPLACE_VST2=0" "-DJUCE_STRICT_REFCOUNTEDPOINTER=1" "-DJUCE_WEB_BROWSER=0" "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=1" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=1" "-DJucePlugin_Build_Unity=0" "-DJucePlugin_Build_LV2=0" "-DJucePlugin_Enable_IAA=0" "-DJucePlugin_Enable_ARA=0" "-DJucePlugin_Name=\"spiro\"" "-DJucePlugin_Desc=\"spiro\"" "-DJucePlugin_Manufacturer=\"pole\"" "-DJucePlugin_ManufacturerWebsite=\"https://github.com/p-o-l-e\"" "-DJucePlugin_ManufacturerEmail=\"\"" "-DJucePlugin_ManufacturerCode=0x706f6c65" "-DJucePlugin_PluginCode=0x70737072" "-DJucePlugin_IsSynth=1" "-DJucePlugin_WantsMidiInput=1" "-DJucePlugin_ProducesMidiOutput=0" "-DJucePlugin_IsMidiEffect=0" "-DJucePlugin_EditorRequiresKeyboardFocus=0" "-DJucePlugin_Version=1.0.0" "-DJucePlugin_VersionCode=0x10000" "-DJucePlugin_VersionString=\"1.0.0\"" "-DJucePlugin_VSTUniqueID=JucePlugin_PluginCode" "-DJucePlugin_VSTCategory=kPlugCategSynth" "-DJucePlugin_Vst3Category=\"Instrument|Synth\"" "-DJucePlugin_AUMainType='aumu'" "-DJucePlugin_AUSubType=JucePlugin_PluginCode" "-DJucePlugin_AUExportPrefix=spiroAU" "-DJucePlugin_AUExportPrefixQuoted=\"spiroAU\"" "-DJucePlugin_AUManufacturerCode=JucePlugin_ManufacturerCode" "-DJucePlugin_CFBundleIdentifier=com.pole.spiro" "-DJucePlugin_AAXIdentifier=com.pole.spiro" "-DJucePlugin_AAXManufacturerCode=JucePlugin_ManufacturerCode" "-DJucePlugin_AAXProductId=JucePlugin_PluginCode" "-DJucePlugin_AAXCategory=2048" "-DJucePlugin_AAXDisableBypass=0" "-DJucePlugin_AAXDisableMultiMono=0" "-DJucePlugin_IAAType=0x61757269" "-DJucePlugin_IAASubType=JucePlugin_PluginCode" "-DJucePlugin_IAAName=\"pole: spiro\"" "-DJucePlugin_VSTNumMidiInputs=16" "-DJucePlugin_VSTNumMidiOutputs=16" "-DJucePlugin_ARAContentTypes=0" "-DJucePlugin_ARATransformationFlags=0" "-DJucePlugin_ARAFactoryID=\"com.pole.spiro.factory\"" "-DJucePlugin_ARADocumentArchiveID=\"com.pole.spiro.aradocumentarchive.1.0.0\"" "-DJucePlugin_ARACompatibleArchiveIDs=\"\"" "-DJUCE_STANDALONE_APPLICATION=JucePlugin_Build_Standalone" "-DJUCER_LINUX_MAKE_6D53C8B4=1" "-DJUCE_APP_VERSION=1.0.0" "-DJUCE_APP_VERSION_HEX=0x10000" $(shell $(PKG_CONFIG) --cflags alsa freetype2 libcurl) -pthread -I../../JuceLibraryCode/modules/juce_audio_processors/format_types/VST3_SDK -I../../JuceLibraryCode -I../../JuceLibraryCode/modules $(CPPFLAGS)

  JUCE_CPPFLAGS_VST3 := 
  JUCE_CFLAGS_VST3 := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST3 := -shared -Wl,--no-undefined
  JUCE_VST3DIR := spiro_dbg.vst3
  VST3_PLATFORM_ARCH := $(shell $(CXX) make_helpers/arch_detection.cpp 2>&1 | tr '\n' ' ' | sed "s/.*JUCE_ARCH \([a-zA-Z0-9_-]*\).*/\1/")
  JUCE_VST3SUBDIR := Contents/$(VST3_PLATFORM_ARCH)-linux
  JUCE_TARGET_VST3 := $(JUCE_VST3DIR)/$(JUCE_VST3SUBDIR)/spiro_dbg.so
  JUCE_VST3DESTDIR := $(HOME)/.vst3
  JUCE_COPYCMD_VST3 := $(JUCE_OUTDIR)/$(JUCE_VST3DIR) $(JUCE_VST3DESTDIR)

  JUCE_CPPFLAGS_STANDALONE_PLUGIN := 
  JUCE_TARGET_STANDALONE_PLUGIN := spiro_dbg

  JUCE_CPPFLAGS_SHARED_CODE :=  "-DJUCE_SHARED_CODE=1"
  JUCE_TARGET_SHARED_CODE := spiro_dbg.a

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -fPIC -g -ggdb -O3 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++17 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) $(shell $(PKG_CONFIG) --libs alsa freetype2 libcurl) -fvisibility=hidden -lrt -ldl -lpthread $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DNDEBUG=1" "-DJUCE_DISPLAY_SPLASH_SCREEN=0" "-DJUCE_USE_DARK_SPLASH_SCREEN=1" "-DJUCE_PROJUCER_VERSION=0x70005" "-DJUCE_MODULE_AVAILABLE_juce_audio_basics=1" "-DJUCE_MODULE_AVAILABLE_juce_audio_devices=1" "-DJUCE_MODULE_AVAILABLE_juce_audio_formats=1" "-DJUCE_MODULE_AVAILABLE_juce_audio_plugin_client=1" "-DJUCE_MODULE_AVAILABLE_juce_audio_processors=1" "-DJUCE_MODULE_AVAILABLE_juce_audio_utils=1" "-DJUCE_MODULE_AVAILABLE_juce_core=1" "-DJUCE_MODULE_AVAILABLE_juce_data_structures=1" "-DJUCE_MODULE_AVAILABLE_juce_events=1" "-DJUCE_MODULE_AVAILABLE_juce_graphics=1" "-DJUCE_MODULE_AVAILABLE_juce_gui_basics=1" "-DJUCE_MODULE_AVAILABLE_juce_gui_extra=1" "-DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1" "-DJUCE_USE_OGGVORBIS=1" "-DJUCE_USE_MP3AUDIOFORMAT=0" "-DJUCE_USE_LAME_AUDIO_FORMAT=0" "-DJUCE_USE_WINDOWS_MEDIA_FORMAT=0" "-DJUCE_VST3_CAN_REPLACE_VST2=0" "-DJUCE_STRICT_REFCOUNTEDPOINTER=1" "-DJUCE_WEB_BROWSER=0" "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=1" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=1" "-DJucePlugin_Build_Unity=0" "-DJucePlugin_Build_LV2=0" "-DJucePlugin_Enable_IAA=0" "-DJucePlugin_Enable_ARA=0" "-DJucePlugin_Name=\"spiro\"" "-DJucePlugin_Desc=\"spiro\"" "-DJucePlugin_Manufacturer=\"pole\"" "-DJucePlugin_ManufacturerWebsite=\"https://github.com/p-o-l-e\"" "-DJucePlugin_ManufacturerEmail=\"\"" "-DJucePlugin_ManufacturerCode=0x706f6c65" "-DJucePlugin_PluginCode=0x70737072" "-DJucePlugin_IsSynth=1" "-DJucePlugin_WantsMidiInput=1" "-DJucePlugin_ProducesMidiOutput=0" "-DJucePlugin_IsMidiEffect=0" "-DJucePlugin_EditorRequiresKeyboardFocus=0" "-DJucePlugin_Version=1.0.0" "-DJucePlugin_VersionCode=0x10000" "-DJucePlugin_VersionString=\"1.0.0\"" "-DJucePlugin_VSTUniqueID=JucePlugin_PluginCode" "-DJucePlugin_VSTCategory=kPlugCategSynth" "-DJucePlugin_Vst3Category=\"Instrument|Synth\"" "-DJucePlugin_AUMainType='aumu'" "-DJucePlugin_AUSubType=JucePlugin_PluginCode" "-DJucePlugin_AUExportPrefix=spiroAU" "-DJucePlugin_AUExportPrefixQuoted=\"spiroAU\"" "-DJucePlugin_AUManufacturerCode=JucePlugin_ManufacturerCode" "-DJucePlugin_CFBundleIdentifier=com.pole.spiro" "-DJucePlugin_AAXIdentifier=com.pole.spiro" "-DJucePlugin_AAXManufacturerCode=JucePlugin_ManufacturerCode" "-DJucePlugin_AAXProductId=JucePlugin_PluginCode" "-DJucePlugin_AAXCategory=2048" "-DJucePlugin_AAXDisableBypass=0" "-DJucePlugin_AAXDisableMultiMono=0" "-DJucePlugin_IAAType=0x61757269" "-DJucePlugin_IAASubType=JucePlugin_PluginCode" "-DJucePlugin_IAAName=\"pole: spiro\"" "-DJucePlugin_VSTNumMidiInputs=16" "-DJucePlugin_VSTNumMidiOutputs=16" "-DJucePlugin_ARAContentTypes=0" "-DJucePlugin_ARATransformationFlags=0" "-DJucePlugin_ARAFactoryID=\"com.pole.spiro.factory\"" "-DJucePlugin_ARADocumentArchiveID=\"com.pole.spiro.aradocumentarchive.1.0.0\"" "-DJucePlugin_ARACompatibleArchiveIDs=\"\"" "-DJUCE_STANDALONE_APPLICATION=JucePlugin_Build_Standalone" "-DJUCER_LINUX_MAKE_6D53C8B4=1" "-DJUCE_APP_VERSION=1.0.0" "-DJUCE_APP_VERSION_HEX=0x10000" $(shell $(PKG_CONFIG) --cflags alsa freetype2 libcurl) -pthread -I../../JuceLibraryCode/modules/juce_audio_processors/format_types/VST3_SDK -I../../JuceLibraryCode -I../../JuceLibraryCode/modules $(CPPFLAGS)

  JUCE_CPPFLAGS_VST3 := 
  JUCE_CFLAGS_VST3 := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST3 := -shared -Wl,--no-undefined
  JUCE_VST3DIR := spiro.vst3
  VST3_PLATFORM_ARCH := $(shell $(CXX) make_helpers/arch_detection.cpp 2>&1 | tr '\n' ' ' | sed "s/.*JUCE_ARCH \([a-zA-Z0-9_-]*\).*/\1/")
  JUCE_VST3SUBDIR := Contents/$(VST3_PLATFORM_ARCH)-linux
  JUCE_TARGET_VST3 := $(JUCE_VST3DIR)/$(JUCE_VST3SUBDIR)/spiro.so
  JUCE_VST3DESTDIR := $(HOME)/.vst3
  JUCE_COPYCMD_VST3 := $(JUCE_OUTDIR)/$(JUCE_VST3DIR) $(JUCE_VST3DESTDIR)

  JUCE_CPPFLAGS_STANDALONE_PLUGIN := 
  JUCE_TARGET_STANDALONE_PLUGIN := spiro

  JUCE_CPPFLAGS_SHARED_CODE :=  "-DJUCE_SHARED_CODE=1"
  JUCE_TARGET_SHARED_CODE := spiro.a

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -fPIC -Ofast -flto $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++17 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) $(shell $(PKG_CONFIG) --libs alsa freetype2 libcurl) -fvisibility=hidden -flto -lrt -ldl -lpthread $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS_ALL := \

OBJECTS_VST3 := \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_VST3_dd633589.o \

OBJECTS_STANDALONE_PLUGIN := \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_Standalone_1a871192.o \

OBJECTS_SHARED_CODE := \
  $(JUCE_OBJDIR)/mixer_88d9a485.o \
  $(JUCE_OBJDIR)/rotor_1aee65f6.o \
  $(JUCE_OBJDIR)/product_69f18cab.o \
  $(JUCE_OBJDIR)/sigma_8be184a1.o \
  $(JUCE_OBJDIR)/snh_69da69a9.o \
  $(JUCE_OBJDIR)/lfo_ea4aaa71.o \
  $(JUCE_OBJDIR)/vca_f5652810.o \
  $(JUCE_OBJDIR)/blur_9b875075.o \
  $(JUCE_OBJDIR)/chaos_a6bb5c7c.o \
  $(JUCE_OBJDIR)/constants_12381b4b.o \
  $(JUCE_OBJDIR)/delay_31b184ff.o \
  $(JUCE_OBJDIR)/envelopes_8f0eaa9f.o \
  $(JUCE_OBJDIR)/feeder_96217e99.o \
  $(JUCE_OBJDIR)/filters_8ed9fdb7.o \
  $(JUCE_OBJDIR)/fonts_bc52d300.o \
  $(JUCE_OBJDIR)/interface_d454f235.o \
  $(JUCE_OBJDIR)/iospecs_5c0260ee.o \
  $(JUCE_OBJDIR)/modmatrix_4e6ae53f.o \
  $(JUCE_OBJDIR)/node_fb0e8890.o \
  $(JUCE_OBJDIR)/oscillator_d2b44d6e.o \
  $(JUCE_OBJDIR)/spawner_6b06e8a4.o \
  $(JUCE_OBJDIR)/utility_a2e8caa8.o \
  $(JUCE_OBJDIR)/cell_816cb3f0.o \
  $(JUCE_OBJDIR)/EnvelopeDisplay_4e196233.o \
  $(JUCE_OBJDIR)/ListSlider_790c9528.o \
  $(JUCE_OBJDIR)/Socket_2b3a4d7c.o \
  $(JUCE_OBJDIR)/SpriteSlider_8fb03d8f.o \
  $(JUCE_OBJDIR)/PluginProcessor_a059e380.o \
  $(JUCE_OBJDIR)/PluginEditor_94d4fb09.o \
  $(JUCE_OBJDIR)/Display_6a853823.o \
  $(JUCE_OBJDIR)/BinaryData_ce4232d4.o \
  $(JUCE_OBJDIR)/include_juce_audio_basics_8a4e984a.o \
  $(JUCE_OBJDIR)/include_juce_audio_devices_63111d02.o \
  $(JUCE_OBJDIR)/include_juce_audio_formats_15f82001.o \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_ARA_31a052ed.o \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_utils_e32edaee.o \
  $(JUCE_OBJDIR)/include_juce_audio_processors_10c03666.o \
  $(JUCE_OBJDIR)/include_juce_audio_processors_ara_2a4c6ef7.o \
  $(JUCE_OBJDIR)/include_juce_audio_processors_lv2_libs_12bdca08.o \
  $(JUCE_OBJDIR)/include_juce_audio_utils_9f9fb2d6.o \
  $(JUCE_OBJDIR)/include_juce_core_f26d17db.o \
  $(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o \
  $(JUCE_OBJDIR)/include_juce_events_fd7d695.o \
  $(JUCE_OBJDIR)/include_juce_graphics_f817e147.o \
  $(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o \
  $(JUCE_OBJDIR)/include_juce_gui_extra_6dee1c1a.o \

.PHONY: clean all strip VST3 Standalone

all : VST3 Standalone

VST3 : $(JUCE_OUTDIR)/$(JUCE_TARGET_VST3)
Standalone : $(JUCE_OUTDIR)/$(JUCE_TARGET_STANDALONE_PLUGIN)


$(JUCE_OUTDIR)/$(JUCE_TARGET_VST3) : $(OBJECTS_VST3) $(RESOURCES) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE)
	@command -v $(PKG_CONFIG) >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@$(PKG_CONFIG) --print-errors alsa freetype2 libcurl
	@echo Linking "spiro - VST3"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)/$(JUCE_VST3DIR)/$(JUCE_VST3SUBDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_VST3) $(OBJECTS_VST3) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_VST3) $(RESOURCES) $(TARGET_ARCH)
	-$(V_AT)[ ! "$(JUCE_VST3DESTDIR)" ] || (mkdir -p $(JUCE_VST3DESTDIR) && cp -R $(JUCE_COPYCMD_VST3))

$(JUCE_OUTDIR)/$(JUCE_TARGET_STANDALONE_PLUGIN) : $(OBJECTS_STANDALONE_PLUGIN) $(RESOURCES) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE)
	@command -v $(PKG_CONFIG) >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@$(PKG_CONFIG) --print-errors alsa freetype2 libcurl
	@echo Linking "spiro - Standalone Plugin"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_STANDALONE_PLUGIN) $(OBJECTS_STANDALONE_PLUGIN) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_STANDALONE_PLUGIN) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) : $(OBJECTS_SHARED_CODE) $(RESOURCES)
	@command -v $(PKG_CONFIG) >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@$(PKG_CONFIG) --print-errors alsa freetype2 libcurl
	@echo Linking "spiro - Shared Code"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(AR) -rcs $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(OBJECTS_SHARED_CODE)

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_VST3_dd633589.o: ../../JuceLibraryCode/include_juce_audio_plugin_client_VST3.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_plugin_client_VST3.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_VST3) $(JUCE_CFLAGS_VST3) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_Standalone_1a871192.o: ../../JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_plugin_client_Standalone.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_STANDALONE_PLUGIN) $(JUCE_CFLAGS_STANDALONE_PLUGIN) -o "$@" -c "$<"

$(JUCE_OBJDIR)/mixer_88d9a485.o: ../../Source/cell/mixer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling mixer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/rotor_1aee65f6.o: ../../Source/cell/rotor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling rotor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/product_69f18cab.o: ../../Source/cell/product.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling product.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/sigma_8be184a1.o: ../../Source/cell/sigma.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling sigma.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/snh_69da69a9.o: ../../Source/cell/snh.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling snh.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/lfo_ea4aaa71.o: ../../Source/cell/lfo.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling lfo.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/vca_f5652810.o: ../../Source/cell/vca.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling vca.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/blur_9b875075.o: ../../Source/cell/blur.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling blur.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/chaos_a6bb5c7c.o: ../../Source/cell/chaos.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling chaos.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/constants_12381b4b.o: ../../Source/cell/constants.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling constants.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/delay_31b184ff.o: ../../Source/cell/delay.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling delay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/envelopes_8f0eaa9f.o: ../../Source/cell/envelopes.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling envelopes.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/feeder_96217e99.o: ../../Source/cell/feeder.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling feeder.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/filters_8ed9fdb7.o: ../../Source/cell/filters.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling filters.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/fonts_bc52d300.o: ../../Source/cell/fonts.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling fonts.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/interface_d454f235.o: ../../Source/cell/interface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling interface.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/iospecs_5c0260ee.o: ../../Source/cell/iospecs.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling iospecs.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/modmatrix_4e6ae53f.o: ../../Source/cell/modmatrix.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling modmatrix.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/node_fb0e8890.o: ../../Source/cell/node.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling node.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/oscillator_d2b44d6e.o: ../../Source/cell/oscillator.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling oscillator.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/spawner_6b06e8a4.o: ../../Source/cell/spawner.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling spawner.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/utility_a2e8caa8.o: ../../Source/cell/utility.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling utility.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/cell_816cb3f0.o: ../../Source/cell/cell.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling cell.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/EnvelopeDisplay_4e196233.o: ../../Source/EnvelopeDisplay.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling EnvelopeDisplay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ListSlider_790c9528.o: ../../Source/ListSlider.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ListSlider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Socket_2b3a4d7c.o: ../../Source/Socket.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Socket.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SpriteSlider_8fb03d8f.o: ../../Source/SpriteSlider.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SpriteSlider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PluginProcessor_a059e380.o: ../../Source/PluginProcessor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PluginProcessor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PluginEditor_94d4fb09.o: ../../Source/PluginEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PluginEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Display_6a853823.o: ../../Source/Display.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Display.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_basics_8a4e984a.o: ../../JuceLibraryCode/include_juce_audio_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_devices_63111d02.o: ../../JuceLibraryCode/include_juce_audio_devices.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_devices.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_formats_15f82001.o: ../../JuceLibraryCode/include_juce_audio_formats.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_formats.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_ARA_31a052ed.o: ../../JuceLibraryCode/include_juce_audio_plugin_client_ARA.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_plugin_client_ARA.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_utils_e32edaee.o: ../../JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_plugin_client_utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_processors_10c03666.o: ../../JuceLibraryCode/include_juce_audio_processors.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_processors.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_processors_ara_2a4c6ef7.o: ../../JuceLibraryCode/include_juce_audio_processors_ara.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_processors_ara.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_processors_lv2_libs_12bdca08.o: ../../JuceLibraryCode/include_juce_audio_processors_lv2_libs.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_processors_lv2_libs.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_utils_9f9fb2d6.o: ../../JuceLibraryCode/include_juce_audio_utils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_core_f26d17db.o: ../../JuceLibraryCode/include_juce_core.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_core.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o: ../../JuceLibraryCode/include_juce_data_structures.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_data_structures.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_events_fd7d695.o: ../../JuceLibraryCode/include_juce_events.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_events.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_graphics_f817e147.o: ../../JuceLibraryCode/include_juce_graphics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_graphics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o: ../../JuceLibraryCode/include_juce_gui_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_extra_6dee1c1a.o: ../../JuceLibraryCode/include_juce_gui_extra.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_extra.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

clean:
	@echo Cleaning spiro
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping spiro
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

-include $(OBJECTS_VST3:%.o=%.d)
-include $(OBJECTS_STANDALONE_PLUGIN:%.o=%.d)
-include $(OBJECTS_SHARED_CODE:%.o=%.d)
