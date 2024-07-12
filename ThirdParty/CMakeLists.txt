cmake_minimum_required(VERSION 3.6)
project(thirdParty)

add_library(${PROJECT_NAME} STATIC empty.cpp)

#后续三方库都可以在这边统一链接
include_directories(${LAYA_ROOT_PATH}/ThirdParty/ohos-specific/aki/include)
# ArkTs FFI framework, refer to: https://gitee.com/openharmony-sig/aki/tree/master
add_subdirectory(ohos-specific/aki)
target_link_libraries(${PROJECT_NAME} 
    aki_jsbind
)

#    ------------------------------ V8
add_library(v8_monolith STATIC IMPORTED GLOBAL)
set_target_properties(v8_monolith PROPERTIES
  IMPORTED_LOCATION ${LAYA_ROOT_PATH}/ThirdParty/v8_9.1/lib/ohos/libv8_monolith.a
  INTERFACE_INCLUDE_DIRECTORIES ${LAYA_ROOT_PATH}/ThirdParty/v8_9.1/include
)

target_link_libraries(${PROJECT_NAME}
    ${LAYA_ROOT_PATH}/ThirdParty/curl/lib/ohos/libcurl.a
    ${LAYA_ROOT_PATH}/ThirdParty/websockets/lib/ohos/libwebsockets.a
    ${LAYA_ROOT_PATH}/ThirdParty/jpeg-turbo/lib/ohos/libjpeg.a
    ${LAYA_ROOT_PATH}/ThirdParty/jpeg-turbo/lib/ohos/libturbojpeg.a
    ${LAYA_ROOT_PATH}/ThirdParty/png/lib/ohos/libpng.a
    ${LAYA_ROOT_PATH}/ThirdParty/freetype/lib/ohos/libfreetype.a
    ${LAYA_ROOT_PATH}/ThirdParty/zip/lib/ohos/libzip.a
    ${LAYA_ROOT_PATH}/ThirdParty/ogg/lib/ohos/libvorbisfile.a
    ${LAYA_ROOT_PATH}/ThirdParty/ogg/lib/ohos/libvorbis.a
    ${LAYA_ROOT_PATH}/ThirdParty/ogg/lib/ohos/libogg.a
    ${LAYA_ROOT_PATH}/ThirdParty/openssl/lib/ohos/libssl.a
    ${LAYA_ROOT_PATH}/ThirdParty/openssl/lib/ohos/libcrypto.a
    ${LAYA_ROOT_PATH}/ThirdParty/bullet/lib/ohos/libBulletDynamics.a
    ${LAYA_ROOT_PATH}/ThirdParty/bullet/lib/ohos/libBulletCollision.a
    ${LAYA_ROOT_PATH}/ThirdParty/bullet/lib/ohos/libLinearMath.a
    ${LAYA_ROOT_PATH}/ThirdParty/zlib/lib/ohos/libz.a
    )

 target_include_directories(${PROJECT_NAME} PUBLIC 
    ${LAYA_ROOT_PATH}/ThirdParty/curl/include/ohos
    ${LAYA_ROOT_PATH}/ThirdParty/png/include/ohos
    ${LAYA_ROOT_PATH}/ThirdParty/jpeg-turbo/include/ohos
    ${LAYA_ROOT_PATH}/ThirdParty/freetype/include/ohos
    ${LAYA_ROOT_PATH}/ThirdParty/zip/include/ohos
    ${LAYA_ROOT_PATH}/ThirdParty/v8_9.1/include
    ${LAYA_ROOT_PATH}/ThirdParty/ogg/include/ohos
    ${LAYA_ROOT_PATH}/ThirdParty/websockets/include/ohos
    ${LAYA_ROOT_PATH}/ThirdParty/openssl/include/ohos
    ${LAYA_ROOT_PATH}/ThirdParty/openal/include/ohos
    ${LAYA_ROOT_PATH}/ThirdParty/bullet/include/ohos/bullet
)

# put "thirdParty" into External folder, too
set_target_properties(${PROJECT_NAME} 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    FOLDER "ThirdParty"
)