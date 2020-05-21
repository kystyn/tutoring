project(sort LANGUAGES C)

cmake_minimum_required(VERSION 3.8)

add_library(${PROJECT_NAME} SHARED draft.c)
