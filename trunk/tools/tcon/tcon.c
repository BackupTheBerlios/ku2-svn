/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/ioctl.h>

#define BUFSZ 256

#define ESC_RED( __txt ) "\033[0;31m"__txt"\033[0m"
#define ESC_GREEN( __txt ) "\033[0;32m"__txt"\033[0m"
#define ESC_BLUE( __txt ) "\033[0;34m"__txt"\033[0m"

#if 0
#define dprintf(...) printf(##__VA_ARGS__)
#else
#define dprintf(...)
#endif

enum
{
	FIN_NONE,
	FIN_ZERO,
	FIN_RN,
	FIN_R,
	FIN_N
}	opt_final = FIN_ZERO;
int opt_ohex = 0;
int opt_ihex = 0;
int aopt_hpline;

int sock;

void print_data_hex( int out, const char *data, int len )
{
	char tmp[4*aopt_hpline+1];
	int i, pos, tlen;
	
	for ( pos = 0; len > 0; pos += tlen, len -= tlen )
	{
		tlen = len > aopt_hpline ? aopt_hpline : len;
		for ( i = 0; i < tlen; i++ )
			sprintf(tmp+i*4, "%.2X%c ",
					(unsigned char)data[pos+i],
					isprint(data[pos+i]) ? data[pos+i] : '*');
		if ( out )
			printf(ESC_BLUE("HEX: %s")"\n", tmp); else
			printf(ESC_GREEN("HEX: %s")"\n", tmp);
	}
}

int tcon_connect( char *host, char *port )
{
	u_int16_t _port;
	struct sockaddr_in addr;
	struct hostent *p_he;
	char *ch;
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if ( sock == -1 )
	{
		fprintf(stderr, "Cannot create the socket: %s\n", strerror(errno));
		return -1;
	}
	
	_port = strtol(port, &ch, 10);
	if ( *ch != 0 )
	{
		fprintf(stderr, "Invalid port\n");
		close(sock);
		return -1;
	}
	
	p_he = gethostbyname(host);
	if ( p_he == NULL )
	{
		fprintf(stderr, "Invalid host\n");
		close(sock);
		return -1;
	}
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = *((int*)p_he->h_addr_list[0]);
	addr.sin_port = htons(_port);
	
	if ( connect(sock, (void*)&addr, sizeof(addr)) == -1 )
	{
		fprintf(stderr, "Cannot connect: %s\n", strerror(errno));
		close(sock);
		return -1;
	}
	
	return 0;
}

void tcon_disconnect( void )
{
	shutdown(sock, SHUT_RDWR);
	close(sock);
}

int tcon_write( int fd, const void *data, size_t len )
{
	ssize_t wrote;
	
	do
	{
		wrote = write(fd, data, len);
		if ( wrote == -1 )
		{
			fprintf(stderr,
					ESC_RED("write(%d) failed: %s")"\n", fd, strerror(errno));
			return -1;
		}
		if ( wrote == 0 )
		{
			fprintf(stderr, ESC_RED("write(%d) returned zero")"\n", fd);
			return -2;
		}
	}	while ( (len -= wrote) > 0 );
	
	return 0;
}

int go_on = 1, inp_cnt, cont = 0;
char in_buf[BUFSZ];
char *in_srcp = in_buf;
char out_buf[BUFSZ];
char *out_srcp = out_buf;
char tmp_buf[BUFSZ];
char *dstp;

int next_printable( char **p, int len )
{
	while ( (--len > 0) && !isprint((*p)[1]) ) (*p)++;
	return len > 0 ? 0 : -1;
}

