#include <netinet/in.h>				// sockaddr_in
#include "debug.h"
#include "ConnectionWithChild.h"
#include "ParentThread.h"


ParentThread::ParentThread(AsyncSocket *socket, ConnectionWithChild **init_children, int init_maxChildren) :
	serverSocket(socket),
	children(init_children),
	maxChildren(init_maxChildren),
	socketfdToSend(-1)
{
	manager.add(this);						// a mo�e za p�tla?
	for(int i = 0; i < maxChildren; ++i)
		manager.add(children[i]);
}


ConnectionWithChild *ParentThread::getChild() const
{
	static int no = -1;
	if (maxChildren == ++no)
		no = 0;
	return children[no];
}


void ParentThread::onReadable()
{
	/**
	 * Teraz powinni�my odebra� po��czenie (accept()). Mog�o si� jednak
	 * zdarzy� tak, �e poprzednio odebrane po��czenie nie zosta�o jeszcze
	 * przekazane �adnemu z proces�w potomnych (gdy� np. procesy by�y zbyt
	 * zaj�te sob�, lub te� posiadaj� ju� maksymaln� dozwolon� liczb�
	 * otwartych po��cze�). Je�li wi�c mamy takie 'zaleg�e' po��czenie do
	 * przekazania, to teraz nie przyjmiemy nowego po��czenia, cho� wiemy,
	 * �e n nas czeka (bo przecie� onReadable()).  Spr�bujemy za to wys�a�
	 * tamto ponownie.
	 */

	if (-1 == socketfdToSend)
		socketfdToSend = serverSocket->accept(&peer);

	/**
	 * Ponawiaj pr�by przekazania po��czenia dop�ty nie zostanie uwie�czone
	 * sukcesem, lub liczba pr�b zr�wna si� z ilo�ci� proces�w potomnych. 
	 * Je�li si� nie uda, od�� na p�niej - widocznie procesy-dzieci s�
	 * zbyt zaj�te.
	 */

	bool sent = false;
	int count = 0;
	while(!sent && count++ < maxChildren)
		sent = getChild()->sendSocket(socketfdToSend, &peer);

	/**
	 * W tym miejscu nie zamykamy tego socketa. Zrobi to zaraz po wys�aniu
	 * go do dziecka, obiekt zajumuj�cy si� komunikacj� z dzieckiem
	 * (ConnectionWithChild).
	 */

	/** Je�li uda�o Ci si� przekaza� po��czenie, to odnotuj ten fakt. */

	if (sent)
		socketfdToSend = -1;
}
