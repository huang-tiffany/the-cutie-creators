# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


set(CPACK_BUILD_SOURCE_DIRS "/Users/Tiffany/Desktop/csci1230/the-cutie-creators;/Users/Tiffany/Desktop/csci1230/the-cutie-creators/build/build-the-cutie-creators-Qt_6_7_3_for_macOS-Debug")
set(CPACK_CMAKE_GENERATOR "Ninja")
set(CPACK_COMPONENTS_ALL "Unspecified;headers;pkgconfig")
set(CPACK_COMPONENT_HEADERS_DEPENDS "libraries")
set(CPACK_COMPONENT_HEADERS_DESCRIPTION "C/C++ header files for use with FreeType")
set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C/C++ Headers")
set(CPACK_COMPONENT_HEADERS_GROUP "Development")
set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Library used to build programs which use FreeType")
set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Libraries")
set(CPACK_COMPONENT_LIBRARIES_GROUP "Development")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "/Users/Tiffany/Qt/Tools/CMake/CMake.app/Contents/share/cmake-3.29/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "the-cutie-creators built using CMake")
set(CPACK_DMG_SLA_USE_RESOURCE_FILE_LICENSE "ON")
set(CPACK_GENERATOR "TGZ")
set(CPACK_INNOSETUP_ARCHITECTURE "x64")
set(CPACK_INSTALL_CMAKE_PROJECTS "/Users/Tiffany/Desktop/csci1230/the-cutie-creators/build/build-the-cutie-creators-Qt_6_7_3_for_macOS-Debug;the-cutie-creators;ALL;/")
set(CPACK_INSTALL_PREFIX "/usr/local")
set(CPACK_MODULE_PATH "/Users/Tiffany/Qt/6.7.3/macos/lib/cmake/Qt6;/Users/Tiffany/Qt/6.7.3/macos/lib/cmake/Qt6/3rdparty/extra-cmake-modules/find-modules;/Users/Tiffany/Qt/6.7.3/macos/lib/cmake/Qt6/3rdparty/kwin;/Users/Tiffany/Qt/6.7.3/macos/lib/cmake/Qt6;/Users/Tiffany/Qt/6.7.3/macos/lib/cmake/Qt6/3rdparty/extra-cmake-modules/find-modules;/Users/Tiffany/Qt/6.7.3/macos/lib/cmake/Qt6/3rdparty/kwin;/Users/Tiffany/Qt/6.7.3/macos/lib/cmake/Qt6;/Users/Tiffany/Qt/6.7.3/macos/lib/cmake/Qt6/3rdparty/extra-cmake-modules/find-modules;/Users/Tiffany/Qt/6.7.3/macos/lib/cmake/Qt6/3rdparty/kwin;/Users/Tiffany/Qt/6.7.3/macos/lib/cmake/Qt6;/Users/Tiffany/Qt/6.7.3/macos/lib/cmake/Qt6/3rdparty/extra-cmake-modules/find-modules;/Users/Tiffany/Qt/6.7.3/macos/lib/cmake/Qt6/3rdparty/kwin;/Users/Tiffany/Qt/6.7.3/macos/lib/cmake/Qt6;/Users/Tiffany/Qt/6.7.3/macos/lib/cmake/Qt6/3rdparty/extra-cmake-modules/find-modules;/Users/Tiffany/Qt/6.7.3/macos/lib/cmake/Qt6/3rdparty/kwin;/Users/Tiffany/Desktop/csci1230/the-cutie-creators/freetype/builds/cmake")
set(CPACK_NSIS_DISPLAY_NAME "the-cutie-creators 2.13.3")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
set(CPACK_NSIS_PACKAGE_NAME "the-cutie-creators 2.13.3")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OBJDUMP_EXECUTABLE "/usr/bin/objdump")
set(CPACK_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX14.4.sdk")
set(CPACK_OUTPUT_CONFIG_FILE "/Users/Tiffany/Desktop/csci1230/the-cutie-creators/build/build-the-cutie-creators-Qt_6_7_3_for_macOS-Debug/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "/Users/Tiffany/Desktop/csci1230/the-cutie-creators/freetype/README")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The FreeType font rendering library.")
set(CPACK_PACKAGE_FILE_NAME "the-cutie-creators-2.13.3-Darwin")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "the-cutie-creators 2.13.3")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "the-cutie-creators 2.13.3")
set(CPACK_PACKAGE_NAME "the-cutie-creators")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "Humanity")
set(CPACK_PACKAGE_VERSION "2.13.3")
set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "13")
set(CPACK_PACKAGE_VERSION_PATCH "3")
set(CPACK_RESOURCE_FILE_LICENSE "/Users/Tiffany/Desktop/csci1230/the-cutie-creators/freetype/LICENSE.TXT")
set(CPACK_RESOURCE_FILE_README "/Users/Tiffany/Qt/Tools/CMake/CMake.app/Contents/share/cmake-3.29/Templates/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_WELCOME "/Users/Tiffany/Qt/Tools/CMake/CMake.app/Contents/share/cmake-3.29/Templates/CPack.GenericWelcome.txt")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_GENERATOR "TBZ2;TGZ;TXZ;TZ")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/Users/Tiffany/Desktop/csci1230/the-cutie-creators/build/build-the-cutie-creators-Qt_6_7_3_for_macOS-Debug/CPackSourceConfig.cmake")
set(CPACK_SOURCE_RPM "OFF")
set(CPACK_SOURCE_TBZ2 "ON")
set(CPACK_SOURCE_TGZ "ON")
set(CPACK_SOURCE_TXZ "ON")
set(CPACK_SOURCE_TZ "ON")
set(CPACK_SOURCE_ZIP "OFF")
set(CPACK_SYSTEM_NAME "Darwin")
set(CPACK_THREADS "1")
set(CPACK_TOPLEVEL_TAG "Darwin")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "/Users/Tiffany/Desktop/csci1230/the-cutie-creators/build/build-the-cutie-creators-Qt_6_7_3_for_macOS-Debug/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
