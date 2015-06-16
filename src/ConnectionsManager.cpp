#include <sys/time.h>				// struct timeval dla ::select(..)
#include <sys/types.h>				// uint
#include <unistd.h>					// ::select(..)
#include "SystemException.h"		// rzucamy wyj�tkiem, gdy select zwr�ci -1
#include "AsyncConnection.h"		// wcze�niej by�a tylko deklaracja zapowiadaj�ca, a my wywo�ujemy metody z tej klasy
#include "ConnectionsManager.h"		// definicja tej klasy


void ConnectionsManager::add(AsyncConnection *c)
{
	connections.add(c);
}


void ConnectionsManager::main()
{
	fd_set rfds, wfds;					// read and write file/socket descriptors
	uint i, rfds_count = 0, wfds_count = 0;
	int fd, max_fd = -1;
	AsyncConnection *c;

	while(connections.getCount() > 0) {
		// nigdy si� nie ko�czymy? normalnie nie, przynajmniej na razie.

		FD_ZERO(&rfds);
		FD_ZERO(&wfds);

		update();

		for(i = 0; i < connections.getCount(); ++i) {		// przele� wszystkie..

			c = (AsyncConnection *) connections[i];		// we� i-te po��czenie
			fd = (int) *c;						// we� jego deskryptor

			if ( c->isReadNeeded() ) {		// jesli to i-te po��czenie chce czyta�, to
				FD_SET(fd, &rfds);			// odnotuj to
				rfds_count++;				// i policz, ile ich jest (chc�cych czyta�)
				if (fd > max_fd)			// pami�taj maksymalny deskryptor - dla ::select(..)
					max_fd = fd;
			}

			if ( c->isWriteNeeded() ) {		// analogicznie, jesli chce zapisywa�...
				FD_SET(fd, &wfds);
				wfds_count++;
				if (fd > max_fd)
					max_fd = fd;
			}
		}

		// nie sprawdzamy, czy jest sens wchodzi� do select'a (tzn. czy jest
		// chocia� jedno po��czenie, kt�re chce zapisywa� lub odczytywa�),
		// gdy� wiemy, �e tym conajmniej jednym po��czeniem b�dzie gniazdo w
		// dziedzinie UNIXa - czyli kana� komunikacjyny z procesem-rodzicem.

		int rc = select(max_fd + 1, (rfds_count > 0? &rfds: NULL), (wfds_count > 0? &wfds: NULL), NULL, NULL);

		if (-1 == rc)
			throw SystemException("select() failed!");

		update();

		if (rfds_count > 0) {								// by�y jakie� do zapisu?
			for(i = 0; i < connections.getCount(); ++i) {	// to sprawd�my je
				c = (AsyncConnection *) connections[i];		// we� i-te po��czenie
				fd = (int) *c;								// we� jego deskryptor
				if (FD_ISSET(fd, &rfds))					// mo�na czyta�?
					c->onReadable();						// wywo�aj obs�ug� tej sytuacji
			}
		}

		update();

		if (wfds_count > 0) {								// by�y jakies do zapisu?
			for(i = 0; i < connections.getCount(); ++i) {	// to sprawd�my je
				c = (AsyncConnection *) connections[i];		// we� i-te po��czenie
				fd = (int) *c;								// we� jego deskryptor
				if (FD_ISSET(fd, &wfds))					// mo�na pisa�?
					c->onWriteable();						// wywo�aj obs�ug� tej sytuacji
			}
		}
	}
}


void ConnectionsManager::update()
{
	AsyncConnection *c;
	for(uint i = 0; i < connections.getCount(); ++i) {		// przele� wszystkie..
		c = (AsyncConnection *) connections[i];		// we� i-te po��czenie
		if (-1 == (int) *c) {				// je�li ju� nieaktywne, to
			c->finalize();					// wywo�aj pseudo-desktrukor, czyli niech obiekt wie, �e ju� zauwa�ylismy zako�czenie po��czenia
			connections.remove(i--);		// usu� go z tablicy
		}
	}
}
