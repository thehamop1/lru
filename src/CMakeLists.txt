set(LRU_SRCS
    LRU.cpp
)

set(MAIN_SRCS
    main.cpp
)

add_library(LRU ${LRU_SRCS})

add_executable(MAIN ${MAIN_SRCS})
target_link_libraries(MAIN LRU)