#ifndef	CONNECTIONSMANAGER_H
#define	CONNECTIONSMANAGER_H

#include "Vector.h"

// deklaracja zapowiadaj�ca - bo argument metody add
class AsyncConnection;			

/**
 * Obiekt tej klasy potrafi zarz�dza� grup� obiekt�w, kt�re implementuj�
 * interfejs AsyncConnection i realizuj� nieblokuj�ce po��czenie z u�yciem
 * UNIXowych deskryptor�w.  Obiektowi tej klasy oddaje si� sterowanie
 * zar�wno w procesie-rodzicu, jak i w procesach potomnych.  Dzi�ki klasie
 * AsyncConnection metody z innych obiekt�w mog� by� wywo�ywane w
 * odpowiednich dla nich momentach, tj. gdy zajd� zdarzenia "mo�liwo��
 * pisania na deskryptorze" lub/i "mo�liwo�� czytania z deskryptora". Obiekt
 * tej klasy usuwa zamkni�te po��czenia w spos�b umo�liwiaj�cy ich dalsze
 * wykorzystanie po re-inicjalizacji.
 */

class ConnectionsManager {
public:
		/**
		 * Tu si� oddaje sterowanie. Potem tyko wyskakuje do metod obiekt�w
		 * AsyncConnection::onReadable() i onWriteable().  dlatego trzeba
		 * przed oddaniem sterowania doda� conajmniej jeden �ywy obiekt
		 * AsyncConnection.
		 */
	void main();

		/**
		 * Dodaj nowe po��czenie. Je�li zostanie ono zamkni�te (zwr�ci -1
		 * jako sw�j deskryptor) zostanie 'automatycznie' usuni�te z naszej
		 * tablicy obs�ugiwanych po��cze�. Musimy powiadomi� inne obiekty o
		 * tym, �e 'zauwa�yli�my' zamkni�cie po��czenia. Zrobimy to poprzez
		 * wywo�anie metody AsyncConnection::finalize(). Wtedy obiekt ten
		 * (jak i inne) b�d� wiedzia�y, �e nie znajduje si� on ju� w naszej
		 * tablicy, a wi�c mo�na na tym obiekcie utoworzy� nowe po��czenie i
		 * na nowo doda� do tego manager'a.
		 */
	void add(AsyncConnection *);

private:
	/** tu pami�tamy wszystkie obs�ugiwane po��czenia */
	Vector connections;		

	/** Metoda sprawdza, czy po��czenia s� aktywne, i ewentualnie je usuwa. */
	void update();
};

#endif
