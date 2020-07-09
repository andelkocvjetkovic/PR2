#include <iostream>
#include <vector>
#include<sstream>
#include<string>
#include<stdlib.h>
#include<time.h>
#include <iterator>
#include<thread>
#include<mutex>
using namespace std;

const char* crt = "\n-------------------------------------------\n";
enum OznakaKrvneGrupe { O, A, B, AB };
const char* OznakaKrvneGrupeString[] = { "O", "A", "B", "AB" };

class Datum {
	int _dan, _mjesec, _godina;
public:
	Datum(int dan, int mjesec, int godina) :
		_dan(dan), _mjesec(mjesec), _godina(godina) {}
	int getDan()const { return _dan; }
	int getMjesec()const { return _mjesec; }
	int getGodina()const { return _godina; }
	string ToString(const char* znak = ".") {
		string znakS = znak;
		// podrazumijevani separator je tacka ".", pa ispisuje 1.2.2018
		stringstream objTemp;
		objTemp << _dan << znakS << _mjesec << znakS << _godina;
		return objTemp.str();
	}
};

template <class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int _trenutnoElemenata;
	int _keyElement; // kljucni element, a podrazumijevano T1 predstavlja kljucni element i tada je vrijednost 1, a u slucaju da je T2 onda ce vrijednost biti 2
public:
	Kolekcija(int keyElement = 1) {
		_trenutnoElemenata = 0;
		_keyElement = keyElement;
	}
	Kolekcija(const Kolekcija<T1, T2, max>& org) :
		_trenutnoElemenata(org._trenutnoElemenata),
		_keyElement(org._keyElement) {
		for (int i = 0; i < _trenutnoElemenata; i++) {
			_elementi1[i] = (*org._elementi1[i]);
			_elementi2[i] = (*org._elementi2[i]);
		}
	}
	~Kolekcija() {
		
	}
	bool AddElement( T1 ele1, const T2& ele2) {
		if (_trenutnoElemenata == max)
			return false;
		_elementi1[_trenutnoElemenata] = ele1;
		_elementi2[_trenutnoElemenata] = ele2;
		_trenutnoElemenata++;
		return true;
	}

	void SetElement2(int lokacija, T2 ele2) {
		_elementi2[lokacija] = ele2;
	}
	void SetKeyElement(int keyElement) {
		if (keyElement < 1 || keyElement>2)
			throw exception("Nedozvoljena vrijednost!");
		_keyElement = keyElement;
	}
	int GetTrenutno() const { return _trenutnoElemenata; }
	int GetMax() const { return max; }

	T1 GetElement1(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception("Nepostojeca lokacija!");
		return _elementi1[lokacija];
	}
	T2 GetElement2(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception("Nepostojeca lokacija!");
		return _elementi2[lokacija];
	}
	void Sort(string nacinSortiranja) {
		//kljucne rijeci za sortiranje su ASC i DESC, a sortiranje se vrsi prema kljucnom elementu
		if (nacinSortiranja == "ASC") {
			int ok;
			for (int i = _trenutnoElemenata - 1; i > 0; i--) {
				ok = 1;
				for (int j = 0; j < i; j++) {
					if (_keyElement == 1) {
						if (_elementi1[j + 1] < _elementi1[j]) {
							T1 temp1 = _elementi1[j];
							T2 temp2 = _elementi2[j];

							_elementi1[j] = _elementi1[j + 1];
							_elementi2[j] = _elementi2[j + 1];

							_elementi1[j + 1] = temp1;
							_elementi2[j + 1] = temp2;
							ok = 0;
						}
					}
					else if (_keyElement == 2) {
						if (_elementi2[j + 1] < _elementi2[j]) {
							T1 temp1 = _elementi1[j];
							T2 temp2 = _elementi2[j];

							_elementi1[j] = _elementi1[j + 1];
							_elementi2[j] = _elementi2[j + 1];

							_elementi1[j + 1] = temp1;
							_elementi2[j + 1] = temp2;
							ok = 0;
						}
					}

				}
				if (ok)
					break;
			}
		}
		else if (nacinSortiranja == "DESC") {
			for (int i = 0; i < (_trenutnoElemenata - 1); i++) {
				for (int j = 0; j < _trenutnoElemenata - i - 1; j++) {
					if (_keyElement == 1) {
						if (_elementi1[j] < _elementi1[j + 1]) {
							T1 temp1 = _elementi1[j];
							T2 temp2 = _elementi2[j];

							_elementi1[j] = _elementi1[j + 1];
							_elementi2[j] = _elementi2[j + 1];

							_elementi1[j + 1] = temp1;
							_elementi2[j + 1] = temp2;
						}

					}
					else if (_keyElement == 2) {
						if (_elementi2[j] < _elementi2[j + 1]) {
							T1 temp1 = _elementi1[j];
							T2 temp2 = _elementi2[j];

							_elementi1[j] = _elementi1[j + 1];
							_elementi2[j] = _elementi2[j + 1];

							_elementi1[j + 1] = temp1;
							_elementi2[j + 1] = temp2;
						}
					}
				}
			}
		}
		else
			cout << "Pogresan unos, nacina sortiranja. Koristitie ASC ili DESC kao kljucne rijeci";
	}
	friend ostream& operator<<(ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}
};

