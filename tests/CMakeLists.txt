set(TESTS
    mountwatcher
    pipes
    signals
)

foreach(TEST ${TESTS})
    add_executable(${TEST}_ctest ${TEST}_ctest.cpp)
    target_link_libraries(${TEST}_ctest PRIVATE mmolch::mountwatcher)
    add_test(${TEST} ${TEST}_ctest)
endforeach()
