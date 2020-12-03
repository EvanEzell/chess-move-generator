#include <stdio.h>
#include "defs.h"
#include "data.h"
#include "protos.h"

long captures = 0;
long ep_captures = 0;
long promotions = 0;

BOOL checkboard() {
    int piece_count, color_count, i;

    piece_count = 0;
    color_count = 0;

    for (i = 0; i < 64; i++) {
        if (piece[i] != EMPTY) piece_count++;
        if (color[i] != EMPTY) color_count++;
    }

    return piece_count == color_count;
}

void print_board() {
    int i;

    printf("    ----------------\n");

    for (i = 0; i < 64; i++) {
        if ( i % 8 == 0 ) printf("%3d|", 8 - i / 8);
        switch (color[i]) {
            case WHITE:
                printf("%2c",piece_char[piece[i]]);
                break;
            case BLACK:
                printf("%2c",piece_char[piece[i]] + 'a' - 'A');
                break;
            case EMPTY:
                printf(" .");
                break;
        }
        if ( (i + 1) % 8 == 0) printf("|\n");
    }

    printf("    ----------------\n");
    printf("     A B C D E F G H\n");
}

/* initialize the board */
void init_board() {
    int i;

    possible_moves = 0;
    ply = 0;
    fifty = 0;
    castle = 15;
    ep = -1;
    side = WHITE;
    xside = BLACK;

    for (i = 0; i < 64; i++) {
        piece[i] = init_piece[i];
        color[i] = init_color[i];
    }
}

long push_promote(int from, int to, int type, int depth) {
    int i;
    move m;
    past_move pm;
    long count;

    count = 0;

    for (i = KNIGHT; i <= QUEEN; i++) {
        m.from = from;
        m.to = to;
        m.promote = i;
        m.type = type | PROMOTE;

        if (makemove(m)) {
            if (m.type & PROMOTE && depth == DEPTH) promotions++;
            if (m.type & CAPTURE && depth == DEPTH) captures++;
            if (depth == DEPTH) {
                undomove();
                count++;
            } else {
                pm = last_move;
                count += gen(depth+1);
                last_move = pm;
                undomove();
            }
        }
    }

    return count;
}

/* push move onto move stack */
long push_move(int from, int to, int type, int depth) {
    //printf("Push Move: (from = %d, to = %d, type = %d)\n", from, to, type);

    move m;
    past_move pm;
    long count;

    /* handle promotional move */
    if (type & PAWN_MOVE) {
        if (side == WHITE) {
            if (to <= H8) {
                return push_promote(from, to, type, depth);
            }
        } else { /* side is black */
            if (to >= A1) {
                return push_promote(from, to, type, depth);
            }
        }
    }

    m.from = from;
    m.to = to;
    m.type = type;

    if (makemove(m)) {
        if (m.type & CAPTURE && depth == DEPTH) { 
            captures++;
            if (m.type & EP_CAPTURE) ep_captures++;
        }
        if (depth == DEPTH) {
            undomove();
            return 1;
        } else {
            pm = last_move;
            count = gen(depth+1);
            last_move = pm;
            undomove();
            return count;
        }
    } else {
        return 0;
    }
}

/* generate all possible pseudo-legal moves and 
 * push them onto the move stack */