class KrvnaGrupa {
	/*postoje 4 krvne grupe koje su navedene u enumeracije OznakaKrvneGrupe, pri cemu svaka od navedenih moze imati pozitivan (+) i negativan (-) Rh faktor*/
	OznakaKrvneGrupe _oznaka;//npr: AB
	char _rhFaktor; // + ili -
					/*prethodno pomenuto je izuzetno bitno iz razloga sto postoje jasna pravila vezana za darivanje krvi tj. koji primalac moze primiti krv od kojeg donatora sto je prikazano u tabeli, a naredna dva vector-a su zaduzena da cuvaju informacije o tome, npr. za krvnu grupu A+ vector donatori ce cuvati vrijednosti: A+ i AB+ */
	vector<KrvnaGrupa> _donatori; //krvne grupe kojima odredjena krvna grupa moze donirati krv.
	vector<KrvnaGrupa> _primaoci; //krvne grupe od kojih odredjena krvna grupa moze primiti krv.
public:
	KrvnaGrupa(OznakaKrvneGrupe oznaka, char rhFaktor) :
		_oznaka(oznaka), _rhFaktor(rhFaktor) {}
	void SetDonatori(vector<KrvnaGrupa> donatori) {
		_donatori = donatori;
	}
	void SetPrimaoci(vector<KrvnaGrupa> primaoci) {
		_primaoci = primaoci;
	}
	vector<KrvnaGrupa> getDonatori()const {
		return _donatori;
	}
	vector<KrvnaGrupa> getPrimaoci() const {
		return _primaoci;
	}
	string ToString() {
		/*
		Ispis podataka o krvnoj grupi treba biti u formatu:
		-------------------------------------------
		Krvna grupa -> 0+
		-------------------------------------------
		Donori  ->  0+, A+, B+, AB+
		Primaoci ->	0+, 0-
		-------------------------------------------
		*/
		stringstream obj;
		obj << crt;
		obj << "Krvna grupap -> ";
		obj << OznakaKrvneGrupeString[this->_oznaka];
		obj << _rhFaktor;
		obj << crt;
		obj << "Donori -> ";
		vector<KrvnaGrupa>::iterator itr = _donatori.begin();
		for (itr; itr != _donatori.end(); itr++) {
			obj << OznakaKrvneGrupeString[itr->_oznaka];
			obj << itr->_rhFaktor;
			if(itr + 1 != _donatori.end())
			obj << ", ";
		}
		obj << "\n";
		itr = _primaoci.begin();
		obj << "Primaoci -> ";
		for (itr; itr != _primaoci.end(); itr++) {
			obj << OznakaKrvneGrupeString[itr->_oznaka];
			obj << itr->_rhFaktor;
			if(itr + 1 != _primaoci.end())
				obj << ", ";

		}
		obj << crt;
		return obj.str();
	}
	OznakaKrvneGrupe getKrvnaGrupaOznaka()const {
		return this->_oznaka;
	}
	string getKrvnaGrupaString()const {
		stringstream obj;
		obj << OznakaKrvneGrupeString[this->_oznaka];
		return obj.str();
	}
	char getRhFaktor()const {
		return this->_rhFaktor;
	} 
};

