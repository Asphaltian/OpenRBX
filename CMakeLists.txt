cmake_minimum_required(VERSION 3.25...4.2 FATAL_ERROR)

# MSVC runtime library flags are selected by an abstraction
cmake_policy(SET CMP0091 NEW)

project(openrbx C CXX)

if (NOT WIN32)
  message(FATAL_ERROR "Windows is the only supported platform (for now...)")
endif()

include(CheckCXXSourceCompiles)
include(CMakeDependentOption)
include(CMakePushCheckState)
include("${CMAKE_CURRENT_LIST_DIR}/cmake/reccmp.cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
option(ENABLE_CLANG_TIDY "Enable clang-tidy")
if (ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY_BIN NAMES "clang-tidy")
  set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_BIN}")
  set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_BIN}")

  # boost 1.34's scoped_lock throws, and clang-tidy reports it as an error whatever /EHsc the
  # command line carries. The diagnostic is inside the vendored header, not our code.
  set_source_files_properties(common/app/util/Name.cpp common/app/util/standardout.cpp
                              common/app/v8world/World.cpp
                              PROPERTIES SKIP_LINTING ON)
endif()

math(EXPR bits "8 * ${CMAKE_SIZEOF_VOID_P}")
message(STATUS "Building ${bits}-bit RBXGS")
if (NOT bits EQUAL 32)
  message(WARNING "Only 32-bit binaries are supported")
endif()

# Only cl 14.00.50727.762 (VC8 SP1) reproduces the original codegen.
set(MSVC_FOR_DECOMP FALSE)
if (MSVC)
  if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "14.0" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "15.0")
    set(MSVC_FOR_DECOMP TRUE)
  endif()
endif()
message(STATUS "MSVC for decompilation: ${MSVC_FOR_DECOMP}")

function(add_cxx_warning WARNING)
  if (OPENRBX_WERROR)
    set(compiler_option "-Werror=${WARNING}")
  else()
    set(compiler_option "-W${WARNING}")
  endif()
  string(MAKE_C_IDENTIFIER "COMPILER_SUPPORTS${compiler_option}" varname)

  cmake_push_check_state(RESET)
  set(CMAKE_REQUIRED_FLAGS "${compiler_option} ")
  if (MSVC)
    string(APPEND CMAKE_REQUIRED_FLAGS "/WX")
  else()
    string(APPEND CMAKE_REQUIRED_FLAGS "-Werror")
  endif()
  check_cxx_source_compiles("int main() { return 0; }" ${varname})
  cmake_pop_check_state()

  if (${varname})
    add_compile_options(${compiler_option})
  endif()
endfunction()

option(OPENRBX_WERROR "Treat warnings as errors" OFF)
option(OPENRBX_DECOMP_ASSERT "Assert struct size" ${MSVC_FOR_DECOMP})

add_cxx_warning(parentheses)

find_program(IWYU_BIN NAMES "include-what-you-use")
if(IWYU_BIN)
  set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${IWYU_BIN}" -target i686-pc-windows-msvc "-Xiwyu" "--mapping_file=${PROJECT_SOURCE_DIR}/tools/iwyu.imp")
endif()

# --- Third party dependencies ---
# Lua went into app.lib and SDL is headers only, so neither has a library here.
set(OPENRBX_3RDPARTY "${PROJECT_SOURCE_DIR}/3rdparty")

set(OPENRBX_BOOST_INCLUDE_DIR "${OPENRBX_3RDPARTY}/boost_1_34_1")
set(OPENRBX_G3D_INCLUDE_DIR   "${OPENRBX_3RDPARTY}/g3d/source/include")
set(OPENRBX_LUA_INCLUDE_DIR   "${OPENRBX_3RDPARTY}/lua-5.1.1/src")
set(OPENRBX_SDL_INCLUDE_DIR   "${OPENRBX_3RDPARTY}/SDL-1.2.6/include")
set(OPENRBX_MESA_INCLUDE_DIR  "${OPENRBX_3RDPARTY}/mesa-6.5/include")
set(OPENRBX_G3D_EXTRA_INCLUDE_DIRS
  "${OPENRBX_3RDPARTY}/g3d/source/png"
  "${OPENRBX_3RDPARTY}/g3d/source/zlib"
)

