#ifndef __ROBOT_H__
#define __ROBOT_H__

//All possible moves the robot can perform.
typedef enum {
    MOVE_L,
    MOVE_R,
    MOVE_D,
    MOVE_U
} Moves;


//Returns a copy of the given move.
void *move_copy(void *move);

//Destroys the given move.
void move_destroy(void *move);

#endif /* __ROBOT_H__ */