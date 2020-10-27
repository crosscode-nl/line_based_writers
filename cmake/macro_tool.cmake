include(FetchContent)
FetchContent_Declare(
        macro_tool
        GIT_REPOSITORY https://github.com/crosscode-nl/macro_tool
        GIT_TAG v1.3.0
)
FetchContent_MakeAvailable(macro_tool)
target_link_libraries(${PROJECT_NAME} macro_tool)