//Klasu proglasiti apstraktnom
class Osoba abstract {
protected:
	char* _imePrezime;
	KrvnaGrupa _krvnaGrupa;
public:
	Osoba(const char* imePrezime, KrvnaGrupa krvnaGrupa) : _krvnaGrupa(krvnaGrupa) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
	}
	Osoba(const Osoba& org) :
		_imePrezime(new char[strlen(org._imePrezime) + 1]),
		_krvnaGrupa(org._krvnaGrupa) {
		strcpy_s(_imePrezime, strlen(org._imePrezime) + 1, org._imePrezime);
	}
	~Osoba() {
		delete[] _imePrezime;
		_imePrezime = nullptr;
	}
	Osoba& operator=(const Osoba& obj) {
		if (this != &obj) {
			delete[] _imePrezime;
			_imePrezime = nullptr;
			_imePrezime = new char[strlen(obj._imePrezime) + 1];
			strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
			_krvnaGrupa = obj._krvnaGrupa;
		}
		return *this;
	}
	string getIme()const {
		string imePre = _imePrezime;
		return imePre;
	}
	string getKrvnaGrupaString()const {
		stringstream obj;
		obj << this->_krvnaGrupa.getKrvnaGrupaString();
		obj << this->_krvnaGrupa.getRhFaktor();
		return obj.str();
	}
	KrvnaGrupa getKrvnaGrupa() {
		return _krvnaGrupa;
	}
	virtual void info() = 0;
};
bool ValidanFormat(string telefon) {
	return true;
}
class Donator : public Osoba {
	//u slucaju da broj telefona nije u validnom formatu potrebno ga je postaviti na podrazumijevanu vrijednost: 000-000-000
	string _telefon; //regex: 000/000-000 ili 000-000-000
	Datum _datumPosljednjegDoniranja;
	bool _podsjetiMe;
	bool _kontaktirajMe;

public:
	Donator(const char* imePrezime, KrvnaGrupa krvnaGrupa, string telefon, Datum dpd, bool remind = true, bool contact = true)
		: Osoba(imePrezime, krvnaGrupa), _datumPosljednjegDoniranja(dpd) {
		if (ValidanFormat(telefon))
			_telefon = telefon;
		else
			_telefon = "000-000-000";
	}
	Donator(const Donator& org) :
		Osoba(org),
		_telefon(org._telefon),
		_datumPosljednjegDoniranja(org._datumPosljednjegDoniranja),
		_podsjetiMe(org._podsjetiMe),
		_kontaktirajMe(org._kontaktirajMe){}
	Donator& operator=(const Donator& obj) {
		if (this != &obj) {
			Osoba::operator=(obj);
			_telefon=(obj._telefon);
			_datumPosljednjegDoniranja = obj._datumPosljednjegDoniranja;
			_podsjetiMe = obj._podsjetiMe;
			_kontaktirajMe = obj._kontaktirajMe;
		}
		return *this;
	}
	bool getKontaktirajMe()const {
		return _kontaktirajMe;
	}
	virtual void info() {

	}
	void setNoviDatum(Datum noviDatum) {
		_datumPosljednjegDoniranja = noviDatum;
	}
};

class Zahtjev {
	string _ustanova;
	Datum _datumZahtjeva;
	KrvnaGrupa _krvnaGrupa;
	double _kolicina;
public:
	Zahtjev(string ustanova, Datum datum, KrvnaGrupa krvnaGrupa, double kolicina) :
		_ustanova(ustanova), _datumZahtjeva(datum), _krvnaGrupa(krvnaGrupa), _kolicina(kolicina) { }