long gen(int depth) {
    int i, j, n;
    long count;

    count = 0;

    for(i = 0; i < 64; i++) {
        if (color[i] == side) {
            if (piece[i] == PAWN) {
                if (side == WHITE) {
                    if (COL(i) != 0 && color[i - 9] == BLACK) {
                        count += push_move(i, i - 9, PAWN_MOVE | CAPTURE, depth); 
                    }
                    if (COL(i) != 7 && color[i - 7] == BLACK) {
                        count += push_move(i, i - 7, PAWN_MOVE | CAPTURE, depth);
                    }
                    if (color[i - 8] == EMPTY) {
                        count += push_move(i, i - 8, PAWN_MOVE, depth);
                        if (i >= 48 && color[i - 16] == EMPTY) {
                            count += push_move(i, i - 16, PAWN_MOVE | COFFEE_JUMP, depth);
                        }
                    }
                } else {
                    if (COL(i) != 0 && color[i + 7] == WHITE) {
                        count += push_move(i, i + 7, PAWN_MOVE | CAPTURE, depth);
                    }
                    if (COL(i) != 7 && color[i + 9] == WHITE) {
                        count += push_move(i, i + 9, PAWN_MOVE | CAPTURE, depth);
                    }
                    if (color[i + 8] == EMPTY) {
                        count += push_move(i, i + 8, PAWN_MOVE, depth);
                        if (i <= 15 && color[i + 16] == EMPTY) {
                            count += push_move(i, i + 16, PAWN_MOVE | COFFEE_JUMP, depth);
                        }
                    }
                }
            } else { /* piece is not a pawn */
                for (j = 0; j < n_dir[piece[i]]; j++) {
                    for (n = i;;) {
                        n = board120[board64[n] + dir_offsets[piece[i]][j]];
                        if (n == -1) break;
                        if (color[n] == side) break;
                        if (color[n] == xside) {
                            count += push_move(i, n, CAPTURE, depth);
                            break;
                        }

                        count += push_move(i, n, 0, depth);

                        if (!slide[piece[i]]) break;
                    }
                }
            }
        }
    }

    /* castle moves */
    if (side == WHITE) {
        if (castle & 1) {
            count += push_move(E1, G1, CASTLE, depth);
        }
        if (castle & 2) {
            count += push_move(E1, C1, CASTLE, depth);
        }
    } else { /* side is black */
        if (castle & 4) {
            count += push_move(E8, G8, CASTLE, depth);
        }
        if (castle & 8) {
            count += push_move(E8, C8, CASTLE, depth);
        }
    }

    /* en passant moves */
    if (ep != -1) {
        if (side == WHITE) {
            if (COL(ep) != 0 && 
                    color[ep + 7] == WHITE && 
                    piece[ep + 7] == PAWN)
            {
                count += push_move(ep + 7, ep, PAWN_MOVE | CAPTURE | EP_CAPTURE, depth);
            }
            if (COL(ep) != 7 &&
                    color[ep + 9] == WHITE &&
                    piece[ep + 9] == PAWN)
            {
                count += push_move(ep + 9, ep, PAWN_MOVE | CAPTURE | EP_CAPTURE, depth);
            }
        } else { /* side is black */
            if (COL(ep) != 0 &&
                    color[ep - 9] == BLACK &&
                    piece[ep - 9] == PAWN)
            {
                count += push_move(ep - 9, ep, PAWN_MOVE | CAPTURE | EP_CAPTURE, depth);
            }
            if (COL(ep) != 7 &&
                    color[ep - 7] == BLACK &&
                    piece[ep - 7] == PAWN)
            {
                count += push_move(ep - 7, ep, PAWN_MOVE | CAPTURE | EP_CAPTURE, depth);
            }
        }
    }

    return count;
}

