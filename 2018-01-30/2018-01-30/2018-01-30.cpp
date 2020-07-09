#include <iostream>
#include <vector>
#include <iterator>
#include <thread>
#include<mutex>
#include<string>
#include <iomanip>
using namespace std;
const char* crt = "\n------------------------------------------------------------\n";
vector<string> listaZabranjeniRijeci;
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
	Datum(const Datum& dat)
	{
		_dan = new int(*dat._dan);
		_mjesec = new int(*dat._mjesec);
		_godina = new int(*dat._godina);
	}
	Datum(Datum&& obj) noexcept :
		_dan(obj._dan),
		_mjesec(obj._mjesec),
		_godina(obj._godina)
	{
		obj._dan = NULL;
		obj._godina = NULL;
		obj._mjesec = NULL;
	}
	~Datum()
	{
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	Datum& operator=(const Datum& obj)
	{
		if (this != &obj)
		{
			delete _dan; _dan = nullptr;
			delete _mjesec; _mjesec = nullptr;
			delete _godina; _godina = nullptr;
			_dan = new int(*(obj._dan));
			_mjesec = new int(*(obj._mjesec));
			_godina = new int(*(obj._godina));
		}
		return *this;
	}
	bool operator ==(const Datum& obj)
	{
		if (*this->_dan == *obj._dan && *this->_mjesec == *obj._mjesec && *this->_godina == *obj._godina)
			return true;
		return false;

	}
	bool operator>(const Datum& desni)
	{
		return (*_dan + *_mjesec * 30 + *_godina * 365) > (*desni._dan + *desni._mjesec * 30 + *desni._godina * 365);
	}
	friend ostream& operator<<(ostream& COUT, const Datum& obj)
	{
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}
	int getDani()const
	{
		return *_dan + *_mjesec * 30 + *_godina * 365;
	}
};
class Izuzetak : public exception
{
	friend ostream& operator<< (ostream& COUT, const Izuzetak& obj)
	{
		COUT << "Poruka: " << obj.what() << endl;
		COUT << "Funkcija: " << obj._funkcija << endl;
		return COUT;
	}
private:
	string _funkcija;
public:
	Izuzetak(const char* msg, string funkcija) :
		exception(msg), _funkcija(funkcija)
	{}

};

template <class T1, class T2>
class Kolekcija
{
	T1* _elementi1;
	T2* _elementi2;
	int _trenutnoElemenata;
	bool _omoguciDupliranjeElemenata;
	void dealociraj()
	{
		delete[]_elementi1;
		delete[]_elementi2;
		_elementi1 = NULL;
		_elementi2 = NULL;
	}
public:
	Kolekcija(bool omoguciDupliranjeElemenata = false)
	{
		_trenutnoElemenata = 0;
		_omoguciDupliranjeElemenata = omoguciDupliranjeElemenata;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}
	Kolekcija(Kolekcija<T1, T2>& org) :
		_elementi1(new T1[org._trenutnoElemenata]),
		_elementi2(new T2[org._trenutnoElemenata]),
		_trenutnoElemenata(org._trenutnoElemenata),
		_omoguciDupliranjeElemenata(org._omoguciDupliranjeElemenata)
	{
		for (int i = 0; i < org._trenutnoElemenata; i++)
		{
			_elementi1[i] = org._elementi1[i];
			_elementi2[i] = org._elementi2[i];
		}
	}
	Kolekcija(Kolekcija<T1, T2>&& org) :
		_elementi1(org._elementi1),
		_elementi2(org._elementi2),
		_trenutnoElemenata(org._trenutnoElemenata),
		_omoguciDupliranjeElemenata(org._omoguciDupliranjeElemenata)
	{
		org._elementi1 = NULL;
		org._elementi2 = NULL;
	}
	~Kolekcija()
	{
		delete[]_elementi2; _elementi2 = nullptr;
		delete[]_elementi1; _elementi1 = nullptr;
		_trenutnoElemenata = 0;
	}
	Kolekcija& operator=(const Kolekcija<T1, T2>& obj)
	{
		if (this != &obj)
		{
			dealociraj();
			_trenutnoElemenata = obj._trenutnoElemenata;
			_omoguciDupliranjeElemenata = obj._omoguciDupliranjeElemenata;
			_elementi1 = new T1[obj._trenutnoElemenata];
			_elementi2 = new T2[obj._trenutnoElemenata];
			for (int i = 0; i < obj._trenutnoElemenata; i++)
			{
				_elementi1[i] = obj._elementi1[i];
				_elementi2[i] = obj._elementi2[i];
			}
		}
		return *this;
	}
	Kolekcija& operator=(Kolekcija<T1, T2>&& obj) noexcept
	{
		if (this != &obj)
		{
			dealociraj();
			_elementi1 = obj._elementi1;
			_elementi2 = obj._elementi2;
			obj._elementi1 = NULL;
			obj._elementi2 = NULL;
			_trenutnoElemenata = obj._trenutnoElemenata;
			_omoguciDupliranjeElemenata = obj._omoguciDupliranjeElemenata;
		}
		return *this;
	}
	int GetTrenutno() const { return _trenutnoElemenata; }
	T1& GetElement1(int lokacija) const
	{
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw Izuzetak("Nepostojeca lokacija", __FUNCTION__);
		return _elementi1[lokacija];
	}

