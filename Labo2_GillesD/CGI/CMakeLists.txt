cmake_minimum_required(VERSION 2.6)
project(Labo2_GillesD)
add_executable(Labo2 cgi.c)

include_directories(/usr/include/mariadb /usr/include/mariadb/mysql)

target_link_libraries(Labo2 mariadb)