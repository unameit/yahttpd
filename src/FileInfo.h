#ifndef	FILEINFO_H
#define	FILEINFO_H

#include <time.h>		// time_t
#include <sys/types.h>	// off_t

// typedef	unsigned long	off_t;

/**
 * Obiekty tej klasy zawieraj� informacje na temat istniej�cych plik�w takie
 * jak: data ostatniej modyfikacji i rozmiar. Klasa pozwala na uzyskiwanie
 * tych informacji bez otwierania wskazanego pliku.
 */

class FileInfo {
public:
	/** Pobierz informacje na temat pliku, kt�rego nazw� Ci przekazuj� jako argument */
	FileInfo(const char *filename = NULL);

	/** Pobierz informacje na temat pliku, kt�rego deskryptor Ci przekazuj� jako parametr. */
	FileInfo(int fd);

	/** Pobierz informacje na temat pliku, kt�rego nazw� Ci przypisuj� */
	FileInfo& operator=(const char *filename);

	/** Pobierz informacje na temat pliku, kt�rego deskryptor Ci przypisuj�. */
	FileInfo& operator=(int fd);

	/** Podaj mi rozmiar tego pliku. */
	off_t getSize() const { return size; }

	/** Podaj mi czas ostatniej modyfikacji obiektu, o kt�rym przechowujesz informacj�. */
	time_t getMTime() const { return mtime; }

	/** Czy obiekt o kt�rym informacj� przechowujesz, jest katalogiem? */
	bool isDirectory() const { return dir; }
private:
	/** Tu zapami�tujemy rozmiar pliku w bajtach. */
	off_t	size;

	/** Tu zapami�tujemu czas ostatniej modyifikacji. */
	time_t	mtime;

	/** Tu, czy jest to katalog, czy plik. */
	bool	dir;

	/** Pobierz informacje o pliku wskazywanym przez nazw� podan� jako paramter. */
	void stat(const char *filename);

	/** Pobierz informacje o pliku wskazywanym przez ten deskryptor, kt�ry Ci przekazuj�, jako argument. */
	void stat(int fd);
};

#endif
