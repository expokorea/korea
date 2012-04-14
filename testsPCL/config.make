# add custom variables to this file

# OF_ROOT allows to move projects outside apps/* just set this variable to the
# absoulte path to the OF root folder

OF_ROOT = ../../..


# USER_CFLAGS allows to pass custom flags to the compiler
# for example search paths like:
# USER_CFLAGS = -I src/objects

USER_CFLAGS = -I ../common/src/kinect $(shell pkg-config pcl_common-1.5 pcl_surface-1.5 pcl_filters-1.5 openni-dev eigen3 --cflags)


# USER_LDFLAGS allows to pass custom flags to the linker
# for example libraries like:
# USER_LD_FLAGS = libs/libawesomelib.a

USER_LDFLAGS = -lusb-1.0 -fopenmp


# use this to add system libraries for example:
# USER_LIBS = -lpango
 
USER_LIBS = $(shell pkg-config avahi-client --libs) -lusb-1.0 -lpcl_surface -lpcl_registration -lpcl_sample_consensus -lpcl_features -lm -lpcl_kdtree -lpcl_filters -lpcl_search -lflann_cpp -lpcl_common -lpcl_io -lOpenNI -lpcl_octree -lpcl_segmentation


# change this to add different compiler optimizations to your project

USER_COMPILER_OPTIMIZATION = -march=native -mtune=native -Os -fopenmp


EXCLUDE_FROM_SOURCE="bin,.xcodeproj,obj,.git"

USER_SOURCE_DIR = ../common/src/kinect