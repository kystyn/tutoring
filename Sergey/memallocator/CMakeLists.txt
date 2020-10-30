project(memallocator)

cmake_minimum_required(VERSION 3.8)

add_executable(${PROJECT_NAME} memallocator.c memallocator.h test.c)

