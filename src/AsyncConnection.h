#ifndef	ASYNCCONNECTION_H
#define	ASYNCCONNECTION_H

/**
 * Klasa ta jest tylko interfejsem kt�ry musz� implementowa� obiekty chc�ce
 * by� obs�ugiwane przez ConnectionsManager'a.
 *
 * Generalnie opiera si� na mechanizmie asynchronicznych unix'owych
 * socket'�w, ale ich obs�uga odbywa si� w spos�b zdarzeniowy, tzn. gdy
 * mo�liwy jest zapis/odczyt na danym deskryptorze, to generowane jest
 * odpowiednie zdarzenie obs�uguj�ce t� sytuacj� (czyli wywo�ywana
 * odpowiednia metoda dla obiektu klasy imlementuj�cej ten interfejs).
 */

class AsyncConnection {
public:
	 /** chcesz czyta�? */
	virtual bool isReadNeeded() const { return false; }

	/** chcesz pisa�? */
	virtual bool isWriteNeeded() const { return false; }

	/** no to czytaj, bo teraz mo�na */
	virtual void onReadable() {}	 

	/** analogicznie - pisz */
	virtual void onWriteable() {}	 

	/**
	 * daj mi sw�j UNIXowy deskryptor pliku/gniazda.
	 * -1 oznacza, �e po��czenie jest ju� zamkni�te.
	 */
	virtual operator int() const { return -1; }

	/**
	 * je�li -1 == (int) *this, wi�c po��czenie jest ju� zamkni�te.
	 * wywo�aj wi�c wtedy ten niby destruktor
	 */
	virtual void finalize() {}

	/**
	 * czy zosta�o ju� wywo�ane finalize() ?
	 * musisz zapami�ta� ten fakt, �eby inne obiekty nie przeoczy�y
	 * re-inicjalizacji po��czenia
	 * (to zamiast niepotrzebnych new i delete)
	 */
	virtual bool isFinalized() const { return true; }


protected:
	/**
	 * chroniony konstruktor - niech nie istniej� obiekty tej klasy. 
	 * Przeznaczona jest ona tylko do dziedziczenia.
	 */
	AsyncConnection() {}
};

#endif
