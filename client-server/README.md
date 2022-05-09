## What I added to the code

In addition to the implementing the basic functionality of the chat server, I refactored the given code and split it into modular chunks. For both the client and the server, I created send_msg and receive_msg functions for sending and receiving messages back and forth. I also removed the verbosity of a lot of the error checking if statements by creating a function called check that implemented the basic -- display error message and then exit functionality -- that was constantly being repeated across the code. 


With regard to the CLI, I added a slight improvement by outputting "server: " and "client: " when each are "speaking", so as to facillitate a clearer and more enjoyable user experience. 
