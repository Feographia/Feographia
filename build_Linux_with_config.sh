${CMAKE_CMD} \
    -H./ \
    -B./build_wx \
\
    -DSUPRESS_VERBOSE_OUTPUT=OFF \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DCMAKE_COLOR_MAKEFILE=ON \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=inst \
\
    -DBUILD_TESTING=ON \
\
    && ${CMAKE_CMD} --build ./build_wx -- -j1 && ${CMAKE_CMD} -E env CTEST_OUTPUT_ON_FAILURE=1 ${CMAKE_CMD} --build ./build_wx --target test

#    && ${CMAKE_CMD} --build ./build_wx -- -j1
