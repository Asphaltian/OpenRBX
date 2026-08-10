# Vendored dependencies, built from source. Added before the parent turns /W3 on.

if (MSVC)
  add_compile_options(/W0)
endif()

# --- G3D 6.09 ---
# WebService.pdb puts the IJG jpeg and libpng objects in graphics3d and the zlib objects in glg3d.
set(G3D "${CMAKE_CURRENT_SOURCE_DIR}/g3d/source")

file(GLOB OPENRBX_GRAPHICS3D_SOURCES "${G3D}/G3Dcpp/*.cpp" "${G3D}/IJG/*.c" "${G3D}/png/*.c")
file(GLOB OPENRBX_GLG3D_SOURCES "${G3D}/GLG3Dcpp/*.cpp" "${G3D}/zlib/*.c")

add_library(graphics3d STATIC ${OPENRBX_GRAPHICS3D_SOURCES})
add_library(glg3d STATIC ${OPENRBX_GLG3D_SOURCES})

foreach(name graphics3d glg3d)
  # GWindow.h includes <SDL_events.h> under G3D_WIN32, hence SDL on the include path.
  target_include_directories(${name} PRIVATE
    "${G3D}/include" "${G3D}/png" "${G3D}/zlib" "${G3D}/IJG"
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL-1.2.6/include"
  )
  target_compile_definitions(${name} PRIVATE WIN32 _LIB _MBCS _CRT_SECURE_NO_WARNINGS)
endforeach()

# --- Boost 1.34.1 ---
# Sources and BOOST_THREAD_BUILD_LIB from libs/thread/build/Jamfile.v2.
set(BOOST "${CMAKE_CURRENT_SOURCE_DIR}/boost_1_34_1")

add_library(boost_signals STATIC
  "${BOOST}/libs/signals/src/connection.cpp"
  "${BOOST}/libs/signals/src/named_slot_map.cpp"
  "${BOOST}/libs/signals/src/signal_base.cpp"
  "${BOOST}/libs/signals/src/slot.cpp"
  "${BOOST}/libs/signals/src/trackable.cpp"
)

add_library(boost_thread STATIC
  "${BOOST}/libs/thread/src/barrier.cpp"
  "${BOOST}/libs/thread/src/condition.cpp"
  "${BOOST}/libs/thread/src/exceptions.cpp"
  "${BOOST}/libs/thread/src/mutex.cpp"
  "${BOOST}/libs/thread/src/once.cpp"
  "${BOOST}/libs/thread/src/recursive_mutex.cpp"
  "${BOOST}/libs/thread/src/thread.cpp"
  "${BOOST}/libs/thread/src/tss_dll.cpp"
  "${BOOST}/libs/thread/src/tss_hooks.cpp"
  "${BOOST}/libs/thread/src/tss_pe.cpp"
  "${BOOST}/libs/thread/src/tss.cpp"
  "${BOOST}/libs/thread/src/xtime.cpp"
)
target_compile_definitions(boost_thread PRIVATE BOOST_THREAD_BUILD_LIB=1)

# auto_link.hpp asks the linker for exactly these names.
set_target_properties(boost_signals PROPERTIES OUTPUT_NAME "libboost_signals-vc80-mt-1_34_1")
set_target_properties(boost_thread  PROPERTIES OUTPUT_NAME "libboost_thread-vc80-mt-1_34_1")

foreach(name boost_signals boost_thread)
  target_include_directories(${name} PRIVATE "${BOOST}")
endforeach()

# --- Mesa 6.5 ---
# The Release configuration of Mesa's own VC7 projects, exclusions included.
set(MESA "${CMAKE_CURRENT_SOURCE_DIR}/mesa-6.5")

