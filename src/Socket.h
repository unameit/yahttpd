#ifndef	SOCKET_H
#define	SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include "Descriptor.h"

#ifndef	__linux__
typedef	int	socklen_t;
#endif

/**
 * Obiekty tej klasy to standardowe gniazda (socket'y) systemu UNIX(R).
 * Klasa rozszerza mo�liwo�ci klasy Descriptor o metody specyficzne dla
 * deskryptor�w gniazd (takie jak: connect, bind, listen, accept).
 */

class Socket : public Descriptor {
public:
	/** Otw�rz obiekt Socket, ale go ie otwieraj */
	Socket() {}

	/** Utw�rz obiekt Socket, z deskryptora, kt�ry Ci daj�. */
	Socket(int i) : Descriptor(i) {}

	/** Utw�rz Socket ze wskazanej rodziny i podanego typu */
	Socket(int domain, int type, int protocol = 0);

	/** Po��cz to gniazdo z podanym komputerem, na podanym porcie. */
	void connect(const char *hostName, ushort port);

	/** Po��cz to gniazdo z komputerem, kt�rego adres podany jest w tej strukturze. */
	void connect(const struct sockaddr_in *addr);

	/** Zr�b bind tego Socket'a, czyli przypisz go do interfejsu sieciowego wskazywanego podan� nazw� komputera oraz do portu o podanym numerze. */
	void bind(const char *hostName, ushort port);

	/** Przypisz to gniazdo do adresu podanego w tej strukturze danych przekazanej jako argument. */
	void bind(const struct sockaddr_in *addr);

	/** Akceptuj po��czenie ze �wiata. Zwr�� mi deskryptor nowo utworzonego w tym celu gniazda. */
	int accept();

	/** Akceptuj po��czenie ze �wiata. Adres maszyny, sk�d pochodzi po��czenie zapisz do struktury przekazywanej jako parametr. */
	int accept(struct sockaddr_in *);

	/** Pozw�l na backlog oczekuj�cych po��cze� w kolejce na zaakceptowanie przez nas. */
	void listen(int backlog);

	/** Podany deskryptor niech b�dzie nowym moim. */
	Socket& operator=(int);
private:
	/** Zapytaj DNSa o IP tego komputera i wpisz mi to do struktury sockaddr_in. */
	void convert(struct sockaddr_in *addr, const char *hostName, ushort port) const;
};

#endif
