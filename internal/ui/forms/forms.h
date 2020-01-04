//
// Created by nik on 1/1/20.
//

#ifndef IT350_PZ_APP_FORMS_H
#define IT350_PZ_APP_FORMS_H

#pragma once

#include <ncurses.h>
#include <limits.h>
#include <form.h>

#include "ui/input.h"
// #include "ui/views/form.h"
#include "ui/state.h"


#define DBORDER(win) wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);


void region_form_construct(state_t* state);

void municipality_form_construct(state_t* state);

void person_form_construct(state_t* state);

void address_form_construct(state_t* state);

#endif //IT350_PZ_APP_FORMS_H
