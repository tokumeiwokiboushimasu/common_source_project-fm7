cmake .. -DCMAKE_TOOLCHAIN_FILE="$PWD/../cmake/toolchains/toolchain_native_gcc11.cmake" \
      -DCMAKE_BUILD_TYPE=Relwithdebinfo \
      -DCMAKE_C_FLAGS_RELWITHDEBINFO=" \
      		-gz \
      		-ggdb \
		-O3 \
		-msse2 \
		-mfpmath=sse \
		-flto \
		-flto-compression-level=9 \
		-ffat-lto-objects \
		-Wa,--compress-debug-sections=zlib \
		" \
      -DCMAKE_CXX_FLAGS_RELWITHDEBINFO=" \
      		-gz \
      		-ggdb \
		-O3 \
		-msse2 \
		-flto \
		-flto-compression-level=9 \
		-ffat-lto-objects \
		-Wa,--compress-debug-sections=zlib \
		" \
      -DCMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO="\
		-ggdb \
		-gz \
		-O3 \
		-flto=6 \
		-msse2 \
		-Wl,--compress-debug-sections=zlib \
		-Wa,--compress-debug-sections=zlib \
		" \
      -DCMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO="\
      		-ggdb \
		-gz \
		-O3 \
		-flto=6 \
		-msse2 \
		-Wl,--compress-debug-sections=zlib \
		-Wa,--compress-debug-sections=zlib \
		" \
	-DCSP_BUILD_WITH_CXX20=ON
