#include<iostream>
#include<vector>
#include<string>
#include<iterator>
#include<exception>
#include<regex>
#include<thread>
#include<mutex>
using namespace std;

const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO RE\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U,\n"
"\tOSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE\n"
"\tPOMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE!\n"
"7. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"-------------------------------------------------------------------------------\n";

const char* crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };

char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	if (dealociraj)
		delete[]sadrzaj;
	return temp;
}
bool ValidirajEmail(string email) {
	/*
		email adresa mora biti u formatu text.text (sa ili bez tacke),
	dok domena moze biti fit.ba ili edu.fit.ba
	nakon institucije (fit ili edu.fit), osim drzavne (.ba), dozvoljene su domene .com i .org.
	*/
	string uvijet = "[a-z]+\\.?[a-z]+@(fit|edu.fit)(.ba|.com|.org)";
	regex regx(uvijet);
	if (regex_match(email, regx))
		return true;
	return false;
}
template<class T1, class T2>
class Kolekcija {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
	bool _omoguciDupliranje;
public:
	Kolekcija(bool omoguciDupliranje = true) {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_omoguciDupliranje = omoguciDupliranje;
	}
	Kolekcija(const Kolekcija& org) :
		_trenutno(org._trenutno),
		_elementi1(new T1[org._trenutno]),
		_elementi2(new T2[org._trenutno]),
		_omoguciDupliranje(org._omoguciDupliranje) {
		for (int i = 0; i < _trenutno; i++) {
			_elementi1[i] = org._elementi1[i];
			_elementi2[i] = org._elementi2[i];
		}
	}
	Kolekcija& operator=(const Kolekcija& obj) {
		if (this != &obj) {
			delete[]_elementi1; _elementi1 = nullptr;
			delete[]_elementi2; _elementi2 = nullptr;
			_elementi1 = new T1[obj._trenutno];
			_elementi2 = new T2[obj._trenutno];
			_trenutno = obj._trenutno;
			_omoguciDupliranje = obj._omoguciDupliranje;
			for (int i = 0; i < _trenutno; i++) {
				_elementi1[i] = obj._elementi1[i];
				_elementi2[i] = obj._elementi2[i];
			}
		}
		return *this;
	}
	~Kolekcija() {
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
	}
	T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
	int getTrenutno() const { return _trenutno; }
	bool AddElement(const T1& elem1, const T2& elem2) {
		//ukoliko nije dozvoljeno dupliranje elemenata (provjeravaju se T1 i T2), metoda AddElement baca izuzetak
		if (!_omoguciDupliranje) {
			for (int i = 0; i < _trenutno; i++) {
				if (_elementi1[i] == elem1 || _elementi2[i] == elem2) {
					throw exception("Dupliranje nije dozvoljeno");
				}
			}
		}
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];
		for (int i = 0; i < _trenutno; i++) {
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		temp1[_trenutno] = elem1;
		temp2[_trenutno] = elem2;
		_trenutno++;
		delete[]_elementi1;
		delete[]_elementi2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		temp1 = NULL;
		temp2 = NULL;
		return true;
	}


