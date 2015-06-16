#ifndef	SYSTEMEXCEPTION_H
#define	SYSTEMEXCEPTION_H

#include "Exception.h"

/**
 * Klasa pochodna klasy Exception zapewniaj�ca dodatkowo komunikaty o
 * b��dach pochodz�ce bezpo�rednio od systemu operacyjnego (na podstawie
 * zmiennej errno i funkcji strerror).
 */

class SystemException : public Exception {
public:
	/** Zwyk�y konstruktor. */
	SystemException(const SString &details) : Exception() { init(details); addErrnoMsg(); }
protected:
	/** Konstruktor bez-parametr�w chroniony: tylko dla klas pochodnych */
	SystemException() {}

	/** Metoda dodaj�ca systemowy opis b��du. */
	void addErrnoMsg();

	/** Podaj mi nazw� klasy obiekt�w, do kt�rej nale�ysz */
	virtual const char *getName() const { return "SystemError"; }
};

#endif
