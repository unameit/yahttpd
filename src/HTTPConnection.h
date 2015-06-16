#ifndef	HTTPCONNECTION_H
#define	HTTPCONNECTION_H

#include <netinet/in.h>			// private: struct sockaddr_in peer;
#include "AsyncConnection.h"
#include "AsyncSocket.h"
#include "HTTPRequest.h"

class Properties;
class File;

/**
 * Klasa implementuj�ca interfejs AsyncConnection i realizuj�ca po��czenie z
 * klientem (cze�ci� obiektu HTTPConnection jest obiekt AsyncSocket).
 * Zajmuje si� tylko odbieraniem (i zapami�tywaniem) danych przes�anych od
 * klienta, oraz wysy�aniem odpowiedzi wygenerowanej przez obiekt
 * HTTPRequest. Obiekt tej klasy potrafi te� wys�a� plik wskazany przez
 * HTTPRequest'a.
 */

class HTTPConnection : public AsyncConnection {
public:
	/** Utw�rz obiekt. Zapami�taj tych par� rzeczy, kt�re Ci si� nied�ugo przydadz�. */
	HTTPConnection(const char *hostName, const Properties *mimeTypes,
		const char *documentRoot, const char *userDir);

	/** desktruktor */
	virtual ~HTTPConnection() {}

	/** inicjuj nowe po��czenie (na podanym deskryptorze) */
	HTTPConnection& operator=(int sockfd);

	/** Ustaw sobie adres IP 'drugiej strony' celem zapisania tej informacji w logu. */
	void setPeer(struct sockaddr_in *p) { peer = *p; }

private:
	/** socket - po��czenie ze �wiatem */
	AsyncSocket sock;

	/** tu zapami�tujemy adres klienta - w celu zapisu w logu */
	struct sockaddr_in peer;

	/** ten obiekt parsuje zapytanie przegl�darki i czyni odpowied� */
	HTTPRequest request;

	/** 32k - powinno wystarczy� */
	static const size_t bufferSize = 32 * 1024;

	/** bufor do odbierania i wysy�ania danych */
	char buffer[bufferSize];

	/** ile ju� odebrali�my/wys�ali�my */
	size_t nread, nwritten;

	/** ile trzeba b�dzie wys�a� bajt�w nag��wka */
	size_t responseLength;

	/** czy odebrali�my ju� kompletny request? */
	bool requestReceived;

	/** czy manager wie, �e po��czenie zako�czone? */
	bool finalized;

	/** Wska�nik do pliku, kt�ry od czasu do czasu zdarzy si� nam wys�a�. */
	File *file;

	/** metody, kt�re musz� by� 'przykryte' dla ConnectionsManager'a: */

	bool isReadNeeded() const { return !requestReceived && !finalized; }

	/** Odpowiedz na pytanie, czy ten obiekt jest w tym momencie zaineresowany zapisem. */
	bool isWriteNeeded() const { return requestReceived && !finalized; }

	/** Metoda wywo�ywana, gdy odczyt jest mo�liwy */
	void onReadable();

	/** Metoda wywo�ywana, gdy zapis jest mo�liwy */
	void onWriteable();

	/** Zwr�� deskryptor naszego po��czenia - czyli socket'u. */
	operator int() const { return (int) sock; }

	/** 'Zfinalizuj si�', tzn. odnotuj fakt, �e ConnectionsManager zauwa�y�, i� po��czenie zosta�o zako�czone i usun�� ten obiekt ze swojej tablicy. B�dzie wi�c mo�na po re-inicjalizacji poda� mu go znowu. */
	void finalize();

	/** Odpowiada na pytanie, czy obiekt jest ju� 'zfianlizowany'. */
	bool isFinalized() const { return finalized; }
};

#endif
