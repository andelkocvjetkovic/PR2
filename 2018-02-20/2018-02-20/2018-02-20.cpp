#include <iostream>
#include <vector>
#include<iterator>
#include<string>
#include<sstream>
#include<thread>
#include<mutex>


using namespace std;
mutex mutObj;
const char* crt = "\n-------------------------------------------\n";
enum OznakaKrvneGrupe { O, A, B, AB };
enum Sortiranje { ASC, DESC };
const char* OznakaKrvneGrupeString[] = { "O", "A", "B", "AB" };

class Datum
{
	int* _dan, * _mjesec, * _godina;
public:
	Datum(int dan, int mjesec, int godina)
	{
		_dan = new int(dan), _mjesec = new int(mjesec), _godina = new int(godina);
	}
	Datum(const Datum& org) :
		_dan(new int(*org._dan)),
		_mjesec(new int(*org._mjesec)),
		_godina(new int(*org._godina))
	{   }
	~Datum() { delete _dan; delete _mjesec; delete _godina; }
	Datum& operator=(const Datum& obj) {
		if (this != &obj)
		{
			delete _dan; delete _mjesec; delete _godina;
			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
		}
		return *this;
	}
	friend ostream& operator<<(ostream& COUT, const Datum& obj)
	{
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina << endl;
		return COUT;
	}
	int getDane()const {
		return *_dan + (*_mjesec * 30) + (*_godina * 365);
	}
	string toString() {
		stringstream ob;
		ob << "Datum donacije: ";
		ob << *_dan;
		ob << ".";
		ob << *_mjesec;
		ob << ".";
		ob << *_godina;
		ob << "\n";
		return ob.str();
	}
	bool operator==(const Datum& desni) {
		if (*_dan == *desni._dan)
			if (*_mjesec == *desni._mjesec)
				if (*_godina == *desni._godina)
					return true;
		return false;
	}
};

template <class T1, class T2, int max, Sortiranje sortiranje = ASC>
class Kolekcija
{
	T1* _elementi1[max];
	T2* _elementi2[max];
	int _trenutnoElemenata;
	bool _sortiranje;
	void bubleSort() {
		if (_sortiranje == ASC) {
			int ok;
			for (int i = _trenutnoElemenata - 1; i > 0; i--) {
				ok = 1;
				for (int j = 0; j < i; j++) {
					if (*_elementi2[j + 1] < *_elementi2[j]) {
						T1* temp1 = new T1(*_elementi1[j]);
						T2* temp2 = new T2(*_elementi2[j]);

						*_elementi1[j] = *_elementi1[j + 1];
						*_elementi2[j] = *_elementi2[j + 1];

						*_elementi1[j + 1] = *temp1;
						*_elementi2[j + 1] = *temp2;
						ok = 0;
						temp1 = NULL;
						temp2 = NULL;
					}
				}
				if (ok)
					break;
			}

		}
		else if (_sortiranje == DESC) {
			for (int i = 0; i < (_trenutnoElemenata - 1); i++) {
				for (int j = 0; j < _trenutnoElemenata - i - 1; j++) {
					if (*_elementi2[j] < *_elementi2[j + 1]) {
						T1* temp1 = new T1(*_elementi1[j]);
						T2* temp2 = new T2(*_elementi2[j]);

						*_elementi1[j] = *_elementi1[j + 1];
						*_elementi2[j] = *_elementi2[j + 1];

						*_elementi1[j + 1] = *temp1;
						*_elementi2[j + 1] = *temp2;

						temp1 = NULL;
						temp2 = NULL;
					}
				}
			}

		}
	}
public:
	Kolekcija(bool sort = true)
	{
		_trenutnoElemenata = 0;
		_sortiranje = sortiranje;
	}
	Kolekcija(const Kolekcija<T1, T2, max, sortiranje>& org) :
		_trenutnoElemenata(org._trenutnoElemenata),
		_sortiranje(org._sortiranje) {
		*_elementi1 = new T1[max];
		*_elementi2 = new T2[max];
		for (int i = 0; i < _trenutnoElemenata; i++) {
			_elementi1[i] = new T1(*org._elementi1[i]);
			_elementi2[i] = new T1(*org._elementi2[i]);
		}
	}
	~Kolekcija()
	{
		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}
	Kolekcija& operator=(const Kolekcija<T1, T2, max, sortiranje>& obj) {
		if (this != &obj) {
			for (size_t i = 0; i < _trenutnoElemenata; i++)
			{
				delete _elementi1[i]; _elementi1[i] = nullptr;
				delete _elementi2[i]; _elementi2[i] = nullptr;
			}
			*_elementi1 = new T1[max];
			*_elementi2 = new T2[max];
			for (int i = 0; i < obj._trenutnoElemenata; i++) {
				*_elementi1[i] = new T1(*obj._elementi1[i]);
				*_elementi2[i] = new T2(*obj._elementi2[i]);
			}
			_trenutnoElemenata = obj._trenutnoElemenata;
			_sortiranje = obj._sortiranje;
		}
		return *this;
	}
	bool AddElement(T1 ele1, T2 ele2)
	{
		/*ukoliko je vrijednost atributa _sortiranje true, prilikom svakog dodavanja elementa se treba
		vrsiti sortiranje po redoslijedu definisanom u formalnom template argumentu sortiranje*/
		if (_trenutnoElemenata == max)
			return false;
		_elementi1[_trenutnoElemenata] = new T1(ele1);
		_elementi2[_trenutnoElemenata] = new T2(ele2);
		_trenutnoElemenata++;
		bubleSort();
		return true;
	}
	void SetElement2(int lokacija, const T2& ele2)
	{
		_elementi2[lokacija] = new T2(ele2);
	}
	void SetSortiranje(bool sortiranje)
	{
		_sortiranje = sortiranje;
	}
	int GetTrenutno() const { return _trenutnoElemenata; }
	int GetMax() const { return max; }

