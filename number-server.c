#include "http-server.h"
#include <string.h>

char const HTTP_404_NOT_FOUND[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
char const HTTP_200_OK[] = "HTTP/1.1 200 OK\r\nContent-TypeL text/plain\r\n\r\n";
int num = 0;

void handle_404(int client_sock, char *path)  {
    printf("SERVER LOG: Got request for unrecognized path \"%s\"\n", path);

    char response_buff[BUFFER_SIZE];
    snprintf(response_buff, BUFFER_SIZE, "Error 404:\r\nUnrecognized path \"%s\"\r\n", path);
    // snprintf includes a null-terminator

    // TODO: send response back to client?
    write(client_sock, HTTP_404_NOT_FOUND, strlen(HTTP_404_NOT_FOUND));
    write(client_sock, response_buff, strlen(response_buff));
}

void handle_200(int client_sock) {
	write(client_sock, HTTP_200_OK, strlen(HTTP_200_OK));
}

void shownum(int client_sock) {
	char response_buffer[BUFFER_SIZE];
	snprintf(response_buffer, BUFFER_SIZE, "Your number is %d\r\n", num);
	handle_200(client_sock);
	write(client_sock, response_buffer, strlen(response_buffer));
}

void increment(int client_sock) {
	num++;
	handle_200(client_sock);
}

void add(int client_sock, int n) {
	num += n;
	handle_200(client_sock);
}
void handle_response(char *request, int client_sock) {
    char path[256];

    printf("\nSERVER LOG: Got request: \"%s\"\n", request);

    // Parse the path out of the request line (limit buffer size; sscanf null-terminates)
    if (sscanf(request, "GET %255s", path) != 1) {
        printf("Invalid request line\n");
        return;
    }

    if (strcmp(path, "/shownum") == 0) {
	    shownum(client_sock);
	    return;
    } else if (strcmp(path, "/increment") == 0) {
	    increment(client_sock);
	    return;
    } else if (strstr(path, "/add") != NULL) {
	    int n = 0;
	    sscanf(path, "/add?value=%d", &n);
	    add(client_sock, n);
	    return;
    }

    handle_404(client_sock, path);
}

int main(int argc, char *argv[]) {
    int port = 0;
    if(argc >= 2) { // if called with a port number, use that
        port = atoi(argv[1]);
    }

    start_server(&handle_response, port);
}
