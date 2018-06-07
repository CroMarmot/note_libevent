all: select_s select_socket_s

select_s:
	gcc -o select_s select_s.c

select_socket_s:
	gcc -o select_socket_s select_socket_s.c

clean:
	rm -f bufferevent_sample epoll_c epoll_reactor epoll_s sample_s select_s select_socket_s test

