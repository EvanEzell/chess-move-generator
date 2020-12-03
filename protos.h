/* board prototypes */
void init_board();
long push_promote(int from, int to, int type, int depth);
BOOL parse_fen();
long push_move(int from, int to, int type, int depth);
long gen(int depth);
BOOL makemove(move m);
void undomove();
BOOL in_check(int side);
BOOL attack(int i, int side);
void print_board();