	friend ostream& operator<<(ostream& COUT, const Zahtjev& obj) {
		COUT << "Ustanova: " << obj._ustanova << endl;
		//COUT << "Datum: " << obj._datumZahtjeva.ToString() << endl;
		//COUT << "Krvna grupa: " << obj._krvnaGrupa.ToString() << endl;
		COUT << "Kolicina: " << obj._kolicina << endl;
		return COUT;
	}
	string getKrvnaGrupaString()const {
		string krvnaGrupa = _krvnaGrupa.getKrvnaGrupaString();
		krvnaGrupa += _krvnaGrupa.getRhFaktor();
		return krvnaGrupa;
	}
	double getKolicinu()const {
		return _kolicina;
	}
	KrvnaGrupa getKrvnaGrupa()const {
		return _krvnaGrupa;
	}

};
mutex mutx;
class TransfuzijskiCentar {
private:
	//stanje zaliha za svaku pojedinu krvnu grupu
	Kolekcija<KrvnaGrupa*, double, 8> _zalihe;
	//evidentira svaku donaciju krvi
	Kolekcija<Osoba*, double, 100> _donacije;
	vector<Zahtjev> _zahtjevi;
	void posaljiPoruku(string poruka) {
		mutx.lock();
		cout << poruka << endl;
		mutx.unlock();
	}
public:
	
	void AddDonaciju(Datum datumDocnacije,Osoba* donatorKrvi,double kolicina) {
		/*
		prilikom svake donacije je potrebno povecati zalihe za tu krvnu grupu
		ukoliko krvna grupa vec postoji na zalihi onda se uvecava samo njena kolicina
		takodjer, donatoru se postavlja nova vrijednost datuma posljednje donacije
		*/
		//datum donacije, donator, kolicina
		bool donirao = false;
		int indeks = -1;
		for (int i = 0; i < _donacije.GetTrenutno(); i++) {
			if (donatorKrvi->getKrvnaGrupaString() == _donacije.GetElement1(i)->getKrvnaGrupaString() && 
				donatorKrvi->getIme() == _donacije.GetElement1(i)->getIme())
			{
				donirao = true;
				indeks = i;
				break;
			}
		}
		if (donirao) {
			Donator* pokD = dynamic_cast<Donator*>(_donacije.GetElement1(indeks));
			if (pokD != NULL) {
				pokD->setNoviDatum(datumDocnacije);
				_donacije.SetElement2(indeks, kolicina);
			}
		}
		else if (!donirao) {
			Donator* pokD = dynamic_cast<Donator*>(donatorKrvi);
			_donacije.AddElement(new Donator(*pokD), kolicina);
			pokD->setNoviDatum(datumDocnacije);
		}

		//provjera zaliha krvi
		bool imaNaZalihama = false;
		indeks = -1;
		for (int i = 0; i < _zalihe.GetTrenutno(); i++) {
		
			string drugi = _zalihe.GetElement1(i)->getKrvnaGrupaString() + _zalihe.GetElement1(i)->getRhFaktor();
			if (donatorKrvi->getKrvnaGrupaString() == (_zalihe.GetElement1(i)->getKrvnaGrupaString() + _zalihe.GetElement1(i)->getRhFaktor())) {
				imaNaZalihama = true;
				indeks = i;
				break;
			}
		}
		if (imaNaZalihama) {
			double novaKolicina = _zalihe.GetElement2(indeks);
			novaKolicina += kolicina;
			_zalihe.SetElement2(indeks, novaKolicina);
			return;
		}
		else if (!imaNaZalihama) {
			//_zalihe.AddElement(new KrvnaGrupa(donatorKrvi->getKrvnaGrupa()), kolicina)
			_zalihe.AddElement(new KrvnaGrupa(donatorKrvi->getKrvnaGrupa()), kolicina);
			return;
		}
	}
	void DodajZahtjev(Zahtjev zahtjevKrvi) {
	/*ukoliko transfuzijski centar posjeduje zahtijevane kolicine na stanju, 
	odmah ce ih ustupiti prema zahtjevu,
	a u slucaju da ne posjeduje onda ce 
	(koristeci multithreading) kontaktirati (poslati SMS poruku sa odgovarajucim sadrzajem)
	sve donore koji zadovoljavaju sljedece uslove:
	- mogu donirati krv zahtijevanoj krvnoj grupi
	- oznacili su da zele biti kontaktirani
	- nisu davali krv u posljednja 3 mjeseca
	*/
		double kol = 0;
		for (int i = 0; i < zahtjevKrvi.getKrvnaGrupa().getPrimaoci().size(); i++) {
			string zahtjev = zahtjevKrvi.getKrvnaGrupa().getPrimaoci()[i].getKrvnaGrupaString() + 
				zahtjevKrvi.getKrvnaGrupa().getPrimaoci()[i].getRhFaktor();
			for (int j = 0; j < _zalihe.GetTrenutno(); j++) {
				string stanjeKrvi = _zalihe.GetElement1(j)->getKrvnaGrupaString() + _zalihe.GetElement1(j)->getRhFaktor();
				if (zahtjev == stanjeKrvi) {
					kol += _zalihe.GetElement2(j);
				}
			}
		}

		if (kol >= zahtjevKrvi.getKolicinu()) {
			cout << "Uspjeh uspjesno zaprimljen. Krv saljemo odmah";
		}
		else if (kol < zahtjevKrvi.getKolicinu()) {
			for (int i = 0; i < _donacije.GetTrenutno(); i++) {
				for (int j = 0; j < _donacije.GetElement1(i)->getKrvnaGrupa().getDonatori().size(); j++) {
					string zahtjev = zahtjevKrvi.getKrvnaGrupa().getKrvnaGrupaString() + zahtjevKrvi.getKrvnaGrupa().getRhFaktor();
					string krvDonaatora = _donacije.GetElement1(i)->getKrvnaGrupa().getDonatori()[j].getKrvnaGrupaString() +
						_donacije.GetElement1(i)->getKrvnaGrupa().getDonatori()[j].getRhFaktor();
					if (zahtjev == krvDonaatora) {
						Donator* temp = dynamic_cast<Donator*>(_donacije.GetElement1(i));
						if (temp->getKontaktirajMe()) {
							int danZahtjeva;
							int danZadnjeDonacijeDonatora;
							int rez;// danZahtjeva - danZadnjeDonacijeDonatora
							rez = 91;
							if (rez > 90) {
								thread tr1(&TransfuzijskiCentar::posaljiPoruku, this, "Postovani " + temp->getIme() +
									"Trenutno smo u nedostatku vase krvne grupe. Molimo vas da se odazavote ovom pozivu i da donirate krv"
								);
								tr1.join();
							}
						}

					}
				}
			}
		}
	}
	void GetZahvalniceZahvalnice(int broj) {

	}
};