	Kolekcija<T1, T2> operator()(int OD, int DO) {
		/*objekat kolekcija2 ce biti inicijalizovan elementima koji se u objektu kolekcija1 nalaze na lokacijama 1 - 4
   ukljucujuci i te lokacije. u konkretnom primjeru to ce biti parovi sa vrijednostima: 1 1 2 2 3 3 4 4
   primjeri u kojima opseg nije validan, te bi funkcija trebala baciti izuzetak*/
		if (OD < 0 || OD > _trenutno || OD > DO || DO >= _trenutno)
			throw exception("Nespostojeca lokacija");
		Kolekcija<T1, T2> temp;
		for (int i = OD; i <= DO; i++) {
			temp.AddElement(this->_elementi1[i], this->_elementi2[i]);
		}
		return temp;

	}
	bool operator==(const Kolekcija<T1, T2>& desni) {
		if (_trenutno == desni._trenutno) {
			int brojac = 0;
			for (int i = 0; i < _trenutno; i++) {
				if (_elementi1[i] == desni._elementi1[i] &&
					_elementi2[i] == desni._elementi2[i]) {
					brojac++;
				}
			}
			if (brojac == _trenutno)
				return true;
		}
		return false;
	}
	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
};
class Datum {
	int* _dan, * _mjesec, * _godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum& org) :
		_dan(new int(*org._dan)),
		_mjesec(new int(*org._mjesec)),
		_godina(new int(*org._godina)) {}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	Datum& operator=(const Datum& obj) {
		if (this != &obj) {
			delete _dan; _dan = nullptr;
			delete _mjesec; _mjesec = nullptr;
			delete _godina; _godina = nullptr;
			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
		}
		return *this;
	}
	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
	bool operator==(const Datum desni) {
		if (*_godina == *desni._godina)
			if (*_mjesec == *desni._mjesec)
				if (*_dan == *desni._dan)
					return true;
		return false;
	}

	long int getDani() {
		long int brojDana = (*_godina * 365) + (*_mjesec * 30) + *_dan;
		return brojDana;
	}
};

class Predmet {
	char* _naziv;
	//int se odnosi na ocjenu u opsegu od 1 – 5, a datum na momenat postizanja ocjene
	Kolekcija<int, Datum> _ocjene;
public:
	Predmet(const char* naziv = "", int ocjena = 0, Datum datum = Datum()) {
		_naziv = GetNizKaraktera(naziv);
		if (ocjena > 0)
			_ocjene.AddElement(ocjena, datum);
	}
	Predmet(const Predmet& org) {
		_naziv = GetNizKaraktera(org._naziv);
		_ocjene = org._ocjene;
	}
	~Predmet() {
		delete[] _naziv; _naziv = nullptr;
	}
	double getProsjecna() const {
		double suma = 0;
		for (int i = 0; i < _ocjene.getTrenutno(); i++) {
			suma += _ocjene.getElement1(i);
		}
		return suma / static_cast<double>(_ocjene.getTrenutno());
	}
	Predmet& operator=(const Predmet& obj) {
		if (this != &obj) {
			delete[] _naziv; _naziv = nullptr;
			_naziv = GetNizKaraktera(obj._naziv);
			_ocjene = obj._ocjene;
		}
		return *this;
	}
	bool operator==(const Predmet& desni) {
		if (strcmp(_naziv, desni._naziv) == 0)
			if (_ocjene == desni._ocjene)
				return true;
		return false;
	}
	void AddOcjena(int ocjena, Datum datum) {
		_ocjene.AddElement(ocjena, datum);
	}
	char* GetNaziv() { return _naziv; }
	Kolekcija<int, Datum>& GetOcjene() { return _ocjene; }
	friend ostream& operator<<(ostream& COUT, const Predmet& obj) {
		// ispisuje: naziv predmeta, ocjene (zajedno sa datumom polaganja) i prosjecnu ocjenu na predmetu
 // ukoliko predmet nema niti jednu ocjenu prosjecna treba biti 0
		COUT << "Naziv predmeta: " << obj._naziv << endl;
		double suma = 0;
		if (obj._ocjene.getTrenutno() > 0) {
			COUT << "Ispis ocjena iz predmeta " << endl;
			for (int i = 0; i < obj._ocjene.getTrenutno(); i++) {
				COUT << "Ocijena[" << i + 1 << "]-> " << obj._ocjene.getElement1(i);
				COUT << "\tDatum polaganja-> " << obj._ocjene.getElement2(i) << endl;
				suma += obj._ocjene.getElement1(i);
			}
		}
		COUT << "Prosjecna ocijena iz predmeta iznosi-> " << obj.getProsjecna() << endl;
		return COUT;
	}
};