set(OPENRBX_MESA_CORE_SOURCES
  "${MESA}/src/mesa/array_cache/ac_context.c"
  "${MESA}/src/mesa/array_cache/ac_import.c"
  "${MESA}/src/mesa/glapi/glapi.c"
  "${MESA}/src/mesa/glapi/glthread.c"
  "${MESA}/src/mesa/main/accum.c"
  "${MESA}/src/mesa/main/api_arrayelt.c"
  "${MESA}/src/mesa/main/api_loopback.c"
  "${MESA}/src/mesa/main/api_noop.c"
  "${MESA}/src/mesa/main/api_validate.c"
  "${MESA}/src/mesa/main/attrib.c"
  "${MESA}/src/mesa/main/blend.c"
  "${MESA}/src/mesa/main/bufferobj.c"
  "${MESA}/src/mesa/main/buffers.c"
  "${MESA}/src/mesa/main/clip.c"
  "${MESA}/src/mesa/main/colortab.c"
  "${MESA}/src/mesa/main/context.c"
  "${MESA}/src/mesa/main/convolve.c"
  "${MESA}/src/mesa/main/debug.c"
  "${MESA}/src/mesa/main/depth.c"
  "${MESA}/src/mesa/main/depthstencil.c"
  "${MESA}/src/mesa/main/dispatch.c"
  "${MESA}/src/mesa/main/dlist.c"
  "${MESA}/src/mesa/main/drawpix.c"
  "${MESA}/src/mesa/main/enable.c"
  "${MESA}/src/mesa/main/enums.c"
  "${MESA}/src/mesa/main/eval.c"
  "${MESA}/src/mesa/main/execmem.c"
  "${MESA}/src/mesa/main/extensions.c"
  "${MESA}/src/mesa/main/fbobject.c"
  "${MESA}/src/mesa/main/feedback.c"
  "${MESA}/src/mesa/main/fog.c"
  "${MESA}/src/mesa/main/framebuffer.c"
  "${MESA}/src/mesa/main/get.c"
  "${MESA}/src/mesa/main/getstring.c"
  "${MESA}/src/mesa/main/hash.c"
  "${MESA}/src/mesa/main/hint.c"
  "${MESA}/src/mesa/main/histogram.c"
  "${MESA}/src/mesa/main/image.c"
  "${MESA}/src/mesa/main/imports.c"
  "${MESA}/src/mesa/main/light.c"
  "${MESA}/src/mesa/main/lines.c"
  "${MESA}/src/mesa/main/matrix.c"
  "${MESA}/src/mesa/main/mm.c"
  "${MESA}/src/mesa/main/occlude.c"
  "${MESA}/src/mesa/main/pixel.c"
  "${MESA}/src/mesa/main/points.c"
  "${MESA}/src/mesa/main/polygon.c"
  "${MESA}/src/mesa/main/rastpos.c"
  "${MESA}/src/mesa/main/renderbuffer.c"
  "${MESA}/src/mesa/main/state.c"
  "${MESA}/src/mesa/main/stencil.c"
  "${MESA}/src/mesa/main/texcompress.c"
  "${MESA}/src/mesa/main/texcompress_fxt1.c"
  "${MESA}/src/mesa/main/texcompress_s3tc.c"
  "${MESA}/src/mesa/main/texenvprogram.c"
  "${MESA}/src/mesa/main/texformat.c"
  "${MESA}/src/mesa/main/teximage.c"
  "${MESA}/src/mesa/main/texobj.c"
  "${MESA}/src/mesa/main/texrender.c"
  "${MESA}/src/mesa/main/texstate.c"
  "${MESA}/src/mesa/main/texstore.c"
  "${MESA}/src/mesa/main/varray.c"
  "${MESA}/src/mesa/main/vtxfmt.c"
  "${MESA}/src/mesa/math/m_debug_clip.c"
  "${MESA}/src/mesa/math/m_debug_norm.c"
  "${MESA}/src/mesa/math/m_debug_xform.c"
  "${MESA}/src/mesa/math/m_eval.c"
  "${MESA}/src/mesa/math/m_matrix.c"
  "${MESA}/src/mesa/math/m_translate.c"
  "${MESA}/src/mesa/math/m_vector.c"
  "${MESA}/src/mesa/math/m_xform.c"
  "${MESA}/src/mesa/shader/arbprogparse.c"
  "${MESA}/src/mesa/shader/arbprogram.c"
  "${MESA}/src/mesa/shader/atifragshader.c"
  "${MESA}/src/mesa/shader/grammar/grammar_mesa.c"
  "${MESA}/src/mesa/shader/nvfragparse.c"
  "${MESA}/src/mesa/shader/nvprogram.c"
  "${MESA}/src/mesa/shader/nvvertexec.c"
  "${MESA}/src/mesa/shader/nvvertparse.c"
  "${MESA}/src/mesa/shader/program.c"
  "${MESA}/src/mesa/shader/shaderobjects.c"
  "${MESA}/src/mesa/shader/shaderobjects_3dlabs.c"
  "${MESA}/src/mesa/shader/slang/slang_analyse.c"
  "${MESA}/src/mesa/shader/slang/slang_assemble.c"
  "${MESA}/src/mesa/shader/slang/slang_assemble_assignment.c"
  "${MESA}/src/mesa/shader/slang/slang_assemble_conditional.c"
  "${MESA}/src/mesa/shader/slang/slang_assemble_constructor.c"
  "${MESA}/src/mesa/shader/slang/slang_assemble_typeinfo.c"
  "${MESA}/src/mesa/shader/slang/slang_compile.c"
  "${MESA}/src/mesa/shader/slang/slang_compile_function.c"
  "${MESA}/src/mesa/shader/slang/slang_compile_operation.c"
  "${MESA}/src/mesa/shader/slang/slang_compile_struct.c"
  "${MESA}/src/mesa/shader/slang/slang_compile_variable.c"
  "${MESA}/src/mesa/shader/slang/slang_execute.c"
  "${MESA}/src/mesa/shader/slang/slang_execute_x86.c"
  "${MESA}/src/mesa/shader/slang/slang_export.c"
  "${MESA}/src/mesa/shader/slang/slang_library_noise.c"
  "${MESA}/src/mesa/shader/slang/slang_library_texsample.c"
  "${MESA}/src/mesa/shader/slang/slang_link.c"
  "${MESA}/src/mesa/shader/slang/slang_preprocess.c"
  "${MESA}/src/mesa/shader/slang/slang_storage.c"
  "${MESA}/src/mesa/shader/slang/slang_utility.c"
  "${MESA}/src/mesa/swrast/s_aaline.c"
  "${MESA}/src/mesa/swrast/s_aatriangle.c"
  "${MESA}/src/mesa/swrast/s_accum.c"
  "${MESA}/src/mesa/swrast/s_alpha.c"
  "${MESA}/src/mesa/swrast/s_arbshader.c"
  "${MESA}/src/mesa/swrast/s_atifragshader.c"
  "${MESA}/src/mesa/swrast/s_bitmap.c"
  "${MESA}/src/mesa/swrast/s_blend.c"
  "${MESA}/src/mesa/swrast/s_blit.c"
  "${MESA}/src/mesa/swrast/s_buffers.c"
  "${MESA}/src/mesa/swrast/s_context.c"
  "${MESA}/src/mesa/swrast/s_copypix.c"
  "${MESA}/src/mesa/swrast/s_depth.c"
  "${MESA}/src/mesa/swrast/s_drawpix.c"
  "${MESA}/src/mesa/swrast/s_feedback.c"
  "${MESA}/src/mesa/swrast/s_fog.c"
  "${MESA}/src/mesa/swrast/s_imaging.c"
  "${MESA}/src/mesa/swrast/s_lines.c"
  "${MESA}/src/mesa/swrast/s_logic.c"
  "${MESA}/src/mesa/swrast/s_masking.c"
  "${MESA}/src/mesa/swrast/s_nvfragprog.c"
  "${MESA}/src/mesa/swrast/s_points.c"
  "${MESA}/src/mesa/swrast/s_readpix.c"
  "${MESA}/src/mesa/swrast/s_span.c"
  "${MESA}/src/mesa/swrast/s_stencil.c"
  "${MESA}/src/mesa/swrast/s_texcombine.c"
  "${MESA}/src/mesa/swrast/s_texfilter.c"
  "${MESA}/src/mesa/swrast/s_texstore.c"
  "${MESA}/src/mesa/swrast/s_triangle.c"
  "${MESA}/src/mesa/swrast/s_zoom.c"
  "${MESA}/src/mesa/swrast_setup/ss_context.c"
  "${MESA}/src/mesa/swrast_setup/ss_triangle.c"
  "${MESA}/src/mesa/tnl/t_array_api.c"
  "${MESA}/src/mesa/tnl/t_array_import.c"
  "${MESA}/src/mesa/tnl/t_context.c"
  "${MESA}/src/mesa/tnl/t_pipeline.c"
  "${MESA}/src/mesa/tnl/t_save_api.c"
  "${MESA}/src/mesa/tnl/t_save_loopback.c"
  "${MESA}/src/mesa/tnl/t_save_playback.c"
  "${MESA}/src/mesa/tnl/t_vb_arbprogram.c"
  "${MESA}/src/mesa/tnl/t_vb_arbshader.c"
  "${MESA}/src/mesa/tnl/t_vb_cull.c"
  "${MESA}/src/mesa/tnl/t_vb_fog.c"
  "${MESA}/src/mesa/tnl/t_vb_light.c"
  "${MESA}/src/mesa/tnl/t_vb_normals.c"
  "${MESA}/src/mesa/tnl/t_vb_points.c"
  "${MESA}/src/mesa/tnl/t_vb_program.c"
  "${MESA}/src/mesa/tnl/t_vb_render.c"
  "${MESA}/src/mesa/tnl/t_vb_texgen.c"
  "${MESA}/src/mesa/tnl/t_vb_texmat.c"
  "${MESA}/src/mesa/tnl/t_vb_vertex.c"
  "${MESA}/src/mesa/tnl/t_vertex.c"
  "${MESA}/src/mesa/tnl/t_vertex_generic.c"
  "${MESA}/src/mesa/tnl/t_vp_build.c"
  "${MESA}/src/mesa/tnl/t_vtx_api.c"
  "${MESA}/src/mesa/tnl/t_vtx_eval.c"
  "${MESA}/src/mesa/tnl/t_vtx_exec.c"
  "${MESA}/src/mesa/tnl/t_vtx_generic.c"
)