int parse_output_message( void )
{
	for ( dstp = tmp_buf, out_srcp = out_buf;
		  (out_srcp-out_buf < inp_cnt) && (dstp-tmp_buf < BUFSZ);
		  dstp++, out_srcp++ )
	{
		if ( *out_srcp == '\\' )
		{
			if ( out_srcp-out_buf < inp_cnt-1 )
			{
				switch ( *(++out_srcp) )
				{
					case '0':
						*dstp = 0;
						break;
					case '\\':
						*dstp = '\\';
						break;
					case 'r':
						*dstp = '\r';
						break;
					case 'n':
						*dstp = '\n';
						break;
					case 'f':
					/*
					if ( (i >= 4) && (*tmp == '/') && (tmp[1] == 'f') )
					{
						// Writing from file
						int fd, r;
						
						tmp[i-1] = 0;
						printf("%s\n", tmp+2);
						fd = open(tmp+2, O_RDONLY);
						if ( fd == -1 )
						{
							fprintf(stderr, "Error opening a file: %s\n", strerror(errno));
							return EXIT_FAILURE;
						}
						while ( (r = read(fd, tmp, 2048)) > 0 )
							write(sock, tmp, r);
						close(fd);
					}	else
					
						
					}
					*/
						break;
					case 'q':
						go_on = 0;
						return 0;
					default:
						fprintf(stderr,
								ESC_RED("ERROR: invalid control sequence (%.2X%c)")"\n",
							   *out_srcp, isprint(*out_srcp) ? *out_srcp : '*');
						return -1;
				}
			}	else
				break;
		}	else
		if ( isprint(*out_srcp) )
		{
			*dstp = *out_srcp;
		}	else
		{
			// Inserting end of message:
			switch ( opt_final )
			{
				case FIN_NONE:
					break;
				case FIN_ZERO:
					*dstp = 0;
					break;
				case FIN_RN:
					if ( dstp-tmp_buf+1 >= BUFSZ )
					{
						cont = 1;
						goto __parse_output_end;
					}
					dstp[0] = '\r';
					dstp[1] = '\n';
					dstp++;
					break;
				case FIN_R:
					*dstp = '\r';
					break;
				case FIN_N:
					*dstp = '\n';
					break;
			}
			
			next_printable(&out_srcp, inp_cnt-(out_srcp-out_buf));
		}
	}
	
	__parse_output_end:
	memmove(out_buf, out_srcp, inp_cnt -= (out_srcp-out_buf));
	out_srcp = out_buf + inp_cnt;
	return 0;
}

int place_newline( void )
{
	*dstp = '\n';
	return 0;
}

int place_nonprint( void )
{
	if ( dstp-tmp_buf+3 >= BUFSZ )
	{
		dprintf(ESC_RED("\n===")"\n");
		cont = 1;
		return -1;
	}
	sprintf(dstp, "\\%.2X", (unsigned char)*in_srcp);
	*(dstp += 3) = '.';
	return 0;
}

int parse_input_message( void )
{
	dprintf(ESC_RED("@in_buf: %p, in_srcp: %p, minus: %d, inp_cnt: %d")"\n",
		   in_buf, in_srcp, (in_srcp-in_buf), inp_cnt);
	for ( dstp = tmp_buf, in_srcp = in_buf;
		  (in_srcp-in_buf < inp_cnt) && (dstp-tmp_buf < BUFSZ);
		  dstp++, in_srcp++ )
	{
		if ( isprint(*in_srcp) )
		{
			//if ( *in_srcp == '\\' )
			//{
			//	
			//}	else
				*dstp = *in_srcp;
		}	else
		switch ( *in_srcp )
		{
			case 0:
			{
				if ( opt_final == FIN_ZERO )
				{
					if ( place_newline() == -1 )
						goto __parse_input_end;
				}	else
				{
					if ( place_nonprint() == -1 )
						goto __parse_input_end;
				}
				break;
			}
			case '\r':
			{
				if ( opt_final == FIN_R )
				{
					if ( place_newline() == -1 )
						goto __parse_input_end;
				}	else
				if ( (opt_final == FIN_RN) && \
					 (in_srcp-in_buf < inp_cnt-1) && \
					 (in_srcp[1] == '\n') )
				{
					in_srcp++;
					if ( place_newline() == -1 )
						goto __parse_input_end;
				}	else
				{
					if ( place_nonprint() == -1 )
						goto __parse_input_end;
				}
				break;
			}
			case '\n':
			{
				if ( opt_final == FIN_N )
				{
					if ( place_newline() == -1 )
						goto __parse_input_end;
				}	else
				{
					if ( place_nonprint() == -1 )
						goto __parse_input_end;
				}
				break;
			}
			default:
			{
				if ( place_nonprint() == -1 )
					goto __parse_input_end;
			}
		}
	}
	
	__parse_input_end:
	dprintf(ESC_RED("inp_cnt: %d")"\n", inp_cnt);
	inp_cnt -= (in_srcp-in_buf);
	dprintf(ESC_RED("in_buf: %p, in_srcp: %p, minus: %d, inp_cnt: %d")"\n",
		   in_buf, in_srcp, (in_srcp-in_buf), inp_cnt);
	memmove(in_buf, in_srcp, inp_cnt);
	in_srcp = in_buf + inp_cnt;
	return 0;
}