class Uspjeh {
	eRazred _razred;
	//string se odnosi na napomenu o polozenom predmetu
	Kolekcija<Predmet, string>* _polozeniPredmeti;
public:
	Uspjeh(eRazred razred = PRVI) {
		_razred = razred;
		_polozeniPredmeti = new Kolekcija<Predmet, string>(false);
	}
	Uspjeh(const Uspjeh& org) :
		_razred(org._razred),
		_polozeniPredmeti(new Kolekcija<Predmet, string>(*org._polozeniPredmeti)) {}
	~Uspjeh() { delete _polozeniPredmeti; _polozeniPredmeti = nullptr; }
	Uspjeh& operator=(const Uspjeh& obj) {
		if (this != &obj) {
			delete _polozeniPredmeti; _polozeniPredmeti = nullptr;
			_razred = obj._razred;
			_polozeniPredmeti = new Kolekcija<Predmet, string>(*obj._polozeniPredmeti);
		}
		return *this;
	}
	bool addPredmet(Predmet predmet, string napomena, eRazred razred) {
		/*
		uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
		tom prilikom onemoguciti:
			- dodavanje istih (moraju biti identicne vrijednosti svih clanova) predmeta na nivou jednog razreda,
			- dodavanje predmeta kod kojih je prosjecna ocjena manja od 2.5
			- dodavanje vise od 5 predmeta na nivou jednog razreda
		razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
		Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
		*/

		if (_polozeniPredmeti->getTrenutno() != 0) {
			if (_polozeniPredmeti->getTrenutno() > 5)
				return false;
			double suma = 0;
			for (int i = 0; i < predmet.GetOcjene().getTrenutno(); i++) {
				suma += predmet.GetOcjene().getElement1(i);
			}
			suma = suma / static_cast<double>(predmet.GetOcjene().getTrenutno());
			if (suma < 2.5)
				return false;
			//for (int i = 0; i < _polozeniPredmeti->getTrenutno(); i++) {
			//	if (_polozeniPredmeti->getElement1(i) == predmet) {
			//		return false;
			//	}
			//}
		}
		try
		{
			_polozeniPredmeti->AddElement(predmet, napomena);
		}
		catch (exception& err)
		{
			return false;
		}
		return true;
	}
	Kolekcija<Predmet, string>* GetPredmeti() { return _polozeniPredmeti; }
	eRazred GetERazred() { return _razred; }
	friend ostream& operator<< (ostream& COUT, const Uspjeh& obj) {
		COUT << "Razred: " << obj._razred << " " << "Polozeni predmeti:" << endl << *obj._polozeniPredmeti << endl;
		return COUT;
	}
};
mutex cuvar;
class Kandidat {
	char* _imePrezime;
	string _emailAdresa;
	string _brojTelefona;
	vector<Uspjeh> _uspjeh;
	void posaljiMail(string email) {
		cuvar.lock();
		cout << email << endl;
		cuvar.unlock();
	}
	void posaljiPoruku(string sms) {
		cuvar.lock();
		cout << sms << endl;
		cuvar.unlock();
	}
	void posaljiThread(eRazred razred) {
		vector<Uspjeh>::iterator itr = _uspjeh.begin();
		int brojacPredmeta = 0;
		int brojacUkupniUspjeh = 0;
		double sumaRazred = 0;
		double sumaUkupnoSkolovanje = 0;
		for (itr; itr != _uspjeh.end(); itr++) {
			for (int i = 0; i < itr->GetPredmeti()->getTrenutno(); i++) {
				if (itr->GetERazred() == razred) {
					sumaRazred += itr->GetPredmeti()->getElement1(i).getProsjecna();
					brojacPredmeta++;
				}
				sumaUkupnoSkolovanje += itr->GetPredmeti()->getElement1(i).getProsjecna();
				brojacUkupniUspjeh++;
			}
		}
		sumaRazred = sumaRazred / static_cast<double>(brojacPredmeta);
		sumaUkupnoSkolovanje = sumaUkupnoSkolovanje / static_cast<double>(brojacUkupniUspjeh);
		string imePrezime = _imePrezime;
		string email = "\nFROM:info@fit.ba\nTO: " + _emailAdresa + "\nPostovani " + imePrezime + " evidentirali ste uspjeh za " + to_string(razred)
			+ " razred. Dosadasnji uspjeh (prosjek) na nivou " + to_string(razred) + " razreda iznosi " + to_string(sumaRazred) + " , a ukupni uspjeh u toku skolovanja iznosi " + to_string(sumaUkupnoSkolovanje) + " .\nPozdrav.\n";

		thread slanjeEmaila(&Kandidat::posaljiMail, this, email);
		slanjeEmaila.join();
		if (sumaRazred > 4.5) {
			string sms = "\nSvaka cast za uspjeh " + to_string(sumaRazred) + " u " + to_string(razred) + " razredu.\n";
			thread slanjeSms(&Kandidat::posaljiPoruku, this, sms);
			slanjeSms.join();
		}
	}
public:
	Kandidat(const char* imePrezime, string emailAdresa, string brojTelefona) {
		_imePrezime = GetNizKaraktera(imePrezime);
		_brojTelefona = brojTelefona;
		if (ValidirajEmail(emailAdresa))
			_emailAdresa = emailAdresa;
		else
			_emailAdresa = "notSet@edu.fit.ba";
	}
	Kandidat(const Kandidat& org) :
		_emailAdresa(org._emailAdresa),
		_brojTelefona(org._brojTelefona),
		_uspjeh(org._uspjeh),
		_imePrezime(GetNizKaraktera(org._imePrezime)) {    }
	~Kandidat() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	Kandidat& operator=(const Kandidat& obj) {
		if (this != &obj) {
			delete[] _imePrezime; _imePrezime = nullptr;
			_imePrezime = GetNizKaraktera(obj._imePrezime);
			_emailAdresa = obj._emailAdresa;
			_brojTelefona = obj._brojTelefona;
			_uspjeh = obj._uspjeh;
		}
		return *this;
	}
	friend ostream& operator<< (ostream& COUT, Kandidat& obj) {
		COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
		for (size_t i = 0; i < obj._uspjeh.size(); i++)
			COUT << obj._uspjeh[i];
		return COUT;
	}
	vector<Uspjeh>& GetUspjeh() { return _uspjeh; }
	string GetEmail() { return _emailAdresa; }
	string GetBrojTelefona() { return _brojTelefona; }
	char* GetImePrezime() { return _imePrezime; }
	bool AddPredmet(eRazred razred, Predmet predmet, string napomena) {
		/*
   uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
   tom prilikom onemoguciti:
   - dodavanje istih (moraju biti identicne vrijednosti svih clanova) predmeta na nivou jednog razreda,
   - dodavanje predmeta kod kojih je prosjecna ocjena manja od 2.5
   - dodavanje vise od 5 predmeta na nivou jednog razreda
   razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
   Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
   */
   /*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
FROM:info@fit.ba
TO: emailKorisnika
Postovani ime i prezime, evidentirali ste uspjeh za X razred. Dosadasnji uspjeh (prosjek)
na nivou X razreda iznosi Y, a ukupni uspjeh u toku skolovanja iznosi Z.
Pozdrav.
FIT Team.
ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh 4.X u X razredu".
slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
*/
		bool imaRazred = false;
		int indexRazred = -1;

		for (int i = 0; i < _uspjeh.size(); i++) {
			if (_uspjeh[i].GetERazred() == razred) {
				imaRazred = true;
				indexRazred = i;
				break;
			}
		}
		if (imaRazred) {
			if (_uspjeh[indexRazred].addPredmet(predmet, napomena, razred)) {
				//thread
				posaljiThread(razred);
				return true;
			}
			return false;
		}
		else if (!imaRazred) {
			Uspjeh tempUspjeh(razred);
			if (tempUspjeh.addPredmet(predmet, napomena, razred)) {
				_uspjeh.push_back(tempUspjeh);
				//thread
				posaljiThread(razred);
			}
			return true;
		}
		return false;
	}