set(OPENRBX_MESA_GDI_SOURCES
  "${MESA}/src/mesa/drivers/common/driverfuncs.c"
  "${MESA}/src/mesa/drivers/windows/gdi/wgl.c"
  "${MESA}/src/mesa/drivers/windows/gdi/wmesa.c"
)

set(OPENRBX_MESA_OSMESA_SOURCES
  "${MESA}/src/mesa/drivers/common/driverfuncs.c"
  "${MESA}/src/mesa/drivers/osmesa/osmesa.c"
)

set(OPENRBX_MESA_GLU_SOURCES
  "${MESA}/src/glu/sgi/libnurbs/interface/bezierEval.cc"
  "${MESA}/src/glu/sgi/libnurbs/interface/bezierPatch.cc"
  "${MESA}/src/glu/sgi/libnurbs/interface/bezierPatchMesh.cc"
  "${MESA}/src/glu/sgi/libnurbs/interface/glcurveval.cc"
  "${MESA}/src/glu/sgi/libnurbs/interface/glinterface.cc"
  "${MESA}/src/glu/sgi/libnurbs/interface/glrenderer.cc"
  "${MESA}/src/glu/sgi/libnurbs/interface/glsurfeval.cc"
  "${MESA}/src/glu/sgi/libnurbs/interface/incurveeval.cc"
  "${MESA}/src/glu/sgi/libnurbs/interface/insurfeval.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/arc.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/arcsorter.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/arctess.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/backend.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/basiccrveval.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/basicsurfeval.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/bin.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/bufpool.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/cachingeval.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/ccw.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/coveandtiler.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/curve.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/curvelist.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/curvesub.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/dataTransform.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/displaylist.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/flist.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/flistsorter.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/hull.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/intersect.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/knotvector.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/mapdesc.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/mapdescv.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/maplist.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/mesher.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/monoTriangulationBackend.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/monotonizer.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/mycode.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/nurbsinterfac.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/nurbstess.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/patch.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/patchlist.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/quilt.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/reader.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/renderhints.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/slicer.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/sorter.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/splitarcs.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/subdivider.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/tobezier.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/trimline.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/trimregion.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/trimvertpool.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/uarray.cc"
  "${MESA}/src/glu/sgi/libnurbs/internals/varray.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/directedLine.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/gridWrap.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/monoChain.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/monoPolyPart.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/monoTriangulation.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/partitionX.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/partitionY.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/polyDBG.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/polyUtil.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/primitiveStream.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/quicksort.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/rectBlock.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/sampleComp.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/sampleCompBot.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/sampleCompRight.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/sampleCompTop.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/sampleMonoPoly.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/sampledLine.cc"
  "${MESA}/src/glu/sgi/libnurbs/nurbtess/searchTree.cc"
  "${MESA}/src/glu/sgi/libtess/dict.c"
  "${MESA}/src/glu/sgi/libtess/geom.c"
  "${MESA}/src/glu/sgi/libtess/memalloc.c"
  "${MESA}/src/glu/sgi/libtess/mesh.c"
  "${MESA}/src/glu/sgi/libtess/normal.c"
  "${MESA}/src/glu/sgi/libtess/priorityq.c"
  "${MESA}/src/glu/sgi/libtess/render.c"
  "${MESA}/src/glu/sgi/libtess/sweep.c"
  "${MESA}/src/glu/sgi/libtess/tess.c"
  "${MESA}/src/glu/sgi/libtess/tessmono.c"
  "${MESA}/src/glu/sgi/libutil/error.c"
  "${MESA}/src/glu/sgi/libutil/glue.c"
  "${MESA}/src/glu/sgi/libutil/mipmap.c"
  "${MESA}/src/glu/sgi/libutil/project.c"
  "${MESA}/src/glu/sgi/libutil/quad.c"
  "${MESA}/src/glu/sgi/libutil/registry.c"
)

# RelWithDebInfo links incrementally, which keeps .idata a section of its own. The three
# originals carry no .idata, so the Release configs linked without it.
set(OPENRBX_MESA_LINK_OPTIONS "/INCREMENTAL:NO")

# StringPooling and EnableFunctionLevelLinking off every Mesa vcproj. RelWithDebInfo already
# carries the /O2 /Ob1 that InlineFunctionExpansion="1" asks for, but neither of these, and
# without /Gy the linker has no COMDATs to fold.
set(OPENRBX_MESA_COMPILE_OPTIONS "/GF" "/Gy")

# The static core the three DLLs share. Each DLL exists for its import library.
add_library(mesa_core STATIC ${OPENRBX_MESA_CORE_SOURCES})
target_include_directories(mesa_core PRIVATE
  "${MESA}/include" "${MESA}/src/mesa" "${MESA}/src/mesa/glapi" "${MESA}/src/mesa/main"
  "${MESA}/src/mesa/shader" "${MESA}/src/mesa/shader/slang" "${MESA}/src/mesa/shader/grammar"
)
target_compile_definitions(mesa_core PRIVATE NDEBUG WIN32 _LIB _DLL BUILD_GL32 MESA_MINWARN)
target_compile_options(mesa_core PRIVATE ${OPENRBX_MESA_COMPILE_OPTIONS})

