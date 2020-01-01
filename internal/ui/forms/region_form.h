//
// Created by nik on 12/30/19.
//

#ifndef IT350_PZ_APP_REGION_FORM_H
#define IT350_PZ_APP_REGION_H

#pragma once

#include <ncurses.h>
#include <limits.h>
#include <form.h>

#include "ui/state.h"


#define DBORDER(win) wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);


void region_form_construct(state_t* state);

#endif //IT350_PZ_APP_REGION_FORM_H