	Kolekcija<Predmet, float> operator()(Datum datumOD, Datum datumDO) {
		//vraca kolekciju predmeta koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
		//float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na predmetu
		Kolekcija<Predmet, float> tempKolekcija(true);
		float prosjek = 0;
		vector<Uspjeh>::iterator itr = _uspjeh.begin();
		for (int i = 0; i < _uspjeh.size(); i++) {
			for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++) {
				bool postojiDan = false;
				Predmet tempPredmet;
				if (_uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getTrenutno() > 1) {
					for (int k = 0; k < _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getTrenutno(); k++) {
						Datum tempDatum = _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getElement2(k);
						if (tempDatum.getDani() >= datumOD.getDani() && tempDatum.getDani() <= datumDO.getDani()) {
							tempPredmet = _uspjeh[i].GetPredmeti()->getElement1(j);
							postojiDan = true;
						}

					}
					if (postojiDan) {
						Datum temp;
						//for (int k = _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getTrenutno()-1; k > 0; k--) {
						//	//datum Posljednje zadnje ocijene
						//	temp = _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getElement2(k + 1);
						//	prosjek = temp.getDani() - _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getElement2(k).getDani();
						//}
						temp = _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getElement2(0);
						for (int k = 1; k < _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getTrenutno(); k++) {
							prosjek += temp.getDani() - _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getElement2(k).getDani();
							temp = _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getElement2(k);
						}
						if (prosjek < 0)
							prosjek = abs(prosjek);
						prosjek /= static_cast<double>(_uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getTrenutno());
						tempKolekcija.AddElement(tempPredmet, prosjek);
					}

				}
			}
		}
		return tempKolekcija;

	}
	Uspjeh* operator[](eRazred razred) {
		//vraca uspjeh kandidata ostvaren u prvom razredu
		Uspjeh* tempUspjeh = NULL;
		vector<Uspjeh>::iterator itr = _uspjeh.begin();
		for (itr; itr < _uspjeh.end(); itr++) {
			if (itr->GetERazred() == razred) {
				tempUspjeh = new Uspjeh(razred);
				for (int i = 0; i < itr->GetPredmeti()->getTrenutno(); i++) {
					tempUspjeh->addPredmet(itr->GetPredmeti()->getElement1(i), itr->GetPredmeti()->getElement2(i), razred);
				}
			}
		}
		return tempUspjeh;
	}
};
const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Na sta se odnosi pojam reprezentacije tipa?\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Na koji nacin se moze izbjeci pojavljivanje vise podobjekata bazne klase u slucaju visestrukog nasljedjivanja?\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}

