#ifndef	EXCEPTION_H
#define	EXCEPTION_H

#include "SString.h"

/**
 * obiekty tej klasy s� wyj�tkami j�zyka C++ rzucanymi (jak nazwa wskazuje)
 * w sytacjach wyj�tkowych (np. niepowodzenia wywo�ania funkcji systemowej).
 * U�atwa to kontrol� sterowania programem oraz generowania tre�ciwych
 * komunikat�w o b��dach i miejscach ich wyst�pienia.
 */

class Exception {
public:
	/** Konstruktor zwyk�ego wyj�tku z kr�tkim opisem. */
	Exception(const char *details) { init(details); }

	/** Konstruktor wyj�tku z opcjonalnym rozbudowanym opisem - ��cznie z dodatkowymi argumentami. */
	Exception(const SString &details) { init(details); }

	/** daj mi tekst Ci� opisuj�cy.. */
	operator const char*() const { return string; }
protected:
	/** Chroniony konstruktor bez-parametrowy dla klas pochodnych. Innymi s�owy - nie pozw�l na istnienie obiekt�w tej klasy bez tre�ci. */
	Exception() {}

	/** Inicjuj ten obiekt */
	void init(const SString&);

	/** Podaj mi nazw� okre�laj�c� nazw� Twojej klasy */
	virtual const char *getName() const { return "Exception"; }

	/** Totaj trzymamy ci�g znak�w opisuj�cy ka�dy obiekt tej klasy. */
	SString	string;
private:
	;
};

#endif
