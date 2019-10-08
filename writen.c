

ssize_t writen( int fd,const void *buff,size_t nbytes )
{
	size_t nleft;
	ssize_t nwrite;
	const char *ptr;
	
	ptr = buff;
	nleft = nbytes;
	while( nleft >  0 ){
		if( (nwrite = write(fd,buff,nleft)) <=0 ){
			if( nwrite<0&&errno == EINTR )
				nwrite = 0;
			else
				return -1;
		}
		nleft -= nwrite;
		ptr += nwrite;
	}
	return nbytes;
	
}