void main() {

	/**************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
	3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU code.TXT
	5. NAZIVI FUNKCIJA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U.OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI PO ŽELJI. TAKOÐER, MOŽETE DODAVATI KOLIKO ŽELITE TESTNIH PODATAKA
	****************************************************************************/
	Datum dat_12_01_2018(12, 1, 2018), dat_01_02_2018(1, 2, 2018);
	cout << dat_01_02_2018.ToString() << endl; //podrazumijevani separator je tacka ".", pa ispisuje 1.2.2018
	cout << dat_12_01_2018.ToString("/") << endl; //separator je proslijedjen, pa ispisuje 12/1/2018
	srand(time(NULL));
	Kolekcija<int, int, 10> kolekcija1;
	for (size_t i = 0; i < kolekcija1.GetMax(); i++)
		if (!kolekcija1.AddElement(rand() % 10, rand() % 100 + 1700))
			cout << "Elementi " << rand() % 10 << " i " << rand() % 100 + 1700 << " nisu dodati u kolekciju" << endl;

	cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl << endl;

	kolekcija1.Sort("ASC"); //kljucne rijeci za sortiranje su ASC i DESC, a sortiranje se vrsi prema kljucnom elementu
	cout << kolekcija1 << endl << endl << endl;
	kolekcija1.Sort("DESC");
	cout << kolekcija1 << endl;

	kolekcija1.SetKeyElement(2);//postavlja kljucni element na T2

	kolekcija1.Sort("DESC"); //sada se sortiranje vrsi po elementu T2
	cout << kolekcija1 << endl;

	KrvnaGrupa
		O_poz(O, '+'), O_neg(O, '-'),
		A_poz(A, '+'), A_neg(A, '-'),
		B_poz(B, '+'), B_neg(B, '-'),
		AB_poz(AB, '+'), AB_neg(AB, '-');

	vector<KrvnaGrupa> donori_O_poz{ O_poz, A_poz, B_poz, AB_poz };
	O_poz.SetDonatori(donori_O_poz);
	//ili krace napisano
	O_poz.SetPrimaoci(vector<KrvnaGrupa>{O_poz, O_neg});
	A_poz.SetDonatori(vector<KrvnaGrupa>{A_poz, AB_poz});
	A_poz.SetPrimaoci(vector<KrvnaGrupa>{O_poz, A_poz, O_neg, A_neg});

	B_poz.SetDonatori(vector<KrvnaGrupa>{B_poz, AB_poz});
	B_poz.SetPrimaoci(vector<KrvnaGrupa>{O_poz, B_poz, O_neg, B_neg});
	/*
	Ispis podataka o krvnoj grupi treba biti u formatu:
	-------------------------------------------
	Krvna grupa -> 0+
	-------------------------------------------
	Donori  ->  0+, A+, B+, AB+
	Primaoci ->	0+, 0-
	-------------------------------------------
	*/
	cout << O_poz.ToString() << endl;

	Osoba* jasmin = new Donator("Jasmin Azemovic", B_poz, "061-111-222", Datum(12, 2, 2017), true, true);
	Osoba* adel = new Donator("Adel Handzic", A_neg, "061-222-333", Datum(9, 1, 2017), true, true);
	Osoba* goran = new Donator("Goran Skondric", B_neg, "061-333-444", Datum(9, 3, 2017), true, true);

	TransfuzijskiCentar tcMostar;
	/*
	prilikom svake donacije je potrebno povecati zalihe za tu krvnu grupu
	ukoliko krvna grupa vec postoji na zalihi onda se uvecava samo njena kolicina
	takodjer, donatoru se postavlja nova vrijednost datuma posljednje donacije
	*/
	//datum donacije, donator, kolicina
	tcMostar.AddDonaciju(Datum(20, 5, 2017), jasmin, 2.5);
	tcMostar.AddDonaciju(Datum(20, 5, 2017), adel, 3);
	tcMostar.AddDonaciju(Datum(6, 5, 2017), goran, 1.2);


	tcMostar.AddDonaciju(Datum(10, 9, 2017), jasmin, 2);
	tcMostar.AddDonaciju(Datum(18, 10, 2017), adel, 1.8);
	tcMostar.AddDonaciju(Datum(15, 9, 2017), goran, 3.8);

	Zahtjev zahtjev_B_poz("Tranfuziologija KCUS", Datum(18, 2, 2018), B_poz, 15),
		zahtjev_0_neg("Tranfuziologija Bakir Nakas", Datum(20, 2, 2018), O_neg, 8);

	/*ukoliko transfuzijski centar posjeduje zahtijevane kolicine na stanju, odmah ce ih ustupiti prema zahtjevu,
	a u slucaju da ne posjeduje onda ce (koristeci multithreading) kontaktirati (poslati SMS poruku sa odgovarajucim sadrzajem)
	sve donore koji zadovoljavaju sljedece uslove:
	- mogu donirati krv zahtijevanoj krvnoj grupi
	- oznacili su da zele biti kontaktirani
	- nisu davali krv u posljednja 3 mjeseca
	*/
	tcMostar.DodajZahtjev(zahtjev_B_poz);
	tcMostar.DodajZahtjev(zahtjev_0_neg);

	//printa zahvalnice (zahvaljujemo se ime i prezime donatoru na ukupno doniranih X doza krvi) za TOP 2 donatora krvi
	//tcMostar.GetZahvalniceZahvalnice(2);

	delete adel;
	delete jasmin;
	delete goran;
}