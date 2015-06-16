#ifndef	PARENTTHREAD_H
#define	PARENTTHREAD_H

#include <netinet/in.h>			// sockaddr_in
#include "AsyncConnection.h"
#include "ConnectionsManager.h"
#include "AsyncSocket.h"
#include "SystemException.h"

class ConnectionWithChild;

/**
 * Obiekt tej klasy, istniej�cy tylko w procesie-rodzica, odpowiedzialny
 * jest za przyjmowanie nowych po��cze� od klient�w oraz przekazywanie ich
 * procesom-dzieciom za pomoc� obiekt�w ConnectionWithChild.  Klasa
 * implementuje interfejs AsyncConnection, do obs�ugi asynchronicznego
 * gniazda po��czeniowego, na kt�rym nas�uchuje w oczekiwaniu na nowe
 * po��czenia "ze �wiata".
 */

class ParentThread : public AsyncConnection {
public:
	/** Standardowy konstruktor */
	ParentThread(AsyncSocket *socket, ConnectionWithChild **children, int maxChildren);

	/** Standardowy (wirtualny) destruktor. */
	virtual ~ParentThread() {}

	/** tu oddajemy sterowanie w procesie-rodzica */
	void main() { manager.main(); }

private:
	/** manager naszych po��cze� */
	ConnectionsManager manager;

	/* obiekt AsyncSocket na kt�rym nas�uchujemy */
	AsyncSocket *serverSocket;

	/** Wska�nik do tablicy ze wska�nikami do obiekt�w ConnectionWithChild */
	ConnectionWithChild **children;

	/** liczba proces�w-dzieci jak� posiadamy */
	const int maxChildren;

	/** tu pami�tamy deskryptor gniazda, kt�re zaraz wy�lemy procesowi-dziecku */
	int socketfdToSend;

	/** tu pami�tamy adres klienta, kt�ry pode�lemy te� dziecku */
	struct sockaddr_in peer;

	/** metoda zwraca wska�nik do proponowanego procesu-dziecka, kt�re powinni�my obarczy� nast�pnym zadaniem. */
	ConnectionWithChild *getChild() const;

	// metody dla ConnectionsManager'a

	bool isReadNeeded() const { return true; }
	bool isWriteNeeded() const { return false; }

	void onReadable();

	operator int() const { return (int) *serverSocket; }

	void finalize() { throw SystemException("serverSocket has been closed!!"); }
};

#endif
