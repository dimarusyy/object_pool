include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

FetchContent_Declare(
    googletest-git
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG         origin/master
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest-git)