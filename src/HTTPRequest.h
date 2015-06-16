#ifndef	HTTPREQUEST_H
#define	HTTPREQUEST_H

#include <sys/types.h>	// uint
#include <string.h>		// strlen
#include "File.h"		// private member
#include "FileInfo.h"	// private member

class Properties;

/**
 * Obiekt tej klasy zajmuje si� parsowaniem zapytania klienta, zrozumieniem
 * go oraz wygenerowaniem w�a�ciwej odpowiedzi (zgodnie z protoko�em HTTP). 
 * Wskazuje te� oraz otwiera plik, kt�rego wys�aniem zaj�� si� ma obiekt
 * HTTPConnection.
 */

class HTTPRequest {
public:

	/**
	 * Standardowy konstruktor, w kt�rym podajemy wska�niki do p�niej
	 * wykorzystywanych obiekt�w.
	 */

	HTTPRequest(const char *hostName, const Properties *mimeTypes,
		const char *documentRoot, const char *userDir);

	/**
	 * Metoda prawdza to co nam przys�a�a przegl�darka (a ja to podaj� w
	 * buforze tekstowym jako string - czyli zako�czony znakiem '\0'). 
	 * Zwraca true, je�li request jest ju� odebrany w ca�o�ci (zwykle
	 * oznacza to, �e ko�czy si� dwoma znakami ko�ca linii '\n' - ale nie
	 * jest to regu��, cho� tak powinno by�). Lub zwraca false, je�li
	 * request nie jest jeszcze kompletny i nale�y spodziewa� si�, �e co� z
	 * sieci zaraz dojdzie. Wtedy metoda ta zostanie wywo�ana ponownie z
	 * uzupe�nionym buforem.
	 */

	bool isComplete(const char *request);

	/**
	 * Metoda ta na podstawie zapytania przegl�darki (podawanego w metodzie
	 * wy�ej, ale podana tak�e tutaj jeszcze raz, tylko tym razem mo�n� j�
	 * zniszczy� - a nawet trzeba. Bufor przenaczony jest przecie� teraz na
	 * odpowied�) formu�uje nag��wek odpowiedzi. Mog� to ju� by� np. jakie�
	 * informacje o pliku, kt�ry za�yczy�a sobie przegl�darka w zapytaniu. 
	 * Odpowied� t� umieszcza we wskazanym buforze tekstowym, kt�rego
	 * wielko�� podana jest jako drugi argument.  Naturalnie odpowied� nie
	 * mo�e przekroczy� tego rozmiaru, gdy� spowoduje to "SEGMENTATION
	 * FAULT" i zako�czenie bie��cego procesu. Nale�y si� jednak spodziewa�,
	 * �e dany bufor b�dzie mia� conajmniej 32 kilobajty, a wi�c
	 * wystarczaj�co du�o.  Metoda ta zwraca d�ugo�� w bajtach nag��wka
	 * odpowiedzi, kt�ry nale�y wys�a�.
	 */

	uint getResponse(char *buffer, uint bufferLength);

	/**
	 * Metoda ta zwraca wska�nik do obiektu File, kt�rego zawarto�� nale�y
	 * wys�a�, zaraz za nag��wkiem.  Zwraca NULL je�li nie nale�y nic wi�cej
	 * (poza nag��wkiem) wysy�a�.  Je�li sko�cz� z plikiem, to sam go zamkn�
	 * - Ty wi�c o jego zamykanie nie musisz martwi�.
	 */

	File *getFile() { return sendFile ? &file: NULL; }

	/** Zwraca metod� bie��cego zapytania. Metoda na potrzeby wpisu w pliku acces.log */
	const char *getMethod() const { return method; }

	/** Zwraca URI bie��cego zapytania. Metoda na potrzeby wpisu w pliku acces.log */
	const char *getURI() const { return uri; }

private:
	/** Nazwa komputera na kt�rym jeste�my uruchomieni - potrzebna w polu Location odpowiedzi HTTP. */
	const char *const hostName;

	/** Obiekt 'rozpoznaj�cy' typy plik�w po rozszerzeniach ich nazwy. */
	const Properties *const mimeTypes;

	/** Gdzie sk�aowane s� dokumenty z g��wnego drzewa. */
	const char *const documentRoot;

	/** W kt�rym katalogu w katalgoach u�ytkownik�w szuka� index.html i innych dokument�w. */
	const char *const userDir;

	/** wysy�amy co� opr�cz nag��wka? / wysy�amy plik? */
	bool haveContent, sendFile;

	/** tu pami�tamy dane o pliku, kt�rym interesuje si� klient */
	FileInfo fileinfo;

	/** a tu jest ewentualnie ten plik. */
	File file;

	/** metoda przerabia URI na pe�n� scie�k� dost�pu do pliku */
	const char *resolvePathName(char *);

	/**
	 * Prywatna klasa klasy HTTPRequest. Jej obiekty s� nag��wkami
	 * odpowiedzi wysy�anymi do klient�w. Atrybutami tych oiekt�w s�: kod
	 * odpowiedzi, jej kr�tki opis, opis w HTMLu
	 */

	class ResponseHeader {
	public:
		ResponseHeader(int c, const char *d, const char *hd) :
			code(c), description(d), htmlDescription(hd), htmlDescriptionLen(strlen(htmlDescription)) {}
		int getCode() const { return code; }
		const char *getDescription() const { return description; }
		const char *getHTMLDescription() const { return htmlDescription; }
		uint getHTMLDescriptionLen() const { return htmlDescriptionLen; }
	private:
		const int code;
		const char *const description;
		const char *const htmlDescription;
		const uint htmlDescriptionLen;
	};

	const ResponseHeader hdr200, hdr301, hdr400, hdr404, hdr501;

	/** Metoda generuje nag��wke odpowiedzi korzystaj�c z podanych obiekt�w. */

	void generateResponseHeader(char *buffer, uint bufferLength,
		const ResponseHeader &hdr,
		const char *location,
		ulong contentLength, const char *contentType,
		time_t mtime) const;

	char method[10], uri[500];
};

#endif
