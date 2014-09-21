BOARD_SIZE = 8

def canPlace(board, col):
	left = right = col

	for c in reversed(board):
		left, right = left - 1, right + 1

		if col == 3:
			print "checking " c

		if c in (col, left, right):
		   return False

	return True

def placeQueen(board):
	if len(board) >= BOARD_SIZE:
		printBoard(board)
		return True

	for col in range(BOARD_SIZE):
		if canPlace(board, col) and placeQueen(board + [col]):
			return True
	
	return False

def printBoard(board):
	for row in board:
		for col in range(BOARD_SIZE):
			if board[row] == col:
				print "1",
			else:
				print "0",

		print

placeQueen([])