# FMOD ships fmodex_vc.lib prebuilt, so it is always on disk.
add_library(fmodex IMPORTED UNKNOWN)
set_property(TARGET fmodex PROPERTY IMPORTED_LOCATION "${OPENRBX_3RDPARTY}/fmod/api/lib/fmodex_vc.lib")
set_property(TARGET fmodex PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${OPENRBX_3RDPARTY}/fmod/api/inc")

# Built from source next door, under the names the original linked.
add_subdirectory(3rdparty)

set(OPENRBX_BOOST_LIBS boost_signals boost_thread)
set(OPENRBX_G3D_LIBS   graphics3d glg3d)
set(OPENRBX_MESA_LIBS  opengl32 glu32 osmesa32)

# Below here is ours. The vendored targets already exist, so their /W0 never meets this.
if (MSVC)
  add_compile_options(/W3)
endif()

# --- Engine libraries ---
# One OBJECT library per Roblox static library. Link order decides which folded copy survives.

set(OPENRBX_APP_SOURCES
  common/app/security/SecurityContext.cpp
  common/app/humanoid/Humanoid.cpp
  common/app/reflection/reflection_object.cpp
  common/app/reflection/reflection_property.cpp
  common/app/reflection/signal.cpp
  common/app/reflection/type.cpp
  common/app/util/Debug.cpp
  common/app/v8tree/Instance.cpp
  common/app/util/ExponentialRunningAverage.cpp
  common/app/util/Extents.cpp
  common/app/util/Face.cpp
  common/app/util/Guid.cpp
  common/app/util/Handle.cpp
  common/app/util/IndexBox.cpp
  common/app/util/Log.cpp
  common/app/util/Math.cpp
  common/app/util/Name.cpp
  common/app/util/NormalId.cpp
  common/app/util/Quaternion.cpp
  common/app/util/Profiling.cpp
  common/app/util/Rect.cpp
  common/app/util/RunningAverage.cpp
  common/app/util/standardout.cpp
  common/app/v8kernel/Body.cpp
  common/app/util/Debug.cpp
  common/app/v8datamodel/Accoutrement.cpp
  common/app/v8datamodel/custommesh.cpp
  common/app/v8datamodel/Decal.cpp
  common/app/v8datamodel/Flag.cpp
  common/app/v8datamodel/FlagStand.cpp
  common/app/v8datamodel/DebugSettings.cpp
  common/app/v8datamodel/FaceInstance.cpp
  common/app/v8datamodel/Feature.cpp
  common/app/v8datamodel/ModelInstance.cpp
  common/app/v8datamodel/JointInstance.cpp
  common/app/v8datamodel/Message.cpp
  common/app/v8datamodel/PVInstance.cpp
  common/app/v8datamodel/PartInstance.cpp
  common/app/v8datamodel/SpawnLocation.cpp
  common/app/v8datamodel/Team.cpp
  common/app/v8datamodel/Tool.cpp
  common/app/v8datamodel/Workspace.cpp
  common/app/v8kernel/Cofm.cpp
  common/app/v8kernel/Connector.cpp
  common/app/v8kernel/Constants.cpp
  common/app/v8kernel/Kernel.cpp
  common/app/v8kernel/Link.cpp
  common/app/v8kernel/Pair.cpp
  common/app/v8kernel/Point.cpp
  common/app/v8kernel/SimBody.cpp
  common/app/v8world/Assembly2.cpp
  common/app/v8world/AssemblyStage2.cpp
  common/app/v8world/Ball.cpp
  common/app/v8world/Block.cpp
  common/app/v8world/Clump2.cpp
  common/app/v8world/ClumpStage2.cpp
  common/app/v8world/ContactManager.cpp
  common/app/v8world/Contact.cpp
  common/app/v8world/Controller.cpp
  common/app/v8world/Edge.cpp
  common/app/v8world/IMoving.cpp
  common/app/v8world/GlueJoint.cpp
  common/app/v8world/IPipelined.cpp
  common/app/v8world/IWorldStage.cpp
  common/app/v8world/Joint.cpp
  common/app/v8world/JointBuilder.cpp
  common/app/v8world/JointStage.cpp
  common/app/v8world/Mechanism.cpp
  common/app/v8world/MotorJoint.cpp
  common/app/v8world/MultiJoint.cpp
  common/app/v8world/Primitive.cpp
  common/app/v8world/SimJobStage.cpp
  common/app/v8world/SleepStage2.cpp
  common/app/v8world/RigidJoint.cpp
  common/app/v8world/RotateJoint.cpp
  common/app/v8world/SnapJoint.cpp
  common/app/v8world/WeldJoint.cpp
  common/app/v8world/SpatialHash.cpp
  common/app/v8world/World.cpp
  common/app/util/Timer.cpp
  common/app/util/Units.cpp
  common/app/humanoid/FallingDown.cpp
  common/app/humanoid/Flying.cpp
  common/app/humanoid/Freefall.cpp
  common/app/humanoid/GettingUp.cpp
  common/app/humanoid/Jumping.cpp
  common/app/humanoid/Running.cpp
  common/app/humanoid/Seated.cpp
  common/app/script/LuaInstanceBridge.cpp
  common/app/script/Script.cpp
  common/app/script/ScriptContext.cpp
  common/app/tool/GroupDragTool.cpp
  common/app/tool/NullTool.cpp
  common/app/tool/PartDragTool.cpp
  common/app/tool/ToolsArrow.cpp
  common/app/util/RunStateOwner.cpp
  common/app/util/Sound.cpp
  common/app/v8datamodel/Backpack.cpp
  common/app/v8datamodel/Camera.cpp
  common/app/v8datamodel/CharacterAppearance.cpp
  common/app/v8datamodel/DataModel.cpp
  common/app/v8datamodel/DebrisService.cpp
  common/app/v8datamodel/Explosion.cpp
  common/app/v8datamodel/ForceField.cpp
  common/app/v8datamodel/GameSettings.cpp
  common/app/v8datamodel/GeometryService.cpp
  common/app/v8datamodel/GlobalSettings.cpp
  common/app/v8datamodel/Gyro.cpp
  common/app/v8datamodel/Hopper.cpp
  common/app/v8datamodel/Lighting.cpp
  common/app/v8datamodel/LocakBackpack.cpp
  common/app/v8datamodel/Mouse.cpp
  common/app/v8datamodel/Seat.cpp
  common/app/v8datamodel/Selection.cpp
  common/app/v8datamodel/Stats.cpp
  common/app/v8datamodel/Teams.cpp
  common/app/v8datamodel/TimerService.cpp
  common/app/v8datamodel/ToolMouseCommand.cpp
  common/app/v8datamodel/ToolsPart.cpp
  common/app/v8datamodel/UserController.cpp
  common/app/v8datamodel/Value.cpp
  common/app/v8datamodel/Visit.cpp
  common/app/v8datamodel/factoryregistration.cpp
  common/app/v8tree/Service.cpp
)
set(OPENRBX_NETWORK_SOURCES
  common/network/Player.cpp
  common/network/Players.cpp
  common/network/API.cpp
  common/network/Client.cpp
  common/network/NetworkSettings.cpp
  common/network/Replicator.cpp
  common/network/Server.cpp
)

# The Network project built these 21 objects into network.lib itself; only _CRT_SECURE_NO_WARNINGS differs.
set(OPENRBX_RAKNET_SOURCES
  3rdparty/RakNet30/Source/BitStream.cpp
  3rdparty/RakNet30/Source/CheckSum.cpp
  3rdparty/RakNet30/Source/DS_HuffmanEncodingTree.cpp
  3rdparty/RakNet30/Source/DataBlockEncryptor.cpp
  3rdparty/RakNet30/Source/FileOperations.cpp
  3rdparty/RakNet30/Source/GetTime.cpp
  3rdparty/RakNet30/Source/NetworkIDManager.cpp
  3rdparty/RakNet30/Source/PacketLogger.cpp
  3rdparty/RakNet30/Source/PluginInterface.cpp
  3rdparty/RakNet30/Source/RPCMap.cpp
  3rdparty/RakNet30/Source/RakNetTypes.cpp
  3rdparty/RakNet30/Source/RakPeer.cpp
  3rdparty/RakNet30/Source/RakSleep.cpp
  3rdparty/RakNet30/Source/Rand.cpp
  3rdparty/RakNet30/Source/ReliabilityLayer.cpp
  3rdparty/RakNet30/Source/SHA1.cpp
  3rdparty/RakNet30/Source/SimpleMutex.cpp
  3rdparty/RakNet30/Source/SocketLayer.cpp
  3rdparty/RakNet30/Source/StringCompressor.cpp
  3rdparty/RakNet30/Source/StringTable.cpp
  3rdparty/RakNet30/Source/rijndael.cpp
)
set(OPENRBX_RBXVIEW_SOURCES
)
set(OPENRBX_RENDERLIB_SOURCES
)
set(OPENRBX_APPDRAW_SOURCES
)

set(OPENRBX_ENGINE_INCLUDE_DIRS
  "${PROJECT_SOURCE_DIR}/common/app/include"
  "${PROJECT_SOURCE_DIR}/common/network/include"
  "${PROJECT_SOURCE_DIR}/common/rbxview/include"
  "${PROJECT_SOURCE_DIR}/common/renderlib/include"
  "${PROJECT_SOURCE_DIR}/common/appdraw/include"
  "${PROJECT_SOURCE_DIR}/3rdparty/RakNet30/Source"
  "${PROJECT_SOURCE_DIR}/util"
)

# On the command line of every Roblox library.
set(OPENRBX_3RDPARTY_INCLUDE_DIRS
  "${OPENRBX_BOOST_INCLUDE_DIR}"
  "${OPENRBX_G3D_INCLUDE_DIR}"
  ${OPENRBX_G3D_EXTRA_INCLUDE_DIRS}
  "${OPENRBX_SDL_INCLUDE_DIR}"
)

# Verbatim from the command lines in WebService.pdb. _VC80_UPGRADE marks a VS2003 conversion.
set(OPENRBX_COMMON_DEFINITIONS
  WIN32
  NDEBUG
  _LIB
  _VC80_UPGRADE=0x0710
  _MBCS
)

set(OPENRBX_ENGINE_LIBRARIES)

# Creates an OBJECT library only when it has sources, so the tree configures while empty.
function(add_engine_library NAME)
  cmake_parse_arguments(ARGS "" "" "DEFINITIONS;OPTIONS;SOURCES" ${ARGN})
  if (NOT ARGS_SOURCES)
    message(STATUS "Engine library '${NAME}' has no sources yet, skipping")
    return()
  endif()
  add_library(${NAME} OBJECT ${ARGS_SOURCES})
  target_compile_definitions(${NAME} PRIVATE ${OPENRBX_COMMON_DEFINITIONS} ${ARGS_DEFINITIONS})
  target_include_directories(${NAME} PRIVATE ${OPENRBX_ENGINE_INCLUDE_DIRS} ${OPENRBX_3RDPARTY_INCLUDE_DIRS})
  if (MSVC_FOR_DECOMP AND ARGS_OPTIONS)
    target_compile_options(${NAME} PRIVATE ${ARGS_OPTIONS})
  elseif (MSVC)
    # boost::thread throws. The decomp build carries /EHsc in CMAKE_CXX_FLAGS, the conformance
    # build has nothing of its own, and clang-tidy reads the flags off the target.
    target_compile_options(${NAME} PRIVATE /EHsc)
  endif()
  set(OPENRBX_ENGINE_LIBRARIES ${OPENRBX_ENGINE_LIBRARIES} ${NAME} PARENT_SCOPE)
endfunction()

# /GS- /fp:fast and full inline expansion are unique to App among the Roblox libraries.
add_engine_library(app
  DEFINITIONS _CRT_SECURE_NO_WARNINGS _RELEASE
  OPTIONS /O2 /Ob2 /Oi /Oy /GF /Gy /GS- /fp:fast
  SOURCES ${OPENRBX_APP_SOURCES}
)

# App is the only library that touches FMOD, from Sound.cpp.
if (TARGET app)
  target_link_libraries(app PRIVATE fmodex)
endif()

# Network does not define _CRT_SECURE_NO_WARNINGS; only the RakNet files inside it do.
add_engine_library(network
  OPTIONS /O2
  SOURCES ${OPENRBX_NETWORK_SOURCES} ${OPENRBX_RAKNET_SOURCES}
)

if (TARGET network AND OPENRBX_RAKNET_SOURCES)
  set_source_files_properties(${OPENRBX_RAKNET_SOURCES}
    PROPERTIES COMPILE_DEFINITIONS "_CRT_SECURE_NO_WARNINGS")
endif()

add_engine_library(rbxview
  DEFINITIONS _RELEASE
  OPTIONS /O2 /Ob2 /Oi /Oy /GF /GS-
  SOURCES ${OPENRBX_RBXVIEW_SOURCES}
)

# RenderLib is the only library built with /Wp64.
add_engine_library(renderlib
  OPTIONS /O2 /Wp64
  SOURCES ${OPENRBX_RENDERLIB_SOURCES}
)

add_engine_library(appdraw
  DEFINITIONS _RELEASE
  OPTIONS /O2 /GS-
  SOURCES ${OPENRBX_APPDRAW_SOURCES}
)

# --- WebService.dll target ---
# The only objects compiled /GL, so the link ran /LTCG.
set(OPENRBX_WEBSERVICE_SOURCES
  WebService/ThumbnailGenerator.cpp
  WebService/WebService.cpp
  WebService/WebServiceMaps.cpp
  common/win/VersionInfo.cpp
)

add_library(webservice SHARED
  ${OPENRBX_WEBSERVICE_SOURCES}
  util/decomp.cpp
)
target_compile_definitions(webservice PRIVATE ${OPENRBX_COMMON_DEFINITIONS} _USRDLL _CRT_SECURE_NO_WARNINGS
  _ATL_NO_COM_SUPPORT)
reccmp_add_target(webservice ID WEBSERVICE)

# ThumbnailGenerator is the only file that includes a Mesa header.
target_include_directories(webservice PRIVATE
  "${PROJECT_SOURCE_DIR}/WebService"
  ${OPENRBX_ENGINE_INCLUDE_DIRS}
  ${OPENRBX_3RDPARTY_INCLUDE_DIRS}
  "${OPENRBX_MESA_INCLUDE_DIR}"
)

if (OPENRBX_ENGINE_LIBRARIES)
  target_link_libraries(webservice PRIVATE ${OPENRBX_ENGINE_LIBRARIES})
endif()

# opengl32 and glu32 here are Mesa's, not the system libraries.
target_link_libraries(webservice PRIVATE
  ${OPENRBX_G3D_LIBS}
  ${OPENRBX_BOOST_LIBS}
  ${OPENRBX_MESA_LIBS}
)

# fmodex_vc.lib was pulled in at the final link of WebService.dll, not into app.lib.
target_link_libraries(webservice PRIVATE fmodex)
add_custom_command(TARGET webservice POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_if_different
    "${OPENRBX_3RDPARTY}/fmod/api/fmodex.dll" "$<TARGET_FILE_DIR:webservice>"
  COMMENT "Copying fmodex.dll next to WebService.dll"
)

# From the import table of the original binary.
target_link_libraries(webservice PRIVATE
  advapi32
  dbghelp
  gdi32
  ole32
  oleaut32
  setupapi
  shell32
  shlwapi
  user32
  version
  wininet
  winmm
  ws2_32
)

if (MSVC)
  target_sources(webservice PRIVATE WebService/WebService.def)
  target_link_options(webservice PRIVATE "-MAP:$<TARGET_FILE_DIR:webservice>/$<TARGET_FILE_BASE_NAME:webservice>.map")
endif()

set_property(TARGET webservice PROPERTY OUTPUT_NAME "WebService")
set_property(TARGET webservice PROPERTY PREFIX "")
set_property(TARGET webservice PROPERTY SUFFIX ".dll")

add_custom_target(rbxgs DEPENDS webservice)

# --- Modern compiler compatibility ---
if (MSVC)
  if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "15")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  endif()
  # Visual Studio 2017 version 15.7 needs "/Zc:__cplusplus" for __cplusplus
  if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "19.14.26428")
    add_compile_options(-Zc:__cplusplus)
  endif()
