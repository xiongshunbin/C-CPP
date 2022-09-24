// coordin.h -- structure templates and function prototypes
// structure template
#ifndef __COORDIN_H__
#define __COORDIN_H__

struct polar
{
	double distance;	// distance from origin 
	double angle;		// direction from origin
};

struct rect
{
	double x;			// horizontal diatance from origin
	double y;			// vertical diatance from origin
};

// prototypes
polar rect_to_polar(rect xypos);
void show_polar(polar dapos);

#endif
