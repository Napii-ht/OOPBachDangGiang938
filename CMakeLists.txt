cmake_minimum_required(VERSION 3.28)
project(BachDang938 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tìm SFML 3 đã cài sẵn. Nếu không có thì tự tải SFML 3.1.0 từ GitHub (cần mạng, build lần đầu hơi lâu).
find_package(SFML 3 COMPONENTS Graphics Audio QUIET)
if(NOT SFML_FOUND)
    include(FetchContent)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.1.0
        GIT_SHALLOW ON)
    FetchContent_MakeAvailable(SFML)
endif()

file(GLOB_RECURSE NGUON CONFIGURE_DEPENDS "*.cpp" "*.h")
list(FILTER NGUON EXCLUDE REGEX "/(build|out|cmake-build[^/]*)/")

add_executable(BachDang938 ${NGUON})
target_link_libraries(BachDang938 PRIVATE SFML::Graphics SFML::Audio)

# Chuỗi tiếng Việt trong code là UTF-8, MSVC cần cờ này
if(MSVC)
    target_compile_options(BachDang938 PRIVATE /utf-8 /W3)
else()
    target_compile_options(BachDang938 PRIVATE -Wall -Wextra)
endif()

# Chạy từ Visual Studio thì lấy thư mục dự án làm thư mục làm việc để tìm thấy TaiNguyen/
set_target_properties(BachDang938 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Chép TaiNguyen cạnh file chạy sau mỗi lần build
add_custom_command(TARGET BachDang938 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/TaiNguyen" "$<TARGET_FILE_DIR:BachDang938>/TaiNguyen")
