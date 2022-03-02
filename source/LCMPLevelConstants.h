//
//  JSLevelConstants.h
//  JSONDemo
//
//  This file contains all of the details that define the JSON structure of a LevelModel.
//  Created by Walker White on 1/20/16.
//
//  Author: Walker White
//  Version: 1/20/16
//
#ifndef __LCMP_LEVEL_CONSTANTS_H__
#define __LCMP_LEVEL_CONSTANTS_H__

/** The global fields of the level model */
#define LAYERS_FIELD         "layers"
#define WIDTH_FIELD         "width"
#define HEIGHT_FIELD        "height"
#define T_WIDTH_FIELD         "tilewidth"
#define T_HEIGHT_FIELD        "tileheight"
#define X_FIELD         "x"
#define Y_FIELD        "y"
#define ROTATION_FIELD        "rotation"
#define TILES_FIELD        0
#define OBJECTS_FIELD        1
#define THIEFSPAWN_FIELD        2
#define COPSPAWN_FIELD        3

#define WALLS_FIELD        "data"
#define OBSTACLES_FIELD        "objects"

#define ELLIPSE_FIELD        "ellipse"
#define POLYGON_FIELD        "polygon"


/** The physics fields for each object */
#define POSITION_FIELD      "pos"
#define SIZE_FIELD          "size"
#define BODYTYPE_FIELD      "bodytype"
#define DENSITY_FIELD       "density"
#define FRICTION_FIELD      "friction"
#define RESTITUTION_FIELD   "restitution"
#define DAMPING_FIELD       "damping"
#define ROTATION_FIELD      "rotation"
#define STATIC_VALUE        "static"

/** The drawing fields for each object */
#define TEXTURE_FIELD       "texture"
#define DEBUG_COLOR_FIELD   "debugcolor"
#define DEBUG_OPACITY_FIELD "debugopacity"

/** The source for our level file */
#define LEVEL_ONE_FILE      "json/testmall.json"
/** The key for our loaded level */
#define LEVEL_ONE_KEY       "testmall"

#define WALL_TEXTURE_KEY       "wall"
#define OBSTACLE_TEXTURE_KEY    "obstacle"


#endif /* defined(__LCMP_LEVEL_CONSTANTS_H__) */
