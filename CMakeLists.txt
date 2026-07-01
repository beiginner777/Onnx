cmake_minimum_required(VERSION 3.16)
project(ONNXInferenceServer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- FetchContent ----
include(FetchContent)

# asio (standalone, header-only)
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG        asio-1-30-2
)
FetchContent_GetProperties(asio)
if(NOT asio_POPULATED)
    FetchContent_Populate(asio)
endif()
# asio is header-only; create interface target manually
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)

# spdlog (fast C++ logging, header-only)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.13.0
)
FetchContent_MakeAvailable(spdlog)

# nlohmann/json (JSON for Modern C++, header-only)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(json)

# ---- ONNX Runtime (Day 3: integrate) ----
# option(USE_ONNXRUNTIME "Enable ONNX Runtime" OFF)
# if(USE_ONNXRUNTIME)
#     find_package(onnxruntime REQUIRED)
# endif()

# ---- Sources ----
set(SOURCES
    src/main.cpp
    src/ConfigManager.cpp
    src/HttpServer.cpp
    src/InferManager.cpp
    src/ModelManager.cpp
    src/BatchProcessor.cpp
    src/MetricsManager.cpp
)

set(HEADERS
    src/ConfigManager.h
    src/HttpServer.h
    src/InferManager.h
    src/ModelManager.h
    src/BatchProcessor.h
    src/MetricsManager.h
)

add_executable(onnx_server ${SOURCES} ${HEADERS})

target_include_directories(onnx_server PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(onnx_server PRIVATE
    asio
    spdlog::spdlog_header_only
    nlohmann_json::nlohmann_json
)

# ONNX Runtime linkage (Day 3)
# if(USE_ONNXRUNTIME)
#     target_link_libraries(onnx_server PRIVATE onnxruntime::onnxruntime)
#     target_compile_definitions(onnx_server PRIVATE HAS_ONNXRUNTIME)
# endif()

# ---- Platform-specific ----
if(WIN32)
    target_link_libraries(onnx_server PRIVATE ws2_32 wsock32)
    target_compile_definitions(onnx_server PRIVATE _WIN32_WINNT=0x0A00)
    target_compile_options(onnx_server PRIVATE /utf-8)
endif()

if(UNIX)
    target_link_libraries(onnx_server PRIVATE pthread)
endif()

# ---- Tests (Day 5) ----
# enable_testing()
# find_package(GTest REQUIRED)
# add_subdirectory(test)

# ---- Install ----
install(TARGETS onnx_server RUNTIME DESTINATION bin)
install(FILES config.ini DESTINATION etc)
