#define _GNU_SOURCE 1
#ifndef __USE_BSD
#define __USE_BSD /* for strdup on linux/glibc */
#endif /* __USE_BSD */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "xdo.h"
#include "xdotool.h"
#include "string_utils.c"
#include "queue.h"

int isValidCmd(char * cmd);
char * stripPrePost(char * cmd);

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void urldecode(char *dst, const char *src) {
    char a, b;
    while (*src) {
        if ((*src == '%') &&
                ((a = src[1]) && (b = src[2])) &&
                (isxdigit(a) && isxdigit(b))) {
            if (a >= 'a')
                a -= 'a' - 'A';
            if (a >= 'A')
                a -= ('A' - 10);
            else
                a -= '0';
            if (b >= 'a')
                b -= 'a' - 'A';
            if (b >= 'A')
                b -= ('A' - 10);
            else
                b -= '0';
            *dst++ = 16 * a + b;
            src += 3;
        } else if (*src == '+') {
            *dst++ = ' ';
            src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst++ = '\0';
}

const char *xdotool = "xdotool";
context_t context;
const char *cmd_k = "k";
const char *cmd_t = "t";
const char *ok = "ok\0";

void initXdo() {
    context.xdo = xdo_new(NULL);
    context.windows = NULL;
    context.nwindows = 0;
    context.have_last_mouse = False;
    context.debug = (getenv("DEBUG") != NULL);

    if (context.xdo == NULL) {
        fprintf(stderr, "Failed creating new xdo instance\n");
        return 1;
    }
    context.xdo->debug = context.debug;
}

int main(int argc, char *argv[]) {
    //system("ifconfig");
    initXdo();
    int udpSocket, nBytes;
    char buffer[1024];
    struct sockaddr_in serverAddr, clientAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size, client_addr_size;
    int i;

    /*Create UDP socket*/
    udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /*Bind socket with address struct*/
    bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    /*Initialize size variable to be used later on*/
    addr_size = sizeof serverStorage;

    while(1){
        /* Try to receive any incoming UDP datagram. Address and port of
        requesting client will be stored on serverStorage variable */
        nBytes = recvfrom(udpSocket,buffer,1024,0,(struct sockaddr *)&serverStorage, &addr_size);
        buffer[nBytes] = '\0';
        resolveCmdsFromBuffer(buffer);
        //sendto(udpSocket,ok,sizeof(ok),0,(struct sockaddr *)&serverStorage,addr_size);
    }

    close(udpSocket);
    xdotool_destroy(context);
    exit(0);
    return 0; /* we never get here */
}

int hasUnResolvedCmd = 0;
char *unresolvedCmd = 0;
const char pre = '<';
const char post = '>';
const char space = ' ';
const char end = '|';

void xdotool_call(char *cmd) {
    char * stripCmd = stripPrePost(cmd);
    printf("Calling %s\n", stripCmd);
    struct SplitResult args = str_split(stripCmd, space);
    if (args.tokens) {
        if (strcmp(*(args.tokens), cmd_t) == 0) {
            char * value = *(args.tokens + 1);
            char * decodedValue = malloc(strlen(value) + 1);
            urldecode(decodedValue, value);
            printf("URI parse value %s, %s\n", value, decodedValue);
            args.tokens[1] = decodedValue;
            free(value);
        }
        xdotool_execute(context, args.count, args.tokens);
        free(args.tokens);
    }
    free(stripCmd);
}

int resolveCmdsFromBuffer(char *buffer) {
    if (strlen(buffer) < 2 && buffer[0] == end) {
        return 0;
    }
    SplitResult cmds = str_split(buffer, end);
    for (int i = 0; i < cmds.count && (cmds.tokens + i); i++) {
        char * cmd = *(cmds.tokens + i);
        if (isValidCmd(cmd)) {
            xdotool_call(cmd);
            free(cmd);
        } else {
            if (hasUnResolvedCmd) {
                char *resCmd = str_concat(unresolvedCmd, cmd);
                //printf("Resolved cmd %s\n", resCmd);
                if (isValidCmd(resCmd)) {
                    xdotool_call(resCmd);
                }
                free(unresolvedCmd);
                free(resCmd);
                hasUnResolvedCmd = 0;
            } else {
                hasUnResolvedCmd = 1;
                unresolvedCmd = cmd;
            }
            //printf("Error: invalid cmd %s \n", cmd);
        }
    }
    return 1;
}

int isValidCmd(char * cmd) {
    int size = strlen(cmd);
    if (cmd[0] != pre)
        return 0;
    if (cmd[size - 1] != post)
        return 0;
    return 1;
}

char * stripPrePost(char * cmd) {
    int size = strlen(cmd);
    char *s = malloc(sizeof (char)*size - 1);
    memcpy(s, cmd + 1, size - 2);
    s[size - 2] = '\0';
    return s;
}
