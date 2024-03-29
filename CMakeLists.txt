cmake_minimum_required(VERSION 3.10)

project(NuAnalysis)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO MINUIT2 MATHCORE)

if(ROOT_FOUND)
  message("ROOT found: ${ROOT_INCLUDE_DIRS}.")
else()
  message(FATAL_ERROR "ROOT not found.")
endif()

find_package(Eigen3 REQUIRED CONFIG)
if(Eigen3_FOUND)
  message("Eigen3 found.")
else()
  message(FATAL_ERROR "Eigen3 not found.")
endif()

get_target_property(inc_dir Eigen3::Eigen INTERFACE_INCLUDE_DIRECTORIES)
message("[DEBUG] inc_dir: ${inc_dir}")
set(EIGEN_FAST_MATH 0)

include_directories(${ROOT_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)
list(REMOVE_ITEM sources ${PROJECT_SOURCE_DIR}/src/optimize.cpp)
list(REMOVE_ITEM headers ${PROJECT_SOURCE_DIR}/include/optimize.h)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_executable(NuAnalysis analysis.cpp ${sources} ${headers})
target_link_libraries(NuAnalysis ${ROOT_LIBRARIES})
root_generate_dictionary(obj_dict event.h neutrino.h imatch.h pmatch.h interaction.h particle.h crthit.h fmatch.h dataset.h MODULE NuAnalysis LINKDEF include/linkdef.h)
add_library(Event SHARED src/event.cpp src/neutrino.cpp src/pmatch.cpp src/imatch.cpp src/interaction.cpp src/particle.cpp src/crthit.cpp src/fmatch.cpp src/dataset.cpp src/csv_parser.cpp src/utilities.cpp obj_dict)
target_link_libraries(Event ${ROOT_LIBRARIES})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)
add_executable(Reweight reweight.cpp ${sources} ${headers})
target_link_libraries(Reweight ${ROOT_LIBRARIES} Event)

add_executable(Sensitivity sensitivity.cpp)
target_link_libraries(Sensitivity ${ROOT_LIBRARIES} Event Eigen3::Eigen)
