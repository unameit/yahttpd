#ifndef	PROPERTIES_H
#define	PROPERTIES_H

#include "Vector.h"
#include "String.h"
#include <stdio.h>

/**
 * Prywatna klasa klasy Properties zapami�tuj�ca (jako obiekty klasy String)
 * klucz i odpowiadaj�c� mu warto��.
 */

class PropertiesNode {
public:
	PropertiesNode(const char *k, const char *v) : key(k), value(v) {}
	const char *getKey() const { return key; }
	const char *getValue() const { return value; }
private:
	String key, value;
};

/**
 * Obiekty tej klasy potrafi� wczyta� plik tekstowy zawieraj�cy pary
 * klucz-warto�� oraz zapami�ta� go tak (dzi�ki obiektowi klasy Vector), by
 * odszukiwanie danej warto�ci na podstawie podanego klucza odbywa�o si�
 * szybko. Klasa ta rozpoznaje linie komentarza rozpoczynaj�ce si� od znaku
 * "#". Akceptuje odwrotn� kolejno�� w pliku tekstowym, a tak�e wiele kluczy
 * dla jednej warto�ci.
 */


class Properties {
public:
	Properties() {}
	Properties(const char *, bool = false, bool = false);
	~Properties() { clear(); }

	/** Dodaj par� klucz-warto��. */
	void add(const char *, const char *);

	/** Podaj mi warto�� odpowiadaj�c� kluczowi podanemu jako argument. */
	const char *getValue(const char *) const;

	/** poka� ca�� swoj� zawarto�� */
	void showContents() const;

	/** usu� ca�� swoj� zawarto�� */
	void clear();
private:
	void load(FILE *, bool, bool);
	int	go2nextLine(FILE *) const;
	int go2nextWord(FILE *) const;

	Vector nodes;
};

#endif
