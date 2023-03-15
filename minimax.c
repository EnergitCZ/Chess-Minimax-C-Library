#include <math.h>
#include <stdlib.h>
#include <time.h>

void place_to_xy(int place, int* x, int* y) {
	/*
		Convert numbers 0 - 63 to XY coordinates
	*/
	*x = (int) floor(place / 8);
	*y = (int) place % 8;
}

void xy_to_place(int* place, int x, int y) {
	/*
		Convert XY coordinates to numbers 0 - 63
	*/
	*place = x*8 + y;
}

int eval_board(int board[64]) {
	/*
		Evaluate a score for a board
	*/
	int score = 0;
	for (int i = 0; i < 64; i++){ // Taken from https://avenir-condominium.com/cs/sachove-figurky-jmena-pohyby-a-hodnoty/
		int piece = board[i];	
		switch (piece) {
			case 1:
				score = score + 12;
				break;
			case 2:
				score = score + 9;
				break;
			case 3:
				score = score + 3;
				break;
			case 4:
				score = score + 3;
				break;
			case 5:
				score = score + 5;
				break;
			case 6:
				score = score + 1;
				break;
			case -1:
				score = score - 12;
				break;
			case -2:
				score = score - 9;
				break;
			case -3:
				score = score - 3;
				break;
			case -4:
				score = score - 3;
				break;
			case -5:
				score = score - 5;
				break;
			case -6:
				score = score - 1;
				break;
		}
	}
	return score;
}

int check_move(int board[64], int x, int y, int board_place, int max_turn) {
	/*
		Check if a place is valid and if there is a piece
	*/
	if (!(x >= 0 && x <= 7 && y >= 0 && y <= 7)) { // Place does not exist
		return 0;
	} else if (board[board_place] == 0) { // Place is empty
		return 1;
	} else if (board[board_place] < 0) {
		if (max_turn) {
			return 2; // Place of other players figure
		} else {
			return 3; // Place of this players figure
		}
	} else if (board[board_place] > 0) {
		if (max_turn) {
			return 3; // Place of this players figure
		} else {
			return 2; // Place of other players figure
		}
	} else {
		return -1; // Should not happen
	}
}

void get_moves(int board[64], int board_place, int moves[64], int* move_count, int max_turn) {
	/*
		Get all possible moves for the "positive" player
	*/
	int x, y;
	place_to_xy(board_place, &x, &y);
	int piece = board[board_place];
	if (!max_turn) {
		piece = piece * -1;
	}
	switch (piece) {
		case 1: { // King
				for (int x1 = -1; x1 < 2; x1++) {
					for (int y1 = -1; y1 < 2; y1++) {
						if (x1 == 0 && y1 == 0) continue;
						int x2 = x + x1;
						int y2 = y + y1;
						int place;
						xy_to_place(&place, x2, y2);
						int check = check_move(board, x2, y2, place, max_turn);
						if (check == 1 || check == 2) {
							moves[*move_count] = place;
							*move_count = *move_count + 1;
						}
					}	
				}
			}
			break;
		case 2: { // Queen
				for (int mod = 0; mod < 8; mod++) {
					int modx, mody;
					if (mod == 0) {modx = 1; mody = 1;}
					if (mod == 1) {modx = 1; mody = -1;}
					if (mod == 2) {modx = -1; mody = 1;}
					if (mod == 3) {modx = -1; mody = -1;}
					if (mod == 4) {modx = 0; mody = 1;}
					if (mod == 5) {modx = 1; mody = 0;}
					if (mod == 6) {modx = 0; mody = -1;}
					if (mod == 7) {modx = -1; mody = 0;}
					for (int i = 0; i < 8; i++) {
						int x1 = modx * i;
						int y1 = mody * i;
						if (x1 == 0 && y1 == 0) continue;
						int x2 = x + x1;
						int y2 = y + y1;
						int place;
						xy_to_place(&place, x2, y2);
						int check = check_move(board, x2, y2, place, max_turn);
						if (check == 1) {
							moves[*move_count] = place;
							*move_count = *move_count + 1;
						} else if (check == 2) {
							moves[*move_count] = place;
							*move_count = *move_count + 1;
							break;
						} else if (check == 3) {
							break;
						}
					}
				}
			}
			break;
		case 3: { // Bishop
				for (int mod = 0; mod < 4; mod++) {
					int modx, mody;
					if (mod == 0) {modx = 1; mody = 1;}
					if (mod == 1) {modx = 1; mody = -1;}
					if (mod == 2) {modx = -1; mody = 1;}
					if (mod == 3) {modx = -1; mody = -1;}
					for (int i = 0; i < 8; i++) {
						int x1 = modx * i;
						int y1 = mody * i;
						if (x1 == 0 && y1 == 0) continue;
						int x2 = x + x1;
						int y2 = y + y1;
						int place;
						xy_to_place(&place, x2, y2);
						int check = check_move(board, x2, y2, place, max_turn);
						if (check == 1) {
							moves[*move_count] = place;
							*move_count = *move_count + 1;
						} else if (check == 2) {
							moves[*move_count] = place;
							*move_count = *move_count + 1;
							break;
						} else if (check == 3) {
							break;
						}
					}
				}
			}
			break;
		case 4: { // Knight
				int moves_aval[8][2] = {{-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}};
				for (int i = 0; i < 8; i++) {
					int move_x = moves_aval[i][0];
					int move_y = moves_aval[i][1];
					int x1 = x + move_x;
					int y1 = y + move_y;
					int place;
					xy_to_place(&place, x1, y1);
					int check = check_move(board, x1, y1, place, max_turn);
					if (check == 1 || check == 2) {
						moves[*move_count] = place;
						*move_count = *move_count + 1;
					}
				}
			}
			break;
		case 5: { // Rook
				for (int mod = 0; mod < 8; mod++) {
					int modx, mody;
					if (mod == 4) {modx = 0; mody = 1;}
					if (mod == 5) {modx = 1; mody = 0;}
					if (mod == 6) {modx = 0; mody = -1;}
					if (mod == 7) {modx = -1; mody = 0;}
					for (int i = 0; i < 8; i++) {
						int x1 = modx * i;
						int y1 = mody * i;
						if (x1 == 0 && y1 == 0) continue;
						int x2 = x + x1;
						int y2 = y + y1;
						int place;
						xy_to_place(&place, x2, y2);
						int check = check_move(board, x2, y2, place, max_turn);
						if (check == 1) {
							moves[*move_count] = place;
							*move_count = *move_count + 1;
						} else if (check == 2) {
							moves[*move_count] = place;
							*move_count = *move_count + 1;
							break;
						} else if (check == 3) {
							break;
						}
					}
				}
			}
			break;
		case 6: { // Pawn
				int x1, y1, place;

				if (max_turn) {
					x1 = x + 1;
					y1 = y;
				} else {
					x1 = x - 1;
					y1 = y;
				}
				
				xy_to_place(&place, x1, y1);

				if (check_move(board, x1, y1, place, max_turn) == 1) {
					moves[*move_count] = place;
					*move_count = *move_count + 1;
				}

				if (max_turn) {
					if (x == 1) {
						if (check_move(board, x + 1, y, place, max_turn) == 1) {
							x1 = x + 2;
							y1 = y;

							xy_to_place(&place, x1, y1);

							if (check_move(board, x1, y1, place, max_turn) == 1) {
								moves[*move_count] = place;
								*move_count = *move_count + 1;
							}
						}
					}
				} else {
					if (x == 6) {
						if (check_move(board, x - 1, y, place, max_turn) == 1) {
							x1 = x - 2;
							y1 = y;

							xy_to_place(&place, x1, y1);

							if (check_move(board, x1, y1, place, max_turn) == 1) {
								moves[*move_count] = place;
								*move_count = *move_count + 1;
							}
						}
					}
				}

				if (max_turn) {
					x1 = x + 1;
					y1 = y - 1;
				} else {
					x1 = x - 1;
					y1 = y - 1;
				}

				xy_to_place(&place, x1, y1);

				if (check_move(board, x1, y1, place, max_turn) == 2) {
					moves[*move_count] = place;
					*move_count = *move_count + 1;
				}

				if (max_turn) {
					x1 = x + 1;
					y1 = y + 1;
				} else {
					x1 = x - 1;
					y1 = y + 1;
				}

				xy_to_place(&place, x1, y1);

				if (check_move(board, x1, y1, place, max_turn) == 2) {
					moves[*move_count] = place;
					*move_count = *move_count + 1;
				}
			}
			break;
	}
}