void main() {

	cout << PORUKA;
	cin.get();

	cout << GetOdgovorNaPrvoPitanje() << endl;
	cin.get();
	cout << GetOdgovorNaDrugoPitanje() << endl;
	cin.get();

	Datum temp,
		datum19062019(19, 6, 2019),
		datum20062019(20, 6, 2019),
		datum30062019(30, 6, 2019),
		datum05072019(5, 7, 2019);

	int kolekcijaTestSize = 9;
	Kolekcija<int, int> kolekcija1(false);
	for (int i = 0; i <= kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i);

	try {
		//ukoliko nije dozvoljeno dupliranje elemenata (provjeravaju se T1 i T2), metoda AddElement baca izuzetak
		kolekcija1.AddElement(3, 3);
	}
	catch (exception& err) {
		cout << err.what() << crt;
	}
	cout << kolekcija1 << crt;

	/*objekat kolekcija2 ce biti inicijalizovan elementima koji se u objektu kolekcija1 nalaze na lokacijama 1 - 4
	ukljucujuci i te lokacije. u konkretnom primjeru to ce biti parovi sa vrijednostima: 1 1 2 2 3 3 4 4*/
	Kolekcija<int, int> kolekcija2 = kolekcija1(1, 4);
	cout << kolekcija2 << crt;
	try {
		//primjeri u kojima opseg nije validan, te bi funkcija trebala baciti izuzetak
		Kolekcija<int, int> temp1 = kolekcija1(1, 14);//imamo 10 elemenata
		Kolekcija<int, int> temp2 = kolekcija1(-1, 8);//lokacija -1 ne postoji
	}
	catch (exception& err) {
		cout << err.what() << crt;
	}

	//parametri: nazivPredmeta, prva ocjena, datum
	Predmet Matematika("Matematika", 5, datum19062019),
		Fizika("Fizika", 5, datum20062019),
		Hemija("Hemija", 2, datum30062019),
		Engleski("Engleski", 5, datum05072019);

	Matematika.AddOcjena(3, datum05072019);
	Matematika.AddOcjena(5, datum05072019);

	// ispisuje: naziv predmeta, ocjene (zajedno sa datumom polaganja) i prosjecnu ocjenu na predmetu
	// ukoliko predmet nema niti jednu ocjenu prosjecna treba biti 0
	cout << Matematika << endl;

	if (ValidirajEmail("text.text@edu.fit.ba"))
		cout << "Email validan" << crt;
	if (ValidirajEmail("texttext@edu.fit.ba"))
		cout << "Email validan" << crt;
	if (ValidirajEmail("texttext@fit.ba"))
		cout << "Email validan" << crt;
	if (ValidirajEmail("texttext@fit.com"))
		cout << "Email validan" << crt;
	if (ValidirajEmail("texttext@edu.fit.org"))
		cout << "Email validan" << crt;

	/*
	email adresa mora biti u formatu text.text (sa ili bez tacke),
	dok domena moze biti fit.ba ili edu.fit.ba
	nakon institucije (fit ili edu.fit), osim drzavne (.ba), dozvoljene su domene .com i .org.

	za provjeru validnosti email adrese koristiti globalnu funkciju ValidirajEmail, a unutar nje regex metode.
	validacija email adrese ce se vrsiti unutar konstruktora klase Kandidat, a u slucaju da nije validna
	postaviti je na defaultnu adresu: notSet@edu.fit.ba
	*/

	Kandidat jasmin("Jasmin Azemovic", "jasmin@fit.ba", "033 281 172");
	Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");
	Kandidat emailNotValid("Ime Prezime", "korisnik@klix.ba", "033 281 170");

	/*
	uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
	tom prilikom onemoguciti:
	- dodavanje istih (moraju biti identicne vrijednosti svih clanova) predmeta na nivou jednog razreda,
	- dodavanje predmeta kod kojih je prosjecna ocjena manja od 2.5
	- dodavanje vise od 5 predmeta na nivou jednog razreda
	razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
	Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	if (jasmin.AddPredmet(DRUGI, Fizika, "Napomena 1"))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(DRUGI, Hemija, "Napomena 2"))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Engleski, "Napomena 3"))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 4"))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati Matematiku jer je vec dodana u prvom razredu
	if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 5"))
		cout << "Predmet uspjesno dodan!" << crt;

	/*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
	FROM:info@fit.ba
	TO: emailKorisnika
	Postovani ime i prezime, evidentirali ste uspjeh za X razred. Dosadasnji uspjeh (prosjek)
	na nivou X razreda iznosi Y, a ukupni uspjeh u toku skolovanja iznosi Z.
	Pozdrav.
	FIT Team.
	ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh 4.X u X razredu".
	slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
	*/
	cout << jasmin << crt;

	//vraca kolekciju predmeta koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
	//float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na predmetu
	Kolekcija<Predmet, float> jasminUspjeh = jasmin(Datum(18, 06, 2019), Datum(21, 06, 2019));
	cout << jasminUspjeh << crt;

	Uspjeh* uspjeh_Irazred = jasmin[PRVI];//vraca uspjeh kandidata ostvaren u prvom razredu
	if (uspjeh_Irazred != nullptr)
		cout << *uspjeh_Irazred << crt;

	cin.get();
	system("pause>0");
}