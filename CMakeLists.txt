cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
project(databaseApp)

set(SOURCES src/main.cpp src/mainwindow.cpp src/cardsTableWidget.cpp src/leftMenu.cpp)
set(HEADERS include/mainwindow.h include/cardsTableWidget.h include/leftMenu.h include/styles.h)

set(CMAKE_PREFIX_PATH "C:/Qt/6.5.1")
find_package(Qt6 COMPONENTS Widgets REQUIRED)


include_directories("C:/Program Files/libpqxx/include" "include")
link_directories("C:/Program Files/libpqxx/lib" "C:/Program Files/PostgreSQL/16/lib")


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} pqxxd libpq Ws2_32 Qt6::Widgets)