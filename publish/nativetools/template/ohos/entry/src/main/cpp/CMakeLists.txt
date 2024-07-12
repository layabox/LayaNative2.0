cmake_minimum_required(VERSION 3.6)
project(laya)

#set(USE_CMAKE_BUILD_FROM_SOURCE ON)
set(USE_CMAKE_BUILD_FROM_SOURCE OFF)

if(USE_CMAKE_BUILD_FROM_SOURCE)
    # 根目录
    set(NATIVER_RENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../..)
    add_definitions(-DOHOS)

    # sys - cpp
    file(GLOB_RECURSE source_files
        ${CMAKE_CURRENT_SOURCE_DIR}/napi_init.cpp
    )
    add_library(${PROJECT_NAME} SHARED ${source_files})

    find_library( # Sets the name of the path variable.
        libnapi-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        ace_napi.z)

    include_directories(${NATIVER_RENDER_ROOT_PATH}/Conch/source/)

    add_subdirectory(${NATIVER_RENDER_ROOT_PATH}/Conch/build/conch/proj.ohos layaair)
    target_link_libraries(${PROJECT_NAME} PUBLIC layaair ${libnapi-lib})
else()
    set(LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/layaair/libs/${OHOS_ARCH}")

    add_library(layaair SHARED IMPORTED)
    set_target_properties(layaair
        PROPERTIES
        IMPORTED_LOCATION ${LIBRARY_DIR}/liblayaair.so)

    add_definitions(-DOHOS)

    # sys - cpp
    file(GLOB_RECURSE source_files
        ${CMAKE_CURRENT_SOURCE_DIR}/napi_init.cpp
    )
    add_library(${PROJECT_NAME} SHARED ${source_files})

    find_library( # Sets the name of the path variable.
        libnapi-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        ace_napi.z)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        layaair
        ${libnapi-lib})
endif()
