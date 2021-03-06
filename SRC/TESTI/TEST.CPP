#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <stdarg.h>
#include <iostream.h>

#include "int/int.h"
#include "ipx/ipx.h"
#include "ipxdefs.h"
#include "error/error.h"

#define MAX_PLAYERS 8

int ipxofs[MAX_PLAYERS] = 
{
	0, 0, 0, 0,
	0, 0, 0, 0
};

char ipxstream[MAX_PLAYERS][MAXDATASIZE];
struct nodeaddr serveraddr;
char servername[10];
struct packet far *send, far *rec[RECEIVERS];
unsigned short sendl, sendseg, recl[RECEIVERS], recseg[RECEIVERS];
int ipxavail;
int NETWORK_MODE=SERVER;
char name1[]="Hanunpeli";

int PLAYERS=1;

void allocipxmem() 
{
	int a;
	send = ( struct packet far * ) MK_FP( getlowmem( sizeof( struct packet )  / 16 + 1 ), 0 );
	if ( send == NULL ) 
	{
		cout << "Can't alloc lowmem!\n" << endl;
		ipxavail = 0;
		return;
	}
	sendl = lsel;
	if (sendl==0)
	{
			cout << "sendl check failed"<< endl;
			ipxavail = 0;
	}
	sendseg = lseg;
	if (sendseg==0)
	{
			cout << "sendseg check failed"<< endl;
			ipxavail = 0;
	}
	for ( a = 0; a < RECEIVERS; a ++  ) 
	{
		rec[a] = ( struct packet far * ) MK_FP( getlowmem( sizeof( struct packet )  / 16 + 1 ), 0 );
		if ( rec[a] == NULL ) 
		{
			cout << "Can't alloc lowmem!"<< endl;
			ipxavail = 0;
			return;
		}
		recl[a] = lsel;
		recseg[a] = lseg;
		if (recseg[a]==0)
		{
			cout << "recseg check failed ("<< a <<')'<< endl;
			ipxavail = 0;
		}
		if (recl[a]==0)
		{
			cout << "recl check failed ("<< a <<')'<< endl;
			ipxavail = 0;
		}

	}
}

void freeipxmem() 
{
	int a;
	if (ipxavail)
	{
		freelowmem( sendl );
		for ( a = 0; a < RECEIVERS; a ++  ) 
		{
			freelowmem( recl[a] );
			recseg[a] = lseg;
		}
	}
}

void init()
{
	if ( initipx()  ) 
	{
		char str[256];
		cout << "IPX protocol detected!\n";
		getlocaladdress();
		printf( "IPX address: %s\n", decode_localaddress( str, localadd )  );
		ipxavail = 1;
		allocipxmem();
	}
	else cout << "IPX _NOT_ detected!\n";
}
void flushipx() 
{
	int a,b;
	for ( a = 0; a < PLAYERS; a++  ) 
	if (ipxofs[a]!=0)
	{
		ipxstream[a][ ipxofs[a]++ ] = IPX_END;
		for (b=0;b<ipxofs[a];b++)
		 send->data[b]=ipxstream[a][b];
	    while (send->ecb.inuse);
		//sendpacket( sendseg,T�H�N NODE!!! , send, ( word ) ipxofs[a] );
 // 		RESET
		ipxofs[a] = 0;
	}
}

void sendipxnow( struct nodeaddr *node, int type, ... ) 
{
	int a = 0;
// 	TEMP VARIABLES 
	int b;
	struct nodeaddr n;
	char *str;
// 	TEMP VARIABLES END
	auto va_list ap;
	va_start( ap, type );

	while (send->ecb.inuse);
	switch( type ) 
	{
		case IPX_SERVERSEARCH:send->data[a++] = ( char ) type;
							  for (b=0;b<6;b++)
							   send->data[a++] = localadd->node.node[b];
							  break;
		case IPX_SERVERANSWER:send->data[a++] = ( char ) type;
							  n = va_arg( ap, struct nodeaddr );
					 		  for ( b = 0; b < 6; b++ ) 
					 		   send->data[a++ ] = n.node[b];
							  str = va_arg( ap, char * );
							  for ( b = 0; b < 10; b++ ) 
							  send->data[a++ ] = str[b];
							  break;
		case IPX_SERVERSHUTDOWN:send->data[a++] = ( char ) type;
							  n = va_arg( ap, struct nodeaddr );
					 		  for ( b = 0; b < 6; b++ ) 
					 		   send->data[a++ ] = n.node[b];
							  break;
		default:error( "IPX message %d not yet implemented\n", type );
	}

	send->data[a++] = IPX_END;
	sendpacket( sendseg, *node, send, ( word ) a );
	va_end( ap );
}

