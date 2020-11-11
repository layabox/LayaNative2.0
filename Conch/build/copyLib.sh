#!/bin/sh
echo ${CONFIGURATION_BUILD_DIR}
echo ${EFFECTIVE_PLATFORM_NAME}
echo ${PRODUCT_NAME}
SOURCE_PATH=${CONFIGURATION_BUILD_DIR}/lib${PRODUCT_NAME}.a
echo $SOURCE_PATH
DES_PATH=${SRCROOT}
echo $DES_PATH
if [ ${EFFECTIVE_PLATFORM_NAME} = "-iphoneos" ];then
cp -r $SOURCE_PATH $DES_PATH/../../../libs/ios
else
cp -r $SOURCE_PATH $DES_PATH/../../../libs/ios-sim
fi
