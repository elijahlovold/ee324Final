#pragma once

#include "xil_types.h"
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <vector>

#define MAX_SPRITE_ADDR 45

#define X_RES 1920
#define Y_RES 1080
#define SPRITE_RES 40

#define MAX_X_COORDS X_RES/SPRITE_RES
#define MAX_Y_COORDS Y_RES/SPRITE_RES

#define GRID_CONTROLLER_BASE_ADDR 0x4123_0000