# The installer ships OPENGL32.DLL, GLU32.DLL and OSMESA32.DLL, and each vcproj names its
# OutputFile that way. The case reaches the import table of every dependent, WebService.dll
# included, so it is part of the comparison rather than a cosmetic detail.
add_library(opengl32 SHARED ${OPENRBX_MESA_GDI_SOURCES})
set_target_properties(opengl32 PROPERTIES OUTPUT_NAME "OPENGL32" SUFFIX ".DLL")
target_include_directories(opengl32 PRIVATE
  "${MESA}/windows/VC7/main" "${MESA}/include" "${MESA}/src/mesa" "${MESA}/src/mesa/main"
  "${MESA}/src/mesa/glapi" "${MESA}/src/mesa/swrast" "${MESA}/src/mesa/shader"
)
target_compile_definitions(opengl32 PRIVATE
  NDEBUG WIN32 _WINDOWS _USRDLL GDI_EXPORTS _DLL BUILD_GL32 MESA_MINWARN)
target_compile_options(opengl32 PRIVATE ${OPENRBX_MESA_COMPILE_OPTIONS})
target_link_libraries(opengl32 PRIVATE mesa_core winmm kernel32 user32 gdi32 advapi32)
target_link_options(opengl32 PRIVATE ${OPENRBX_MESA_LINK_OPTIONS}
  "/DEF:${MESA}/src/mesa/drivers/windows/gdi/mesa.def")

