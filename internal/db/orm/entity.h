//
// Created by nik on 12/21/19.
//

#ifndef IT350_PZ_APP_ENTITY_H
#define IT350_PZ_APP_ENTITY_H

#pragma once


#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#include "db/dbc.h"
#include "db/orm/region.h"
#include "db/orm/municipality.h"
#include "db/orm/address.h"
#include "db/orm/library.h"

#define _cleanup(func) __attribute__((cleanup(func)))

#endif //IT350_PZ_APP_ENTITY_H
