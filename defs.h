/*
 * Eazy Engine
 * File: defs.h
 * Author: Evan Ezell
 *
 * Useful definitions.
 */

/* bool type */
#define BOOL        int
#define TRUE          1
#define FALSE         0

/* piece colors */
#define WHITE         0
#define BLACK         1
#define EMPTY         6

/* piece types */
#define PAWN          0
#define KNIGHT        1
#define BISHOP        2
#define ROOK          3
#define QUEEN         4
#define KING          5

/* index to col/row calculation */
#define COL(i)        (i & 7)
#define ROW(i)        (7 - (i >> 3))

/* type flags */
#define CAPTURE       1
#define CASTLE        2
#define EP_CAPTURE    4
#define COFFEE_JUMP   8
#define PAWN_MOVE    16
#define PROMOTE      32

/* chess squares */
#define A8            0
#define B8            1
#define C8            2
#define D8            3
#define E8            4
#define F8            5
#define G8            6
#define H8            7
#define A1           56
#define B1           57
#define C1           58
#define D1           59
#define E1           60
#define F1           61
#define G1           62
#define H1           63

typedef struct {
    char from; 
    char to;
    char promote;
    char type;
} move;

typedef struct {
    move m;
    int capture;
    int ep; 
    int fifty;
    char castle;
} past_move;
