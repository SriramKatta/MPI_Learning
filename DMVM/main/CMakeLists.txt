add_executable(dmvm main.c)
target_link_libraries(dmvm PRIVATE dmvm_lib)
target_link_libraries(dmvm PRIVATE MPI::MPI_C)
target_compile_definitions(dmvm PRIVATE ARRAY_ALIGNMENT=64)