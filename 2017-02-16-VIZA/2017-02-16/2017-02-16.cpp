#include<iostream>
#include <vector>
using namespace std;
const char* crt = "\n-----------------------------------------------\n";


enum Status { ZAHTJEV_ZAPRIMLJEN, ODUSTAO, ODBIJENA, IZDATA, ISTEKLA, PONISTENA };
enum Sortiranje { RASTUCI, OPADAJUCI };
enum Drzava { SAD, AUSTRALIJA, ENGLESKA };

class Datum
{
	int* _dan, * _mjesec, * _godina;
	bool prijestupna()const
	{
		int god = *(_godina);
		if (god % 4 == 0)
			if (god % 100 == 0)
				if (god % 400 == 0)
					return true;
		return false;
	}
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000)
	{
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum& org) :
		_dan(new int(*(org._dan))), _mjesec(new int(*(org._mjesec))), _godina(new int(*(org._godina)))
	{   }
	Datum& operator=(const Datum& obj)
	{
		if (this != &obj)
		{
			delete _dan; _dan = NULL;
			delete _mjesec; _mjesec = NULL;
			delete _godina; _godina = NULL;
			_dan = new int(*(obj._dan));
			_mjesec = new int(*(obj._mjesec));
			_godina = new int(*(obj._godina));
		}
		return *this;
	}
	~Datum()
	{
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	Datum& operator++()
	{
		int dan = *(this->_dan);
		int mjesec = *(this->_mjesec);
		if (dan >= 28)
		{
			if (dan == 28)
			{
				if (mjesec == 2)
				{
					if (prijestupna())
					{
						(*(this->_dan))++;
						return *this;
					}
					else
					{
						(*(this->_mjesec))++;
						(*(this->_dan)) = 1;
						return *this;
					}
				}
				else if (mjesec == 1 || mjesec == 3 || mjesec == 5 || mjesec == 7 ||
					mjesec == 8 || mjesec == 10 || mjesec == 12)
				{
					if (dan < 31)
					{
						(*(this->_dan))++;
						return *this;
					}
					else
					{
						if (mjesec == 12)
						{
							(*(this->_godina))++;
							(*(this->_mjesec)) = 1;
							(*(this->_dan)) = 1;
							return *this;
						}
						else
						{
							(*(this->_mjesec))++;
							(*(this->_dan)) = 1;
							return *this;
						}
					}
				}
				else
				{
					if (dan < 30)
					{
						(*(this->_dan))++;
						return *this;
					}
					else
					{

						(*(this->_mjesec))++;
						(*(this->_dan)) = 1;
						return *this;
					}
				}
			}
		}
		else
		{
			(*(this->_dan))++;
			return *this;
		}

	}
	bool operator==(const Datum& obj)
	{
		return ((*(obj._dan) == *(this->_dan)) &&
			(*(obj._mjesec) == *(this->_mjesec)) &&
			(*(obj._godina) == *(this->_godina)));

	}
	friend ostream& operator<< (ostream& COUT, Datum& obj)
	{
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
};
template<class T1, class T2, int max>
class FITKolekcija
{
	T1* _elementi1[max];
	T2* _elementi2[max];
	int _trenutno;
public:
	FITKolekcija()
	{
		for (size_t i = 0; i < max; i++)
		{
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		_elementi1[0] = new T1[max];
		_elementi2[0] = new T2[max];
		_trenutno = 0;
	}
	~FITKolekcija()
	{
		for (size_t i = 0; i < max; i++)
		{
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}
	T1** GetT1() { return _elementi1; }
	T2** GetT2() { return _elementi2; }
	int GetTrenutno() { return _trenutno; }
	void Dodaj(T1 elem, T2 elem2)
	{
		_elementi1[0][_trenutno] = elem;
		_elementi2[0][_trenutno] = elem2;
		_trenutno++;
		if (_trenutno > max)
		{
			throw exception("Prekoracenje maximalne velicine kolekcije");
		}
	}
	void Sortiraj(Sortiranje nacin)
	{
		if (nacin == 0)
		{
			int ok;
			for (int i = _trenutno - 1; i > 0; i--)
			{
				ok = 1;
				for (int j = 0; j < i; j++)
				{
					if ((_elementi1[0][j + 1]) < (_elementi1[0][j]))
					{
						T1 stTemp = (_elementi1[0][j]);
						T2 ocTemp = (_elementi2[0][j]);

						(_elementi1[0][j]) = (_elementi1[0][j + 1]);
						(_elementi2[0][j]) = (_elementi2[0][j + 1]);

						(_elementi1[0][j + 1]) = stTemp;
						(_elementi2[0][j + 1]) = ocTemp;
						ok = 0;

					}
				}
				if (ok)
					break;
			}
			return;
		}
		else if (nacin == 1)
		{
			int ok;
			for (int i = _trenutno - 1; i > 0; i--)
			{
				ok = 1;
				for (int j = 0; j < i; j++)
				{
					if ((_elementi1[0][j + 1]) > (_elementi1[0][j]))
					{
						T1 stTemp = (_elementi1[0][j]);
						T2 ocTemp = (_elementi2[0][j]);

						(_elementi1[0][j]) = (_elementi1[0][j + 1]);
						(_elementi2[0][j]) = (_elementi2[0][j + 1]);

						(_elementi1[0][j + 1]) = stTemp;
						(_elementi2[0][j + 1]) = ocTemp;
						ok = 0;

					}
				}
				if (ok)
					break;
			}
			return;
		}
		return;
	}
	friend ostream& operator<< (ostream& COUT, FITKolekcija& obj)
	{
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj._elementi1[0][i] << " " << obj._elementi2[0][i] << endl;
		return COUT;
	}
};


class Viza
{
protected:
	Drzava _drzava;
	Datum _vazenjeOD;
	Datum _vazenjeDO;
	char* _napomena;
	// string se odnosi na razlog promjene statusa, a mogu�e je samo 10 izmjena statusa
	FITKolekcija<Status, string, 10> _statusi;
public:
	Viza(Drzava drzava, Datum _OD, Datum _DO, const char* napomena = nullptr) :_vazenjeDO(_DO), _vazenjeOD(_OD)
	{
		if (napomena != nullptr)
		{
		
			_napomena = new char[strlen(napomena) + 1];
			strcpy_s(_napomena, strlen(napomena) + 1, napomena);
		}
		else
			_napomena = nullptr;
		_drzava = drzava;
	}
	Viza(const Viza& org)
	{
		if (org._napomena != NULL)
		{
			_napomena = new char[strlen(org._napomena) + 1];
			strcpy_s(_napomena, strlen(org._napomena) + 1, org._napomena);
		}
		else
		{
			_napomena = NULL;
		}
		_drzava=org._drzava;
		_vazenjeOD = org._vazenjeOD;
		_vazenjeDO = org._vazenjeDO;
	}
	~Viza() { delete[] _napomena; _napomena = nullptr; }
	Viza& operator=(const Viza& obj)
	{
		if (this != &obj)
		{
			if (_napomena != NULL)
			{
				delete[]_napomena;
			}
			if (obj._napomena == NULL)
			{
				_napomena = NULL;
			}
			else
			{
				_napomena = new char[strlen(obj._napomena) + 1];
				strcpy_s(_napomena, strlen(obj._napomena) + 1, obj._napomena);
			}
			_drzava = obj._drzava;
			_vazenjeOD = obj._vazenjeOD;
			_vazenjeDO = obj._vazenjeDO;
		}
		return *this;
	}
	bool operator==(const Viza& obj)
	{
		return true;
	}
	Datum GetVazenjeOD() { return _vazenjeOD; }
	Datum GetVazenjeDO() { return _vazenjeDO; }
	char* GetNapomena() { return _napomena; }
	FITKolekcija<Status, string, 10>& GetStatuse() { return _statusi; }
	virtual void Info() = 0;
};

class PoslovnaViza :public Viza
{
	string _kompanija; //naziv kompanije koja potvr�uje da posluje sa aplikantom
public:
	PoslovnaViza(Drzava drzava, Datum _OD, Datum _DO, string kompanija, char* napomena = nullptr) : Viza(drzava, _OD, _DO, napomena)
	{
		_kompanija = kompanija;
	}
	friend ostream& operator << (ostream& COUT, PoslovnaViza& obj)
	{
		COUT << obj._drzava << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._kompanija << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI -> " << obj._statusi << endl;
		return COUT;
	}
	virtual void Info()
	{

	}
};
class TuristickaViza : public Viza
{
	string _adresa; //adresa hotele ili drugog smje�taja na kojoj �e aplikant boraviti
public:
	TuristickaViza(Drzava drzava, Datum _OD, Datum _DO, string adresa, char* napomena = nullptr) : Viza(drzava, _OD, _DO, napomena)
	{
		_adresa = adresa;
	}
	friend ostream& operator << (ostream& COUT, TuristickaViza& obj)
	{
		COUT << obj._drzava << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._adresa << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI -> " << obj._statusi << endl;
		return COUT;
	}
	virtual void Info()
	{

	}
};

class Aplikant
{
	char* _imePrezime;
	Datum* _datumRodjenja;
	char* _brojPasosa;
	vector<Viza*> _vize;
public:
	Aplikant(const char* imePrezime,Datum datumRodjenja,const char* brPasosa)
	{
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
		vel = strlen(brPasosa) + 1;
		_brojPasosa = new char[vel];
		strcpy_s(_brojPasosa, vel, brPasosa);
		_datumRodjenja = new Datum(datumRodjenja);
	}
	Aplikant(const Aplikant& org) :
		_imePrezime(new char[strlen(org._imePrezime)+1]), _datumRodjenja(new Datum(*(org._datumRodjenja))),
		_brojPasosa(new char[strlen(org._brojPasosa)+1]),_vize(org._vize.size())
	{
		strcpy_s(_imePrezime, strlen(org._imePrezime) + 1, org._imePrezime);
		strcpy_s(_brojPasosa, strlen(org._brojPasosa) + 1, org._brojPasosa);
		for (int i = 0; i < org._vize.size(); i++)
		{
			PoslovnaViza* tempP = dynamic_cast<PoslovnaViza*>(org._vize[i]);
			TuristickaViza* tempT = dynamic_cast<TuristickaViza*>(org._vize[i]);
			if (tempP != NULL)
			{
				_vize[i] = tempP;
			}
			else if (tempT != NULL)
				_vize[i] = tempT;
			else
			{
				continue;
			}
			
		}
	}
	~Aplikant()
	{
		delete[]_imePrezime; _imePrezime = nullptr;
		delete[]_brojPasosa; _brojPasosa = nullptr;
		delete _datumRodjenja; _datumRodjenja = nullptr;
		for (size_t i = 0; i < _vize.size(); i++)
		{
			delete _vize[i]; _vize[i] = nullptr;
		}
	}
	friend ostream& operator << (ostream& COUT, Aplikant& obj)
	{
		COUT << obj._imePrezime << " " << obj._datumRodjenja << " " << obj._brojPasosa << endl;
		COUT << "VIZE -> " << endl;
		for (size_t i = 0; i < obj._vize.size(); i++)
			COUT << obj._vize[i] << endl;
		return COUT;
	}
	vector<Viza* > GetVize() { return _vize; }
	bool DodajVizu(Viza* vizu)
	{
		PoslovnaViza* tempP = dynamic_cast<PoslovnaViza*>(vizu);
		TuristickaViza* tempT = dynamic_cast<TuristickaViza*>(vizu);
		if (tempP != NULL)
		{
			_vize.push_back(new PoslovnaViza(*tempP));
			return true;
		}
		else if (tempT != NULL)
		{
			_vize.push_back(new TuristickaViza(*tempT));
			return true;
		}
		return false;
	}
	bool DodajStatus(Viza* vizu, Status statusVize, const char* napomena = NULL)
	{
		for (int i = 0; i < _vize.size(); i++)
		{
			if (_vize[i] == vizu)
				return false;
		}
		return true;
	}
	void PrintajVizu(Viza* viza)
	{

	}
};

void main()
{

	/****************************************************************************
	// 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	// 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME
	//    ERROR �E BITI OZNACENO KAO "RE"
	// 3. SPA�AVAJTE PROJEKAT KAKO BI SE SPRIJE�ILO GUBLJENJE URA�ENOG ZADATKA
	// 4. PROGRAMSKI CODE SE TAKO�ER NALAZI U FAJLU CODE.TXT
	// 5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTI�NI ONIMA KOJI SU KORI�TENI U TESTNOM CODE-U,
	//    OSIM U SLU�AJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMO�NE FUNKCIJE MO�ETE IMENOVATI
	//    I DODAVATI PO �ELJI.
	// 6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNA�ENO.
	****************************************************************************/

	Datum danas(15, 2, 2017);
	Datum sutra(++danas); // inkrementiranjem se uvecava dan za 1
	Datum prekosutra;
	Datum dat15022017(15, 2, 2017), dat18082017(18, 8, 2017), dat11052018(11, 5, 2018), dat11122017(11, 12, 2017);


	prekosutra = danas;
	if (prekosutra == danas)
		cout << "Datumi su jednaki" << endl;
	cout << danas << endl << sutra << endl << prekosutra << crt;

	int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

	FITKolekcija<int, int, 7> brojevi;
	brojevi.Dodaj(v196, v6);
	brojevi.Dodaj(v13, v32);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v63, v13);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v196, v6);

	cout << brojevi << crt;
	//NA KOJI NA�IN �E SE VR�ITI SORTIRANJE PRVENSTVENO ZAVISI OD VRIJEDNOSTI PARAMETRA, A VR�I SE NA OSNOVU VRIJEDNOSTI TIPA T1
	brojevi.Sortiraj(RASTUCI);
	cout << brojevi << crt;
	brojevi.Sortiraj(OPADAJUCI);
	cout << brojevi << crt;

	try
	{
		//U SLU�AJU PREKORA�ENJA DEFINISANE VELI�INE KOLEKCIJE, POTREBNO JE BACITI IZUZETAK
		brojevi.Dodaj(v13, v196);
	}
	catch (exception& err)
	{
		cout << "Error -> " << err.what() << endl;
	}

	Aplikant jasmin("Jasmin Azemovic", Datum(15, 5, 1979), "BS6985645");
	PoslovnaViza poslovna(SAD, dat15022017, dat18082017, "Microsoft");


	// NOVA VIZA SE NE MO�E DODATI UKOLIKO:
	// -	JE APLIKANT MLA�I OD 10 GODINA
	// -	U TOM PERIODU (ODNOSI SE NA PERIOD VIZE OD - DO) NE POSTOJI VA�E�A (POJAM VA�E�A PODRAZUMIJEVA DA JE POSLJEDNJI STATUS VIZE BIO IZDATA TJ. DA VIZA NIJE 	PONI�TENA ILI ISTEKLA) VIZA ZA TU DR�AVU, BEZ OBZIRA NA NJENU VRSTU.	
	// -	JE NEKA OD RANIJIH VIZA PONI�TENA (OVO SE ODNOSI SAMO NA VIZE ZA DR�AVU: SAD)

	if (jasmin.DodajVizu(&poslovna))
		cout << "Podaci o vizi uspje�no zaprimljeni" << endl;

	// STATUSI SE MOGU DODAVATI SAMO U REDOSLIJEDU U KOJEM SE NALAZE U ENUMERACIJI NPR.
	// -	NIJE MOGU�E ODBITI VIZU AKO PRETHODNO NIJE IMALA STATUS DA JE ZAHTJEV_ZAPRIMLJEN;
	// -	NIJE MOGU�E PONI�TITI VIZU KOJA PRETHODNO NIJE IZDATA

	if (jasmin.DodajStatus(&poslovna, ZAHTJEV_ZAPRIMLJEN, "provjeriti navode sa predstavnicima kompanije Microsoft"))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;


	if (jasmin.DodajStatus(&poslovna, PONISTENA, "predstavnicima kompanije Microsoft nemaju informaciju o posjeti navedenog aplikanta"))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	TuristickaViza turisticka(ENGLESKA, dat18082017, dat11122017, "Whitehall Pl, London SW1A 2BD, UK");

	if (jasmin.DodajStatus(&turisticka, ZAHTJEV_ZAPRIMLJEN))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

//	if (jasmin.DodajStatus(&turisticka, IZDATA))
//	{
//		cout << "Status vize uspijesno promijenjen" << endl;
//		//U ZASEBNOJ NITI URADITI PRINTANJE VIZE...KORISTITI METODU SLEEP_FOR
//		jasmin.PrintajVizu(&turisticka);
//	}
//	else
//		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;
//	//VRA�A PODATKE O SVIM VIZAMA KOJE SU IZDATE ZA ODRE�ENU DR�AVU, A KOJE SU IMALE ODRE�ENI STATUS
//	vector<Viza*> jasminVize = jasmin.GetVizeByDrzava(SAD, PONISTENA);
//
//	//UNUTAR NAPOMENE MIJENJA SADR�AJ "nemaju informaciju" SA "su upoznti sa" - KORISTITI REGEX
//	jasmin.ZamijeniRijec("nemaju informaciju", "su upoznti sa");
}