#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "arpa/inet.h"

#include "event.h"

//读回调处理
void read_callback(struct bufferevent * pBufEv, void * pArg)
{
    //获取输入缓存
    struct evbuffer * pInput = bufferevent_get_input(pBufEv);
    //获取输入缓存数据的长度
    int nLen = evbuffer_get_length(pInput);
    //获取数据的地址
    const char * pBody = (const char *)evbuffer_pullup(pInput, nLen);

    //进行数据处理
    printf("receive data:%s, size:%d\n", pBody, nLen);

    //写到输出缓存,由bufferevent的可写事件读取并通过fd发送
    //bufferevent_write(pBufEv, pResponse, nResLen);

    return ;
}

//写回调处理
void write_callback( struct bufferevent * pBufEv, void * pArg )
{
    printf("write_callback");
    return ;
}

//事件回调处理
void event_callback(struct bufferevent * pBufEv, short sEvent, void * pArg)
{
    //成功连接通知事件
    if(BEV_EVENT_CONNECTED == sEvent)
    {
        bufferevent_enable(pBufEv, EV_READ);
        bufferevent_write(pBufEv, "hihi", 4);
    }

    return ;
}

int main( void )
{
    struct event_base * pEventBase = NULL;
    struct bufferevent * pBufEv = NULL;

    //创建事件驱动句柄
    pEventBase = event_base_new();
    //创建socket类型的bufferevent
    pBufEv = bufferevent_socket_new(pEventBase, -1, 0);

    //设置回调函数, 及回调函数的参数
    bufferevent_setcb(pBufEv, read_callback, write_callback, event_callback, NULL);

    struct sockaddr_in tSockAddr;
    memset(&tSockAddr, 0, sizeof(tSockAddr));
    tSockAddr.sin_family = AF_INET;
    tSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    tSockAddr.sin_port = htons(32000);

    //连接服务器
    if( bufferevent_socket_connect(pBufEv, (struct sockaddr*)&tSockAddr, sizeof(tSockAddr)) < 0)
    {
        return 0;
    }
    //开始事件循环
    event_base_dispatch(pEventBase);
    //事件循环结束 资源清理
    bufferevent_free(pBufEv);
    event_base_free(pEventBase);
    return 0;
}