endif()

if (OPENRBX_DECOMP_ASSERT)
  message(STATUS "Decomp asserts enabled")
  # The size assertions live in the engine headers, so every target needs this, not just the DLL.
  set_property(TARGET webservice ${OPENRBX_ENGINE_LIBRARIES}
    APPEND PROPERTY COMPILE_DEFINITIONS "ENABLE_DECOMP_ASSERTS")
endif()

if (MSVC_FOR_DECOMP)
  # util/decomp.cpp is ours, not theirs, so it stays out of the LTCG list.
  if (OPENRBX_WEBSERVICE_SOURCES)
    set_source_files_properties(
      ${OPENRBX_WEBSERVICE_SOURCES}
      PROPERTIES COMPILE_OPTIONS "/O2;/GL"
    )
    target_link_options(webservice PRIVATE "/LTCG")
  endif()

  # The original linked /OPT:REF. Keeping the COMDATs costs no accuracy; turn off once reachable.
  option(OPENRBX_KEEP_UNREFERENCED "Keep unreferenced functions so reccmp can compare them" ON)
  if (OPENRBX_KEEP_UNREFERENCED)
    target_link_options(webservice PRIVATE "/OPT:NOREF,ICF")
  else()
    target_link_options(webservice PRIVATE "/OPT:REF,ICF")
  endif()

  # Every Roblox object used the shared CRT (/MD) and depends on Microsoft.VC80.CRT 8.0.50727.762.
  set_property(TARGET webservice PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

  # The original shipped a separate PDB (/Zi), not embedded debug info.
  set_property(TARGET webservice PROPERTY MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>")

  # The original also passed /X with every system include listed. Not reproduced, no codegen effect.
  set(CMAKE_CXX_FLAGS "/EHsc /TP /GR /nologo")
  set(CMAKE_CXX_FLAGS_DEBUG "/Od /D_DEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/O2 /Zi /DNDEBUG")
  set(CMAKE_CXX_FLAGS_MINSIZEREL "/O1 /DNDEBUG")

  # As above without /TP, which would compile G3D's vendored C as C++.
  set(CMAKE_C_FLAGS "/nologo")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}")

  set(CMAKE_STATIC_LINKER_FLAGS "/machine:I386")

  set(CMAKE_SHARED_LINKER_FLAGS "/machine:I386")
  set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/incremental:yes /debug")
  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/incremental:no")
  set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "/incremental:no /debug")
  set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "/incremental:no")
