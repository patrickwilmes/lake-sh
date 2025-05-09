set(SRC
        User.cpp
        User.h
        IO.cpp
        IO.h
        Utils.cpp
        Utils.hpp
)

add_library(AK ${SRC})
target_include_directories(AK PUBLIC .)