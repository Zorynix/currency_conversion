cmake_minimum_required(VERSION 3.12)
project(pg_service_template CXX)


# Adding userver dependency
find_package(userver COMPONENTS core postgresql chaotic REQUIRED)

file(GLOB_RECURSE SCHEMAS ${CMAKE_CURRENT_SOURCE_DIR}/schemas/*.yaml)
userver_target_generate_chaotic(${PROJECT_NAME}-chgen
    ARGS
        # Map '/components/schemas/*' JSONSchema types to C++ types in 'models::hello' namespace
        -n "/components/schemas/([^/]*)/=models::rates::{0}"
        -f "(.*)={0}"
        # Don't call clang-format
        --clang-format=clang-format
        # Generate serializers for responses
        --generate-serializers
    OUTPUT_DIR
        ${CMAKE_CURRENT_BINARY_DIR}/src
    SCHEMAS
        ${SCHEMAS}
    RELATIVE_TO
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
    src/hello.hpp
    src/hello.cpp
    src/components/rate_manager/component.hpp
    src/components/rate_manager/component.cpp
    src/components/rate_manager/secdist.hpp
    src/components/rate_manager/secdist.cpp
    src/handlers/currencies/handler.hpp
    src/handlers/currencies/handler.cpp
    src/handlers/rates/handler.hpp
    src/handlers/rates/handler.cpp
    src/handlers/update/handler.hpp
    src/handlers/update/handler.cpp

)
target_include_directories(${PROJECT_NAME}_objs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver::postgresql ${PROJECT_NAME}-chgen)


# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)



# Unit Tests
add_executable(${PROJECT_NAME}_unittest
    src/hello_test.cpp
)
target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver::utest)
add_google_tests(${PROJECT_NAME}_unittest)


# Benchmarks
add_executable(${PROJECT_NAME}_benchmark
    src/hello_benchmark.cpp
)
target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE ${PROJECT_NAME}_objs userver::ubench)
add_google_benchmark_tests(${PROJECT_NAME}_benchmark)


# Functional Tests
include(UserverTestsuite)

userver_testsuite_add_simple()


# Install
include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
  message(STATUS "Set install prefix: $ENV{PREFIX}")
  file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
  set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
