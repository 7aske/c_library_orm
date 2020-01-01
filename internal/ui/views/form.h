//
// Created by nik on 1/1/20.
//

#ifndef IT350_PZ_APP_FORM_H
#define IT350_PZ_APP_FORM_H

#pragma once

#include "ui/forms/region_form.h"


void form_add_data(state_t* state, void* data, size_t size);

state_t* form_set_type(state_t* state, form_type_e ftype);

#endif //IT350_PZ_APP_FORM_H
