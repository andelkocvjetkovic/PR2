#include <iostream>
#include <vector>
#include <regex>
#include<algorithm>
#include<iterator>

using namespace std;

enum Predmeti { Matematika, Historija, Geografia, Hemija, Biologija, Engleski, Muzicko, Fizicko };
enum Razredi { I1, I2, I3, II1, II2, II3, III1, III2, III3, IV1, IV2, IV3 };
const char* ispisPredmeti[] = { "Matematika", "Historija", "Geografia", "Hemija", "Biologija", "Engleski", "Muzicko", "Fizicko" };
const char* ispisRazredi[] = { "I1", "I2", "I3", "II1", "II2", "II3", "III1", "III2", "III3", "IV1", "IV2", "IV3" };
const char* crt = "\n------------------------------------------------\n";
template<class T1, class T2>
class Kolekcija
{
	friend ostream& operator<< <>(ostream& COUT, const Kolekcija<T1, T2>& obj);
private:
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	Kolekcija()
	{
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	Kolekcija(const Kolekcija<T1, T2>& org) :
		_elementi1(new T1[org._trenutno]),
		_elementi2(new T2[org._trenutno]),
		_trenutno(org._trenutno)
	{
		for (int i = 0; i < org._trenutno; i++)
		{
			_elementi1[i] = org._elementi1[i];
			_elementi2[i] = org._elementi2[i];
		}
	}
	~Kolekcija()
	{
		delete[]_elementi1; _elementi1 = NULL;
		delete[]_elementi2; _elementi2 = NULL;
	}
	Kolekcija& operator=(const Kolekcija<T1, T2>& obj)
	{
		if (this != &obj)
		{
			delete[]_elementi1;
			delete[]_elementi2;
			_elementi1 = new T1[obj._trenutno];
			_elementi2 = new T2[obj._trenutno];
			_trenutno = obj._trenutno;
			for (int i = 0; i < _trenutno; i++)
			{
				_elementi1[i] = obj._elementi1[i];
				_elementi2[i] = obj._elementi2[i];
			}
		}
		return *this;
	}
	int GetTrenutno()const { return _trenutno; }
	void umanjiTrenutno() { _trenutno--; }
	void uvecajTrenutno() { _trenutno++; }
	T1& GetElement1(int lokacija)const
	{
		return _elementi1[lokacija];
	}
	T2& GetElement2(int lokacija)const
	{
		return _elementi2[lokacija];
	}
	void AddElement(const T1& elem1, const  T2& elem2)
	{
		T1* temp1 = new T1[GetTrenutno() + 1];
		T2* temp2 = new T2[GetTrenutno() + 1];
		for (int i = 0; i < GetTrenutno(); i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		temp1[GetTrenutno()] = elem1;
		temp2[GetTrenutno()] = elem2;
		uvecajTrenutno();
		delete[]_elementi1;
		delete[]_elementi2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		temp1 = NULL;
		temp2 = NULL;
	}
	void RemoveElement(int lokacija)
	{
		if (lokacija >= GetTrenutno())
			return;
		for (size_t j = lokacija; j < _trenutno - 1; j++)
		{
			_elementi1[j] = _elementi1[j + 1];
			_elementi2[j] = _elementi2[j + 1];
		}
		_trenutno--;

	}
};
template<class T1, class T2>
ostream& operator<< (ostream& COUT, const Kolekcija<T1, T2>& obj)
{
	for (int i = 0; i < obj.GetTrenutno(); i++)
	{
		COUT << obj.GetElement1(i) << "\t" << obj.GetElement2(i) << endl;
	}
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

	Datum(const Datum& obj)
	{
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
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
			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
		}
		return *this;
	}
	friend ostream& operator<< (ostream& COUT, const Datum& obj)
	{
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}
	bool operator==(const Datum& obj)
	{
		if (*_godina == *obj._godina)
			if (*_mjesec == *obj._mjesec)
				if (*_dan = *obj._dan)
					return true;
		return false;
	}
};
bool ProvjeriValidnostBrojaTelefona(string telefon);
class Osoba
{
	friend ostream& operator<<(ostream& COUT, const Osoba& obj);
protected:
	char* _imePrezime;
	Datum* _datumRodjenja;
	string _brojTelefona;
public:
	Osoba(const char* imePrezime, Datum datum)
	{
		_imePrezime = new char[strlen(imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(imePrezime) + 1, imePrezime);
		_datumRodjenja = new Datum(datum);
		_brojTelefona = "";
	}
	Osoba(const Osoba& obj) :_brojTelefona(obj._brojTelefona)
	{
		_imePrezime = new char[strlen(obj._imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
		_datumRodjenja = new Datum(*obj._datumRodjenja);
	}
	~Osoba() { delete[]_imePrezime; delete _datumRodjenja; }
	Osoba& operator=(const Osoba& obj)
	{
		if (this != &obj)
		{
			delete[]_imePrezime;
			_imePrezime = new char[strlen(obj._imePrezime) + 1];
			strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
			_brojTelefona = obj._brojTelefona;
			_datumRodjenja->operator=(*obj._datumRodjenja);

		}
		return *this;
	}
	bool setBrojTelefona(string brojTelefona)
	{
		if (ProvjeriValidnostBrojaTelefona(brojTelefona))
		{
			_brojTelefona = brojTelefona;
			return true;
		}
		return false;
	}
	bool operator==(const Osoba& obj)
	{
		if (_brojTelefona == obj._brojTelefona)
			if (*_datumRodjenja == *obj._datumRodjenja)
				if (strcmp(_imePrezime, obj._imePrezime) == 0)
					return true;
		return false;
	}


};
ostream& operator<<(ostream& COUT, const Osoba& obj)
{
	COUT << "Ime Prezime: " << obj._imePrezime << endl;
	COUT << "Broj telefona: " << obj._brojTelefona << endl;
	COUT << "Datum rodenja: " << *obj._datumRodjenja << endl;
	return COUT;
}
class Nastavnik :public Osoba
{
	friend ostream& operator<<(ostream& COUT, const Nastavnik& obj)
	{
		COUT << static_cast<Osoba>(obj);
		COUT << "Predmeti i razredi: " << endl;
		for (int i = 0; i < obj._predmetiRazredi->GetTrenutno(); i++)
		{
			COUT << "Predmet: " << ispisPredmeti[obj._predmetiRazredi->GetElement1(i)] <<
				"\tRazred: " << ispisRazredi[obj._predmetiRazredi->GetElement2(i)] << endl;
		}
		return COUT;
	}
	Kolekcija<Predmeti, Razredi>* _predmetiRazredi;
public:
	Nastavnik(const char* imePrezime, Datum datumRodenja) :
		Osoba(imePrezime, datumRodenja), _predmetiRazredi(new Kolekcija<Predmeti, Razredi>)
	{   }
	Nastavnik(const Nastavnik& org) :
		Osoba(org), _predmetiRazredi(new Kolekcija<Predmeti, Razredi>(*org._predmetiRazredi))
	{   }
	Nastavnik& operator=(const Nastavnik& obj)
	{
		if (this != &obj)
		{
			Osoba::operator=(obj);
			_predmetiRazredi->operator=(*obj._predmetiRazredi);
		}
		return *this;
	}
	bool AddPredmetRazred(Predmeti predmet, Razredi razred)
	{
		/*NASTAVNIK MOZE PREDAVATI ISTI PREDMET, ALI TO MORA BITI U RAZLICITIM ODJELJENJIMA
		I NAJVISE U DVA RAZREDA (NA DVIJE RAZLICITE GODINE)*/
		int brojac = 0;
		for (int i = 0; i < _predmetiRazredi->GetTrenutno(); i++)
		{
			if (_predmetiRazredi->GetElement1(i) == predmet &&
				_predmetiRazredi->GetElement2(i) == razred)
				return false;
			if (_predmetiRazredi->GetElement1(i) == predmet)
				brojac++;
		}
		if (brojac >= 2)
			return false;
		_predmetiRazredi->AddElement(predmet, razred);
		return true;
	}
	bool RemovePredmetRazred(Predmeti predmet)
	{
		for (int i = 0; i < _predmetiRazredi->GetTrenutno(); i++)
		{
			if (_predmetiRazredi->GetElement1(i) == predmet)
			{
				_predmetiRazredi->RemoveElement(i);
				return true;
			}
		}
		return false;
	}
	bool operator==(const Nastavnik obj)
	{
		if (Osoba::operator==(obj))
		{
			if (_predmetiRazredi->GetTrenutno() == obj._predmetiRazredi->GetTrenutno())
			{
				for (int i = 0; i < _predmetiRazredi->GetTrenutno(); i++)
				{
					if (_predmetiRazredi->GetElement1(i) == obj._predmetiRazredi->GetElement1(i) &&
						_predmetiRazredi->GetElement2(i) == obj._predmetiRazredi->GetElement2(i))
						continue;
					else
						return false;
				}
			}
			return true;

		}
		return false;
	}
	Kolekcija<Predmeti, Razredi>& getPredmetiRazredi()
	{
		return *_predmetiRazredi;
	}
	char* getImePrezime()const { return _imePrezime; }
};
bool ProvjeriValidnostBrojaTelefona(string telefon)//globalna funkcija
{
	//broj telefona treba biti u formatu: +387(61)-222-333
	string uvijet = "\\+387\\(6[0-4]{1}\\)\\-[0-9]{3}\\-[0-9]{3}";
	regex rgx(uvijet);
	if (regex_match(telefon, rgx))
		return true;
	return false;
}
class Ucenik : public Osoba
{
	friend ostream& operator<<(ostream& COUT, const Ucenik& obj)
	{
		COUT << crt;
		COUT << static_cast<Osoba>(obj);
		COUT << "Broj u dnevniku: " << obj._brojUDnevniku << endl;
		COUT << "Razred: " << ispisRazredi[obj._razred] << endl;
		COUT << "Nastavnici: " << endl;
		for (int i = 0; i < obj._nastavnici.size(); i++)
		{
			for (int j = 0; j < obj._nastavnici[i]->getPredmetiRazredi().GetTrenutno(); j++)
			{
				if (obj._razred == obj._nastavnici[i]->getPredmetiRazredi().GetElement2(j))
				{


					COUT << "Ime prezime: " << obj._nastavnici[i]->getImePrezime() << "\tPredmet: " << ispisPredmeti[obj._nastavnici[i]->getPredmetiRazredi().GetElement1(j)] << endl;


				}
			}
		}
		COUT << "Predmeti i ocjene: " << endl;
		for (int i = 0; i < obj._predmetiOcjene.GetTrenutno(); i++)
		{
			COUT << "Predmet: " << ispisPredmeti[obj._predmetiOcjene.GetElement1(i)] << "\tOcijena: " << obj._predmetiOcjene.GetElement2(i) << endl;
		}
		COUT << crt;
		return COUT;
	}
private:
	int _brojUDnevniku;
	Razredi _razred;
	vector<Nastavnik*> _nastavnici;
	Kolekcija<Predmeti, int> _predmetiOcjene;
public:
	Ucenik(const char* imePrezime, Datum datumRodenja, int brojUDnevniku, Razredi razred) :
		Osoba(imePrezime, datumRodenja), _razred(razred), _brojUDnevniku(brojUDnevniku)
	{   }
	Ucenik(const Ucenik& org) :
		Osoba(org), _brojUDnevniku(org._brojUDnevniku), _razred(org._razred),
		_nastavnici(org._nastavnici), _predmetiOcjene(org._predmetiOcjene)
	{   }
	Ucenik& operator=(const Ucenik& obj)
	{
		if (this != &obj)
		{
			_brojUDnevniku = obj._brojUDnevniku;
			_razred = obj._razred;
			_nastavnici.operator=(obj._nastavnici);
			_predmetiOcjene.operator=(obj._predmetiOcjene);
		}
		return *this;
	}
	bool AddNastavnik(Nastavnik& nastavnik)
	{
		/*VODITI RACUNA DA SE DODAJU REFERENCE NA NASTAVNIKE KAKO BI SVI UCENICI
		DIJELILI ISTE INFORMACIJE O NASTAVNICIMA U SLUCAJU DA SE NASTAVNIKU DODA NEKI PREDMET*/
		/*ZA USPJESNO DODAVANJE, NASTAVNIK MORA POSJEDOVATI (PREDAVATI)
		NAJMANJE JEDAN PREDMET U TOM RAZREDU */
		bool flag = true;
		for (int i = 0; i < nastavnik.getPredmetiRazredi().GetTrenutno(); i++)
		{
			if (nastavnik.getPredmetiRazredi().GetElement2(i) == _razred)
				flag = false;
		}
		if (flag)
			return false;
		_nastavnici.push_back(&nastavnik);
	}
	bool AddPredmetOcjenu(Predmeti predmet, int ocijena)
	{
		/*DA BI SE DODALA OCJENA ZA ODREDJENI PREDMET UCENIK MORA POSJEDOVATI NASTAVNIKA
		KOJI PREDAJE TAJ PREDMET*/
		bool flag = true;
		for (int i = 0; i < _nastavnici.size(); i++)
		{

			for (int j = 0; j < _nastavnici[i]->getPredmetiRazredi().GetTrenutno(); j++)
			{
				if (_nastavnici[i]->getPredmetiRazredi().GetElement1(j) == predmet && _nastavnici[i]->getPredmetiRazredi().GetElement2(j) == _razred)
					flag = false;
			}


		}
		if (flag)
			return false;
		_predmetiOcjene.AddElement(predmet, ocijena);
		return true;
	}
	vector<Nastavnik*>& getNastavnici()
	{
		return _nastavnici;
	}
	Kolekcija<Predmeti, int>& getPredmetiOcjene()
	{
		return _predmetiOcjene;
	}
	double GetProsjekByNastavnik(string imePrezime)
	{
		double prosjek = 0;
		int brojac = 0;
		for (int i = 0; i < _nastavnici.size(); i++)
		{
			if (strcmp(_nastavnici[i]->getImePrezime(), imePrezime.c_str()) == 0)
			{
				for (int j = 0; j < _predmetiOcjene.GetTrenutno(); j++)
				{
					for (int k = 0; k < _nastavnici[i]->getPredmetiRazredi().GetTrenutno(); k++)
					{
						if (_predmetiOcjene.GetElement1(j) == _nastavnici[i]->getPredmetiRazredi().GetElement1(k))
						{
							prosjek += _predmetiOcjene.GetElement2(j);
							brojac++;
						}
					}
				}
			}
		}
		if (brojac == 0)
			return 0;
		return prosjek / static_cast<double>(brojac);
	}
};

void main()
{
	/****************************************************************************
	// 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	// 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR
	// CE BITI OZNACENO KAO "RE"
	// 3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	// 4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU CODE_.TXT
	// 5. NAZIV I LISTA PARAMETARA FUNKCIJA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U.
	// 6. NIJE POTREBNO BACATI IZUZETKE. SVE FUNKCIJE VRACAJU TRUE ILI FALSE
	****************************************************************************/

#pragma region TestiranjeDatuma
	Datum danas(26, 11, 2015);
	Datum sutra(danas);
	Datum prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion

#pragma region TestiranjeKolekcije
	Kolekcija<int, int> kolekcija1;
	int brojElemenata = 15;
	for (size_t i = 0; i < brojElemenata; i++)
		kolekcija1.AddElement(i, i);

	kolekcija1.RemoveElement(0);//uklanja element na osnovu lokacije
	cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
	cout << kolekcija1 << endl;

	Kolekcija<int, int> kolekcija2;
	kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;
	if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;
	Kolekcija<int, int> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;
#pragma endregion

#pragma region TestiranjeNastavnika
	Nastavnik jasmin("Jasmin Azemovic", Datum(15, 2, 1980));
	Nastavnik adel("Adel Handzic", Datum(15, 10, 1981));
	Nastavnik nedeljko("Nedeljko Cvjetkovic", Datum(20, 05, 1954));
	Nastavnik Julijana("Julijana Cvjetkovic", Datum(23, 10, 1991));
	/*NASTAVNIK MOZE PREDAVATI ISTI PREDMET, ALI TO MORA BITI U RAZLICITIM ODJELJENJIMA I NAJVISE U DVA
	RAZREDA (NA DVIJE RAZLICITE GODINE)*/
	if (jasmin.AddPredmetRazred(Matematika, I1))
		cout << "Predmeti dodan" << endl;
	if (jasmin.AddPredmetRazred(Historija, I1))
		cout << "Predmeti dodan" << endl;

	if (jasmin.AddPredmetRazred(Geografia, I1))
		cout << "Predmeti dodan" << endl;

	if (jasmin.AddPredmetRazred(Geografia, III1))
		cout << "Predmeti dodan" << endl;

	if (adel.AddPredmetRazred(Historija, III1))
		cout << "Predmeti dodan" << endl;

	if (nedeljko.AddPredmetRazred(Fizicko, III1))
		cout << "Predmeti dodan" << endl;
	if (Julijana.AddPredmetRazred(Biologija, III1))
		cout << "Predmeti dodan" << endl;
	if (Julijana.AddPredmetRazred(Engleski, III1))
		cout << "Predmeti dodan" << endl;
	if (adel.RemovePredmetRazred(Historija))
		cout << "Predmet uklonjen!" << endl;

	Nastavnik goran(adel);
	if (goran == adel)
		cout << "Nastavnici identicni" << endl;
	goran = jasmin;
	if (goran == jasmin)
		cout << "Nastavnici identicni" << endl;

	Kolekcija<Predmeti, Razredi>& predmetiRazredi = jasmin.getPredmetiRazredi();

	if (!jasmin.setBrojTelefona("+38761111222"))
		cout << "Broj telefona nije dodan!" << endl;
	if (!jasmin.setBrojTelefona("+387-61-111-222"))
		cout << "Broj telefona nije dodan!" << endl;
	if (jasmin.setBrojTelefona("+387(61)-111-222"))
		cout << "Broj telefona uspjesno dodan!" << endl;
#pragma endregion

#pragma region TestiranjeUcenika
	Ucenik denis("Denis Music", Datum(8, 10, 1990), 3, III1);
	Ucenik elmin("Elmin Sudic", Datum(15, 6, 1993), 1, I1);
	Ucenik adil("Adil Joldic", Datum(13, 8, 1992), 2, II1);
	/*VODITI RACUNA DA SE DODAJU REFERENCE NA NASTAVNIKE KAKO BI SVI UCENICI DIJELILI ISTE INFORMACIJE O
	NASTAVNICIMA U SLUCAJU DA SE NASTAVNIKU DODA NEKI PREDMET*/
	/*ZA USPJESNO DODAVANJE, NASTAVNIK MORA POSJEDOVATI (PREDAVATI) NAJMANJE JEDAN PREDMET U TOM RAZREDU */
	if (denis.AddNastavnik(jasmin))
		cout << "Nastavnik uspjesno dodan" << endl;
	/*ONEMOGUCITI DODAVANJE ISTIH NASTAVNIKA; POJAM ISTI PODRAZUMIJEVA IDENTICNE VRIJEDNOSTI SVIH ATRIBUTA*/
	if (denis.AddNastavnik(adel))
		cout << "Nastavnik uspjesno dodan" << endl;
	if (denis.AddNastavnik(nedeljko))
		cout << "Nastavnik uspjesno dodan" << endl;
	if (denis.AddNastavnik(Julijana))
		cout << "Nastavnik uspjesno dodan" << endl;
	/*DA BI SE DODALA OCJENA ZA ODREDJENI PREDMET UCENIK MORA POSJEDOVATI NASTAVNIKA KOJI PREDAJE TAJ PREDMET*/
	if (denis.AddPredmetOcjenu(Matematika, 3))
		cout << "Ocjena uspjesno dodana" << endl;
	if (denis.AddPredmetOcjenu(Historija, 4))
		cout << "Ocjena uspjesno dodana" << endl;
	if (denis.AddPredmetOcjenu(Engleski, 3))
		cout << "Ocjena uspjesno dodana" << endl;
	if (denis.AddPredmetOcjenu(Biologija, 5))
		cout << "Ocjena uspjesno dodana" << endl;
	denis.setBrojTelefona("+387(61)-111-222");
	vector<Nastavnik*>& nastavnici = denis.getNastavnici();
	cout << "Ucenik Denis ima dodijeljena " << nastavnici.size() << " nastavnika" << endl;

	Kolekcija<Predmeti, int>& uspjeh = denis.getPredmetiOcjene();
	cout << "Denis ima " << uspjeh.GetTrenutno() << " polozenih predmeta" << endl;
	cout << "Prosjecna ocjena kod nastavnika Jasmin Azemovic je " << denis.GetProsjekByNastavnik("Jasmin Azemovic") << endl;

	Ucenik denis2(denis);
	cout << denis2 << endl;

#pragma endregion
	system("pause");
}