	T2& GetElement2(int lokacija) const
	{
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw Izuzetak("Nepostojeca lokacija", __FUNCTION__);
		return _elementi2[lokacija];
	}
	bool AddElement(const T1& elem1, const T2& elem2)
	{
		/*
		AddElement :: omogucava dodavanje novog elementa u kolekciju. Ukoliko je moguce, osigurati automatsko
		 prosiranje kolekcije prilikom dodavanja svakog novog elementa, te onemoguciti ponavljanje elemenata
		*/
		int brojac = 0;
		for (int i = 0; i < _trenutnoElemenata; i++)
		{
			if (_elementi1[i] == elem1)
				brojac++;
		}
		if (brojac == 0)
		{
			T1* temp1 = new T1[_trenutnoElemenata + 1];
			T2* temp2 = new T2[_trenutnoElemenata + 1];
			for (int i = 0; i < _trenutnoElemenata; i++)
			{
				temp1[i] = _elementi1[i];
				temp2[i] = _elementi2[i];
			}
			temp1[_trenutnoElemenata] = elem1;
			temp2[_trenutnoElemenata] = elem2;
			_trenutnoElemenata++;
			dealociraj();
			_elementi1 = temp1;
			_elementi2 = temp2;
			temp1 = NULL;
			temp2 = NULL;
			return true;
		}
		return false;
	}
	bool RemoveElement(T1 vrijednost)
	{
		/*
		 RemoveElement :: na osnovu parametra tipa T1 uklanja elemente iz kolekcije i ukoliko je moguce smanjuje
		velicinu niza/kolekcije. Prilikom uklanjanja elemenata ocuvati redoslijed njihovog dodavanja
		*/
		int brojac = -1;
		int indeks;
		for (int i = 0; i < _trenutnoElemenata; i++)
		{
			if (_elementi1[i] == vrijednost)
			{
				brojac++;
				indeks = i;
				break;
			}
		}
		if (brojac == 0)
		{
			T1* temp1 = new T1[_trenutnoElemenata - 1];
			T2* temp2 = new T2[_trenutnoElemenata - 1];
			for (int i = 0; i < indeks; i++)
			{
				temp1[i] = _elementi1[i];
				temp2[i] = _elementi2[i];
			}
			for (int i = indeks; i < _trenutnoElemenata - 1; i++)
			{
				temp1[i] = _elementi1[i + 1];
				temp2[i] = _elementi2[i + 1];
			}
			_trenutnoElemenata--;
			dealociraj();
			_elementi1 = temp1;
			_elementi2 = temp2;
			temp1 = NULL;
			temp2 = NULL;
			return true;
		}
		return false;
	}
	bool operator==(const Kolekcija<string, bool>& obj)
	{
		if (this->_elementi1 == obj._elementi1 && this->_elementi2 == obj._elementi2)
			return true;
		return false;
	}
	friend ostream& operator<<(ostream& COUT, const Kolekcija& obj)
	{
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
		return COUT;
	}
};

