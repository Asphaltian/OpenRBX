cmake_minimum_required(VERSION 3.25...4.2 FATAL_ERROR)

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
  set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_BIN}" "--extra-arg=/EHsc")

  set_source_files_properties(Client/App/v8world/Contact.cpp PROPERTIES SKIP_LINTING ON)
endif()

math(EXPR bits "8 * ${CMAKE_SIZEOF_VOID_P}")
message(STATUS "Building ${bits}-bit RBXGS")
if (NOT bits EQUAL 32)
  message(WARNING "Only 32-bit binaries are supported")
endif()

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

set(OPENRBX_3RDPARTY "${PROJECT_SOURCE_DIR}/3rdparty")

set(OPENRBX_BOOST_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Client/boost_1_34_1/src")
set(OPENRBX_G3D_INCLUDE_DIR   "${PROJECT_SOURCE_DIR}/Client/Rendering/g3d/include")
set(OPENRBX_LUA_INCLUDE_DIR   "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src")
set(OPENRBX_SDL_INCLUDE_DIR   "${PROJECT_SOURCE_DIR}/Client/Rendering/SDL-1.2.6/include")
set(OPENRBX_MESA_INCLUDE_DIR  "${PROJECT_SOURCE_DIR}/Client/Rendering/Mesa-6.5/include")
set(OPENRBX_G3D_EXTRA_INCLUDE_DIRS
  "${PROJECT_SOURCE_DIR}/Client/Rendering/g3d/png"
  "${PROJECT_SOURCE_DIR}/Client/Rendering/g3d/zlib"
)

add_library(fmodex IMPORTED UNKNOWN)
set_property(TARGET fmodex PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/Client/fmod/api/lib/fmodex_vc.lib")
set_property(TARGET fmodex PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/Client/fmod/api/inc")

add_subdirectory(3rdparty)

set(OPENRBX_BOOST_LIBS boost_signals boost_thread boost_date_time)
set(OPENRBX_G3D_LIBS   graphics3d glg3d)
set(OPENRBX_MESA_LIBS  opengl32 glu32 osmesa32)

if (MSVC)
  add_compile_options(/W3)
endif()

set(OPENRBX_APP_SOURCES
  Client/App/security/SecurityContext.cpp
  Client/App/v8datamodel/Hopper.cpp
  Client/App/gui/Button.cpp
  Client/App/gui/ChatOutput.cpp
  Client/App/gui/ChatWidget.cpp
  Client/App/gui/ArrowPanel.cpp
  Client/App/gui/EquationDisplay.cpp
  Client/App/gui/GUI.cpp
  Client/App/gui/HealthHud.cpp
  Client/App/gui/GuiDraw.cpp
  Client/App/gui/MenuItem.cpp
  Client/App/gui/ProfanityFilter.cpp
  Client/App/gui/Widget.cpp
  Client/App/humanoid/Humanoid.cpp
  Client/App/reflection/reflection_function.cpp
  Client/App/reflection/reflection_object.cpp
  Client/App/reflection/reflection_property.cpp
  Client/App/reflection/signal.cpp
  Client/App/reflection/type.cpp
  Client/App/util/Debug.cpp
  Client/App/v8tree/enumproperty.cpp
  Client/App/v8xml/XmlElement.cpp
  Client/App/v8xml/SerializerV2.cpp
  Client/App/v8xml/XmlSerializer.cpp
  Client/App/util/IRenderable.cpp
  Client/App/v8tree/Instance.cpp
  Client/App/v8tree/Verb.cpp
  Client/App/util/boost.cpp
  Client/App/util/ExponentialRunningAverage.cpp
  Client/App/util/Extents.cpp
  Client/App/util/Face.cpp
  Client/App/util/FileSystem.cpp
  Client/App/util/Guid.cpp
  Client/App/util/Handle.cpp
  Client/App/util/Hash.cpp
  Client/App/util/Http.cpp
  Client/App/util/IndexBox.cpp
  Client/App/util/Log.cpp
  Client/App/util/Math.cpp
  Client/App/util/MeshId.cpp
  Client/App/util/Name.cpp
  Client/App/util/NormalId.cpp
  Client/App/util/Quaternion.cpp
  Client/App/util/ContentProvider.cpp
  Client/App/util/Utilities.cpp
  Client/App/util/Profiling.cpp
  Client/App/util/Rect.cpp
  Client/App/util/RunningAverage.cpp
  Client/App/util/standardout.cpp
  Client/App/v8kernel/Body.cpp
  Client/App/util/Debug.cpp
  Client/App/v8datamodel/Accoutrement.cpp
  Client/App/v8datamodel/BrickColor.cpp
  Client/App/v8datamodel/custommesh.cpp
  Client/App/v8datamodel/Decal.cpp
  Client/App/v8datamodel/Enums.cpp
  Client/App/v8datamodel/TimeState.cpp
  Client/App/v8datamodel/IEquipable.cpp
  Client/App/v8datamodel/Team.cpp
  Client/App/v8datamodel/Flag.cpp
  Client/App/v8datamodel/FlagStand.cpp
  Client/App/v8datamodel/DebugSettings.cpp
  Client/App/v8datamodel/FaceInstance.cpp
  Client/App/v8datamodel/Feature.cpp
  Client/App/v8datamodel/ICharacterSubject.cpp
  Client/App/v8datamodel/ModelInstance.cpp
  Client/App/v8datamodel/RootInstance.cpp
  Client/App/v8datamodel/JointInstance.cpp
  Client/App/v8datamodel/Message.cpp
  Client/App/v8datamodel/PVInstance.cpp
  Client/App/v8datamodel/PartInstance.cpp
  Client/App/v8datamodel/SpawnLocation.cpp
  Client/App/v8datamodel/Tool.cpp
  Client/App/v8datamodel/Workspace.cpp
  Client/App/v8kernel/Cofm.cpp
  Client/App/v8kernel/Connector.cpp
  Client/App/v8kernel/Constants.cpp
  Client/App/v8kernel/Kernel.cpp
  Client/App/v8kernel/Link.cpp
  Client/App/v8kernel/Pair.cpp
  Client/App/v8kernel/Point.cpp
  Client/App/v8kernel/SimBody.cpp
  Client/App/v8world/Assembly2.cpp
  Client/App/v8world/AssemblyStage2.cpp
  Client/App/v8world/Ball.cpp
  Client/App/v8world/Block.cpp
  Client/App/v8world/Clump2.cpp
  Client/App/v8world/ClumpStage2.cpp
  Client/App/v8world/ContactManager.cpp
  Client/App/v8world/Contact.cpp
  Client/App/v8world/Controller.cpp
  Client/App/v8world/Edge.cpp
  Client/App/v8world/IMoving.cpp
  Client/App/v8world/GlueJoint.cpp
  Client/App/v8world/IPipelined.cpp
  Client/App/v8world/IWorldStage.cpp
  Client/App/v8world/Joint.cpp
  Client/App/v8world/JointBuilder.cpp
  Client/App/v8world/JointStage.cpp
  Client/App/v8world/MacroTypes.cpp
  Client/App/v8world/Mechanism.cpp
  Client/App/v8world/MotorJoint.cpp
  Client/App/v8world/MultiJoint.cpp
  Client/App/v8world/Primitive.cpp
  Client/App/v8world/SimJobStage.cpp
  Client/App/v8world/SleepStage2.cpp
  Client/App/v8world/RigidJoint.cpp
  Client/App/v8world/RotateJoint.cpp
  Client/App/v8world/SnapJoint.cpp
  Client/App/v8world/WeldJoint.cpp
  Client/App/v8world/SpatialHash.cpp
  Client/App/v8world/World.cpp
  Client/App/util/Timer.cpp
  Client/App/util/Units.cpp
  Client/App/util/UserInputBase.cpp
  Client/App/humanoid/Balancing.cpp
  Client/App/humanoid/FallingDown.cpp
  Client/App/humanoid/Flying.cpp
  Client/App/humanoid/Freefall.cpp
  Client/App/humanoid/GettingUp.cpp
  Client/App/humanoid/Jumping.cpp
  Client/App/humanoid/Running.cpp
  Client/App/humanoid/Seated.cpp
  Client/App/script/LuaAtomicClasses.cpp
  Client/App/script/LuaSignalBridge.cpp
  Client/App/script/LuaInstanceBridge.cpp
  Client/App/script/LuaMemory.cpp
  Client/App/script/Script.cpp
  Client/App/script/ScriptEvent.cpp
  Client/App/script/ThreadRef.cpp
  Client/App/script/ScriptContext.cpp
  Client/App/tool/DragTool.cpp
  Client/App/tool/DragUtilities.cpp
  Client/App/tool/Dragger.cpp
  Client/App/tool/GroupDragTool.cpp
  Client/App/tool/MegaDragger.cpp
  Client/App/tool/NullTool.cpp
  Client/App/tool/PartDragTool.cpp
  Client/App/tool/RunDragger.cpp
  Client/App/tool/ToolsArrow.cpp
  Client/App/util/RunStateOwner.cpp
  Client/App/util/Sound.cpp
  Client/App/util/SoundWorld.cpp
  Client/App/v8datamodel/Backpack.cpp
  Client/App/v8datamodel/Camera.cpp
  Client/App/v8datamodel/CharacterAppearance.cpp
  Client/App/v8datamodel/DataModel.cpp
  Client/App/v8datamodel/DebrisService.cpp
  Client/App/v8datamodel/Explosion.cpp
  Client/App/v8datamodel/Filters.cpp
  Client/App/v8datamodel/ForceField.cpp
  Client/App/v8datamodel/GameSettings.cpp
  Client/App/v8datamodel/GeometryService.cpp
  Client/App/v8datamodel/GlobalSettings.cpp
  Client/App/v8datamodel/Gyro.cpp
  Client/App/v8datamodel/ICameraOwner.cpp
  Client/App/v8datamodel/JointsService.cpp
  Client/App/v8datamodel/Lighting.cpp
  Client/App/v8datamodel/LocakBackpack.cpp
  Client/App/v8datamodel/Mouse.cpp
  Client/App/v8datamodel/MouseCommand.cpp
  Client/App/v8datamodel/SafeChat.cpp
  Client/App/v8datamodel/ScriptMouseCommand.cpp
  Client/App/v8datamodel/Seat.cpp
  Client/App/v8datamodel/Selection.cpp
  Client/App/v8datamodel/Stats.cpp
  Client/App/v8datamodel/Surface.cpp
  Client/App/v8datamodel/Surfaces.cpp
  Client/App/v8datamodel/Sky.cpp
  Client/App/v8datamodel/Teams.cpp
  Client/App/v8datamodel/TimerService.cpp
  Client/App/v8datamodel/ToolMouseCommand.cpp
  Client/App/v8datamodel/ToolsPart.cpp
  Client/App/v8datamodel/UserController.cpp
  Client/App/v8datamodel/Value.cpp
  Client/App/v8datamodel/Visit.cpp
  Client/App/v8datamodel/factoryregistration.cpp
  Client/App/v8tree/Service.cpp
)
set(OPENRBX_NETWORK_SOURCES
  Client/Network/Player.cpp
  Client/Network/Players.cpp
  Client/Network/API.cpp
  Client/Network/Client.cpp
  Client/Network/ClientPhysics.cpp
  Client/Network/CrashReporter.cpp
  Client/Network/NetworkSettings.cpp
  Client/Network/Replicator.cpp
  Client/Network/Server.cpp
  Client/Network/Streaming.cpp
)

set(OPENRBX_RAKNET_SOURCES
  Client/Network/raknet30/Source/BitStream.cpp
  Client/Network/raknet30/Source/CheckSum.cpp
  Client/Network/raknet30/Source/DS_HuffmanEncodingTree.cpp
  Client/Network/raknet30/Source/DataBlockEncryptor.cpp
  Client/Network/raknet30/Source/FileOperations.cpp
  Client/Network/raknet30/Source/GetTime.cpp
  Client/Network/raknet30/Source/NetworkIDManager.cpp
  Client/Network/raknet30/Source/PacketLogger.cpp
  Client/Network/raknet30/Source/PluginInterface.cpp
  Client/Network/raknet30/Source/RPCMap.cpp
  Client/Network/raknet30/Source/RakNetTypes.cpp
  Client/Network/raknet30/Source/RakPeer.cpp
  Client/Network/raknet30/Source/RakSleep.cpp
  Client/Network/raknet30/Source/Rand.cpp
  Client/Network/raknet30/Source/ReliabilityLayer.cpp
  Client/Network/raknet30/Source/SHA1.cpp
  Client/Network/raknet30/Source/SimpleMutex.cpp
  Client/Network/raknet30/Source/SocketLayer.cpp
  Client/Network/raknet30/Source/StringCompressor.cpp
  Client/Network/raknet30/Source/StringTable.cpp
  Client/Network/raknet30/Source/rijndael.cpp
)
set(OPENRBX_RBXVIEW_SOURCES
  Client/RbxView/BevelMesh.cpp
  Client/RbxView/BrickMesh.cpp
  Client/RbxView/CylinderMesh.cpp
  Client/RbxView/FileMesh.cpp
  Client/RbxView/HeadMesh.cpp
  Client/RbxView/MaterialFactory.cpp
  Client/RbxView/PBBMesh.cpp
  Client/RbxView/QuadVolume.cpp
  Client/RbxView/SphereMesh.cpp
  Client/RbxView/TorsoMesh.cpp
  Client/RbxView/View.cpp
  Client/RbxView/WedgeMesh.cpp
  Client/RbxView/Part.cpp
)
set(OPENRBX_RENDERLIB_SOURCES
  Client/Rendering/RenderLib/AggregatingSceneManager.cpp
  Client/Rendering/RenderLib/Chunk.cpp
  Client/Rendering/RenderLib/Clusterer.cpp
  Client/Rendering/RenderLib/DepthBlur.cpp
  Client/Rendering/RenderLib/EffectSettings.cpp
  Client/Rendering/RenderLib/Material.cpp
  Client/Rendering/RenderLib/Mesh.cpp
  Client/Rendering/RenderLib/Profiler.cpp
  Client/Rendering/RenderLib/RenderScene.cpp
  Client/Rendering/RenderLib/RenderStats.cpp
  Client/Rendering/RenderLib/RenderSurface.cpp
  Client/Rendering/RenderLib/TextureProxy.cpp
)
set(OPENRBX_APPDRAW_SOURCES
  Client/Rendering/AppDraw/AdornG3D.cpp
  Client/Rendering/AppDraw/Draw.cpp
  Client/Rendering/AppDraw/DrawAdorn.cpp
  Client/Rendering/AppDraw/DrawPrimitives.cpp
  Client/Rendering/AppDraw/Fonts.cpp
  Client/Rendering/AppDraw/HitTest.cpp
  Client/Rendering/AppDraw/Textures.cpp
)

set(OPENRBX_ENGINE_INCLUDE_DIRS
  "${PROJECT_SOURCE_DIR}/Client/App/include"
  "${PROJECT_SOURCE_DIR}/Client/Network/include"
  "${PROJECT_SOURCE_DIR}/Client/RbxView/include"
  "${PROJECT_SOURCE_DIR}/Client/RbxViewBase"
  "${PROJECT_SOURCE_DIR}/Client/Rendering/RenderLib/include"
  "${PROJECT_SOURCE_DIR}/Client/Rendering/AppDraw/include"
  "${PROJECT_SOURCE_DIR}/Client/RbxGraphics/include"
  "${PROJECT_SOURCE_DIR}/Client/Network/raknet30/Source"
  "${PROJECT_SOURCE_DIR}/util"
)

set(OPENRBX_3RDPARTY_INCLUDE_DIRS
  "${OPENRBX_BOOST_INCLUDE_DIR}"
  "${OPENRBX_G3D_INCLUDE_DIR}"
  "${OPENRBX_LUA_INCLUDE_DIR}"
  ${OPENRBX_G3D_EXTRA_INCLUDE_DIRS}
  "${OPENRBX_SDL_INCLUDE_DIR}"
)

set(OPENRBX_COMMON_DEFINITIONS
  WIN32
  NDEBUG
  _LIB
  _VC80_UPGRADE=0x0710
  _MBCS
)

set(OPENRBX_ENGINE_LIBRARIES)

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
    target_compile_options(${NAME} PRIVATE /EHsc)
  endif()
  set(OPENRBX_ENGINE_LIBRARIES ${OPENRBX_ENGINE_LIBRARIES} ${NAME} PARENT_SCOPE)
endfunction()

set(OPENRBX_LUA_SOURCES
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lapi.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lauxlib.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lbaselib.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lcode.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/ldebug.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/ldo.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/ldump.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lfunc.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lgc.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/liolib.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/llex.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lmathlib.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lmem.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lobject.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lopcodes.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lparser.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lstate.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lstring.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lstrlib.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/ltable.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/ltablib.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/ltm.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lundump.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lvm.c"
  "${PROJECT_SOURCE_DIR}/Client/App/lua-5.1.1/src/lzio.c"
)

add_engine_library(lua
  DEFINITIONS _CRT_SECURE_NO_WARNINGS _RELEASE
  OPTIONS /O2 /Ob2 /Oi /Oy /GF /GS- /fp:fast /TP /EHsc
  SOURCES ${OPENRBX_LUA_SOURCES}
)

if (MSVC_FOR_DECOMP)
  list(APPEND OPENRBX_APP_SOURCES Client/boost_1_34_1/libs/iostreams/src/zlib.cpp)
endif()

add_engine_library(app
  DEFINITIONS _CRT_SECURE_NO_WARNINGS _RELEASE BOOST_IOSTREAMS_NO_LIB
  OPTIONS /O2 /Ob2 /Oi /Oy /GF /Gy /GS- /fp:fast
  SOURCES ${OPENRBX_APP_SOURCES}
)

if (TARGET app)
  target_link_libraries(app PRIVATE fmodex)
endif()

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

add_engine_library(appdraw
  DEFINITIONS _RELEASE
  OPTIONS /O2 /GS-
  SOURCES ${OPENRBX_APPDRAW_SOURCES}
)

add_engine_library(renderlib
  OPTIONS /O2 /Wp64
  SOURCES ${OPENRBX_RENDERLIB_SOURCES}
)

if (MSVC_FOR_DECOMP)
  set(OPENRBX_TOOLKITPRO "${OPENRBX_3RDPARTY}/Codejock Software/MFC/Xtreme ToolkitPro v11.2.1")
  set(OPENRBX_TOOLKITPRO_WORKSPACE "${OPENRBX_TOOLKITPRO}/Workspace/ToolkitPro")
  file(READ "${OPENRBX_TOOLKITPRO_WORKSPACE}/ToolkitProStatic_vc80.vcproj" OPENRBX_TOOLKITPRO_VCPROJ)
  string(REGEX MATCHALL "RelativePath=\"[^\"]+\\.cpp\"" OPENRBX_TOOLKITPRO_FILES "${OPENRBX_TOOLKITPRO_VCPROJ}")
  set(OPENRBX_TOOLKITPRO_SOURCES)
  foreach(entry ${OPENRBX_TOOLKITPRO_FILES})
    string(REGEX REPLACE "RelativePath=\"(.*)\"" "\\1" path "${entry}")
    string(REPLACE "\\" "/" path "${path}")
    list(APPEND OPENRBX_TOOLKITPRO_SOURCES "${OPENRBX_TOOLKITPRO_WORKSPACE}/${path}")
  endforeach()

  add_library(toolkitpro STATIC ${OPENRBX_TOOLKITPRO_SOURCES})
  target_compile_definitions(toolkitpro PRIVATE WIN32 _LIB NDEBUG _MBCS _AFXDLL ZLIB_WINAPI)
  target_include_directories(toolkitpro PRIVATE
    "${OPENRBX_TOOLKITPRO_WORKSPACE}"
    "${OPENRBX_TOOLKITPRO}/Source"
    "${OPENRBX_TOOLKITPRO}/Source/Common"
  )
  target_compile_options(toolkitpro PRIVATE /O2 /Ob1 /GF /Gy /Zc:wchar_t)
  set_property(TARGET toolkitpro PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
  set_property(TARGET toolkitpro PROPERTY OUTPUT_NAME "ToolkitPro1121vc80DS")

  set(OPENRBX_OGRE "${PROJECT_SOURCE_DIR}/Client/Rendering/OgreSDK")
  add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/rgmain.lib"
    COMMAND lib /nologo /machine:x86 "/def:${OPENRBX_OGRE}/lib/rgmain.def" "/out:${CMAKE_BINARY_DIR}/rgmain.lib"
    DEPENDS "${OPENRBX_OGRE}/lib/rgmain.def"
  )
  add_custom_target(rgmain_implib DEPENDS "${CMAKE_BINARY_DIR}/rgmain.lib")
  add_library(rgmain UNKNOWN IMPORTED)
  set_property(TARGET rgmain PROPERTY IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/rgmain.lib")
  add_dependencies(rgmain rgmain_implib)
endif()

set(OPENRBX_WEBSERVICE_SOURCES
  WebService/ThumbnailGenerator.cpp
  WebService/WebService.cpp
  Client/win/ErrorUploader.cpp
  Client/win/LogManager.cpp
  Client/win/VersionInfo.cpp
)

add_library(webservice SHARED
  ${OPENRBX_WEBSERVICE_SOURCES}
  util/decomp.cpp
)
target_compile_definitions(webservice PRIVATE ${OPENRBX_COMMON_DEFINITIONS} _USRDLL _CRT_SECURE_NO_WARNINGS
  _ATL_NO_COM_SUPPORT)
reccmp_add_target(webservice ID WEBSERVICE)

target_include_directories(webservice PRIVATE
  "${PROJECT_SOURCE_DIR}/WebService"
  "${PROJECT_SOURCE_DIR}/Client/win"
  ${OPENRBX_ENGINE_INCLUDE_DIRS}
  ${OPENRBX_3RDPARTY_INCLUDE_DIRS}
  "${OPENRBX_MESA_INCLUDE_DIR}"
)


if (OPENRBX_ENGINE_LIBRARIES)
  target_link_libraries(webservice PRIVATE ${OPENRBX_ENGINE_LIBRARIES})
endif()

target_link_libraries(webservice PRIVATE
  ${OPENRBX_G3D_LIBS}
  ${OPENRBX_BOOST_LIBS}
  ${OPENRBX_MESA_LIBS}
)

target_link_libraries(webservice PRIVATE fmodex)
add_custom_command(TARGET webservice POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_if_different
    "${PROJECT_SOURCE_DIR}/Client/fmod/api/fmodex.dll" "$<TARGET_FILE_DIR:webservice>"
  COMMENT "Copying fmodex.dll next to WebService.dll"
)

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
  target_sources(webservice PRIVATE WebService/WebService.def WebService/WebService.rc)
  target_link_options(webservice PRIVATE "-MAP:$<TARGET_FILE_DIR:webservice>/$<TARGET_FILE_BASE_NAME:webservice>.map")
  foreach(lib zlib.lib G3D.lib G3D-debug.lib GLG3D.lib GLG3D-debug.lib sdl.lib sdlmain.lib glut32.lib)
    target_link_options(webservice PRIVATE "/NODEFAULTLIB:${lib}")
  endforeach()
endif()

set_property(TARGET webservice PROPERTY OUTPUT_NAME "WebService")
set_property(TARGET webservice PROPERTY PREFIX "")
set_property(TARGET webservice PROPERTY SUFFIX ".dll")

add_custom_target(rbxgs DEPENDS webservice)

if (MSVC_FOR_DECOMP)
  add_executable(roblox WIN32
    Client/win/AboutRobloxDialog.cpp
    Client/win/BrowserView.cpp
    Client/win/ChildFrm.cpp
    Client/win/ErrorUploader.cpp
    Client/win/LogManager.cpp
    Client/win/MainFrm.cpp
    Client/win/NullDoc.cpp
    Client/win/ObjectBrowser.cpp
    Client/win/Roblox.cpp
    Client/win/RobloxDoc.cpp
    Client/win/RobloxReportView.cpp
    Client/win/RobloxView.cpp
    Client/win/ScintillaCtrl.cpp
    Client/win/ScintillaDocView.cpp
    Client/win/ScriptDoc.cpp
    Client/win/SecureHtmlView.cpp
    Client/win/VersionInfo.cpp
    Client/win/Roblox.rc
    util/decomp.cpp
  )
  target_compile_definitions(roblox PRIVATE ${OPENRBX_COMMON_DEFINITIONS} _CRT_SECURE_NO_WARNINGS _AFXDLL _XTP_STATICLINK)
  target_compile_options(roblox PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/FIStdAfx.h>)
  reccmp_add_target(roblox ID ROBLOX)

  target_include_directories(roblox PRIVATE
    "${PROJECT_SOURCE_DIR}/Client/win"
    ${OPENRBX_ENGINE_INCLUDE_DIRS}
    ${OPENRBX_3RDPARTY_INCLUDE_DIRS}
    "${OPENRBX_MESA_INCLUDE_DIR}"
    "${OPENRBX_TOOLKITPRO}/Source"
    "${OPENRBX_OGRE}/include"
    "${OPENRBX_3RDPARTY}/scintilla/include"
    "${OPENRBX_3RDPARTY}/Microsoft DirectX SDK (November 2007)/Include"
  )

  target_link_libraries(roblox PRIVATE
    toolkitpro
    rgmain
    mfc80
    mfcs80
    ${OPENRBX_ENGINE_LIBRARIES}
    ${OPENRBX_G3D_LIBS}
    ${OPENRBX_BOOST_LIBS}
    ${OPENRBX_MESA_LIBS}
    fmodex
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

  target_link_options(roblox PRIVATE "-MAP:$<TARGET_FILE_DIR:roblox>/$<TARGET_FILE_BASE_NAME:roblox>.map" "/FIXED:NO" "/MANIFEST:NO")
  foreach(lib zlib.lib G3D.lib G3D-debug.lib GLG3D.lib GLG3D-debug.lib sdl.lib sdlmain.lib glut32.lib)
    target_link_options(roblox PRIVATE "/NODEFAULTLIB:${lib}")
  endforeach()

  set_property(TARGET roblox PROPERTY OUTPUT_NAME "Roblox")
endif()

if (MSVC)
  if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "15")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  endif()
  if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "19.14.26428")
    add_compile_options(-Zc:__cplusplus)
  endif()
endif()

if (OPENRBX_DECOMP_ASSERT)
  message(STATUS "Decomp asserts enabled")
  set_property(TARGET webservice ${OPENRBX_ENGINE_LIBRARIES}
    APPEND PROPERTY COMPILE_DEFINITIONS "ENABLE_DECOMP_ASSERTS")
endif()

if (MSVC_FOR_DECOMP)
  if (OPENRBX_WEBSERVICE_SOURCES)
    set_source_files_properties(
      ${OPENRBX_WEBSERVICE_SOURCES}
      PROPERTIES COMPILE_OPTIONS "$<$<STREQUAL:$<TARGET_PROPERTY:NAME>,webservice>:/O2;/GL>"
    )
    target_link_options(webservice PRIVATE "/LTCG")
  endif()

  option(OPENRBX_KEEP_UNREFERENCED "Keep unreferenced functions so reccmp can compare them" ON)
  if (OPENRBX_KEEP_UNREFERENCED)
    target_link_options(webservice PRIVATE "/OPT:NOREF,ICF")
    target_link_options(roblox PRIVATE "/OPT:NOREF,ICF")
  else()
    target_link_options(webservice PRIVATE "/OPT:REF,ICF")
    target_link_options(roblox PRIVATE "/OPT:REF,ICF")
  endif()

  set_property(TARGET webservice roblox PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

  set_property(TARGET webservice roblox PROPERTY MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>")

  set(CMAKE_CXX_FLAGS "/EHsc /TP /GR /nologo")
  set(CMAKE_CXX_FLAGS_DEBUG "/Od /D_DEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/O2 /Zi /DNDEBUG")
  set(CMAKE_CXX_FLAGS_MINSIZEREL "/O1 /DNDEBUG")

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

  set(CMAKE_EXE_LINKER_FLAGS "/machine:I386")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/incremental:yes /debug")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/incremental:no")
  set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "/incremental:no /debug")
  set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "/incremental:no")
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
        "${PROJECT_SOURCE_DIR}/Client/*.cpp"
        "${PROJECT_SOURCE_DIR}/Client/*.h"
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
        Client/App/include
        Client/Network/include
        Client/RbxView/include
        Client/Rendering/RenderLib/include
        Client/Rendering/AppDraw/include
      --path WebService Client
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
