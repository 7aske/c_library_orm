//
// Created by nik on 12/29/19.
//

#ifndef IT350_PZ_APP_INPUT_H
#define IT350_PZ_APP_INPUT_H

#pragma once

#include "ui/state.h"
#include "ui/display.h"

void root_ctx_handler(state_t* state, int input, volatile int* running);

void list_ctx_handler(state_t* state, int input);

void form_ctx_handler(state_t* state, int input);

#endif //IT350_PZ_APP_INPUT_H