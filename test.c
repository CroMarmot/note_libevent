#include <unistd.h>
#include <event.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
char* touppers(char* lpstr,int num)
{
    int i=0;
    char* p = lpstr;
    for(;i<num;++i)
    {
        p[i] = toupper(p[i]);
    }
    return lpstr;
}

void readcb(struct bufferevent *bev, void *ctx)
{
    struct evbuffer *input, *output;
    char *line;
    size_t nread,size,ret;
    char* buffer = NULL;
                                                                                                                               
    input = bufferevent_get_input(bev);
    output = bufferevent_get_output(bev);
    size = evbuffer_get_length(input);
    buffer = (char*)malloc(size+1);
    if(buffer == NULL)
        perror("malloc error");
    memset(buffer,0,size+1);
    nread = evbuffer_remove(input,buffer,size+1);
    printf("in data: %s\n",buffer);
                                                                                                                               
    touppers(buffer,size);
                                                                                                                               
    ret = evbuffer_add(output,buffer,size);
    if(ret == -1)
        perror("add data to buffer failed");
    free(buffer);
}

void do_accept(evutil_socket_t listener, short event, void *arg)
{
    struct event_base *base = arg;
    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);
    int fd = accept(listener, (struct sockaddr*)&ss, &slen);
    if (fd < 0)
    {
        perror("accept");
    }
    else if (fd > FD_SETSIZE)
    {
        close(fd);
    }
    else
    {
        struct bufferevent *bev;
        struct timeval newtime;
        evutil_gettimeofday(&newtime,NULL);
        printf("----accept..%d\n",newtime);
                                                                                                                                   
        evutil_make_socket_nonblocking(fd);
        bev = bufferevent_socket_new(base, fd, 0);
        bufferevent_setcb(bev, readcb, NULL, NULL, NULL);
        bufferevent_setwatermark(bev, EV_READ, 0, 0);
        bufferevent_enable(bev, EV_READ|EV_WRITE);
    }
}

int main()
{
    evutil_socket_t listener;
    struct sockaddr_in sin;
    struct event_base* base;
    struct event* listener_event;
                                                                                                                               
    base = event_base_new();
    if(!base)
        return -2;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(32000);
                                                                                                                               
    listener = socket(AF_INET,SOCK_STREAM,0);
    evutil_make_socket_nonblocking(listener);
                                                                                                                               
    if (bind(listener, (struct sockaddr*)&sin, sizeof(sin)) < 0)
    {
        perror("bind");
        return -2;
    }
    if (listen(listener, 32)<0)
    {
        perror("listen");
        return -2;
    }
    listener_event = event_new(base, listener, EV_READ | EV_PERSIST, do_accept, (void*)base);
    event_add(listener_event, NULL);
    event_base_dispatch(base);
                                                                                                                               
    return 0;
}