endif()

find_program(CLANGFORMAT_BIN NAMES clang-format)
if (EXISTS "${CLANGFORMAT_BIN}")
  execute_process(COMMAND "${CLANGFORMAT_BIN}" --version
    OUTPUT_VARIABLE "CLANGFORMAT_VERSION_OUTPUT"
    RESULT_VARIABLE "CLANGFORMAT_RESULT"
  )
  if (CLANGFORMAT_RESULT EQUAL 0 AND CLANGFORMAT_VERSION_OUTPUT MATCHES "version ([0-9\\.]+)")
    set(CLANGFORMAT_VERSION "${CMAKE_MATCH_1}")
    set(CLANGFORMAT_VERSION_REQUIRED "17.0")
    message(DEBUG "Found clang-format version ${CLANGFORMAT_VERSION} (needs ${CLANGFORMAT_VERSION_REQUIRED}")
    if (CLANGFORMAT_VERSION VERSION_GREATER_EQUAL "${CLANGFORMAT_VERSION_REQUIRED}")
      file(GLOB_RECURSE openrbx_sources CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/WebService/*.cpp"
        "${PROJECT_SOURCE_DIR}/WebService/*.h"
        "${PROJECT_SOURCE_DIR}/common/*.cpp"
        "${PROJECT_SOURCE_DIR}/common/*.h"
      )
      string(REPLACE ";" "\n" openrbx_sources_lines "${openrbx_sources}")
      file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/openrbx_sources.txt" "${openrbx_sources_lines}\n")
      add_custom_target(clang-format ${CLANGFORMAT_BIN} -i "--files=${CMAKE_CURRENT_BINARY_DIR}/openrbx_sources.txt")
    endif()
  endif()
endif()

find_library(CLANG_LIBRARY NAMES "clang" DOC "libclang from host (NOT target) system")
find_package(Python3 COMPONENTS Interpreter QUIET)
if (CLANG_LIBRARY AND TARGET Python3::Interpreter)
  add_custom_target(ncc
    Python3::Interpreter "${PROJECT_SOURCE_DIR}/tools/ncc/ncc.py"
      --clang-lib "${CLANG_LIBRARY}"
      --recurse
      --style tools/ncc/ncc.style
      --skip tools/ncc/skip.yml
      --definition PASCAL WINAPI FAR BOOL CALLBACK HWND__=HWND
      --include
        util
        WebService
        common/app/include
        common/network/include
        common/rbxview/include
        common/renderlib/include
        common/appdraw/include
      --path WebService common
    COMMENT "Verifying naming conventions (ncc)"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
  )
endif()

reccmp_configure()

set(RELEASE_URL "https://github.com/Asphaltian/OpenRBX/releases/download/continuous")
add_custom_target(download-master-progress)
add_custom_target(show-progress)

get_property(RECCMP_TARGETS GLOBAL PROPERTY RECCMP_TARGETS)
foreach(target ${RECCMP_TARGETS})
  get_property(reccmp_id TARGET "${target}" PROPERTY INTERFACE_RECCMP_ID)

  set(progress_local "${PROJECT_BINARY_DIR}/${reccmp_id}-progress-master.json")
  set(download_command COMMAND "${CMAKE_COMMAND}"
    "-DURL=${RELEASE_URL}"
    "-DFILENAME_ONLINE=${reccmp_id}PROGRESS.json"
    "-DFILENAME_LOCAL=${progress_local}"
    "-P" "${PROJECT_SOURCE_DIR}/cmake/download-file.cmake"
  )
  add_custom_target(download-master-progress-${reccmp_id} ${download_command})
  add_dependencies(download-master-progress download-master-progress-${reccmp_id})
  add_custom_command(OUTPUT "${progress_local}" COMMAND ${download_command})
  add_custom_target(show-progress-${reccmp_id}
    COMMENT "Compare progress of ${reccmp_id} with current git master"
    COMMAND reccmp-reccmp --target ${reccmp_id} --diff "${progress_local}"
    DEPENDS "${progress_local}"
  )
  add_dependencies(show-progress show-progress-${reccmp_id})
endforeach()

