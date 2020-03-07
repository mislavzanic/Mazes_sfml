add_executable("main" main.cpp MazeCell.cpp Maze.cpp)
target_link_libraries("main" ${SFML_LIBRARIES})
target_compile_features(main PRIVATE cxx_range_for)

#add_custom_command(
#    TARGET main POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy
#            ${CMAKE_SOURCE_DIR}/data/*
#            ${CMAKE_CURRENT_BINARY_DIR} )
