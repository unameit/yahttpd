#ifndef	VECTOR_H
#define	VECTOR_H

#include <stdlib.h>			// qsort, bsearch
#include <sys/types.h>		// uint

/**
 * Implementacja struktury danych okre�lanej jako dynamiczna tablica.
 * Przechowuje wska�niki do "czegokolwiek" (void *). Po dostarczeniu jej
 * funkcji por�wnuj�cej potrafi sortowa� oraz wyszukiwa� po��dane elementy.
 */

class Vector {
public:
	/** utw�rz pusty wektor */
	Vector(uint = 100, uint = 100);

	/** Usu� tablic� */
	~Vector() { free(data); }

	/** dodaj nowy element do tablicy */
	uint add(void *);

	/** zmie� element o indeksie index na nowy */
	void set(uint index, void *element);

	/** podaj mi i-ty element tablicy */
	void *operator[](uint) const;

	/** zamie� miejscami dwa elementy tablicy */
	void swap(uint i, uint j);

	/** usu� i-ty element tablicy */
	void *remove(uint i);

	/** podaj mi aktualn� liczb� element�w w tej tablicy */
	uint getCount() const { return count; }

	/** ustaw liczb� element�w w tablicy - mo�na j� tylko zmniejszy�. */
	void setCount(uint);

	/** wyrzu� wszysktie elementy */
	void clear() { setCount(0); }

	/** posortuj elementy tablicy wg podanej funkcji por�wnuj�cej. */
	void sort(int (*cmpf)(const void *, const void *)) { qsort(data, count, sizeof(void *), cmpf); }

	/** wyszukaj element w tablicy (maj�c podan� funkcj� por�wnuj�c�). */
	void *search(const void *key, int (*cmpf)(const void *, const void *)) const { return bsearch(key, data, count, sizeof(void *), cmpf); }

	/** podaj mi aktualn� pojemno�� tablicy */
	uint getCapacity() const { return capacity; }

	/** ustaw j� */
	void setCapacity(uint);

	/** o ile zwi�ksza si� pojemno�� tablicy, gdy si� sko�czy w niej miejsce? */
	uint getIncrement() const { return increment; }

	/** ustaw t� warto�� */
	void setIncrement(uint);

private:	
	/** actual capacity */
	uint capacity;

	/** capacity increment */
	uint increment;

	/** number of valid elements */
	uint count;

	void **data;
};

#endif
