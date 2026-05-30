#pragma once

#include "common.h"

typedef struct {
    bool a;
    bool b;
    bool select;
    bool left;
    bool right;
    bool up;
    bool down;
} controls_t;

void controls_init();
void controls_poll(controls_t* controls);