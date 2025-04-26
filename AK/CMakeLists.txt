set(SRC
        User.cpp
        User.h)

add_library(AK ${SRC})
target_include_directories(AK PUBLIC .)