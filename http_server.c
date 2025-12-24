#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#include "files.h"

#define PORT 8081
#define BACKLOG 32
#define BUF_SIZE 512

static int start_listen()
{
  int sock_fd, opt;
  struct sockaddr_in addr;

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd == -1)
    goto err;

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(PORT);

  opt = 1;
  if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    goto err;

  if (bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    goto err;

  if (listen(sock_fd, BACKLOG) == -1)
    goto err;

  printf("Start server on port: %d\n", PORT);
  return sock_fd;

err:
  perror("");
  if (sock_fd != -1) {
    if (close(sock_fd) == -1)
      perror("close");
  }
  exit(EXIT_FAILURE);
}

static void send_response(
    int client,
    const char *status,
    const char *type,
    const unsigned char *data,
    size_t len)
{
    char header[128];
    int hlen = snprintf(header, sizeof(header),
        "HTTP/1.1 %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n\r\n",
        status, type, len);

    send(client, header, hlen, 0);
    send(client, data, len, 0);
}

static void handle_request(int client_fd)
{
  char buf[BUF_SIZE];
  int res;

  res = read(client_fd, buf, sizeof(buf));
  if (res <= 0)
    return;
  buf[res] = '\0';

  if (strncmp(buf, "GET / ", 6) == 0) {
      send_response(client_fd, "200 OK", "text/html",
                    index_html, index_html_len);
  }
  else if (strncmp(buf, "GET /style.css", 14) == 0) {
      send_response(client_fd, "200 OK", "text/css",
                    style_css, style_css_len);
  }
  else if (strncmp(buf, "GET /image.bin", 14) == 0) {
      send_response(client_fd, "200 OK", "application/octet-stream",
                    image_bin, image_bin_len);
  }
  else {
      static const char msg[] = "404 Not Found";
      send_response(client_fd, "404 Not Found", "text/plain",
                    (const unsigned char*)msg, sizeof(msg)-1);
  }
}

int main(void)
{
  int listen_fd, client_fd;
  struct sockaddr_in addr;
  socklen_t addr_len = sizeof(addr);

  listen_fd = start_listen();
  for (;;) {
    client_fd = accept(listen_fd, (struct sockaddr*)&addr, &addr_len);
    if (client_fd == -1) {
      if (errno == EINTR)
        continue;
      else
        goto err;
    }

    handle_request(client_fd);
    close(client_fd);
  }

err:
  perror("");
  close(listen_fd);
  return EXIT_FAILURE;
}
