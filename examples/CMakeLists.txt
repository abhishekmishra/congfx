set(CONGFX_EXAMPLES
    ex0_point
    ex1_fill_lines
    ex2_bubble
    ex3_bubbles
    ex4_echo_keys
    ex5_bouncing_balls
    ex6_move_block
    ex7_noise
)

foreach(example IN LISTS CONGFX_EXAMPLES)
    add_executable(${example} ${example}.c)
    target_include_directories(${example} PRIVATE ${PROJECT_SOURCE_DIR})
    target_compile_features(${example} PRIVATE c_std_99)
endforeach()
