macro(package_add_test TESTNAME SRCS LIB)
    add_executable(${TESTNAME} ${SRCS})
    target_link_libraries(${TESTNAME} gtest gmock gtest_main ${LIB})
    target_include_directories(${TESTNAME} PRIVATE ${CMAKE_SOURCE_DIR}/googletest)
    gtest_discover_tests(${TESTNAME}
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

package_add_test(LRUTEST LRUTest.cpp LRU)