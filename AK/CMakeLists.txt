set(SRC
        User.cpp
        User.h
        IO.cpp
        IO.h)

add_library(AK ${SRC})
target_include_directories(AK PUBLIC .)