class Dogadjaj
{
private:
	Datum _datumOdrzavanja;
	Kolekcija<string, bool>* _obaveze; //cuva informaciju o obavezama koje je potrebno ispuniti prije samog dogadjaja,
					 //string se odnosi na opis, a bool na izvrsenje te obaveze (da li je zavrsena ili ne)

	char* _naziv;
	int _notificirajPrije; //oznacava broj dana prije samog dogadjaja kada ce krenuti notifikacija/podsjetnik

	bool _rekurzivnaNotifikacija; //ako je vrijednost true onda se korisnik notificira svaki dan do _datumaOdrzavanja dogadjaja, 
					  //a pocevsi prije dogadjaja za _brojDanaZaNotifikaciju

public:

	Dogadjaj(Datum datumOdrzavanja, const char* naziv, int brojDana = 1,
		bool rekurzivnaNotifikacija = false) : _datumOdrzavanja(datumOdrzavanja)
	{
		_naziv = new char[strlen(naziv) + 1];
		strcpy_s(_naziv, strlen(naziv) + 1, naziv);

		_notificirajPrije = brojDana;
		_rekurzivnaNotifikacija = rekurzivnaNotifikacija;
		_obaveze = new Kolekcija<string, bool>;
	}

	Dogadjaj(const Dogadjaj& obj) : _datumOdrzavanja(obj._datumOdrzavanja),
		_obaveze(new Kolekcija<string, bool>(*obj._obaveze))
	{
		_naziv = new char[strlen(obj._naziv) + 1];
		strcpy_s(_naziv, strlen(obj._naziv) + 1, obj._naziv);

		_notificirajPrije = obj._notificirajPrije;
		_rekurzivnaNotifikacija = obj._rekurzivnaNotifikacija;
	}
	~Dogadjaj()
	{
		delete[] _naziv;
		_naziv = nullptr;
		delete _obaveze;
		_obaveze = nullptr;
	}
	Dogadjaj& operator=(const Dogadjaj& obj)
	{
		if (this != &obj)
		{
			delete[]_naziv;
			delete _obaveze;

			_naziv = new char[strlen(obj._naziv) + 1];
			strcpy_s(_naziv, strlen(obj._naziv) + 1, obj._naziv);
			_datumOdrzavanja = obj._datumOdrzavanja;
			_obaveze = new Kolekcija<string, bool>(*obj._obaveze);
			_notificirajPrije = obj._notificirajPrije;
			_rekurzivnaNotifikacija = obj._rekurzivnaNotifikacija;
		}
		return *this;
	}
	char* GetNaziv() { return _naziv; }
	Kolekcija<string, bool>* GetObaveze() { return _obaveze; }
	bool AddObavezu(string opis)
	{
		return _obaveze->AddElement(opis, false);

	}
	bool operator==(const Dogadjaj& obj)
	{
		if (this->_datumOdrzavanja == obj._datumOdrzavanja ||
			this->_obaveze == obj._obaveze ||
			strcmp(this->_naziv, obj._naziv) == 0)
			return true;
		return false;
	}
	Datum getDatumObaveze()const
	{
		return _datumOdrzavanja;
	}
};

class Student
{
	int _indeks;
	string _imePrezime;
	vector<Dogadjaj> _dogadjaji;
public:

	Student(int indeks, string imePrezime) : _indeks(indeks), _imePrezime(imePrezime) {}

	int GetIndeks() const { return _indeks; }

	vector<Dogadjaj>& GetDogadjaji() { return _dogadjaji; }

