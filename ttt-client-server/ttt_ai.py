""" @author Dylan Wu """

from sys import maxsize
from ttt_board import *

POSITIONS = [(i, j) for j in range(3) for i in range(3)]
other = lambda player : 'X' if player == 'O' else 'O'

def minimax(board, player, isMax=True, depth=0):
    if not isMax:
        return minimize(board, player, depth+1, -maxsize, maxsize)
    return maximize(board, player, depth+1, -maxsize, maxsize)

def evaluate(board, player):

    winner = winningmove(board, -1, -1)

    if player == winner:
        return 10
    elif other(player) == winner:
        return -10
    elif isdraw(board):
        return 0
    else:
        return None
    

def minimize(board, player, depth, alpha, beta):

    res = evaluate(board, player) 

    if res != None: return (res, (), depth)

    minn = (maxsize, (), 0)
    for i,j in POSITIONS:
        if board[i][j] == ' ':
            boardCopy = [row[::] for row in board]
            boardCopy[i][j] = other(player)
            newRes = maximize(boardCopy, player, depth + 1, alpha, beta)
            newRes = (newRes[0], (i, j), depth)

            if minn[0] == newRes[0]: # minimize depth always
                minn = min(minn, newRes, key = lambda x: x[2])
            else:
                minn = min(minn, newRes) 

            beta = min(beta, minn[0])

            if alpha > minn[0]:
                break


    return minn

def maximize(board, player, depth, alpha, beta):

    res = evaluate(board, player) 

    if res != None: return (res, (), depth)

    maxx = (-maxsize, (), 0)
    for i,j in POSITIONS:
        if board[i][j] == ' ':
            boardCopy = [row[::] for row in board]
            boardCopy[i][j] = player
            newRes = minimize(boardCopy, player, depth + 1, alpha, beta)
            newRes = (newRes[0], (i, j), depth)

            if maxx[0] == newRes[0]: # minimize depth always
                maxx = min(maxx, newRes, key = lambda x: x[2])
            else:
                maxx = max(maxx, newRes) 

            alpha = max(alpha, maxx[0])

            if beta < maxx[0]:
                break

    return maxx

""" AI interface called in ttt.py """
def ai_move(board, player):
    num2ch = list('ABC')
    _, move, _ = minimax(board, player)
    row, col = move
    return f'{num2ch[row]}{col+1}'

if __name__ == '__main__':
    board = [
        ['X', 'O',' '],
        [' ', 'O',' '],
        [' ', 'O',' '],
    ]

    print(evaluate(board, player))
    # print(ai_move(board))

    score, move, depth = minimax(board, 'X')
    print(f'Score: {score}, Move: {move}, Depth: {depth}')