# osmesa's project defines neither BUILD_GL32 nor _DLL, so GLAPI is plain extern and every GL
# entry point resolves out of OPENGL32.DLL instead of being compiled in again. That is what
# keeps the original at 31,232 bytes with nine exports. RuntimeLibrary=0 is what leaves _DLL
# undefined; the explicit msvcrt.lib then still binds the CRT to MSVCR80.dll.
add_library(osmesa32 SHARED ${OPENRBX_MESA_OSMESA_SOURCES})
set_target_properties(osmesa32 PROPERTIES OUTPUT_NAME "OSMESA32" SUFFIX ".DLL"
  MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_include_directories(osmesa32 PRIVATE
  "${MESA}/include" "${MESA}/src/mesa" "${MESA}/src/mesa/main" "${MESA}/src/mesa/glapi"
  "${MESA}/src/mesa/swrast" "${MESA}/src/mesa/shader"
)
target_compile_definitions(osmesa32 PRIVATE
  WIN32 NDEBUG _WINDOWS _USRDLL OSMESA_EXPORTS)
target_compile_options(osmesa32 PRIVATE ${OPENRBX_MESA_COMPILE_OPTIONS})
target_link_libraries(osmesa32 PRIVATE opengl32 winmm msvcrt kernel32 user32 gdi32 advapi32)
target_link_options(osmesa32 PRIVATE ${OPENRBX_MESA_LINK_OPTIONS}
  "/DEF:${MESA}/src/mesa/drivers/osmesa/osmesa.def")

add_library(glu32 SHARED ${OPENRBX_MESA_GLU_SOURCES})
set_target_properties(glu32 PROPERTIES OUTPUT_NAME "GLU32" SUFFIX ".DLL")
target_include_directories(glu32 PRIVATE
  "${MESA}/include" "${MESA}/src/glu/sgi/include" "${MESA}/src/glu/sgi/libnurbs/interface"
  "${MESA}/src/glu/sgi/libnurbs/internals" "${MESA}/src/glu/sgi/libnurbs/nurbtess"
)
target_compile_definitions(glu32 PRIVATE
  WIN32 NDEBUG _WINDOWS _USRDLL GLU_EXPORTS BUILD_GL32 LIBRARYBUILD)
target_compile_options(glu32 PRIVATE ${OPENRBX_MESA_COMPILE_OPTIONS}
  # glu is the only C++ target here and its vcproj omits ExceptionHandling, which VC7 defaults
  # off where CMake passes /EHsc. RuntimeTypeInfo is omitted too and stays on regardless: /GR-
  # takes .data to 0x550 where the original is 0x6d8, four bytes under the /GR build.
  "/EHs-c-")
target_link_libraries(glu32 PRIVATE opengl32 winmm kernel32 user32 gdi32 advapi32)
target_link_options(glu32 PRIVATE ${OPENRBX_MESA_LINK_OPTIONS}
  "/DEF:${MESA}/src/glu/sgi/glu.def")

# None of it is ours.
foreach(name graphics3d glg3d boost_signals boost_thread mesa_core opengl32 osmesa32 glu32)
  set_target_properties(${name} PROPERTIES
    C_CLANG_TIDY "" CXX_CLANG_TIDY "" CXX_INCLUDE_WHAT_YOU_USE "")
endforeach()
