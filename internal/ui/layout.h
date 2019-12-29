//
// Created by nik on 12/29/19.
//

#ifndef IT350_PZ_APP_LAYOUT_H
#define IT350_PZ_APP_LAYOUT_H

#pragma once

#define DBORDER(win) wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

#include "ui/state.h"
#include "ui/format.h"
#include "ui/util.h"

void print_list(state_t* state);

void change_list(state_t* state, int inc);

void print_footer(state_t* state);

#endif //IT350_PZ_APP_LAYOUT_H
