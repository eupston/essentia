project(essentia)

file(GLOB_RECURSE ESSENTIA_SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/essentia/*.cpp)
file(GLOB_RECURSE ESSENTIA_HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/essentia/*.h)

file(GLOB_RECURSE ESSENTIA_ALGO_HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/algorithms/*.h)
file(GLOB_RECURSE ESSENTIA_ALGO_SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/algorithms/*.cpp)

# Remove all source files that are not included in the Algorithms Registrar
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/essentia_algorithms_reg.cpp ALGO_REGISTRAR)
foreach(filepath ${ESSENTIA_ALGO_HEADER_FILES})
    get_filename_component(filename ${filepath} NAME_WE)
    string(FIND "${ALGO_REGISTRAR}" "${filename}" MATCHES)
    if(${MATCHES} EQUAL -1)
        list(REMOVE_ITEM ESSENTIA_ALGO_HEADER_FILES ${filepath})
    endif()
endforeach()

foreach(filepath ${ESSENTIA_ALGO_SOURCE_FILES})
    get_filename_component(filename ${filepath} NAME_WE)
    string(FIND "${ALGO_REGISTRAR}" "${filename}" MATCHES)
    if(${MATCHES} EQUAL -1)
        list(REMOVE_ITEM ESSENTIA_ALGO_SOURCE_FILES ${filepath})
    endif()
endforeach()

set(ESSENTIA_SRC
        ${ESSENTIA_SOURCE_FILES}
        ${ESSENTIA_HEADER_FILES}
        ${ESSENTIA_ALGO_HEADER_FILES}
        ${ESSENTIA_ALGO_SOURCE_FILES}
    )

# Add Registrar back to sources
list(APPEND ESSENTIA_SRC ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/essentia_algorithms_reg.cpp)

# Remove audio context if not specified
list(REMOVE_ITEM ESSENTIA_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/audiocontext.h
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/audiocontext.cpp
    )

# Remove Yaml if not specified
list(REMOVE_ITEM ESSENTIA_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/yamlast.h
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/yamlast.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/jsonconvert.h
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/jsonconvert.cpp
    )

# Remove extractor_freesound if not specified
list(REMOVE_ITEM ESSENTIA_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_freesound/extractor_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_freesound/FreesoundDescriptorsSet.h
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_freesound/FreesoundLowlevelDescriptors.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_freesound/FreesoundLowlevelDescriptors.h
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_freesound/FreesoundRhythmDescriptors.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_freesound/FreesoundRhythmDescriptors.h
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_freesound/FreesoundSfxDescriptors.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_freesound/FreesoundSfxDescriptors.h
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_freesound/FreesoundTonalDescriptors.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_freesound/FreesoundTonalDescriptors.h
        )

# Remove extractor_music if not specified
list(REMOVE_ITEM ESSENTIA_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_music/extractor_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_music/MusicDescriptorsSet.h
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_music/MusicLowlevelDescriptors.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_music/MusicLowlevelDescriptors.h
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_music/MusicRhythmDescriptors.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_music/MusicRhythmDescriptors.h
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_music/MusicTonalDescriptors.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_music/MusicTonalDescriptors.h
        ${CMAKE_CURRENT_SOURCE_DIR}/essentia/utils/extractor_music/tagwhitelist.h
        )

# Add nnls if we are compiling NNLSChroma
option(NNLSCHROMA "Whether to compile NnlsChroma Algo" ON)
if(NNLSCHROMA)
    list(APPEND ESSENTIA_SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/nnls/nnls.c
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/nnls/nnls.h
    )
endif()

# Add Spline if we are compiling Spline or CubicSpline
option(SPLINE "Whether to compile Spline Algo" ON)
if(SPLINE)
    list(APPEND ESSENTIA_SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/spline/splineutil.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/spline/splineutil.h
    )
endif()


# Add bessel if we are compiling SNR
option(SNR "Whether to compile SNR Algo" ON)
if(SNR)
    list(APPEND ESSENTIA_SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cephes/bessel/gamma.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cephes/bessel/hyperg.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cephes/bessel/iv.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cephes/bessel/polevl.cpp
            )
endif()

# https://gitlab.com/libeigen/eigen
add_definitions(-DEIGEN_PERMANENTLY_DISABLE_STUPID_WARNINGS)
add_definitions(-DEIGEN_MPL2_ONLY)
CPMAddPackage(
        NAME EIGEN3
        GITLAB_REPOSITORY libeigen/eigen
        GIT_TAG origin/master)

# https://github.com/libsndfile/libsamplerate
CPMAddPackage(
        NAME libsamplerate
        GITHUB_REPOSITORY libsndfile/libsamplerate
        GIT_TAG origin/master)

#https://github.com/FFTW/fftw3
#TODO compile fftw3 from source to use FFTW algos
#CPMAddPackage(
#        NAME FFTW3
#        GITHUB_REPOSITORY FFTW/fftw3
#        GIT_TAG origin/master)

#https://github.com/mborgerding/kissfft
set(KISSFFT_TEST OFF CACHE BOOL "" FORCE)
set(KISSFFT_TOOLS OFF CACHE BOOL "" FORCE)
CPMAddPackage(
        NAME KISS
        GITHUB_REPOSITORY mborgerding/kissfft
        GIT_TAG origin/master)

add_library(${PROJECT_NAME} ${ESSENTIA_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/essentia
        ${CMAKE_CURRENT_LIST_DIR}/essentia/streaming
        ${CMAKE_CURRENT_LIST_DIR}/essentia/scheduler
        ${CMAKE_CURRENT_LIST_DIR}/essentia/utils
        ${CMAKE_CURRENT_LIST_DIR}/essentia/utils/extractor_freesound
        ${CMAKE_CURRENT_LIST_DIR}/essentia/utils/extractor_music
        ${CMAKE_CURRENT_LIST_DIR}/essentia/utils/tnt
        ${CMAKE_CURRENT_LIST_DIR}/algorithms
    )

target_link_libraries(${PROJECT_NAME}
        Eigen3::Eigen
        samplerate
        kissfft
)
