#ifndef	CONNECTIONTOCHILD_H
#define	CONNECTIONTOCHILD_H

#include <netinet/in.h>				// sockaddr_in
#include "AsyncConnection.h"		// inherited
#include "AsyncMessageQueue.h"		// private member
#include "SystemException.h"		// thrown in finalize()
#include <sys/types.h>				// uint

/**
 * Ka�dy obiekt tej klasy zajmuje si� komunikacj� mi�dzy procesem-rodzicem a
 * jednym procesem-dzieckiem, po stronie procesu-ojca. Potrafi przekaza�
 * dziecku nowe po��czenie (deskryptor gniazda przekazany za pomoc� obiektu
 * AsyncMessageQueue, kt�ry jest jego cz�ci�) oraz odebra� informacj� o
 * aktualnej ilo�ci otwartych po��cze� procesu-dziecka. To on nie pozwala,
 * by proces-dziecko mia�o wi�ksz� ni� dozwolona w pliku konfiguracyjnym
 * ilo�� jednocze�nie otwartych po��cze�.
 */

class ConnectionWithChild : public AsyncConnection {
public:
	/** Standardowy konstruktor, kt�remu podajemy przede wszystkim deskryptor ��cza komunikacyjnego do procesu-dziecka. */
	ConnectionWithChild(int sockfd, int maxChildConnections);
	virtual ~ConnectionWithChild() {}

	/** metoda podaje aktualn� liczb� aktywnych po��cze� dziecka, z kt�rym jest po��czony */
	uint getConnectionsCount() const { return openConnections; }

	/** wy�lij deskryptor do dziecka, albo nie, je�li poprzedni jeszcze nie wys�any */
	bool sendSocket(int sockfd, const struct sockaddr_in *peer);

private:
	/** nasze ��cze komunikacyjne do procesu-dziecka */
	AsyncMessageQueue msgq;

	/** liczba otwartych po��cze� procesu-dziecka */
	uint openConnections;

	/** maksymalan dozwolona liczba po��cze� procesu-dziecka */
	const uint maxConnections;

	/** obiekt Socket oczekuj�cy a wys�anie. */
	Socket socketToSend;

	/** adres klienta (po drugiej stronie socket'u) - wy�lemy go dziecku */
	struct sockaddr_in peer;

	/** metody przys�aniane z AsyncConnection dla ConnectionsManager'a */

	/** zawsze chcemy odbiera� to, co dziecko nam wysy�a */
	bool isReadNeeded() const { return true; }

	/** ale sami wysy�amy tylko wtedy, gdy rzeczywi�cie mamy co� do wys�ania */
	bool isWriteNeeded() const { return -1 != socketToSend; }

	void onReadable();
	void onWriteable();

	operator int() const { return (int) msgq; }

	void finalize() { throw SystemException("Connection with child has been closed!!"); }
};

#endif