int minimax(int board[], int max_depth, int max_turn, int alpha, int beta) {
	/*
		Get best move on a board
	*/
	int best, mm;
	if (max_depth == 0) {
		return eval_board(board);
	}
	if (max_turn) {
		best = -1000;
		for (int i = 0; i < 64; i++) {
			int moves[64] = {};
			int move_count = 0;
			get_moves(board, i, moves, &move_count, 1);
			for (int m = 0; m < move_count; m++) {
				int move = moves[m];
				int og_start = board[i];
				int og_end = board[move];
				board[i] = 0;
				board[move] = og_start;
				mm = minimax(board, max_depth - 1, 0, alpha, beta);
				if (mm > best) {
					best = mm;
				}
				if (best > alpha) {
					alpha = best;
				}
				board[i] = og_start;
				board[move] = og_end;
				if (beta <= alpha) {
					break;
				}
			}
		}
		return best;
	} else {
		best = 1000;
		for (int i = 0; i < 64; i++) {
			int moves[64] = {};
			int move_count = 0;
			get_moves(board, i, moves, &move_count, 0);
			for (int m = 0; m < move_count; m++) {
				int move = moves[m];
				int og_start = board[i];
				int og_end = board[move];
				board[i] = 0;
				board[move] = og_start;
				mm = minimax(board, max_depth - 1, 1, alpha, beta);
				if (mm < best) {
					best = mm;
				}
				if (best < alpha) {
					alpha = best;
				}
				board[i] = og_start;
				board[move] = og_end;
				if (beta <= alpha) {
					break;
				}
			}
		}
		return best;
	}
}

void get_best_place(int board[64], int depth, int* best_x_start, int* best_y_start, int* best_x_end, int* best_y_end) {
	srand(time(NULL));
	int current_max = -1000;
	int current_best_place_start = 0;
	int current_best_place_end = 0;
	int mm, random_number;
	for (int i = 0; i < 64; i++) {
		int moves[64] = {};
		int move_count = 0;
		get_moves(board, i, moves, &move_count, 1);
		for (int m = 0; m < move_count; m++) { // Try every move, get move with the best score
			int move = moves[m];
			int og_start = board[i];
			int og_end = board[move];
			board[i] = 0;
			board[move] = og_start;
			mm = minimax(board, depth, 0, -1000, 1000);
			if (mm > current_max) {
				current_max = mm;
				current_best_place_start = i;
				current_best_place_end = move;
			} else if (mm == current_max) { // Randomness of moves
				random_number = rand() % 3;
				if (random_number == 1) {
					current_max = mm;
					current_best_place_start = i;
					current_best_place_end = move;
				}
			}
			board[i] = og_start;
			board[move] = og_end;
		}
	}
	place_to_xy(current_best_place_start, best_x_start, best_y_start);
	place_to_xy(current_best_place_end, best_x_end, best_y_end);
}