	friend ostream& operator<<(ostream& COUT, const Student& obj)
	{
		COUT << obj._imePrezime << " (" << obj._indeks << ")" << endl;
		return COUT;
	}
	bool AddDogadjaj(Dogadjaj dogadaj)
	{
		//onemoguciti dupliranje dogadjaja
		vector<Dogadjaj>::iterator it = _dogadjaji.begin();
		for (it; it != _dogadjaji.end(); it++)
		{
			if (*it == dogadaj)
				return false;
		}
		_dogadjaji.push_back(dogadaj);
		return true;
	}
	bool operator==(const Student& obj)
	{
		if (this->_indeks == obj._indeks)
			return true;
		return false;
	}
	string getImePrezime()const
	{
		return _imePrezime;
	}
};
mutex m;
class DLWMSReminder
{
private:
	vector<Student> _reminiderList;
	void PosaljiPoruku(string poruka)
	{
		m.lock();
		cout << crt << poruka << crt;
		m.unlock();
	}
public:
	DLWMSReminder() {}
	bool AddStudent(Student student)
	{
		vector<Student>::iterator it = _reminiderList.begin();
		for (it; it != _reminiderList.end(); it++)
		{
			if (*it == student)
				throw Izuzetak("Student je vec dodan", __FUNCTION__);
		}
		_reminiderList.push_back(student);
	}
	bool OznaciObavezuKaoZavrsenu(int brojIndeksa, string naziv, string opis)//150051, "Ispit iz PRIII", "Pregledati video materijale")
	{//da bi bila oznacena kao zavrsena, obaveza mora postojati i mora biti oznacena kao nezavrsena (false)
		for (int i = 0; i < _reminiderList.size(); i++)
		{
			int brojInd = _reminiderList[i].GetIndeks();
			if (brojInd == brojIndeksa)
			{
				for (int j = 0; j < _reminiderList[i].GetDogadjaji().size(); j++)
				{
					string nazv = _reminiderList[i].GetDogadjaji()[j].GetNaziv();
					if (nazv == naziv)
					{
						int tr = _reminiderList[i].GetDogadjaji()[j].GetObaveze()[j].GetTrenutno();
						for (int k = 0; k < tr; k++)
						{
							string opi = _reminiderList[i].GetDogadjaji()[j].GetObaveze()[j].GetElement1(k);
							if (opi == opis)
							{
								bool uvijet = _reminiderList[i].GetDogadjaji()[j].GetObaveze()[j].GetElement2(k);
								if (uvijet == false)
								{
									_reminiderList[i].GetDogadjaji()[j].GetObaveze()[j].GetElement2(k) = true;
									return true;
								}

							}
						}
					}
				}
			}
		}
		return false;
	}
	int PosaljiNotifikacije(Datum datumSlanja)
	{
		/*metodi PosaljiNotifikacije se salje trenutni datum na osnovu cega ona pretrazuje sve studente koje treba
	podsjetiti/notoficirati o dogadjajima koji se priblizavaju.
	Koristeci multithread-ing, svim studentima se salju notifikacije sa sljedecim sadrzajem:
	-------------------------------------------------------------------------
	Postovani Jasmin Azemovic,
	Dogadjaj Ispit iz PRIII je zakazan za 3 dana, a do sada ste obavili 56% obaveza vezanih za ovaj dogadjaj. Neispunjene obaveze su:
	1.Preraditi ispitne zadatke
	2.Samostalno vjezbati
	Predlazemo Vam da ispunite i ostale planirane obaveze.
	FIT Reminder
	-------------------------------------------------------------------------
	Dakle, notifikacije ce biti poslane svim studentima koji su dodali dogadjaj za 30.01.2018. godine i oznacili da
	zele da budu podsjecani ponovo/rekurzivno najmanje 2 dana prije samog dogadjaja (podaci se odnose
	na konkretan dogadjaj: Ispit iz PRIII)

	*/
		int brojac = 0;
		vector<Student>::iterator itStudent = _reminiderList.begin();
		for (itStudent; itStudent != _reminiderList.end(); itStudent++)
		{
			vector<Dogadjaj>::iterator itDogadaji = itStudent->GetDogadjaji().begin();
			for (itDogadaji; itDogadaji != itStudent->GetDogadjaji().end(); itDogadaji++)
			{
				if (itDogadaji->getDatumObaveze() > datumSlanja)
				{
					brojac++;
					thread t1(&DLWMSReminder::PosaljiPoruku, this,
						"Postovani " + itStudent->getImePrezime() + ",\n" +
						"Dogadaj " + itDogadaji->GetNaziv() + " je zakazan za " +
						to_string(itDogadaji->getDatumObaveze().getDani() - datumSlanja.getDani()) + " dana, " +
						"a do sada ste obavili " +
						to_string([itDogadaji]()->int
							{
								int brojac = 0;
								for (int i = 0; i < itDogadaji->GetObaveze()->GetTrenutno(); i++)
								{
									if (itDogadaji->GetObaveze()->GetElement2(i))
									{
										brojac++;
									}
								}
								float rezultat = static_cast<float>(brojac) / static_cast<float>(itDogadaji->GetObaveze()->GetTrenutno()) * 100.0;
								int rezultat2 = static_cast<int>(round(rezultat));
								return rezultat2;
									
							}()) + "% obaveza vezanih za ovaj dogadjaj. Neispunjene obaveze su:\n" +
						[itDogadaji]()->string
							{
								int brojac = 1;
								string str = "";
								for (int i = 0; i < itDogadaji->GetObaveze()->GetTrenutno(); i++)
								{
									str += to_string(brojac) + ". " + itDogadaji->GetObaveze()->GetElement1(i) + "\n";
									brojac++;
								}
								return str;
							}() + "Predlazemo Vam da ispunite i ostale planirane obaveze.\nFIT Reminder" + crt

								);
					t1.join();
				}
			}
		}
		return brojac;
	}
};