void sendipx( int dest, int type, ... ) 
{
	va_list ap;
	va_start( ap, type );
	switch( type ) 
	{
		case IPX_SERVERSEARCH:
		error( "sendipx: Use sendipxnow for serversearch\n" );
		break;
		case IPX_SERVERANSWER:
		error( "sendipx: Use sendipxnow for serveranswer\n" );
		break;
		default:error( "sendipx: Not implemented yet!\n" );
 /*		ipxstream[dest][ipxofs[dest] ++ ] = ( char ) type;
		n = va_arg( ap, struct nodeaddr node );
		for ( a = 0; a < 6; a ++  ) 
		ipxstream[dest][ipxofs[dest] ++ ] = n.node[a];
		break;*/ 
	}
	va_end( ap );
}

void setup_ipx() 
{
	int a;
	if ( opensocket( 0, SOCKET) ) 
		error( "setup_ipx: Couldn't open socket (%Xh)\n", SOCKET );
	for ( a = 0; a < MAX_PLAYERS; a ++  ) ipxofs[a] = 0;
	PLAYERS = 1;
	if ( NETWORK_MODE == SERVER )
	{
		strcpy( servername, name1 );
		for (a=0;a<6;a++)
		 serveraddr.node[a]=localadd->node.node[a];
	}
	initsendpacket(send,sendseg);
	for (a=0;a<RECEIVERS;a++)
	{
	 initreceivepacket(rec[a],recseg[a]);
	 listenforpacket(recseg[a]);
	}
}

void desetup_ipx() 
{	int a;

	for (a=0;a<RECEIVERS;a++)
	 if (rec[a]->ecb.inuse)
	  cancelpacket(recseg[a]);
	flushipx();
	closesocket( ( word ) socketid );
}

void do_server_shit() 
{
	int a,b, data, ofs;
	struct nodeaddr n;
	for ( a = 0; a < RECEIVERS; a ++  ) 
	if ( rec[a]-> ecb.inuse == 0 ) 
	{
		data =  - 1;
		ofs = 0;
		while( data!= IPX_END ) 
		{
			data = rec[a]-> data[ofs ++ ];
			switch ( data ) 
			{
				case IPX_SERVERSEARCH:
				for (b=0;b<6;b++)
				 n.node[b]=rec[a]->data[ofs++];
				sendipxnow( &n, IPX_SERVERANSWER,serveraddr,servername );
				break;

				case IPX_TEXTMESSAGE:
				while ( ( data = rec[a]-> data[ofs ++ ] ) != 0 );
				printf( "\n" );
				break;

				case IPX_END:
				break;
				
				default:error( "IPX: receiver %d: unknown command %d\n", a, data );
			};
		}
		listenforpacket( recseg[a] );
	}
	flushipx();
}

void do_client_shit() 
{

}

int select_server()
{
	int a, b, c,data,ofs,quit,done=0;
	int servers;
	char servernames[10][10],tempname[10];
	struct nodeaddr nodez[10],tempnode;
	struct nodeaddr bc;

	servers=0;
	bc=BROADCAST();

	while ( (!done)&&(!quit) ) 
	{
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		printf("\t\tSelect server\n\n");
		for (a=0;a<servers;a++)
		 printf("%d. %sservernames[a]",a);

	 	sendipxnow(&bc,IPX_SERVERSEARCH);
		for ( a = 0; a < RECEIVERS; a ++  ) 
		if ( !rec[a]->ecb.inuse) 
		{
			data =  - 1;
			ofs = 0;
			while( data!= IPX_END ) 
			{
				data = rec[a]-> data[ofs ++ ];
				switch ( data ) 
				{
					case IPX_SERVERANSWER:
					for (b=0;b<6;b++)
					 tempnode.node[b]=rec[a]->data[ofs++];
					for (b=0;b<6;b++)
					 tempname[b]=rec[a]->data[ofs++];
					if (servers<10)
					{
						for (b=0;(b<servers)&&(memcmp(&tempnode,&nodez[b],6)!=0);b++);
						if (b==servers)
						{
							memcpy(&nodez[servers],&tempnode,6);
							strcpy(servernames[servers],tempname);
							servers++;
						}
					}
					break;
														
					case IPX_SERVERSHUTDOWN:
					for (b=0;b<6;b++)
					 tempnode.node[b]=rec[a]->data[ofs++];
					for (b=0;b<servers&&memcmp(&tempnode,&nodez[b],6)!=0;b++);
					if (b!=servers)
					{
						servers--;
					    for (c=b;c<servers;c++)
						 { 
						 	memcpy(&nodez[c],&nodez[c+1],6);
							strcpy(servernames[c],servernames[c+1]);
						 }
					}
					break;

					case IPX_TEXTMESSAGE:
					while ( ( data = rec[a]-> data[ofs ++ ] ) != 0 );
					break;

					case IPX_END:
					break;
					default:ofs+=IPXMSGLEN[data];
				};
			}
			listenforpacket( recseg[a] );
		}
		if (kbhit()) if (getch()==27) return 0;
	}

	return 1;
}

