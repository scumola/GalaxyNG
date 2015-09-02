#if !defined(INFLUENCE_H_)
#define INFLUENCE_H_

#include "galaxy.h"
#include "list.h"
#include "util.h"

/* this is to enable the current map to be created as an image */
#include <gd.h>
#include <avl.h>

enum map_type {
    AllMaps = -1,
    FirstMap=0,
    EffIndMap=0,
    ShipAttMap,
    ShipDefMap,
    LocationMap,
    PublicMap,
    NbrMaps
};

#endif
