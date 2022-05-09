

## Assignment 4: Communicating States Across a Network (tic-tac-toe)


For starters, you can run the program by entering: 
  1.  ```python3 ttt.py -s``` for client mode
  2. ```python3 ttt.py -c``` for server mode
 
 As an extra feature, you can run 
  1. ```python3 ttt.py -s -ai```
  2. ```python3 ttt.py -s -ai```

  to have an AI play the game for you!
  
  
### Overall design

The code for client and server modes are identical except for how the socket is created. Using Python's
socket module, we canonically bind, listen and accept in server mode, and connect in client mode. In both modes,
when a connection is made, the function ```play_game``` is called, which contains a loop and all of the necessary 
game logic. In other words, both client and server keep a copy of their own game, where the opponent is the client 
or server. The function keeps a turn variable that keeps track of who's turn it is. If it is the current player's
turn, then the program calls ```input()``` to accept a 'move' input from the command line. This move is then parsed, 
checked for errors, and added to the board. Only then is it sent to the opponent (server or client). This prevents
both the server and client from having to do error handling for both cases. On the opponent's turn, the ```recv```
function is called, which blocks until it receives something from the opponent. If an empty string is received, then 
a message is outputed to the current player saying that the opponent has forfeited, and then the program exits. Otherwise,
the move is inserted into the board and everything becomes up-to-date. The upshot is that the current player will know for 
sure that the opponent's move is correct, because the opponent has the same error handling code (in fact it has all of the same code as the current player). Therefore, the amount of error handling and 
checking is reduced, as well as the amount of state that has to be maintained across the network. Since both players 
start out with the same board, and the only thing that ever changes is the move made at each turn, the only thing that
needs to be updated is that one move.

At the end of the game, there will be either a draw or a player will win. Again, both the client and server compute these
same things on their own ends, and end up with the same result relative to themselves. For example, if X wins, then on X's
end it will say "You win!" and on O's end it will say "You lose!"

Choosing who will start at the beginning of the game is simplified by having the client start first (as 'X'). 

Note: I noticed that when I shut down the server and started it back up again, it would not let me use the
same port number as before. In order to reuse the port number, I use the following: ```s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)```
after creating a socket, s, for the server. The server will always run on the port 5131, so no port command line argument is necessary. Similarly, the 
host is set by default to the host of the current device, however, for assignment #5, the program will be modified to accept a hostname argument for communication
between different machines. 

### AI agent

If using the -ai flag, an AI agent that I wrote will play the game instead of you. If both client and server are AI's, 
the game will unsurprisingly end in a draw. The AI makes decisions using the canonical minimax algorithm, and is optimized 
using alpha-beta pruning. 



