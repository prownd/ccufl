#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "util.h"
#include "error.h"
#include "conn.h"

#define BACKLOG 10

int conn_open(const char *ip, uint16_t port, int mask)
{
	int fd;
	struct sockaddr_in addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1){
		//TODO:
		return -1;
	}

	if(mask & CONN_REUSE){
		set_reuse(fd);
	}

	if(mask & CONN_NONBLOCK){
		set_nonblocking(fd);
	}

	if(mask & CONN_ALIVE){
		set_keepalive(fd);
	}

	if(mask & CONN_DEFER){
		set_defer(fd);
	}

	bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	inet_aton(ip, &addr.sin_addr);
	addr.sin_port = htons(port);

	if(bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0){ 
		//TODO:
		close(fd);
		return -1; 
	}   

	if(listen(fd, BACKLOG)){
		//TODO:
		perror("listen");
		close(fd);
		return -1; 
	}   

	return fd;
}

int conn_peer(const char *ip, uint16_t port, int mask)
{
	int fd, status;
	socklen_t socklen;
	struct sockaddr_in addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1){
		//TODO:
		return -1;
	}
	
	if(mask & CONN_REUSE){
		set_reuse(fd);
	}

	if(mask & CONN_NONBLOCK){
		set_nonblocking(fd);
	}

	if(mask & CONN_ALIVE){
		set_keepalive(fd);
	}

	bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	inet_aton(ip, &addr.sin_addr);
	addr.sin_port = htons(port);

	socklen = sizeof(struct sockaddr_in);
	if(mask & CONN_NONBLOCK){
		status = connect(fd, (struct sockaddr *)&addr, socklen);
		if(status == -1 && errno != EINPROGRESS){
			close(fd);
			return -1;
		}
	}else{
		status = connect(fd, (struct sockaddr *)&addr, socklen);
		if(status == -1){
			close(fd);
			return -1;
		}
	}

	return fd;
}

int conn_check(int fd)
{
	int error;
	socklen_t len;

	if(fd < 0)
		return -1;

	error = 0;
	len = sizeof(int);

	if(getsockopt(fd, SOL_SOCKET, SO_ERROR, (void *)&error, &len) == -1){
		//TODO:
		return -1;
	}

	if(error){
		//TODO:
		return -1;
	}

	return 0;
}

int conn_cycle(){
	int fd=-1;
	while(fd == -1){
        	fd = conn_peer(g_config.dip, g_config.dport, CONN_REUSE|CONN_DEFER);    
        	//fd = conn_peer(g_config.dip, g_config.dport, CONN_NONBLOCK|CONN_REUSE);    
        	if(fd == -1){
        	     //log_warn(cycle->log, "create listen fd error!\n");
        	      sleep(1);
        	}
    } 
	return fd;
}

int conn_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
	int val;

again:
	val = select(nfds, readfds, writefds, exceptfds, timeout);
	if(val < 0){
		if(errno == EINTR){
			if(timeout != NULL){
			}
			goto again;
		}
	}

	if(val == 0){
		errno = ETIMEDOUT;
	}

	return val;
}

int conn_readable_tv(int fd, struct timeval *tv)
{
	fd_set fs;

	FD_ZERO(&fs);
	FD_SET(fd, &fs);

	return (conn_select(fd+1, &fs, NULL, NULL, tv));
}

int conn_read_tv(int fd, void *buf, size_t nbytes, struct timeval *tv)
{
	int err;

	err = conn_readable_tv(fd, tv);
	if(err < 0){
		return -3;
	}else if(err == 0){
		return -2;
	}else{
		return read(fd, buf, nbytes);
	}

	return 0;
}

ssize_t conn_read_ex(int fd, void *buf, size_t nbytes, int msec)
{
	ssize_t nread;
	size_t nleft = nbytes;
	char *pos = buf;
	struct timeval tv;
	struct timeval *tp;

	if(msec < 0){
		tp = NULL;
	}else{
		ms2tv(msec, &tv);
		tp = &tv;
	}

	while(nleft > 0)
	{
again:
		nread = conn_read_tv(fd, pos, nleft, tp);
		if(nread < 0){
			if(nread==-1 && errno==EINTR){
				goto again;
			}else if(nread == -2){
				errno = ETIMEDOUT;
				return -1;
			}else if(nread == -3){
				errno = EIO;
				return -1;
			}else{
				return -1;
			}
		}else if(nread == 0){
			break;
		}else{
			pos += nread;
			nleft -= nread;
		}
	}

	return (ssize_t)(nbytes - nleft);
}

int conn_read(int fd, void *buf, size_t len, int timeout)
{
	int rlen;

	rlen = conn_read_ex(fd, buf, len, timeout);
	//fprintf(stderr,"[recv len]:%d\n",rlen);
	if(rlen == 0){
		return CF_RET_PEERCLOSE;
	}else if(rlen==-1 && errno==ETIMEDOUT){
		return CF_RET_ETIMEDOUT;
	}else if(rlen != (int)len){
		return CF_RET_READ;
	}

	return CF_RET_SUCCESS;
}

int conn_writeable_tv(int fd, struct timeval *tv)
{
	fd_set fs;

	FD_ZERO(&fs);
	FD_SET(fd, &fs);

	return (conn_select(fd+1, NULL, &fs, NULL, tv));
}

int conn_write_tv(int fd, void *buf, size_t nbytes, struct timeval *tv)
{
	int err;

	err = conn_writeable_tv(fd, tv);
	if(err < 0){
		return -1;
	}else if(err == 0){
		return -2;
	}

	return write(fd, buf, nbytes);
}

ssize_t conn_write_ex(int fd, void *buf, size_t nbytes, int msec)
{
	int flag;
	ssize_t nwrite;
	size_t nleft = nbytes;
	char *pos = buf;
	struct timeval tv;
	struct timeval *tp;

	if(msec < 0){
		tp = NULL;
	}else{
		ms2tv(msec, &tv);
		tp = &tv;
	}

	flag = get_sockflag(fd);
	if(flag < 0){
		return -1;
	}

	if(!(flag & O_NONBLOCK)){
		if(set_nonblocking(fd) < 0){
			return -1;
		}
	}

	while(nleft > 0)
	{
		nwrite = conn_write_tv(fd, pos, nleft, tp);
		if(nwrite==-1 && errno==EINTR){
			continue;
		}
		if(nwrite <= 0){
			if(nwrite == -2){
				errno = ETIMEDOUT;
			}

			if(!(flag & O_NONBLOCK)){
				set_blocking(fd);
			}

			return -1;
		}

		nleft -= nwrite;
		pos += nwrite;
	}

	if(!(flag & O_NONBLOCK)){
		set_blocking(fd);
	}

	return (ssize_t)(nbytes - nleft);
}

int conn_write(int fd, void *buf, size_t len, int timeout)
{
	int wlen;

	wlen = conn_write_ex(fd, buf, len, timeout);
	if(wlen == 0){
		return CF_RET_PEERCLOSE;
	}else if(wlen==-1 && errno==ETIMEDOUT){
		return CF_RET_ETIMEDOUT;
	}else if(wlen != (int)len){
		return CF_RET_WRITE;
	}

	return CF_RET_SUCCESS;
}

int conn_recv(conn_t *c, int msec)
{
	return 0;
}

int conn_send(conn_t *c, int msec)
{
	return 0;
}
