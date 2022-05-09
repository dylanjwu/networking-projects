

### HW2: Webber

What I did: I wrote a simple command line UI to more easily construct and send HTTP requests

Simply run the program (./webber).

A welcome and prompt will appear.

Enter the required fields for your request (method, hostname, path)

After you enter all fields, a request will be made to the HTTP server (if possible)

..the results of which will be printed to the screen

To send a GET request, try:
- Method: GET
- Hostname: valcourt.org
- Path: /

The server should return an HTTP/1.1 200 OK response

To send an options request, try:
- Method: OPTIONS *
- Hostname: example.com
- Path:  


Note that it does not matter if you type: http://valcourt.org or just valcourt.org. The code I wrote will parse out the protocol prefix, so that we can still find the IP address of the inputted domain.

The program runs in a loop, so you can keep making requests without having to rerun the program.
