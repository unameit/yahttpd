#ifndef	SSTRING_H
#define	SSTRING_H

#include <string.h>

/**
 * Obiekty tej klasy u�atwiaj� operacje na ci�gach znak�w, poprzez
 * zdefiniowanie kilku operator�w oraz posiadanie statycznego bufora o
 * wielko�ci 512 bajt�w. Potrafi� te� formatowa� tekst tak jak czyni to np.
 * funkcja sprintf.
 */

class SString {
public:
	/** Konstruktor bez parametr�w. Nic nie inicjuj. */
	SString();

	/** Utw�rz obiekt zgodnie z podanym formatem (identycznie jak funkcji printf). */
	SString(const char *fmt, ...);

	/** Skopiuj nowy tekst. */
	SString& operator=(const char *str);

	/** Do��cz tekst. */
	SString& operator+=(const char *str);

	/** Daj mi ten tekst, kt�ry przechowujesz. */
	operator const char*() const;
private:
	/** Decydujemy si� na bufor sta�ej wielko�ci. */
	static const int BUF_LEN = 512;

	/** Tu przechowujemy ci�g znak�w dla obiekt�w tej klasy. */
	char s[BUF_LEN];
};

#endif
