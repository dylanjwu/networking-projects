
"""
Simple Tic-Tac-Toe Game
March 20, 2022
Sourced From:  https://www.usna.edu/Users/cs/roche/courses/s17si204/notes/06/files.php?f=tictactoe.c


@author Dylan Wu
Date: 04/24/2022
"""

import socket
import sys
from ttt_board import *
import time
import ttt_ai as ai

"""
Preconditions: connection has been made, player has been decided that is either an ai or not
"""
def play_game(player, connection, is_ai):

	# print(f"Connected by {addr}")
	client_first = connection.recv()
	y_or_n = client_first.decode('utf-8')
	if y_or_n == 'N':
		turn = player

	board = createboard()
	turn = 'X'
	winner = ' '

	printboard(board)

	# loop condition: no winner and no draw
	while winner == ' ' and not isdraw(board):

		if turn != player:
			# connection.send(bytes("Your turn now!", 'utf-8'))
			print("Waiting for opponent...")
			move = connection.recv(1024)
			if not move: #if move is empty, close the connection and exit the program
				print("Your opponent is forfeiting; closing game")
				connection.close()
				exit(0)
		
		elif is_ai: 
			time.sleep(2)	# create 2 sec timeout in case other player is also an AI agent
			move = ai.ai_move(board, player)

		else: # turn == 'O'
			move = input("Your move: ")

		try: # player must always send a valid move to their opponent
			if player != turn:
				move = move.decode('utf-8') 

			row, col = move[0:2]
			rowind = ord(row) - ord('A')
			colind = int(col) - 1

			if board[rowind][colind] == ' ':
				board[rowind][colind] = turn
				winner = winningmove(board, rowind, colind)
				if player == turn: # send move to opponent
					connection.send(bytes(move, 'utf-8'))
				turn = other_player(turn)
			else:
				print("Square occupied; try again.\n")
				continue

		except Exception as e: # print exception to the user
			print(f"Exception: {e}" )	
			continue

		printboard(board)

	# display post-game status 
	if winner == player:
		print(f"You won!\n")
	elif winner == other_player(player):
		print(f"You lose!\n")
	else:
		print("Game ends in a draw.\n")

""" Create socket for server: bind, listen, and then accept connection from client, play game """
def server(host, port, is_ai):
	# https://realpython.com/python-sockets/
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		# Ref'd: https://stackoverflow.com/questions/6380057/python-binding-socket-address-already-in-use
		s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # reuse port after closing server

		s.bind((host, port))
		s.listen()
		print(f"Server listening at host: {host}, port: {port}")
		connection, addr = s.accept()
		with connection:
			play_game('O', connection, is_ai)
			
""" Create socket for client: connecct to server, play game """
def client(host, port, is_ai):
	# Ref'd: https://realpython.com/python-sockets/
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		s.connect((host, port))
		play_game('X', s, is_ai) # client starts first, as 'X'


HOST = socket.gethostbyname(socket.gethostname())
PORT = 5131


if __name__ == '__main__':
	
	is_ai = False
	if len(sys.argv) > 2: 
		is_ai = sys.argv[2] == '-ai'

	if len(sys.argv) > 1:
		if sys.argv[1] == '-c': # client
			client(HOST, PORT, is_ai)
		elif sys.argv[1] == '-s': # server
			server(HOST, PORT, is_ai)

	else: # server
		server(HOST, PORT, is_ai)
