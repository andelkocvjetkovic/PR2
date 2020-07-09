#include <iostream>
#include <iomanip>

using namespace std;

const char* crt = "\n--------------------------------\n";


class KupljeniArtikal {
	char* _naziv;
	float _cijena;
	int _kolicina;
	//konstruktor, destruktor i potrebne funkcije
public:
	KupljeniArtikal(const char* naziv = "N/A", float cijena = 0.f) {
		int len = strlen(naziv) + 1;
		_naziv = new char[len];
		strcpy_s(_naziv, len, naziv);
		_cijena = cijena;
		_kolicina = 1;
	}
	KupljeniArtikal(KupljeniArtikal& drugi) {
		//cout << "konstruktor kopije za " << drugi._naziv << endl;
		int len = strlen(drugi._naziv) + 1;
		_naziv = new char[len];
		strcpy_s(_naziv, len, drugi._naziv);
		_cijena = drugi._cijena;
		_kolicina = drugi._kolicina;
	}
	~KupljeniArtikal() {
		//cout << "Destruktor artikal " << _naziv << endl;
		delete[] _naziv; _naziv = nullptr;
	}
	KupljeniArtikal& operator=(const KupljeniArtikal& obj)
	{
		if (this != &obj)
		{
			delete[]_naziv;
			_naziv = NULL;
			_naziv = new char[strlen(obj._naziv) + 1];
			strcpy_s(_naziv, strlen(obj._naziv) + 1, obj._naziv);
			_cijena = obj._cijena;
			_kolicina = obj._kolicina;
		}
		return *this;
	}
	void operator+=(const float dodaj) {
		_cijena += dodaj;
	}
	void operator++() {
		++_kolicina;
	}
	
	int getKolicina() {
		return _kolicina;
	}
	float getCijena() {
		return _cijena;
	}
	bool operator==(KupljeniArtikal& drugi) {
		return strcmp(drugi._naziv, _naziv) == 0;
	}
	friend ostream& operator<<(ostream& COUT, KupljeniArtikal& a) {

		COUT << setw(10) << a._naziv << " " << setw(4) << a._kolicina << " " << setw(5) << a._cijena << " KM " << setw(5) << a._cijena * a._kolicina << "KM";
		return COUT;
	}
};

template<class Tip1>
class Kolekcija
{
	friend ostream& operator<< <>(ostream& COUT, const Kolekcija<Tip1>& obj);
private:
	Tip1* _elementi1;
	int _trenutno;
public:
	Kolekcija(int trenutno = 0) :_trenutno(trenutno), _elementi1(NULL)
	{   }
		Kolekcija(Kolekcija<Tip1>& org) :
		_trenutno(org._trenutno), _elementi1(new Tip1[org._trenutno])
	{
		for (int i = 0; i < org._trenutno; i++)
		{
			_elementi1[i] = org._elementi1[i];
			
		}
	}
	~Kolekcija()
	{
		delete[] _elementi1; _elementi1 = NULL;
	}
	Kolekcija& operator= (const Kolekcija<Tip1>& obj)
	{
		if (this != &obj)
		{
			delete[]_elementi1;
			_elementi1 = new Tip1[obj._trenutno];

			_trenutno = obj._trenutno;
		}
		return *this;
	}
	int getTrenutno()const { return _trenutno; }
	void setTrenutno()
	{
		_trenutno++;
	}
	bool addElement(Tip1 elem1)
	{
		Tip1* temp1 = new Tip1[getTrenutno() + 1];
		for (int i = 0; i < getTrenutno(); i++)
		{
			temp1[i] = _elementi1[i];
		}
		temp1[getTrenutno()] = elem1;
		setTrenutno();
		delete[]_elementi1;
		_elementi1 = temp1;
		temp1 = NULL;
		return true;
	}
	Tip1& operator[](int index)
	{
		return _elementi1[index];
	}
};
template<class Tip1>
ostream& operator<< <>(ostream& COUT, const Kolekcija<Tip1>& obj)
{
	float ukupno = 0;
	for (int i = 0; i < obj.getTrenutno(); i++) {
		COUT << *(obj._elementi1[i]) << endl;
	}
	COUT << crt;
	return COUT;
}
class Racun {
	static int _brojacRacuna;
	const int _brojRacuna; //brojevi racuna krecu od 1
   /* KupljeniArtikal* _artikli;*/ //niz povecavamo prilikom svakog dodavanja artikla
	Kolekcija<KupljeniArtikal*>_artikli;
public:
	Racun() : _brojRacuna(_brojacRacuna++) {	}
	~Racun() {
		//cout << "Destruktor racun " << _brojRacuna << endl;
		for (int i = 0; i < _artikli.getTrenutno(); i++)
		{
			delete _artikli[i];
			_artikli[i] = NULL;
		}

		/*delete[] _artikli; _artikli = nullptr;*/
	}
	bool operator+=(KupljeniArtikal& a) {
		for (int i = 0; i < _artikli.getTrenutno(); i++) {
			if (a == *(_artikli[i])) {
				for (int j = 0; j < a.getKolicina(); j++)
					_artikli[i]->operator++();
				return true;
			}
		}
		if (_artikli.addElement(new KupljeniArtikal(a)))
		{
			return true;
		}
		return false;
	}
	KupljeniArtikal* operator[](int i) {
		if (i >= 0 && i < _artikli.getTrenutno())
			return _artikli[i];
			return nullptr;
	}