int loop( void )
{
	while ( go_on )
	{
		fd_set rset;
		
		FD_ZERO(&rset);
		FD_SET(0,&rset);
		FD_SET(sock,&rset);
		if ( select(sock+1, &rset, NULL, NULL, NULL) != -1 )
		{
			if ( FD_ISSET(0, &rset) ) // Human input:
			{
				inp_cnt = read(0, out_srcp, BUFSZ-(out_srcp-out_buf));
				if ( inp_cnt == -1 )
				{
					fprintf(stderr, ESC_RED("Error keyboard reading: %s")"\n",
							strerror(errno));
					return -1;
				}
				inp_cnt += out_srcp-out_buf;
				
				__parse_output_again:
				if ( parse_output_message() == -1 )
					return -1;
				
				if ( !go_on )
					return 0;
				
				if ( opt_ohex )
					print_data_hex(1, tmp_buf, dstp-tmp_buf);
				switch ( tcon_write(sock, tmp_buf, dstp-tmp_buf) )
				{
					case -1:
						return -1;
					case -2:
						return 0;
				}
				if ( cont )
				{
					cont = 0;
					goto __parse_output_again;
				}
			}
			if ( FD_ISSET(sock,&rset) ) // Socket input
			{
				inp_cnt = read(sock, in_srcp, BUFSZ-(in_srcp-in_buf));
				if ( inp_cnt == -1 )
				{
					fprintf(stderr, ESC_RED("Error socket reading: %s")"\n",
							strerror(errno));
					return -1;
				}
				if ( inp_cnt == 0 )
				{
					printf(ESC_GREEN("Connection is closed by other side")"\n");
					return 0;
				}
				dprintf(ESC_RED("\n#inp_cnt: %d, diff: %d")"\n", inp_cnt, (in_srcp-in_buf));
				if ( opt_ihex )
					print_data_hex(0, in_srcp, inp_cnt);
				
				__parse_input_again:
				if ( parse_input_message() == -1 )
					return -1;
				
				switch ( tcon_write(1, tmp_buf, dstp-tmp_buf) )
				{
					case -1:
						return -1;
					case -2:
						return 0;
				}
				if ( cont )
				{
					dprintf(ESC_RED("\n-----")"\n");
					cont = 0;
					goto __parse_input_again;
				}
			}
		}
	}
	
	return 0;
}

void get_aopt_lines( void )
{
	//char *env, *ch;
	//long int lines;
	struct winsize ws;
	
	if ( ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1 )
	{
		aopt_hpline = 10;
	}	else
	{
		aopt_hpline = (ws.ws_col-10)/4;
	}
	
	/*
	env = getenv("COLUMNS");
	printf("env: %p\n", env);
	if ( env == NULL )
	{
		aopt_hpline = 10;
	}	else
	{
		printf("env: %s\n", env);
		lines = strtol(env, &ch, 10);
		if ( *ch != 0 )
		{
			aopt_hpline = 10;
		}	else
		{
			aopt_hpline = (lines-10)/4;
		}
	}
	*/
}

int main( int argc, char *argv[] )
{
	int i;
	
	get_aopt_lines();
	
	if ( argc < 3 )
	{
		fprintf(stderr,"Usage: %s [host] [port] [options]\n" \
				"Options:\n" \
				"'-hex` to print hex dump for every message,\n" \
				"'-f{none,zero,rn,r,n}` to set final message suffix.\n", \
				argv[0]);
		return EXIT_FAILURE;
	}
	for ( i = 3; i < argc; i++ )
	{
		if ( !strcmp(argv[i], "-hex") )
			opt_ohex = opt_ihex = 1; else
		if ( !strcmp(argv[i], "-ohex") )
			opt_ohex = 1; else
		if ( !strcmp(argv[i], "-ihex") )
			opt_ihex = 1; else
		if ( !strcmp(argv[i], "-fnone") )
			opt_final = FIN_NONE; else
		if ( !strcmp(argv[i], "-fzero") )
			opt_final = FIN_ZERO; else
		if ( !strcmp(argv[i], "-frn") )
			opt_final = FIN_RN; else
		if ( !strcmp(argv[i], "-fr") )
			opt_final = FIN_R; else
		if ( !strcmp(argv[i], "-fn") )
			opt_final = FIN_N; else
		{
			fprintf(stderr, "Invalid option: %s\n", argv[i]);
			return EXIT_FAILURE;
		}
	}
	
	if ( tcon_connect(argv[1], argv[2]) == -1 )
		return EXIT_FAILURE;
	printf("Connected, session is started:\n\n");
	
	if ( loop() == -1 )
	{
		tcon_disconnect();
		return EXIT_FAILURE;
	}
	
	tcon_disconnect();
	return EXIT_SUCCESS;
}