void main()
{
	/**************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA
	   KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
	3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU CODE_ParcijalniII.TXT
	5. NAZIVI FUNKCIJA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U
	   TESTNOM CODE-U.OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI PO ŽELJI.
****************************************************************************/

#pragma region Datum

	Datum danas(28, 1, 2018), sutra(29, 1, 2018);
	Datum datumIspitaPRIII(30, 1, 2018), datumIspitBPII(31, 1, 2018);
	Datum prekosutra(danas);
	prekosutra = danas;
	cout << danas << endl
		<< sutra << endl
		<< prekosutra << crt;

#pragma endregion

#pragma region Kolekcija

	/*
	AddElement :: omogucava dodavanje novog elementa u kolekciju. Ukoliko je moguce, osigurati automatsko
	prosiranje kolekcije prilikom dodavanja svakog novog elementa, te onemoguciti ponavljanje elemenata
	RemoveElement :: na osnovu parametra tipa T1 uklanja elemente iz kolekcije i ukoliko je moguce smanjuje
	velicinu niza/kolekcije. Prilikom uklanjanja elemenata ocuvati redoslijed njihovog dodavanja
	*/
	const int brElemenata = 10;
	Kolekcija<int, float> kolekcija1;
	for (size_t i = 0; i < brElemenata; i++)
		if (!kolekcija1.AddElement(i, i + (0.6 * i)))
			cout << "Elementi " << i << " i " << i + (0.6 * i) << " nisu dodati u kolekciju" << endl;

	cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << crt;
	cout << kolekcija1 << endl;

	kolekcija1.RemoveElement(1);

	Kolekcija<int, float> kolekcija2;
	kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;

	if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;

	Kolekcija<int, float> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;

#pragma endregion

#pragma region Dogadjaj

	Dogadjaj ispitPRIII(datumIspitaPRIII, "Ispit iz PRIII", 5, true),
		ispitBPII(datumIspitBPII, "Ispit iz BPII", 7, true);
	/*po vlasitom izboru definisati listu zabranjenih rijeci koje ce onemoguciti dodavanje odredjene obaveze.
	Prilikom provjere koristiti regex*/
	listaZabranjeniRijeci.push_back("prepisivati");
	listaZabranjeniRijeci.push_back("wikipedia");
	listaZabranjeniRijeci.push_back("kopirati");
	listaZabranjeniRijeci.push_back("prepisati");
	if (ispitPRIII.AddObavezu("Preraditi pdf materijale")) cout << "Obaveza dodana!" << endl;
	//onemoguciti dupliranje obaveza
	if (!ispitPRIII.AddObavezu("Preraditi pdf materijale")) cout << "Obaveza nije dodana!" << endl;
	if (ispitPRIII.AddObavezu("Pregledati video materijale")) cout << "Obaveza dodana!" << endl;
	if (ispitPRIII.AddObavezu("Preraditi ispitne zadatke")) cout << "Obaveza dodana!" << endl;
	if (ispitPRIII.AddObavezu("Samostalno vjezbati")) cout << "Obaveza dodana!" << endl;

	if (ispitBPII.AddObavezu("Preraditi knjigu SQL za 24 h")) cout << "Obaveza dodana!" << endl;
	if (ispitBPII.AddObavezu("Pregledati video materijale")) cout << "Obaveza dodana!" << endl;
	if (ispitBPII.AddObavezu("Napraviti bazu za konkretnu aplikaciju")) cout << "Obaveza dodana!" << endl;

	Student jasmin(150051, "Jasmin Azemovic"), adel(160061, "Adel Handzic");

	if (jasmin.AddDogadjaj(ispitPRIII) && jasmin.AddDogadjaj(ispitBPII))
		cout << "Dogadjaj uspjesno dodan!" << endl;

	if (adel.AddDogadjaj(ispitPRIII) && adel.AddDogadjaj(ispitBPII))
		cout << "Dogadjaj uspjesno dodan!" << endl;
	//onemoguciti dupliranje dogadjaja
	if (!adel.AddDogadjaj(ispitPRIII))
		cout << "Dogadjaj nije uspjesno dodan!" << endl;

	DLWMSReminder reminder;

	try
	{
		reminder.AddStudent(jasmin);
		reminder.AddStudent(adel);
		//u slucaju dupliranja studenata funkcija baca izuzetak tipa Izuzetak
		reminder.AddStudent(jasmin);
	}
	catch (exception& err)
	{
		cout << crt << *dynamic_cast<Izuzetak*>(&err) << crt;
	}

	//da bi bila oznacena kao zavrsena, obaveza mora postojati i mora biti oznacena kao nezavrsena (false)
	if (reminder.OznaciObavezuKaoZavrsenu(150051, "Ispit iz PRIII", "Pregledati video materijale"))
		cout << "Obaveza oznacena kao zavrsena" << endl;
	if (reminder.OznaciObavezuKaoZavrsenu(150051, "Ispit iz PRIII", "Preraditi ispitne zadatke"))
		cout << "Obaveza oznacena kao zavrsena" << endl;

	/*metodi PosaljiNotifikacije se salje trenutni datum na osnovu cega ona pretrazuje sve studente koje treba
	podsjetiti/notoficirati o dogadjajima koji se priblizavaju.
	Koristeci multithread-ing, svim studentima se salju notifikacije sa sljedecim sadrzajem:
	-------------------------------------------------------------------------
	Postovani Jasmin Azemovic,
	Dogadjaj Ispit iz PRIII je zakazan za 3 dana, a do sada ste obavili 56% obaveza vezanih za ovaj dogadjaj. Neispunjene obaveze su:
	1.Preraditi ispitne zadatke
	2.Samostalno vjezbati
	Predlazemo Vam da ispunite i ostale planirane obaveze.
	FIT Reminder
	-------------------------------------------------------------------------
	Dakle, notifikacije ce biti poslane svim studentima koji su dodali dogadjaj za 30.01.2018. godine i oznacili da
	zele da budu podsjecani ponovo/rekurzivno najmanje 2 dana prije samog dogadjaja (podaci se odnose
	na konkretan dogadjaj: Ispit iz PRIII)

	*/
	int poslato = 0;
	//funkcija vraca broj poslatih podsjetnika/notifikacija
	poslato = reminder.PosaljiNotifikacije(danas);
	cout << "Za " << danas << " poslato ukupno " << poslato << " podsjetnika!" << endl;
	poslato = reminder.PosaljiNotifikacije(sutra);
	cout << "Za " << sutra << " poslato ukupno " << poslato << " podsjetnika!" << endl;

#pragma endregion

	system("pause");
}