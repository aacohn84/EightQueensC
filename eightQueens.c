/*
 * eightQueens.c
 *
 *  Created on: Sep 6, 2013
 *      Author: Aaron Cohn
 */

/*
 * Representation:
 * A queen is represented by a cartesian pair (row, col), where row and col
 * represent the row and column of the queen's position on the board. For any
 * queen Q such that
 *
 * 			(0, 0) <= (Q.row, Q.col) < (8, 8),
 *
 * Q is said to be on the board. Otherwise, Q is said to be off the board. If
 * any queen Q1 is in a position that may be attacked by another queen Q2 under
 * the normal rule for movement in chess, then there is said to be a conflict
 * on the board. When 8 queens are on the board and there are no conflicts on
 * the board, the puzzle is solved.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
	int row;
	int col;
} position;

typedef position queen;

// the queens
queen q[8];

void initQueens();
bool moveQueen(int whichQueen);
bool moveCreatedConflict(int whichQueen);
position getDiag(const position *p, int rowOffset);
bool positionIsOnBoard(position p);
bool queenAtPosition(const queen *q, position p);
void printSolution(bool solved);

int main() {

	initQueens();

	printSolution(moveQueen(0));

	return 0;
}

void initQueens() {
	// each queen starts on it's own row, off the board
	int i;
	for (i = 0; i < 8; i++) {
		q[i].row = i;
		q[i].col = -1;
	}
}

/* Solves the 8 queen's problem with recursion and backtracking */
bool moveQueen(int whichQueen) {

	if (whichQueen >= 8) {
		// puzzle solved
		return true;
	}

	int i;
	bool moveSuccessful = false;
	for (i = 0; i < 8 && moveSuccessful == false; i++) {
		// move one space to the right
		q[whichQueen].col = i;

		if (moveCreatedConflict(whichQueen))
			continue;

		// no conflicts yet... try to move the next queen
		moveSuccessful = moveQueen(whichQueen + 1);
	}

	if (moveSuccessful == false) {
		// take queen off the board
		q[whichQueen].col = -1;
	}

	return moveSuccessful;
}

bool moveCreatedConflict(int whichQueen) {
	/* A conflict occurs when two queens occupy the same row, column, or
	 * diagonal. For any queens q1 and q2,
	 *
	 * q1 and q2 occupy the same row if
	 * 		q1.row == q2.row
	 *
	 * Since it is guaranteed that two queens will never occupy the same row,
	 * we do not check for it.
	 *
	 * q1 and q2 occupy the same column if
	 * 		q1.col == q2.col
	 *
	 * Suppose q1.row < q2.row. Then q1 and q2 occupy the same diagonal if
	 * 		q1.row == q2.row + x 	and		q1.col == q2.col ± x
	 */
	queen *movedQ = &q[whichQueen];
	int i;
	for (i = 0; i < whichQueen; i++) {
		queen *qi = &q[i];

		// check same column
		if (movedQ->col == qi->col)
			return true;

		// check same diagonal
		int x = whichQueen - i;
		position rightDiag = getDiag(qi, x);
		position leftDiag = getDiag(qi, -x);

		if ((positionIsOnBoard(rightDiag) && queenAtPosition(movedQ, rightDiag)) ||
			(positionIsOnBoard(leftDiag) && queenAtPosition(movedQ, leftDiag))) {
			return true;
		}
	}
	return false;
}

/* Given a position p and a row offset, finds the intersection of p's downward
 * diagonal and the row obtained by p.row + offset. A positive offset gives the
 * downward right diagonal, a negative offset gives the downward left diagonal.
 */
position getDiag(const position *p, int rowOffset) {
	return (position) {
		row: p->row + abs(rowOffset),
		col: p->col + rowOffset
	};
}

bool positionIsOnBoard(position p) {
	return (p.row >= 0 && p.row < 8 && p.col >= 0 && p.col < 8);
}

bool queenAtPosition(const queen *q, position p) {
	return (q->row == p.row && q->col == p.col);
}

void printSolution(bool solved) {

	if (!solved) {
		printf("NO SOLUTION");
		return;
	}

	char board[9][18] = {
			" _ _ _ _ _ _ _ _ ",
			"|_|_|_|_|_|_|_|_|",
			"|_|_|_|_|_|_|_|_|",
			"|_|_|_|_|_|_|_|_|",
			"|_|_|_|_|_|_|_|_|",
			"|_|_|_|_|_|_|_|_|",
			"|_|_|_|_|_|_|_|_|",
			"|_|_|_|_|_|_|_|_|",
			"|_|_|_|_|_|_|_|_|",
	};

	int i;
	for (i = 0; i < 8; i++) {
		board[q[i].row + 1][q[i].col * 2 + 1] = 'Q';
	}
	printf("SOLUTION:\n");
	printf("~~~~~~~~~\n");
	for (i = 0; i < 9; i++) {
		printf("%s\n", board[i]);
	}
}