BOOL makemove(move m) {

    /* handle the castle moves */
    if (m.type & CASTLE) {
        int from, to;

        /* check if the castle is valid */
        if (in_check(side)) return FALSE;
        switch (m.to) {
            case G1:
                if (color[F1] != EMPTY ||
                        color[G1] != EMPTY ||
                        attack(F1, BLACK)  ||
                        attack(G1, BLACK))
                    return FALSE;

                from = H1;
                to = F1;
                break;
            case C1:
                if (color[B1] != EMPTY ||
                        color[C1] != EMPTY ||
                        color[D1] != EMPTY ||
                        attack(C1, BLACK)  ||
                        attack(D1, BLACK))
                    return FALSE;

                from = A1;
                to = D1;
                break;
            case G8:
                if (color[F8] != EMPTY ||
                        color[G8] != EMPTY ||
                        attack(F8, WHITE)  ||
                        attack(G8, WHITE))
                    return FALSE;

                from = H8;
                to = F8;
                break;
            case C8:
                if (color[B8] != EMPTY ||
                        color[C8] != EMPTY ||
                        color[D8] != EMPTY ||
                        attack(C8, WHITE)  ||
                        attack(D8, WHITE))
                    return FALSE;

                from = A8;
                to = D8;
                break;
        }
        piece[to] = piece[from];
        color[to] = color[from];
        piece[from] = EMPTY;
        color[from] = EMPTY;
    }

    /* store move so we can undo if necessary */
    last_move.m = m;
    last_move.capture = piece[m.to];
    last_move.castle = castle;
    last_move.ep = ep; 
    last_move.fifty = fifty;

    /* update castling rights */
    castle &= castle_mask[m.from] & castle_mask[m.to];

    /* make the move */
    color[m.from] = EMPTY;
    color[m.to] = side;
    piece[m.to] = piece[m.from];
    piece[m.from] = EMPTY;

    /* promote piece */
    if (m.type & PROMOTE) {
        piece[m.to] = m.promote;
    }

    /* remove piece from ep square */
    if (m.type & EP_CAPTURE) {
        if (side == WHITE) {
            color[ep + 8] = EMPTY;
            piece[ep + 8] = EMPTY;
        } else {
            color[ep - 8] = EMPTY;
            piece[ep - 8] = EMPTY;
        }
    }

    /* set ep square */
    if (m.type & COFFEE_JUMP) {
        if (side == WHITE) {
            ep = m.to + 8;
        } else {
            ep = m.to - 8;
        }
    } else {
        ep = -1;
    }

    /* update fifty-move rule */
    if (m.type & (CAPTURE | PAWN_MOVE)) {
        fifty = 0;
    } else {
        fifty++;
    }

    side ^= 1;
    xside ^= 1;
    ply++;

    /* make sure move did not cause own king to be in check */
    if (in_check(xside)) {
        undomove();
        return FALSE;
    }

    return TRUE;
}


/* test if side's king is in check
 * this could be faster if we use a piece square array */
BOOL in_check(int side) {
    int i;    

    for (i = 0; i < 64; i++)
        if (piece[i] == KING && color[i] == side)
            return attack(i, side ^ 1);

    return TRUE;
}

/* test if square is being attacked by side */
BOOL attack(int sq, int side) {
    int i, j, n; 

    for (i = 0; i < 64; i++) {
        if (color[i] == side) {
            if (piece[i] == PAWN) {
                if (side == WHITE) {
                    if (COL(i) != 0 && i - 9 == sq)
                        return TRUE;
                    if (COL(i) != 7 && i - 7 == sq)
                        return TRUE;
                } else { 
                    if (COL(i) != 0 && i + 7 == sq)
                        return TRUE;
                    if (COL(i) != 7 && i + 9 == sq)
                        return TRUE;
                }
            } else {
                for (j = 0; j < n_dir[piece[i]]; j++) {
                    for (n = i;;) {
                        n = board120[board64[n] + dir_offsets[piece[i]][j]];
                        if (n == -1) break;
                        if (n == sq) return TRUE;
                        if (color[n] != EMPTY) break;
                        if (!slide[piece[i]]) break;
                    }
                }
            }
        }
    }

    return FALSE;
}

/* undo a move */
void undomove() {
    move m;

    ply--;
    xside ^= 1;
    side ^= 1;

    m = last_move.m;
    fifty = last_move.fifty;
    ep = last_move.ep;
    castle = last_move.castle;

    /* unmake the move */
    color[m.from] = side;
    if (m.type & PROMOTE) piece[m.from] = PAWN;
    else piece[m.from] = piece[m.to];
    if (last_move.capture == EMPTY) {
        color[m.to] = EMPTY;
        piece[m.to] = EMPTY;
    } else {
        color[m.to] = xside;
        piece[m.to] = last_move.capture;
    }

    /* put piece back in front of ep square */
    if (m.type & EP_CAPTURE) {
        if (side == WHITE) {
            color[ep + 8] = xside;
            piece[ep + 8] = PAWN;
        } else {
            color[ep - 8] = xside;
            piece[ep - 8] = PAWN;
        }
    }

    if (m.type & CASTLE) {
        int from, to;

        switch (m.to) {
            case G1:
                from = F1;
                to = H1;
                break;
            case C1:
                from = D1;
                to = A1;
                break;
            case G8:
                from = F8;
                to = H8;
                break;
            case C8:
                from = D8;
                to = A8;
                break;
        }
        piece[to] = ROOK;
        color[to] = side;
        piece[from] = EMPTY;
        color[from] = EMPTY;
    }
}
