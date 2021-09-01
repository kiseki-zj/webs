find_package(PTHREAD)
set(SRC 
        src/threadpool.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_library(ws SHARED 
    ${SRC}
)
target_link_libraries(ws PRIVATE 
    Threads::Threads
)
target_include_directories(ws PUBLIC
    src
)
add_subdirectory(src/test)

add_executable(test 
    src/test/test.cpp
)

target_link_libraries(test PRIVATE
    ws
)