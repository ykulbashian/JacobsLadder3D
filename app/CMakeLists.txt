# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.

             src/main/cpp/SurfaceRenderer/surfacerenderer.cpp
             src/main/cpp/SurfaceRenderer/surfacerenderer_render.cpp
             src/main/cpp/SurfaceRenderer/jniapi.cpp
             src/main/cpp/SurfaceRenderer/logger.h
             src/main/cpp/SurfaceRenderer/native_handle.h
             src/main/cpp/SurfaceRenderer/surfacehandler.cpp
             src/main/cpp/SurfaceRenderer/surfaceutils.cpp

             # To use SurfaceRenderer by itself, delete entries
             # below this line  vvv

             src/main/cpp/jniapi.cpp
             src/main/cpp/wallpaper.cpp
             src/main/cpp/wallpapercreator.cpp
             src/main/cpp/Drawing/bufferbindings.cpp
             src/main/cpp/Drawing/drawing.cpp
             src/main/cpp/Drawing/objectbuffers.cpp
             src/main/cpp/Drawing/textures.cpp
             src/main/cpp/Geometry/cube.cpp
             src/main/cpp/Geometry/corner.h
             src/main/cpp/Geometry/dimension.h
             src/main/cpp/Geometry/position.h
             src/main/cpp/Drawing/ColorSets/colorset.cpp
             src/main/cpp/Drawing/ColorSets/facecolor.cpp
             src/main/cpp/Drawing/ColorSets/light.h
             src/main/cpp/Drawing/ColorSets/jacobsladder.cpp
             src/main/cpp/Drivers/cubedriver.cpp
             src/main/cpp/Drivers/cubedrivercreate.cpp
             src/main/cpp/Drivers/cubedrivertouch.cpp
             src/main/cpp/Drivers/Goals/goalcuboid.h
             src/main/cpp/Drivers/Goals/goalline.h
             src/main/cpp/Drivers/Goals/goalobject.cpp
             src/main/cpp/Drivers/Goals/goalplane.h
             src/main/cpp/Drivers/Goals/goalpointtype.h
             src/main/cpp/Drivers/Goals/goals.cpp
             src/main/cpp/Drivers/Grid/gridposition.cpp
             src/main/cpp/Drivers/Grid/gridalgorithm.cpp
             src/main/cpp/Drivers/Grid/gridcreate.cpp
             src/main/cpp/Drivers/Grid/gridcubes.cpp
             src/main/cpp/Drivers/Grid/gridutilities.cpp
             src/main/cpp/Stats/aggregatestats.h
             src/main/cpp/Stats/driverstats.h
             src/main/cpp/Stats/goalstats.h
             src/main/cpp/Stats/gridstats.h
             src/main/cpp/Stats/iterativecollectionsstats.h
             src/main/cpp/Stats/statssnapshot.cpp
             src/main/cpp/Utils/alliterativecollections.cpp
             src/main/cpp/Utils/iterativecollection.cpp
             src/main/cpp/Utils/timer.cpp
             src/main/cpp/Utils/utils.cpp
             src/main/cpp/TestSuite/cubedrivertest.cpp

             # Delete till here ^

             # src/main/cpp/Template/templatecreator.cpp
             # src/main/cpp/Template/templatewallpaper.cpp

             )

 # Specifies a path to native header files.
 include_directories(src/main/cpp/)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
