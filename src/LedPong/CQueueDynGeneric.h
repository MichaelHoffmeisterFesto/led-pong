#pragma once

#include <iostream>

using namespace std;

// These classes implement a generic dynamic queue of object.
// The solution was orignally supplied by Daniel Usacev, Lukas Werk, MMT, Hochschule Karlsruhe.
// The lab excercise was designed by Jürgen Ewert,MMT, Hochschule Karlsruhe.
// It was adopted to using templates / generics.

// Objekte dieser Klasse repräsentieren die Elemente des Queues.
template <class T>
class CElementGeneric
{
public:
	CElementGeneric(const T& data);	// Konstruktor mit Angabe eines Strings (Daten)

	// Der Default-Konstruktor (ohne Parameter)
	// steht nach obiger Deklaration nicht mehr
	// automatischzur Verfügung (wird hier aber auch
	// nicht zwingend benötigt).

	// Eigenen Destruktor benötigen wir da CElement 
	// ein dyn. Element hat.

	~CElementGeneric();

	void TestAAA();

	// Members are public, as well.
	T mData;
	CElementGeneric* mPtrNext;
};

// Ein Objekt dieser Klasse CQueueDyn repräseniert den Queue, bestehend aus
// Objekten der Klasse CElement.
// Ein Queue hat einen Konstruktor und die bekannten Methoden enqueue und pop.
// Darüber hinaus hat der Steck einen Zeiger auf das zuletzt mit enqueue
// eingefügte Element (Objekt der Klasse CElement).
// Ist dieser Zeiger gleich nullptr (Nullpointer), ist der Queue leer.
// Somit benötigen wir kein Root-Element, wie wir es in der letzten
// Imlementierung eines Queues verwendet haben.
template <class T>
class CQueueDynGeneric
{
public:

	CQueueDynGeneric();					// Konstruktor, erzeugt einen leeren Queue.
	~CQueueDynGeneric();				// Destruktor.

	bool enqueue(const T& data);		// Daten an die Warteschlange anhängen.
	T* top();							// Oberste Daten ansehen, aber nicht herausnehmen
	bool dequeue(T& data);				// Daten aus der Warteschlange herausnehmen.
	void GetDisplay(void);				// Gibt den Inhalt der gesamten Warteschlan-
										// ge als string zurück, der aus den Strings 
										// der einzelnen Elemente gebildet ist. Die Warte-
										// schlange selbst bleibt dabei unverändert.
	int GetLength();					// Gibt die Länge der Schlange zurück.
private:
	CElementGeneric<T>* mPtrHead = nullptr;
	CElementGeneric<T>* mPtrTail = nullptr;
	int length = 0;
};

// ----------------------------------------------------------------
// When it is a template, the code needs to be in the header file!!
// ----------------------------------------------------------------

// Constructor with data object
template <class T>
CElementGeneric<T>::CElementGeneric(const T& data)
{
	// this does copy the data into the element!
	mData = data;
	mPtrNext = nullptr;
}

// Destructor to delete inner object
template <class T>
CElementGeneric<T>::~CElementGeneric()
{
}

template <class T>
void CElementGeneric<T>::TestAAA()
{
	int a = 1;
}

// Konstruktor, erzeugt einen leeren Queue.
template <class T>
CQueueDynGeneric<T>::CQueueDynGeneric()
{
	mPtrHead = nullptr;
	mPtrTail = nullptr;

	length = 0;
}

// Destruktor.
template <class T>
CQueueDynGeneric<T>::~CQueueDynGeneric()
{
	T dummy;
	while (dequeue(dummy)) {}				// leer lesen
}

// Daten an die Warteschlange anhängen.
template <class T>
bool CQueueDynGeneric<T>::enqueue(const T& data)
{
	CElementGeneric<T>* tmpPtr = new CElementGeneric<T>(data);	// 1. Knoten erzeugen

	if (nullptr == mPtrHead)					// Fall A - Q leer
	{
		mPtrHead = tmpPtr;					// neues Element ist Head und Tail gleichzeitig
		mPtrTail = tmpPtr;
	}
	else                                    // Fall B - Q nicht leer
	{
		mPtrTail->mPtrNext = tmpPtr;		// aktuelle Tail zeigt auf neue Element
		mPtrTail = tmpPtr;					// neues Element wird neues Tail
	}

	length++;
	return true;
}

// Daten aus der Warteschlange herausnehmen.
// Gibt NULL zurück, wenn leer
template <class T>
T* CQueueDynGeneric<T>::top()
{
	if (mPtrHead == nullptr)
		return nullptr;
	return &mPtrHead->mData;
}

// Daten aus der Warteschlange herausnehmen.
template <class T>
bool CQueueDynGeneric<T>::dequeue(T& data)
{
	if (mPtrHead == nullptr)					// Fall A - leer
	{
		return false;
	}
	else if (mPtrHead == mPtrTail)			// Fall B - letzter Knoten
	{
		data = mPtrTail->mData;				// B.1 lesen
		delete mPtrTail;					// B.2 löschen

		mPtrHead = nullptr;					// B.3 
		mPtrTail = nullptr;
	}
	else                                    // Fall C - Kette
	{
		CElementGeneric<T>* tmpPtr = mPtrHead;		// C.1 Löschknoten merken
		data = mPtrHead->mData;				// C.2 lesen
		mPtrHead = mPtrHead->mPtrNext;		// C.3 HeadPtr umketten
		//tmpPtr->mPtrNext = NULL;			// C.4 prev abketten
		delete tmpPtr;						// C.5 Leseknoten löschen
	}

	length--;
	return true;
}

// Gibt den Inhalt der gesamten Warteschlange als string zurück, der aus den Strings 
// der einzelnen Elemente gebildet ist. Die Warteschlange selbst bleibt dabei unverändert.
template <class T>
void CQueueDynGeneric<T>::GetDisplay(void)
{
}

// Gibt die Länge der Schlange zurück.
template <class T>
int CQueueDynGeneric<T>::GetLength()
{
	return length;
}