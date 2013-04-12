cmake_minimum_required(VERSION 2.8)
project(HYDE C CXX)

if(MSVC)
  add_definitions(/EHsc)
  add_definitions(/D_CRT_SECURE_NO_WARNINGS)
  add_definitions(/wd4244) # dear MSVC, im fully aware and confident about my type conversions. stop hammering me. kthx :s
endif(MSVC)

include_directories("deps/manymouse")

file(GLOB MANYMOUSE_SRCS "deps/manymouse/*.c")
add_library(manymouse-static STATIC ${MANYMOUSE_SRCS})

file(GLOB HYDE_SRCS *.cpp *.hpp *.inl)
file(GLOB HYDE_SAMPLES *.cc)
file(GLOB HYDE_TESTS *.cxx)
list(REMOVE_ITEM HYDE_SRCS "" ${HYDE_SAMPLES} ${HYDE_TESTS})
## list(APPEND HYDE_SRCS platform/w32/others.cpp)
add_library(hyde-static STATIC ${HYDE_SRCS})

add_executable(sample.keyboard              sample.keyboard.cc)
add_executable(sample.mouse                 sample.mouse.cc)
add_executable(sample.gamepad               sample.gamepad.cc)
add_executable(sample.policy                sample.policy.cc)

target_link_libraries(sample.keyboard       hyde-static manymouse-static)
target_link_libraries(sample.mouse          hyde-static manymouse-static)
target_link_libraries(sample.gamepad        hyde-static manymouse-static)
target_link_libraries(sample.policy         hyde-static manymouse-static)
