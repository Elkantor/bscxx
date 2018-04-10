#pragma once

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

#if defined(_WIN32)
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif