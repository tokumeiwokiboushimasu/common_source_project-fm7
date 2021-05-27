#!/bin/sh
ARCH_TRIPLE=i686-w64-mingw32
LIBS_PREFIX="/usr/local/i586-mingw-msvc"

FFMPEG_DIR="${LIBS_PREFIX}/ffmpeg-4.3"
QT5_DIR="${LIBS_PREFIX}/Qt5.15/mingw_82x"

PATH=/opt/llvm-mingw-12/bin:$PATH
cmake .. \
	-DCMAKE_TOOLCHAIN_FILE="$PWD/../cmake/toolchains/toolchain_mingw_cross_llvm12.cmake" \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_C_FLAGS_RELWITHDEBINFO=" \
      		-g2 \
		-ggdb \
		-gz=zlib \
		-O3 \
		-march=i686 \
		-msse -msse2 \
		-mfpmath=sse \
 	        -Wreserved-user-defined-literal \
 	        -fslp-vectorize \
 	        -fvectorize \
		-fstrict-vtable-pointers \
		-fstrict-enums \
		" \
	-DCMAKE_CXX_FLAGS_RELWITHDEBINFO=" \
      		-g2 \
		-ggdb \
		-gz=zlib \
		-O3 \
		-march=i686 \
		-msse -msse2 \
		-mfpmath=sse \
 	        -Wreserved-user-defined-literal \
 	        -fslp-vectorize \
 	        -fvectorize \
		-fstrict-vtable-pointers \
		-fstrict-enums \
		" \
	-DCMAKE_C_FLAGS_RELEASE=" \
		-O3 \
		-march=i686 \
		-msse -msse2 \
		-mfpmath=sse \
 	        -Wreserved-user-defined-literal \
 	        -fslp-vectorize \
 	        -fvectorize \
		-fstrict-vtable-pointers \
		-fstrict-enums \
		" \
	-DCMAKE_CXX_FLAGS_RELEASE=" \
		-O3 \
		-march=i686 \
		-msse -msse2 \
		-mfpmath=sse \
 	        -Wreserved-user-defined-literal \
 	        -fslp-vectorize \
 	        -fvectorize \
		-fstrict-vtable-pointers \
		-fstrict-enums \
		" \
	-DCMAKE_EXE_LINKER_FLAGS_RELEASE="\
		" \
	-DCMAKE_MODULE_LINKER_FLAGS_RELEASE="\
		" \
	-DCMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO="\
      		-g2 \
		-ggdb \
		-gz=zlib \
		-L/usr/i686-w64-mingw32/lib \
		" \
	-DCMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO="\
      		-g2 \
		-ggdb \
		-gz=zlib \
		-L/usr/${ARCH_TRIPLE}/lib \
		" \
	-DLIBAV_ROOT_DIR="${FFMPEG_DIR}" \
	-DQT5_ROOT_PATH="${QT5_DIR}"	\
	-DTARGET_ARCH="${ARCH_TRIPLE}" \
	-DLIBS_PREFIX="${LIBS_PREFIX}" \
	-DUSE_DEVICES_SHARED_LIB=ON \
	
	