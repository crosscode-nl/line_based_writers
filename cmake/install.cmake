configure_file(cmake/package_config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}/${PACKAGE_NAME}Config.cmake @ONLY)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "include/${PROJECT_NAME}.h")

target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include> $<INSTALL_INTERFACE:include>)
target_include_directories(${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_include_directories(${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>)

install(DIRECTORY include/${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        FILES_MATCHING PATTERN "*.h")

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        FILES_MATCHING PATTERN "*.h")

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PACKAGE_NAME}Targets
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include)

include(CMakePackageConfigHelpers)

install(EXPORT ${PACKAGE_NAME}Targets
        NAMESPACE ${PACKAGE_NAME}::
        DESTINATION lib/cmake/${PACKAGE_NAME})

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}/${PACKAGE_NAME}ConfigVersion.cmake"
        VERSION ${Upstream_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}/${PACKAGE_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}/${PACKAGE_NAME}ConfigVersion.cmake"
        DESTINATION
        lib/cmake/${PACKAGE_NAME}
        COMPONENT
        Devel
)
