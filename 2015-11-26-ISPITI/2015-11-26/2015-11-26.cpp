// 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR

// 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"

// 3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA

#include <iostream>
using namespace std;
const char* crt = "\n--------------------------------------------------\n";
template<class T1, class T2>
class Kolekcija
{
	friend ostream& operator<< <>(ostream& COUT, const Kolekcija<T1, T2>& obj);
private:
	T1* _elementi1;
	T2* _elementi2;
	int _trenutnoElemenata;
public:
	Kolekcija() :_elementi1(NULL), _elementi2(NULL), _trenutnoElemenata(0)
	{}
	Kolekcija(const Kolekcija& org) : //Copy Constructor
		_elementi1(new T1[org._trenutnoElemenata]),
		_elementi2(new T2[org._trenutnoElemenata]),
		_trenutnoElemenata(org._trenutnoElemenata)
	{
		for (int i = 0; i < _trenutnoElemenata; i++)
		{
			_elementi1[i] = org._elementi1[i];
			_elementi2[i] = org._elementi2[i];
		}
	}
	Kolekcija& operator=(const Kolekcija& obj)
	{
		if (this != &obj)
		{
			delete[]_elementi1; _elementi1 = NULL;
			delete[]_elementi2; _elementi2 = NULL;
			_elementi1 = new T1[obj._trenutnoElemenata];
			_elementi2 = new T2[obj._trenutnoElemenata];
			_trenutnoElemenata = obj._trenutnoElemenata;
			for (int i = 0; i < _trenutnoElemenata; i++)
			{
				_elementi1[i] = obj._elementi1[i];
				_elementi2[i] = obj._elementi2[i];
			}
		}
		return *this;
	}
	~Kolekcija()
	{
		delete[]_elementi1; _elementi1 = NULL;
		delete[]_elementi2; _elementi2 = NULL;
	}
	void AddElement(const T1& ob1, const T2& ob2)
	{
		T1* temp1 = new T1[_trenutnoElemenata + 1];
		T2* temp2 = new T2[_trenutnoElemenata + 1];
		for (int i = 0; i < _trenutnoElemenata; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		temp1[_trenutnoElemenata] = ob1;
		temp2[_trenutnoElemenata] = ob2;
		delete[]_elementi1;
		delete[]_elementi2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		_trenutnoElemenata++;
	}
	void RemoveElement(int pozicija)
	{
		if (pozicija >= 0 && pozicija < _trenutnoElemenata)
		{
			for (int i = pozicija; i < _trenutnoElemenata - 1; i++)
			{
				_elementi1[i] = _elementi1[i + 1];
				_elementi2[i] = _elementi2[i + 1];
			}
			_trenutnoElemenata--;
		}
	}
	int getTrenutno()const { return _trenutnoElemenata; }
	T1* getElement1()const { return _elementi1; }
	T2* getElement2()const { return _elementi2; }


};
template<class T1, class T2>
ostream& operator<< <>(ostream& COUT, const Kolekcija<T1, T2>& obj)
{
	for (int i = 0; i < obj._trenutnoElemenata; i++)
	{
		COUT << obj._elementi1[i] << '\t' << obj._elementi2[i] << endl;
	}
	COUT << endl;
	return COUT;
}
class Datum
{
	int* _dan, * _mjesec, * _godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000)
	{
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	~Datum()
	{
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	Datum(const Datum& org) :
		_dan(new int(*org._dan)), _mjesec(new int(*org._mjesec)), _godina(new int(*org._godina))
	{	}
	Datum& operator=(const Datum& obj)
	{
		if (this != &obj)
		{
			delete _dan; _dan = NULL;
			delete _mjesec; _mjesec = NULL;
			delete _godina; _godina = NULL;
			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
		}
		return *this;
	}
	bool operator==(const Datum& obj)
	{

		return ((*_dan == *obj._dan) && (*_mjesec == *obj._mjesec) && (*_godina == *obj._godina));
	}
	friend ostream& operator<< (ostream& COUT,const  Datum& obj)
	{
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}
};

class Student
{
	friend ostream& operator<<(ostream& COUT, const Student& obj);
private:
	char* _imePrezime;
	Datum* _datumRodjenja;
public:
	Student(const char* imePrezime = "---", Datum d = Datum())
	{
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_datumRodjenja = new Datum(d);
	}
	Student(const Student& org) :
		_imePrezime(new char[strlen(org._imePrezime) + 1]), _datumRodjenja(new Datum(*org._datumRodjenja))
	{
		strcpy_s(_imePrezime, strlen(org._imePrezime) + 1, org._imePrezime);

	}
	Student& operator=(const Student& obj)
	{
		if (this != &obj)
		{
			delete[]_imePrezime; _imePrezime = NULL;
			delete _datumRodjenja; _datumRodjenja = NULL;
			_imePrezime = new char[strlen(obj._imePrezime) + 1];
			strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
			_datumRodjenja = new Datum(*obj._datumRodjenja);
		}
		return *this;
	}
	~Student() { delete[] _imePrezime; delete _datumRodjenja; }
	bool operator==(const Student& obj)
	{
		return ((strcmp(_imePrezime, obj._imePrezime) == 0) && (*_datumRodjenja == *obj._datumRodjenja));

	}
};
ostream& operator<<(ostream& COUT, const Student& obj)
{
	cout << "Ime Prezime : " << obj._imePrezime << "\tDatum rodenja: " << *obj._datumRodjenja << endl;
	return COUT;
}
class Ispit
{
	friend ostream& operator<<(ostream& COUT, const Ispit& obj);
private:
	Datum _datumOdrzavanja;
	char* _opisIspita;
	Kolekcija<Student, Datum> _prijave;
	Kolekcija<Student, int> _rezultati;
public:
	Ispit(const char* predmet, Datum datumOdrzavnja) :
		_opisIspita(new char[strlen(predmet) + 1]), _datumOdrzavanja(datumOdrzavnja)
	{
		strcpy_s(_opisIspita, strlen(predmet) + 1, predmet);
	}
	Ispit(const Ispit& org) :
		_opisIspita(new char[strlen(org._opisIspita) + 1]), _datumOdrzavanja(org._datumOdrzavanja)
	{
		strcpy_s(_opisIspita, strlen(org._opisIspita) + 1, org._opisIspita);
	}
	~Ispit()
	{
		delete[]_opisIspita; _opisIspita = NULL;
	}
	Ispit& operator=(const Ispit& obj)
	{
		if (this != &obj)
		{
			delete[]_opisIspita; _opisIspita = NULL;
			_opisIspita = new char[strlen(obj._opisIspita) + 1];
			strcpy_s(_opisIspita, strlen(obj._opisIspita) + 1, obj._opisIspita);
			_datumOdrzavanja = obj._datumOdrzavanja;
		}
		return *this;
	}
	bool DodajPrijavu(Student& student, Datum& datum)
	{
		for (int i = 0; i < _prijave.getTrenutno(); i++)
		{
			if (student == _prijave.getElement1()[i])
				return false;
			/*throw exception("Nije dozvoljeno dodavanje istog studenta");*/
		}
		_prijave.AddElement(student, datum);
		return true;
	}
	void DodajRezultat(Student& studen, int ocijena)
	{
		if (ocijena >= 5 && ocijena <= 10)
		{
			for (int i = 0; i < _prijave.getTrenutno(); i++)
			{
				if (_prijave.getElement1()[i] == studen)
				{
					for (int j = 0; j < _rezultati.getTrenutno(); j++)
					{
						if (_rezultati.getElement1()[j] == studen)
							return;
					}
					_rezultati.AddElement(studen, ocijena);
					return;
				}
			}
		}
	}
	void Sortiraj()
	{	
		
		int ok;
		for (int i = _rezultati.getTrenutno() - 1; i>0 ; i--)
		{
			ok = 1;
			for (int j = 0; j < i; j++)
			{
				if (_rezultati.getElement2()[j + 1] < _rezultati.getElement2()[j])
				{
					Student stTemp = _rezultati.getElement1()[j];
					int ocTemp = _rezultati.getElement2()[j];

					_rezultati.getElement1()[j] = _rezultati.getElement1()[j + 1];
					_rezultati.getElement2()[j] = _rezultati.getElement2()[j + 1];

					_rezultati.getElement1()[j + 1] = stTemp;
					_rezultati.getElement2()[j + 1] = ocTemp;
					ok = 0;

				}
			}
			if (ok)
				break;
		}
	}
	void ZakljuciRezultate()
	{
		for (int i = 0; i < _prijave.getTrenutno(); i++)
		{
			bool pronade = false;
			for (int j = 0; j < _rezultati.getTrenutno(); j++)
			{
				if (_prijave.getElement1()[i] == _rezultati.getElement1()[j])
				{
					pronade = true;
					break;
				}
			}
			if (!pronade)
			{
				_rezultati.AddElement(_prijave.getElement1()[i], 5);
			}
		}
	}
};
ostream& operator<<(ostream& COUT, const Ispit& obj)
{
	COUT << "Ispit iz predmeta: " << obj._opisIspita << "\tna datum: " << obj._datumOdrzavanja << endl;
	COUT << "Rezultati ispita:\n " << obj._rezultati << endl;


	return COUT;
}
void main()

{

	//REGIONI olaksavaju organizaciju programskog code-a, te ih mozete ukloniti ukoliko vam otezavaju preglednost



#pragma region TestiranjeKolekcije

	Kolekcija<int, double> kolekcija1;

	for (size_t i = 0; i < 50; i++)

		kolekcija1.AddElement(i, i + 0.3);

	cout << kolekcija1 << endl;

	cout << crt;



	Kolekcija<int, double> kolekcija2;

	kolekcija2 = kolekcija1;

	kolekcija1.RemoveElement(2);//UMANJITI VELIÈINU NIZA

	cout << kolekcija2 << crt;



	if (kolekcija1.getTrenutno() == kolekcija2.getTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;



	Kolekcija<int, double> kolekcija3(kolekcija2);

	cout << kolekcija3 << crt;
	if (kolekcija2.getTrenutno() == kolekcija3.getTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;
#pragma endregion

#pragma region TestiranjeDatuma

	Datum danas(26, 11, 2015);

	Datum sutra(danas);

	Datum prekosutra;

	prekosutra = danas;

	cout << danas << endl << sutra << endl << prekosutra << crt;

#pragma endregion

#pragma region TestiranjeStudenta

	Student denis("Denis Music", Datum(26, 11, 1990));

	Student jasmin("Jasmin Azemovic", Datum(22, 11, 1990));

	Student goran;

	goran = jasmin;

	Student adel("Jasmin Azemovic", Datum(22, 11, 1990));

	Student marija("Marija Herceg", Datum(15, 6, 1990));

#pragma endregion
//
//
//
#pragma region TestiranjeIspita

	Ispit prIII("PRIII::26.11.2015", danas);

	//DodajPrijavu - dodaje novog studenta u listu prijavljenih za ispti. 

	//onemoguciti dodavanje dva ista studenta

	if (prIII.DodajPrijavu(denis, danas))

		cout << denis << " DODAT na spisak" << endl;

	if (prIII.DodajPrijavu(jasmin, danas))

		cout << jasmin << " DODAT na spisak" << endl;

	if (prIII.DodajPrijavu(goran, danas))

		cout << goran << " DODAT na spisak" << endl;

	if (prIII.DodajPrijavu(adel, danas))

		cout << adel << " DODAT na spisak" << endl;

	if (prIII.DodajPrijavu(marija, danas))

		cout << marija << " DODAT na spisak" << endl;



	cout << crt << endl;

	/*DodajRezultat - dodaje ocjenu koju je student ostvario na ispitu.

	za dodavanje ocjene student je prethodno trebao biti prijavljen na ispit

	*/

	prIII.DodajRezultat(goran, 7);

	prIII.DodajRezultat(jasmin, 6);

	prIII.DodajRezultat(adel, 8);

	prIII.DodajRezultat(marija, 9);



	/*Sortiraj - sortira rezultate ispita prema ocjeni*/

	cout << prIII << crt;
	prIII.Sortiraj();
	cout << prIII << crt;

	/*ZakljuciRezultate - dodaje ocjenu 5 svim studentima koji su prijavili ispit

	a nije im evidentirana ocjena*/

	prIII.ZakljuciRezultate();
	cout << prIII << crt;
	prIII.Sortiraj();
	//rezultati ispita se smiju ispisivati jedino ukoliko su prethodno zakljuèeni

	cout << prIII << crt;



	Ispit prIII_2(prIII);

	cout << prIII_2 << crt;

#pragma endregion

}