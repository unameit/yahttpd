#ifndef	CHILDTHREAD_H
#define	CHILDTHREAD_H

#include "AsyncConnection.h"
#include "ConnectionsManager.h"
#include "AsyncMessageQueue.h"
#include "Vector.h"
#include "SystemException.h"

class Properties;

/**
 * Obiekt tej klasy odpowiedzialny s� za przyjmowanie nowych po��cze� od
 * procesu-rodzica, tworzenie z nich obiekt�w HTTPConnection i obs�ug�
 * tych�e.  Jego cz�ci� jest obiekt ConnectonsManager, kt�ry "za�atwia ca��
 * robot�" z po��czeniami.
 */


class ChildThread : public AsyncConnection {
public:
	/** Standardowy konstruktor. */
	ChildThread(int sockfd, const char *hostName, const Properties *mimeTypes,
		const char *documentRoot, const char *userDir);

	/** Standardowy (wirtualny) desktruktor). */
	virtual ~ChildThread();

	/** g��wna funkcja procesu-dziecka. Tu nale�y odda� sterowanie. */
	void main() { manager.main(); }

private:
	/** Manager po��cze� odwalaj�cy de facto ca�� czarn� robot�. */
	ConnectionsManager manager;

	/** nasze ��cze komunikacyjne z procesem-ojcem. */
	AsyncMessageQueue msgq;

	/** tu zapami�tujemy nasze aktywne-otwarte po��czenia z klientam. */
	Vector openedConnections;

	/** tu mamy po��czenia ju� zako�czone - gotowe go re-inicjalizacji (dzi�ki nim unikamy kosztowengo 'new' i 'delete'). */
	Vector closedConnections;

	/** warto�� int, kt�r� nale�y zaraz wys�a� do procesu-rodzica. */
	int intToSend;

	/** Wska�niki do obiekt�w, kt�re musimy przekazywa� nowo-tworzonym obiektom HTTPconnection. */
	const char *const hostName;
	const Properties *const mimeTypes;
	const char *const documentRoot;
	const char *const userDir;

	/** metoda uaktualnia tablic� aktywnych i nieaktywnych po��cze� */
	void updateConnections();

	/** methods inherited from AsyncConnection */

	bool isReadNeeded() const { return true; }
	bool isWriteNeeded() const { return -1 != intToSend; }

	void onReadable();
	void onWriteable();

	operator int() const { return (int) msgq; }

	void finalize() { throw SystemException("[child] AsyncMessageQueue failed!"); }
};

#endif
