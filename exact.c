// $Id: exact.c,v 1.3 2003/01/22 18:18:49 doug Exp $
//

#include <stdio.h>
#include <sys/types.h>
#include <regex.h>

#include "tail.h"
#include "debugmsg.h"
#include "match.h"
#include "conffile.h"

int onepass() {
	int i;
	int start=0;
	match_login *l;

	tail_read();
	for(i=0;i<tail_bufflen;i++) {
		if(tail_buff[i]=='\n' || tail_buff[i]=='\0') {
			tail_buff[i]='\0';
			l=match_line(tail_buff+start);
			if(l) {
				debugmsg(DMSG_STANDARD, "User %s authenticated at %s\n",
					l->username, l->ip);
					auth_add(l->username, l->ip);				
			}
			start=i+1;
		}
	}
	return 0;
}

int main(int argc, char *argv[]) {
	conffile_read("exact.conf");
	match_init();
	if(!tail_open(conffile_param("maillog"))) {
		debugmsg(DMSG_STANDARD,"Tail open failed.  Quitting.\n");
		return 2;
	}
	while(1) {
		onepass();
	}
	return 0;
}

