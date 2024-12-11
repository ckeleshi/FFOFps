function(create_library target_name)
    project(${target_name})

    include("${CMAKE_SOURCE_DIR}/cmake/VC-LTL helper for cmake.cmake")

    add_library(${target_name} SHARED
        ${target_name}.cpp
        ${target_name}.def
    )

    target_link_libraries(${target_name} PRIVATE common)
endfunction()