#!/bin/bash

g++ -o out main.cpp glad.c -lglfw -lGL -lm -lXrandr -lX11 -lpthread -ldl