	friend ostream& operator<<(ostream& COUT, Racun& racun) {
		COUT << "Broj racuna: " << racun._brojRacuna << endl;
		COUT << "Ukupno artikala: " << racun._artikli.getTrenutno();
		COUT << crt;
		float ukupno = 0;
		COUT << racun._artikli;
		for (int i = 0; i < racun._artikli.getTrenutno(); i++)
			ukupno += racun._artikli[i]->getCijena() * racun._artikli[i]->getKolicina();
		COUT << crt;
		COUT << "Ukupno: " << ukupno << endl;
		return COUT;
	}

	KupljeniArtikal* operator()(float ukupnaCijena, int& brojPronadjenih) {
		for (int i = 0; i < _artikli.getTrenutno(); i++) {
			if (_artikli[i]->getCijena() * _artikli[i]->getKolicina() >= ukupnaCijena)
				brojPronadjenih++;
		}

		int brojac = 0;
		KupljeniArtikal* niz = new KupljeniArtikal[brojPronadjenih];
		for (int i = 0; i < _artikli.getTrenutno(); i++) {
			if (_artikli[i]->getCijena() * _artikli[i]->getKolicina() >= ukupnaCijena)
				niz[brojac++] = *(_artikli[i]);
		}
		return niz;
	}
};

int Racun::_brojacRacuna = 1;

void main() {
	KupljeniArtikal cokolada("Milka", 2.29);//naziv i cijena artikla
	KupljeniArtikal sok("Jucy", 1.38);
	KupljeniArtikal cigarete("Drina", 4.32);
	KupljeniArtikal mlijeko("Meggle", 2.87);
	cokolada += 0.27;//povecavamo cijenu artikla, sada je cijena 2,56BAM
	++cokolada;//povecavamo kolicinu artikla, sada je kolicina 2
	++sok;
	Racun racun;
	racun += cokolada;//dodajemo 2 cokolade po cijeni od 2,56BAM
	racun += sok;
	racun += sok;//ako vise puta pokusamo dodati isti artikal, treba da se uveca samo
				 //njegova kolicina na racunu tj.artikli se ne smiju duplirati
	racun += cigarete;
	racun += mlijeko;
	cout << racun << endl; // ispis racuna mozete vidjeti na slici.

	int brojPronadjenih = 0;
	float ukupnaCijena = 5.10;
	cout << crt << "Artikli skuplji od >> " << ukupnaCijena << crt;
	//treba da vrati sve artikle cija je ukupna cijena na racunu (cijena * kolicina)
	//veca od poslane tj. 5.10BAM
	KupljeniArtikal* artikli = racun(ukupnaCijena, brojPronadjenih);
	if (artikli != nullptr) {
		for (size_t i = 0; i < brojPronadjenih; i++) {
			cout << artikli[i] << endl;
		}
	}
	delete[] artikli;
	artikli = nullptr;
}