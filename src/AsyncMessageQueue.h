#include <netinet/in.h>		// sockaddr_in
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include "AsyncSocket.h"

/**
 * Klasa ta (dziedzicz�ca prywatnie klas� AsyncSocket) s�u�y przede
 * wszystkim do komunikacji mi�dzyprocesowej. Opr�cz zwyk�ych danych pozwala
 * tak�e przekazywa� otwarte deskryptory plik�w i gniazd (socket�w) innym
 * procesom. Ze wzgl�du na potrzeby tego projektu oraz wymagane jej cechy
 * zosta�a ona zrealizowana na ��czu komunikacyjnym nieblokuj�cym
 * (asynchronicznym socket'cie w dziedzinie UNIXa).
 */

class AsyncMessageQueue : private AsyncSocket {
public:
	/**
     * Jako argument MUSI by� podany deskryptor gniazda z dziedziny UNIXa -
	 * inne nie potrafi� przekazywa� �ywych deskryptor�w.
     */

	AsyncMessageQueue(int sockfd) : AsyncSocket(sockfd) { init(); }
	~AsyncMessageQueue() {}

	/** Wy�lij deskryptor podany jako argument. */
	void sendDescriptor(int, const struct sockaddr_in *peer);

	/* Odbierz deskryptor. */
	int recvDescriptor(struct sockaddr_in *peer);

	/** Wy�lij liczb� int. */
	void sendInt(int i) { AsyncSocket::Socket::write(&i, sizeof i, true); }

	/** Odbierz liczb� int. */
	int recvInt() { int i; AsyncSocket::Socket::read(&i, sizeof i, true); return i; }

	/** Przypisz temu obiektowi inny, wskazywany podanym deskryptorem. */
	virtual AsyncMessageQueue& operator=(int);

	/** Podaj mi sw�j deskryptor gniazda. */
	operator int() const { return fd; }

	/** Upubliczniamy metod� close(), gdy� zosta�a ona wcze�niej schowana z powody prywatnego dziedziczenia. */
	AsyncSocket::close;

protected:
	/** udost�pniamy fd innym wy�szym klasom (bo go schowali�my przez ": private AsyncSocket") */
	AsyncSocket::fd;	

private:

	/** Struktury i zmienne potrzebne dla wywo�a� funkcji systemowych. */
#ifndef	OLD_MSGHDR
	struct {
		struct cmsghdr cmsg;
		int fd;
	} mymsg;
#endif
	struct iovec iov[1];
	struct msghdr msg;

	void init();
};
