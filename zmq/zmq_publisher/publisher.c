#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <zmq.h>

#define MSG_VERSION 0

/* ***************************************************************** */

struct zmq_msg_hdr {
  char url[32];
  u_int32_t version;
  u_int32_t size;
};

/* ***************************************************************** */

void help() {
  printf("publisher <file name>\n");
  exit(0);
}

/* ***************************************************************** */


int main(int argc, char *argv []) {
  void *context = zmq_ctx_new();
  void *publisher = zmq_socket(context, ZMQ_PUB);
  int rc, size;
  struct zmq_msg_hdr msg_hdr;
  char *endpoint = "tcp://127.0.0.1:5556";
  char *topic = "flow", buffer[512];
  FILE *fd;

  if(argc != 2) help();
  fd = fopen(argv[1], "r");

  if(fd == NULL) {
    printf("Unable to open file %s\n", argv[1]);
    exit(0);
  }

  rc = zmq_bind(publisher, endpoint);
  if(rc != 0) {
    printf("Unable to bind to %s\n", endpoint);
    return -1;
  } else
    printf("Publishing data to %s...\n", endpoint);

  printf("Waiting ntopng to start...\n");
  sleep(3);

  printf("Ready\n");

  msg_hdr.version = 0, snprintf(msg_hdr.url, sizeof(msg_hdr.url), "flow");

  while(fgets(buffer, sizeof(buffer), fd) != NULL) {
    // |00:10:07|joe|10.49.133.251|53651|205.188.250.75|993|0|

    printf("%s", buffer);
    if(buffer[0] == '|') {
      char flow[2048];
      int h, m, s;
      u_int32_t begin, end;
      char *when, *user, *src_ip, *dst_ip;
      char *sport, *dport, *proto;

      when = strtok(buffer, "|");
      user = strtok(NULL, "|");
      src_ip = strtok(NULL, "|");
      sport = strtok(NULL, "|");
      dst_ip = strtok(NULL, "|");
      dport = strtok(NULL, "|");
      proto = strtok(NULL, "|");

      sscanf(when, "%u:%u:%u", &h, &m, &s);
      begin = time(NULL);
      begin -= (begin % 86400);
      begin += h*3600+m*60+s;
      end = begin + 60 /* 1 min */;
      snprintf(flow, sizeof(flow),
	       "{\"8\":\"%s\",\"12\":\"%s\",\"7\":%s,\"11\":%s,\"4\":%s,\"22\":%u,\"21\":%u}",
	       src_ip, dst_ip, sport, dport, proto, begin, end);

      msg_hdr.size = strlen(flow);
      zmq_send(publisher, &msg_hdr, sizeof(msg_hdr), ZMQ_SNDMORE);
      zmq_send(publisher, flow, msg_hdr.size, 0);
    } 
  }

  fclose(fd);
  zmq_close(publisher);
  zmq_ctx_destroy(context);

  return 0;
}

