set(LIB_NAME dmvm_lib)

file(GLOB SRCFILES src/*.c)
file(GLOB HFILES include/*.h)

add_library(${LIB_NAME} ${SRCFILES} ${HFILES})
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)