	T1 GetElement1(int lokacija) const
	{
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception("Nepostojeca lokacija!");
		return *_elementi1[lokacija];
	}
	T2 GetElement2(int lokacija) const
	{
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception("Nepostojeca lokacija!");
		return *_elementi2[lokacija];
	}
	friend ostream& operator<<(ostream& COUT, const Kolekcija& obj)
	{
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}
};

class KrvnaGrupa
{
	friend ostream& operator<<(ostream& COUT, KrvnaGrupa obj);
private:
	/*postoje 4 krvne grupe koje su navedene u enumeracije OznakaKrvneGrupe, pri cemu svaka od navedenih moze imati
	pozitivan (+) i negativan (-) Rh faktor*/
	OznakaKrvneGrupe _oznaka;//npr: AB
	char _rhFaktor; // + ili -
	//prethodno pomenuto je izuzetno bitno iz razloga sto postoje jasna pravila vezana za darivanje krvi tj. 
	//koji primalac moze primiti krv od kojeg donatora sto je prikazano u tabeli, a naredna dva vector-a su zaduzena
	//da cuvaju informacije o tome, npr. za krvnu grupu A+ vector donatori ce cuvati vrijednosti: A+ i AB+ 
	vector<KrvnaGrupa*> _donatori; //krvne grupe kojima odredjena krvna grupa moze donirati krv.
	vector<KrvnaGrupa*> _primaoci; //krvne grupe od kojih odredjena krvna grupa moze primiti krv.
public:
	KrvnaGrupa(OznakaKrvneGrupe oznaka, char rhFaktor) :
		_oznaka(oznaka), _rhFaktor(rhFaktor) {}
	//KrvnaGrupa(const KrvnaGrupa& org):
	//    _oznaka(org._oznaka),
	//    _rhFaktor(org._rhFaktor),
	//    _donatori(org._donatori),
	//    _primaoci(org._primaoci) {}
	void SetDonatori(vector<KrvnaGrupa*> donatori)
	{
		_donatori = donatori;
	}
	void SetPrimaoci(vector<KrvnaGrupa*> primaoci)
	{
		_primaoci = primaoci;
	}
	vector<KrvnaGrupa*> getDonirati() {
		return _donatori;
	}
	vector<KrvnaGrupa*>& getPrimaoci() {
		return _primaoci;
	}
	bool operator== (const KrvnaGrupa& desno) {
		if (strcmp(OznakaKrvneGrupeString[_oznaka], OznakaKrvneGrupeString[desno._oznaka]) == 0)
			if (_rhFaktor == desno._rhFaktor)
				return true;
		return false;
	}
	string ToString() {

	}
	OznakaKrvneGrupe getKrvnuGrupuOznaku() {
		return _oznaka;
	}
	char getRhFaktor() {
		return _rhFaktor;
	}
};

