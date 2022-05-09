"""
Board utility functions used by ttt_ai.py and ttt.py files
"""

EMPTY = ' '

def createboard():
	return [[EMPTY]*3 for _ in range(3)]

def printboard(board):
	print('  1 2 3')
	print(' -------')
	for i in range(len(board)):
		print(f'{chr(ord("A")+i)}|', end="")
		for j in range(len(board[i])):
			print(f'{board[i][j]}|', end="")
		print('\n -------')

def isdraw(board):
	for i in range(3):
		for j in range(3):
			if board[i][j] == EMPTY: 
				return False
	return True

def winningmove(board, i, j):
	if isWin(board, 'X'): return 'X'
	if isWin(board, 'O'): return 'O'
	return EMPTY

def isFull(board):
	for arr in board:
		for el in arr:
			if el == EMPTY:
				return False
	return True

def winsDiagonal(board, player):
	diag1 = player == board[0][0] == board[1][1] == board[2][2]
	diag2 = player == board[0][2] == board[1][1] == board[2][0]
	return diag1 or diag2

def winsColumn(board, player):
	for j in range(len(board[0])):
		if player == board[0][j] == board[1][j] == board[2][j]:
			return True
	return False

def winsRow(board, player):
	for i in range(len(board)):
		if player == board[i][0] == board[i][1] == board[i][2]:
			return True
	return False

def isWin(board, player):
	return (winsRow(board, player) or winsColumn(board, player) 
		or winsDiagonal(board, player))
