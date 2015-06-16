#ifndef	DESCRIPTOR_H
#define	DESCRIPTOR_H

#include <stddef.h>
#include <sys/uio.h>
#include <sys/types.h>

/**
 * Klasa ta obudowuje klasyczne UNIXowe funkcje operuj�ce na deskryptorach w
 * klas� j�zyka C++, by upro�ci� korzystanie z nich, a jednocze�nie zapewni�
 * lepsz� obs�ug� sytuacji awaryjnych (niepowodze� wywo�a� tych�e funkcji
 * systemowych) poprzez rzucanie wyj�tk�w j�zyka C++, a przez to u�atwienie
 * sterowania wykonywaniem programu. Metody tej klasy s� wsp�lne dla plik�w
 * jak i gniazd.
 */


class Descriptor {
protected:
	/** Chroniony konstruktor  - niech nie istnieja obiekty tej klasy tworzone przez inaczej, ni� w klasach pochdnych. */
	Descriptor(int newfd = -1) { fd = newfd; }

	/** Zr�b z tego deskryptora, deskryptor asynchroniczny. */
	void makeAsync();

public:
	/** Przypisz temu obiektowi ca�kiem nowy deskryptor. */
	virtual Descriptor& operator=(int);

	/** Standardowy destruktor. */
	virtual ~Descriptor() { close(); }

	/** Zamknij deskryptor. Zwykle oznacza to tak�e zapisanie danych przetrzymywanych nadal w buforach systemowych. */
	void close();

	/** Odczytaj jeden znak. */
	int read();

	/** Odczytaj nbytes znak�w do bufora wskazywanego przez buffer. Je�li ostatni argument b�dzie true, to rzu� wyj�tkiem EOFException, je�li nie uda�o Ci si� wczyta� ca�ej ��danej porcji danych. */
	size_t read(void *buffer, size_t nbytes, bool full = true);

	/** Podobnie jak wy�ej, lecz dla tablicy bufor�w iovec[] */
	size_t readv(const struct iovec *iov, size_t count, bool full = true);

	/** Zapisz jeden znak. */
	void write(u_char);

	/** Zapisz bufor. Parametry analogicznie, jak przy read. */
	size_t write(const void *buffer, size_t, bool full = true);

	/** Podobnie jak wy�ej. */
	size_t writev(const struct iovec *iov, size_t count, bool full = true);

	/** Zwr�� mi sw�j UNIXowy deskryptor. */
	inline operator int() const { return fd; }

protected:
	/** I tu jest ten nasz deskryptor. */
	int fd;
};

#endif