ostream& operator<<(ostream& COUT, KrvnaGrupa obj) {
	COUT << "Oznaka krvne grupe: " << OznakaKrvneGrupeString[obj._oznaka] << endl;
	COUT << "Rh Faktor: " << obj._rhFaktor << endl;
	COUT << "Osoba moze donirati krv nekoj od navedeni krvni grupa: " << endl;
	for (int i = 0; i < obj._donatori.size(); i++) {
		COUT << OznakaKrvneGrupeString[obj._donatori[i]->_oznaka] << obj._donatori[i]->_rhFaktor << ",";
	}
	COUT << endl;
	COUT << "Osoba moze krv od neke od navedeni krvni grupa: " << endl;
	for (int i = 0; i < obj._primaoci.size(); i++) {
		COUT << OznakaKrvneGrupeString[obj._primaoci[i]->_oznaka] << obj._primaoci[i]->_rhFaktor << ",";
	}
	COUT << endl;
	return COUT;

}
class Osoba
{
	friend ostream& operator<<(ostream& COUT, Osoba& obj);
protected:
	char* _imePrezime;
	KrvnaGrupa* _krvnaGrupa;
public:
	Osoba(const char* imePrezime, KrvnaGrupa* krvnaGrupa) : _krvnaGrupa(krvnaGrupa)
	{
		_imePrezime = new char[strlen(imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(imePrezime) + 1, imePrezime);
	}
	Osoba(const Osoba& org) :
		_imePrezime(new char[strlen(org._imePrezime)]),
		_krvnaGrupa(new KrvnaGrupa(*org._krvnaGrupa)) {}
	~Osoba()
	{
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	Osoba& operator=(const Osoba& obj) {
		if (this != &obj) {
			delete[] _imePrezime; _imePrezime = nullptr;
			_imePrezime = new char[strlen(obj._imePrezime) + 1];
			strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
			*_krvnaGrupa = *obj._krvnaGrupa;
		}
	}
	virtual void Info() = 0;
	KrvnaGrupa getKrvnaGrupa()const {
		return *_krvnaGrupa;
	}
	string getImePrezime() {
		return string(_imePrezime);
	}
	bool operator== (const Osoba& desni) {
		if (strcmp(_imePrezime, desni._imePrezime) == 0)
			if (_krvnaGrupa == desni._krvnaGrupa)
				return true;

		return false;
	}
	string toString() {
		stringstream obj;
		obj << "Ime Prezime: ";
		obj << _imePrezime;
		obj << "\nKrvna grupa: ";
		obj << OznakaKrvneGrupeString[_krvnaGrupa->getKrvnuGrupuOznaku()];
		obj << _krvnaGrupa->getRhFaktor();
		return obj.str();
	}
};
ostream& operator<<(ostream& COUT, Osoba& obj) {
	return COUT;
}
bool ValidanFormat(string email) {
	return true;
}
class Donator : public Osoba
{
private:
	//u slucaju da email nije u validnom formatu potrebno ga je postaviti na podrazumijevanu vrijednost: info@donatori.ba
	string _email; //regex: nekiTekst@nekaDomena.com ili .ba ili .org
	Datum _datumPosljednjegDoniranja;
	bool _kontaktirajMe;

public:
	Donator(const char* imePrezime, KrvnaGrupa* krvnaGrupa, string email, Datum dpd, bool contact = true)
		: Osoba(imePrezime, krvnaGrupa), _datumPosljednjegDoniranja(dpd), _kontaktirajMe(contact)
	{
		if (ValidanFormat(email))
			_email = email;
		else
			_email = "info@donatori.ba";
	}
	Donator(const Donator& org) :
		Osoba(org),
		_email(org._email),
		_datumPosljednjegDoniranja(org._datumPosljednjegDoniranja),
		_kontaktirajMe(org._kontaktirajMe) {}
	Donator& operator=(const Donator& obj) {
		if (this != &obj) {
			Osoba::operator=(obj);
			_datumPosljednjegDoniranja = obj._datumPosljednjegDoniranja;
			_email = obj._email;
			_kontaktirajMe = obj._kontaktirajMe;
		}
	}
	bool kontaktiraj()const {
		return _kontaktirajMe;
	}
	Datum getDatumPosljednjeDonacije() const {
		return _datumPosljednjegDoniranja;
	}
	void setDatumPosljedenjeDonacije(Datum noviDatum) {
		_datumPosljednjegDoniranja = noviDatum;
	}
	virtual void Info() {
		cout << crt;
		cout << "Ime Prezime: " << _imePrezime << endl;
		cout << "Krvna grupa: " << *_krvnaGrupa << endl;
		cout << "Datum posljednjeg doniranja: " << _datumPosljednjegDoniranja << endl;
		cout << "Email adresa: " << _email << endl;
		cout << crt;
	}
	string toString() {
		stringstream obj;
		obj << "Ime Prezime: ";
		obj << this->_imePrezime;
		obj << " ";
		obj << "Krvna Grupa: ";
		obj << this->getKrvnaGrupa();
		return obj.str();
	}
};

class Zahtjev
{
	string _ustanova;
	KrvnaGrupa* _krvnaGrupa;
	double _kolicina;
public:
	Zahtjev(string ustanova, KrvnaGrupa* krvnaGrupa, double kolicina) :
		_ustanova(ustanova), _krvnaGrupa(new KrvnaGrupa(*krvnaGrupa)), _kolicina(kolicina) { }
	Zahtjev(const Zahtjev& org) :
		_ustanova(org._ustanova),
		_krvnaGrupa(new KrvnaGrupa(*org._krvnaGrupa)),
		_kolicina(org._kolicina) {}
	Zahtjev& operator=(const Zahtjev& obj) {
		if (this != &obj) {
			delete _krvnaGrupa;
			_krvnaGrupa = new KrvnaGrupa(*obj._krvnaGrupa);
			_ustanova = obj._ustanova;
			_kolicina = obj._kolicina;
		}
		return *this;
	}
	~Zahtjev()
	{
		delete _krvnaGrupa;
		_krvnaGrupa = NULL;
	}
	double getKolicina() {
		return _kolicina;
	}
	KrvnaGrupa* getKrvnaGrupa()const {
		return _krvnaGrupa;
	}
	friend ostream& operator<<(ostream& COUT, const Zahtjev& obj)
	{
		COUT << "Ustanova: " << obj._ustanova << endl;
		COUT << "Krvna grupa: " << obj._krvnaGrupa->ToString() << endl;
		COUT << "Kolicina: " << obj._kolicina << endl;
		return COUT;
	}
	string toString() {
		stringstream objTemp;
		int oznaka = _krvnaGrupa->getKrvnuGrupuOznaku();
		objTemp << OznakaKrvneGrupeString[oznaka];
		char faktor = _krvnaGrupa->getRhFaktor();
		objTemp << faktor;
		return objTemp.str();
	}

};

class TransfuzijskiCentar
{
	//stanje zaliha za svaku pojedinu krvnu grupu
	Kolekcija<KrvnaGrupa, double, 8, DESC> _zalihe;
	//evidentira svaku donaciju krvi
	Kolekcija<Osoba*, double, 100> _donacije;
	Kolekcija<Osoba*, double, 100> _listaDonatora;
	vector<Zahtjev> _zahtjevi;
	void posaljiEmail(string emails) {
		mutObj.lock();
		cout << emails << endl;
		mutObj.unlock();
	}
public:
	TransfuzijskiCentar() {

	}
	void AddDonaciju(Datum datumDonacije, Osoba* osobaKojaDonira, double kolicina) {
		// prilikom svake donacije je potrebno povecati zalihe za tu krvnu grupu
		// ukoliko krvna grupa vec postoji na zalihi onda se uvecava samo njena kolicina
		// takodjer, donatoru se postavlja nova vrijednost datuma posljednje donacije.
		// ukoliko je neki od dodantora darivao krv prije isteka 3 mjeseca od posljednjeg
		// darivanja, funkcija baca izuzetak datum donacije, donator, kolicina
		bool jeliIkadDonirala = false;
		for (int i = 0; i < _donacije.GetTrenutno(); i++)
		{
			if (*_donacije.GetElement1(i) == *osobaKojaDonira)
			{
				jeliIkadDonirala = true;
				break;
			}
		}
		Donator* pokOsoba = dynamic_cast<Donator*>(osobaKojaDonira);
		if (jeliIkadDonirala)
		{
			Donator* pokOsoba = dynamic_cast<Donator*>(osobaKojaDonira);
			int zadnjaDonacija = pokOsoba->getDatumPosljednjeDonacije().getDane();
			int danasnjaDonacija = datumDonacije.getDane();
			int rezDatuma = danasnjaDonacija - zadnjaDonacija;

			if (rezDatuma > 90)
			{
				_donacije.AddElement(osobaKojaDonira, kolicina);
				pokOsoba->setDatumPosljedenjeDonacije(datumDonacije);
			}
			else {
				string zaThrow;
				zaThrow += pokOsoba->getDatumPosljednjeDonacije().toString();
				zaThrow += osobaKojaDonira->toString();
				double kol = 0;
				for (int i = 0; i < _donacije.GetTrenutno(); i++) {
					if (osobaKojaDonira == _donacije.GetElement1(i)) {
						kol = _donacije.GetElement2(i);
					}
				}
				stringstream obj;
				obj << " kolicina: ";
				obj << kol;
				zaThrow += obj.str();
				throw exception(zaThrow.c_str());
			}
		}
		else
		{
			_donacije.AddElement(osobaKojaDonira, kolicina);
			_listaDonatora.AddElement(osobaKojaDonira, kolicina);
			pokOsoba->setDatumPosljedenjeDonacije(datumDonacije);
		}

		bool postojiGrupa = false;
		int indeks = 0;
		for (int i = 0; i < _zalihe.GetTrenutno(); i++) {
			if (osobaKojaDonira->getKrvnaGrupa() == _zalihe.GetElement1(i)) {
				postojiGrupa = true;
				indeks = i;
			}
		}

		if (postojiGrupa) {
			double novaKolicina = _zalihe.GetElement2(indeks);
			novaKolicina += kolicina;
			_zalihe.SetElement2(indeks, novaKolicina);
		}
		else if (!postojiGrupa) {
			_zalihe.AddElement(osobaKojaDonira->getKrvnaGrupa(), kolicina);
		}
	}
	double ProvjeriStanje(const char* krvnaGrupa) {
		double kolicina = 0;
		string krvG = krvnaGrupa;
		for (int i = 0; i < _zalihe.GetTrenutno(); i++) {
			bool odgovaraKrvnaGrupa = false;
			for (int j = 0; j < _zalihe.GetElement1(i).getDonirati().size(); j++)
			{
				stringstream objTemp;
				int oznaka = _zalihe.GetElement1(i).getDonirati()[j]->getKrvnuGrupuOznaku();
				objTemp << OznakaKrvneGrupeString[oznaka];
				char faktor = _zalihe.GetElement1(i).getDonirati()[j]->getRhFaktor();
				objTemp << faktor;
				if (objTemp.str() == krvnaGrupa) {
					odgovaraKrvnaGrupa = true;
					break;
				}
			}
			if (odgovaraKrvnaGrupa) {
				kolicina += _zalihe.GetElement2(i);
			}
		}
		return kolicina;
	}

	void DodajZahtjev(Zahtjev nekiZahtjev) {
		/*
		tcMostar.DodajZahtjev(zahtjev_0_poz);

	// ukoliko transfuzijski centar posjeduje zahtijevane kolicine na stanju, odmah ce ih ustupiti prema zahtjevu,
	// a u slucaju da ne posjeduje onda ce (koristeci multithreading) kontaktirati (poslati email poruku sa
	// odgovarajucim sadrzajem) sve donore koji zadovoljavaju sljedece uslove:
	// - mogu donirati krv zahtijevanoj krvnoj grupi
	// - oznacili su da zele biti kontaktirani
	// - nisu davali krv u posljednja 3 mjeseca
		*/
		double kolicina = 0;
		for (int i = 0; i < nekiZahtjev.getKrvnaGrupa()->getPrimaoci().size(); i++) {
			KrvnaGrupa* pok1 = nekiZahtjev.getKrvnaGrupa()->getPrimaoci()[i];
			for (int j = 0; j < _zalihe.GetTrenutno(); j++) {
				KrvnaGrupa* pok2 = _zalihe.GetElement1(j);
				if (*pok1 == *pok2)
				{
					kolicina += _zalihe.GetElement2(j);
				}
			}
		}
		if (kolicina >= nekiZahtjev.getKolicina()) {
			cout << "Saljemo krv odmah." << endl;
		}
		else if (kolicina < nekiZahtjev.getKolicina()) {
			for (int i = 0; i < _listaDonatora.GetTrenutno(); i++) {
				for (int j = 0; j < _listaDonatora.GetElement1(i)->getKrvnaGrupa().getDonirati().size(); j++) {
					KrvnaGrupa* temp1 = nekiZahtjev.getKrvnaGrupa();
					KrvnaGrupa* temp2 = _listaDonatora.GetElement1(i)->getKrvnaGrupa().getDonirati()[j];
					if (*temp1 == *temp2) {
						Donator* temp = dynamic_cast<Donator*>(_listaDonatora.GetElement1(i));
						if (temp->kontaktiraj()) {
							//int danasnjiDan = nekiZahtjev.getDatum().getDani();
							//int zadnjaDoniranje = temp->getDatumPosljednjeDonacije().getDane();
							//int rez = danasnjiDan - zadnjaDoniranje;
							int rez = 91;
							if (rez > 90) {
								thread mojThred(
									&TransfuzijskiCentar::posaljiEmail, this, "Postivani " + _listaDonatora.GetElement1(i)->getImePrezime() + "\n" +
									"Trenutno smo u nedostatku vase krvne grupe. Molimo vas da se odazavote ovom pozivu i da donirate krv"
								);
								mojThred.join();
							}
						}
					}


				}
			}
		}
	}
	void IspisiMatricuKompatibilnosti(vector<KrvnaGrupa*> grupe) {
		for (int i = 0; i < grupe.size(); i++) {
			int oznaka = grupe[i]->getKrvnuGrupuOznaku();
			cout << OznakaKrvneGrupeString[oznaka];
			cout << grupe[i]->getRhFaktor();
			cout << "\t\t";
		}
	}

};

void main()
{
	/**************************************************************************
	// 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	// 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME
	// ERROR CE BITI OZNACENO KAO "RE"
	// 3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	// 4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU code.TXT
	// 5. NAZIVI FUNKCIJA MORAJU BITI IDENTIENI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U.OSTALE, POMOANE FUNKCIJE
	// MOŽETE IMENOVATI PO ŽELJI. TAKO?ER, MOŽETE DODAVATI KOLIKO ŽELITE TESTNIH PODATAKA
	****************************************************************************/
	Datum dat_12_03_2018(12, 3, 2018), dat_01_02_2018(1, 2, 2018);
	Datum danas(dat_12_03_2018);
	cout << danas << endl;

	Kolekcija<int, int, 10> kolekcija1;
	for (size_t i = 0; i < kolekcija1.GetMax() - 2; i++)
		if (!kolekcija1.AddElement(10 + i, 170000 + i))
			cout << "Elementi " << 10 + i << " i " << 170000 + i << " nisu dodati u kolekciju" << endl;

	cout << kolekcija1 << endl;
	kolekcija1.SetSortiranje(DESC);
	kolekcija1.AddElement(16, 170016);
	cout << kolekcija1 << endl;

	Kolekcija<int, int, 10> kolekcija2(kolekcija1);
	kolekcija2.SetSortiranje(ASC);
	kolekcija2.AddElement(13, 22222);
	cout << kolekcija2 << endl;


	vector<KrvnaGrupa*> krvneGrupe;
	krvneGrupe.push_back(new KrvnaGrupa(O, '+')); //0 - O_poz
	krvneGrupe.push_back(new KrvnaGrupa(O, '-')); //1 - O_neg
	krvneGrupe.push_back(new KrvnaGrupa(A, '+')); //2 - A_poz
	krvneGrupe.push_back(new KrvnaGrupa(A, '-')); //3 - A_neg
	krvneGrupe.push_back(new KrvnaGrupa(B, '+')); //4 - B_poz
	krvneGrupe.push_back(new KrvnaGrupa(B, '-')); //5 - B_neg
	krvneGrupe.push_back(new KrvnaGrupa(AB, '+'));//6 - AB_poz
	krvneGrupe.push_back(new KrvnaGrupa(AB, '-'));//7 - AB_neg

	vector<KrvnaGrupa*> donori_O_poz{ krvneGrupe[0], krvneGrupe[2], krvneGrupe[4], krvneGrupe[6] };
	krvneGrupe[0]->SetDonatori(donori_O_poz);
	//ili krace napisano 0+
	krvneGrupe[0]->SetPrimaoci(vector<KrvnaGrupa*>{krvneGrupe[0], krvneGrupe[1]});
	//O-
	krvneGrupe[1]->SetDonatori(vector<KrvnaGrupa*>{krvneGrupe[0], krvneGrupe[1], krvneGrupe[2], krvneGrupe[3], krvneGrupe[4], krvneGrupe[5], krvneGrupe[6], krvneGrupe[7]});
	krvneGrupe[1]->SetPrimaoci(vector<KrvnaGrupa*>{krvneGrupe[1]});
	//A+
	krvneGrupe[2]->SetDonatori(vector<KrvnaGrupa*>{krvneGrupe[2], krvneGrupe[6]});
	krvneGrupe[2]->SetPrimaoci(vector<KrvnaGrupa*>{krvneGrupe[0], krvneGrupe[2], krvneGrupe[1], krvneGrupe[3]});
	//A-
	krvneGrupe[3]->SetDonatori(vector<KrvnaGrupa*>{krvneGrupe[2], krvneGrupe[3], krvneGrupe[6], krvneGrupe[7]});
	krvneGrupe[3]->SetPrimaoci(vector<KrvnaGrupa*>{krvneGrupe[3], krvneGrupe[1]});
	//B+
	krvneGrupe[4]->SetDonatori(vector<KrvnaGrupa*>{krvneGrupe[4], krvneGrupe[6]});
	krvneGrupe[4]->SetPrimaoci(vector<KrvnaGrupa*>{krvneGrupe[0], krvneGrupe[4], krvneGrupe[1], krvneGrupe[5]});
	//B-
	krvneGrupe[5]->SetDonatori(vector<KrvnaGrupa*>{krvneGrupe[4], krvneGrupe[5], krvneGrupe[6], krvneGrupe[7]});
	krvneGrupe[5]->SetPrimaoci(vector<KrvnaGrupa*>{krvneGrupe[5], krvneGrupe[1]});
	//AB+
	krvneGrupe[6]->SetDonatori(vector<KrvnaGrupa*>{krvneGrupe[6]});
	krvneGrupe[6]->SetPrimaoci(vector<KrvnaGrupa*>{krvneGrupe[1], krvneGrupe[2], krvneGrupe[3], krvneGrupe[4], krvneGrupe[0], krvneGrupe[5], krvneGrupe[6], krvneGrupe[7]});
	//AB-
	krvneGrupe[7]->SetDonatori(vector<KrvnaGrupa*>{krvneGrupe[6], krvneGrupe[7]});
	krvneGrupe[7]->SetPrimaoci(vector<KrvnaGrupa*>{krvneGrupe[7], krvneGrupe[3], krvneGrupe[5], krvneGrupe[1]});
	for (int i = 0; i < krvneGrupe.size(); i++) {
		cout << *krvneGrupe[i];
	}
	Osoba* jasmin = new Donator("Jasmin Azemovic", krvneGrupe[4], "jasmin@fit.ba", Datum(12, 2, 2017), true); // B+
	Osoba* adel = new Donator("Adel Handzic", krvneGrupe[2], "adelfit.ba", Datum(9, 1, 2017), true);		//A+
	Osoba* goran = new Donator("Goran Skondric", krvneGrupe[0], "goran@fit.net", Datum(9, 3, 2017), true); //O+
																											//1. A+ Adel Goran
																											//2. A- nitko
	jasmin->Info();
	adel->Info();
	goran->Info();

	TransfuzijskiCentar tcMostar;
	// prilikom svake donacije je potrebno povecati zalihe za tu krvnu grupu
	// ukoliko krvna grupa vec postoji na zalihi onda se uvecava samo njena kolicina
	// takodjer, donatoru se postavlja nova vrijednost datuma posljednje donacije.
	// ukoliko je neki od dodantora darivao krv prije isteka 3 mjeseca od posljednjeg
	// darivanja, funkcija baca izuzetak
	//datum donacije, donator, kolicina
	try
	{
		tcMostar.AddDonaciju(Datum(20, 5, 2017), jasmin, 2.5);
		tcMostar.AddDonaciju(Datum(20, 5, 2017), adel, 3);
		tcMostar.AddDonaciju(Datum(6, 5, 2017), goran, 1.2);
	}
	catch (exception& err)
	{
		cout << "Greska-> " << err.what() << endl;
	}
	try
	{
		tcMostar.AddDonaciju(Datum(10, 9, 2017), jasmin, 2);
		tcMostar.AddDonaciju(Datum(18, 10, 2017), adel, 1.8);
		tcMostar.AddDonaciju(Datum(15, 9, 2017), goran, 3.8);
	}
	catch (exception& err)
	{
		cout << "Greska-> " << err.what() << endl;
	}

	Zahtjev zahtjev_A_poz("Tranfuziologija KCUS", krvneGrupe[2], 15),
		zahtjev_A_neg("Tranfuziologija Bakir Nakas", krvneGrupe[1], 8),
		zahtjev_B_poz("Bolnica", krvneGrupe[4], 10);
	//ukoliko na stanju postoji dovoljno iste ili zamjense krvne grupe
	if (tcMostar.ProvjeriStanje("A+") >= 15) {
		cout << "Zahtjev uspjesno zaprimljen kolicina. Trenutno imamo dostupnu kolicinu krvi" << endl;

	}
	else
		tcMostar.DodajZahtjev(zahtjev_A_poz);

	cout << crt;
	if (tcMostar.ProvjeriStanje("B+") >= 15) {
		cout << "Zahtjev uspjesno zaprimljen kolicina. Trenutno imamo dostupnu kolicinu krvi" << endl;

	}
	else
		tcMostar.DodajZahtjev(zahtjev_B_poz);

	// ukoliko transfuzijski centar posjeduje zahtijevane kolicine na stanju, odmah ce ih ustupiti prema zahtjevu,
	// a u slucaju da ne posjeduje onda ce (koristeci multithreading) kontaktirati (poslati email poruku sa
	// odgovarajucim sadrzajem) sve donore koji zadovoljavaju sljedece uslove:
	// - mogu donirati krv zahtijevanoj krvnoj grupi
	// - oznacili su da zele biti kontaktirani
	// - nisu davali krv u posljednja 3 mjeseca
	if (tcMostar.ProvjeriStanje("A-") >= 8) {
		cout << "Zahtjev uspjesno zaprimljen kolicina. Trenutno imamo dostupnu kolicinu krvi" << endl;
	}
	else
		tcMostar.DodajZahtjev(zahtjev_A_neg);


	// ispisuje matricu ili tablicu iz koje je vidljiva kompatibilnost izme?u pojedinih krvnih grupa. 
	// pored naziva i rh faktora krvne grupe, u zagradi ispisati i trenutnu kolieinu na stanju. matrica 
	// prikazana na kraju zadatka
	tcMostar.IspisiMatricuKompatibilnosti(krvneGrupe);

	delete adel;
	delete jasmin;
	delete goran;

	for (size_t i = 0; i < krvneGrupe.size(); i++)
		delete krvneGrupe[i];
}