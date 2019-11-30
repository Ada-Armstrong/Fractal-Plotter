cmake_minimum_required(VERSION 3.8)

SET(PROJECT_NAME fractal)

project(${PROJECT_NAME} LANGUAGES C)

SET(AVX_FLAG "-mavx")
add_definitions(${AVX_FLAG})

SET(SOURCES main.c
	tokenize.c
	frame.c
	arg.c
	ppm.c
	ppm.h
	arg.h
	frame.h
	tokenize.h)

add_executable(${PROJECT_NAME} ${SOURCES})
