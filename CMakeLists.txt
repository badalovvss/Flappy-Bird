cmake_minimum_required(VERSION 3.15)
project(FlappyBirdClone CXX)

# C++ Standardını belirle
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML kütüphanesini bul (Graphics, Window ve System modülleri)
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Kaynak kodları projeye ekle
add_executable(${PROJECT_NAME} main.cpp)

# SFML kütüphanelerini çalıştırılabilir dosyaya bağla
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)