#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<iomanip>
#include<regex>
#include<iterator>
#include<thread>
#include<mutex>
using namespace std;
mutex m;


const char* crt = "\n-------------------------------------------\n";
enum GodinaStudija { PRVA = 1, DRUGA, TRECA };

char* Alociraj(const char* sadrzaj) {
    if (sadrzaj == nullptr)return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    return temp;
}

template<class T1, class T2>
class Dictionary {
    T1* _elementi1;
    T2* _elementi2;
    int* _trenutno;
    void povecajTrenutno()
    {
        (*(_trenutno))++;
    }
public:
    Dictionary() {
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _trenutno = new int(0);
    }

    Dictionary(const Dictionary<T1,T2>& org):
        _elementi1(new T1[*(org._trenutno)]),
        _elementi2(new T2[*(org._trenutno)]),
        _trenutno(new int(*(org._trenutno)))
    {  
        for (int i = 0; i < org.getTrenutno(); i++)
        {
            _elementi1[i] = org._elementi1[i];
            _elementi2[i] = org._elementi2[i];
        }
       
    }
    ~Dictionary() {
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
        delete _trenutno; _trenutno = nullptr;
    }
    Dictionary& operator=(const Dictionary<T1, T2>& obj)
    {
        if (this != &obj)
        {
            delete[]_elementi1; _elementi1 = nullptr;
            delete[]_elementi2; _elementi2 = nullptr;
            delete _trenutno; _trenutno = nullptr;
            _elementi1 = new T1[*(obj._trenutno)];
            _elementi2 = new T2[*(obj._trenutno)];
            _trenutno = new int(*(obj._trenutno));
            for (int i = 0; i < obj.getTrenutno(); i++)
            {
                _elementi1[i] = obj._elementi1[i];
                _elementi2[i] = obj._elementi2[i];
            }
        }
        return *this;
    }
    T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
    T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
    int getTrenutno() const { return *_trenutno; }
    friend ostream& operator<< (ostream& COUT, const Dictionary& obj) {
        for (size_t i = 0; i < *obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
    void AddElement(const T1& elem1,const T2& elem2)
    {
        T1* tempE1 = new T1[getTrenutno()+1];
        T2* tempE2 = new T2[getTrenutno()+1];
        for (int i = 0; i < getTrenutno(); i++)
        {
            tempE1[i] = _elementi1[i];
            tempE2[i] = _elementi2[i];
        }
        tempE1[getTrenutno()] = elem1;
        tempE2[getTrenutno()] = elem2;
        povecajTrenutno();
        delete[]_elementi1;
        delete[]_elementi2;
        _elementi1 = tempE1;
        _elementi2 = tempE2;
        tempE1 = NULL;
        tempE2 = NULL;
        
        
    }
    Dictionary<T1, T2> getRange(int OD, int DO)const 
    {
        //vraca elemente kolekcije koji se nalaze na lokacijama definisanim vrijednostima parametara (npr. 2 - 7). 
        //funkcija baca izuzetak u slucaju da se zahtijeva lokacija koja ne postoji ili je vrijednost posljednje lokacije manja od pocetne
        if ((OD < 0 )||( DO < 0 )|| (DO > getTrenutno()))
            throw exception("Lokacija ne postoji");
        if(DO < OD)
            throw exception("Posljednja lokacije ne moze biti manja od pocetne");

        Dictionary<T1, T2> temp;
        for (size_t i = OD; i < DO; i++)
        {
            temp.AddElement(_elementi1[i], _elementi2[i]);
        }
        return temp;
    }

};
class DatumVrijeme {
    int* _dan, * _mjesec, * _godina, * _sati, * _minuti;
    int toMin()const
    {
        return *_godina * 365 * 24 * 60 * 60 + *_mjesec * 30 * 24 * 60 * 60 + *_dan * 60 * 60 + *_minuti;
    }
public:
    DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
        _sati = new int(sati);
        _minuti = new int(minuti);
    }
    DatumVrijeme(const DatumVrijeme& org):
        _dan(new int(*(org._dan))),
        _mjesec(new int(*(org._mjesec))),
        _godina(new int(*(org._godina))),
        _sati(new int(*(org._sati))),
        _minuti(new int(*(org._minuti)))
    {   }
    ~DatumVrijeme() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
        delete _sati; _sati = nullptr;
        delete _minuti; _minuti = nullptr;
    }
    DatumVrijeme& operator=(const DatumVrijeme& obj)
    {
        if (this != &obj)
        {
            delete _dan; _dan = nullptr;
            delete _mjesec; _mjesec = nullptr;
            delete _godina; _godina = nullptr;
            delete _sati; _sati = nullptr;
            delete _minuti; _minuti = nullptr;
            _dan = new int(*(obj._dan));
            _mjesec = new int(*(obj._mjesec));
            _godina = new int(*(obj._godina));
            _sati = new int(*(obj._sati));
            _minuti = new int(*(obj._minuti));
        }
        return *this;
    }
    friend ostream& operator<< (ostream& COUT, const DatumVrijeme& obj) {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti << endl;
        return COUT;
    }
    char* ToCharArray()
    {
        //treba ispisati: 19/06/2019 10:15
        stringstream obj;
        obj<< setw(2)<< setfill('0')<< *(this->_dan) << "/" << setw(2) <<
            setfill('0') << *(this->_mjesec) << "/" << *(this->_godina) << " " << *(this->_sati) << ":" << *(this->_minuti);

        return Alociraj(obj.str().c_str());
    }
    friend bool ProsaoMinSat(const DatumVrijeme& d1, const DatumVrijeme& d2)
    {
        return d1.toMin() - d2.toMin() >= 60;
    }
    bool operator<(const DatumVrijeme& obj)
    {
        if (*(this->_godina) != *(obj._godina))
            return *(this->_godina) < *(obj._godina);
        else if(*(this->_mjesec) != *(obj._mjesec))
            return *(this->_godina) < *(obj._godina);
        else if (*(this->_dan) != *(obj._dan))
            return *(this->_dan) < *(obj._dan);
        else if (*(this->_sati) != *(obj._sati))
            return *(this->_sati) < *(obj._sati);
        else if (*(this->_minuti) != *(obj._minuti))
            return *(this->_minuti) < *(obj._minuti);
        return false;
            
    }
};

class Predmet {
    char* _naziv;
    int _ocjena;
    string _napomena;
public:
    Predmet(const char* naziv = "", int ocjena = 0, string napomena = "") {
        _naziv = Alociraj(naziv);
        _ocjena = ocjena;
        _napomena = napomena;
    }
    Predmet(const Predmet& org):
        _naziv(new char[(strlen(org._naziv)+1)]),
        _ocjena(org._ocjena),
        _napomena(org._napomena)
    {
        strcpy_s(_naziv, strlen(org._naziv) + 1, org._naziv);
    }
    ~Predmet() {
        delete[] _naziv; _naziv = nullptr;
    }
    Predmet& operator=(const Predmet& obj)
    {
        if (this != &obj)
        {
            delete[]_naziv;
            _naziv = new char[strlen(obj._naziv) + 1];
            strcpy_s(_naziv, strlen(obj._naziv) + 1, obj._naziv);
            _napomena = obj._napomena;
            _ocjena = obj._ocjena;
        }
        return *this;
    }
    friend ostream& operator<< (ostream& COUT, Predmet& obj) {
        COUT << obj._naziv << " (" << obj._ocjena << ") " << obj._napomena << endl;
        return COUT;
    }
    string GetNapomena() { return _napomena; }
    char* GetNaziv() { return _naziv; }
    int GetOcjena() { return _ocjena; }

    void DodajNapomenu(string napomena) {
        _napomena += " " + napomena;
    }
    bool operator==(const Predmet& obj) const
    {
        return strcmp(this->_naziv, obj._naziv) == 0;
    }
};
class Uspjeh {
    GodinaStudija* _godina;
    //datumvrijeme se odnosi na vrijeme evidentiranja polozenog predmeta
    Dictionary<Predmet, DatumVrijeme> _predmeti;    
public:
    Uspjeh(GodinaStudija godina) {
        _godina = new GodinaStudija(godina);
    }
    Uspjeh(const Uspjeh& org):
        _godina(new GodinaStudija(*org._godina)),
        _predmeti(org._predmeti)
    {   }
    ~Uspjeh() { delete _godina; _godina = nullptr; }
    Uspjeh& operator=(const Uspjeh& obj)
    {
        if (this != &obj)
        {
            delete _godina;
            _godina = new GodinaStudija(*obj._godina);
            _predmeti = obj._predmeti;
        }
        return *this;
    }
    Dictionary<Predmet, DatumVrijeme>* GetPredmeti() { return &_predmeti; }
    GodinaStudija* GetGodinaStudija() { return _godina; }

    friend ostream& operator<< (ostream& COUT, const Uspjeh& obj) {
        COUT << *obj._godina << " " << obj._predmeti << endl;
        return COUT;
    }
};

class Student {
    char* _imePrezime;
    string _emailAdresa;
    string _brojTelefona;
    vector<Uspjeh> _uspjeh;
    void setEmail(string email)
    {
        /*
    email adresa mora biti u formatu: text-text@ nakon cega slijedi neka od sljedecih domena: hotmail.com ili
    //outlook.com ili fit.ba. Pod text se podrazumijeva bilo koje slovo, malo ili veliko.
    u slucaju da email adresa nije validna, postaviti je na defaultnu: notSet@fit.ba
    za provjeru koristiti regex
    */
        string uslov1 = "[a-z]+\.[a-z]+@(hotmail.com|outlook.com|fit.ba)";
        regex rgx(uslov1);
        if (regex_match(email, rgx))
        {
            _emailAdresa = email;
            return;
        }
        _emailAdresa = "noSet@fit.ba";
    }
    void slanjeSMS(const GodinaStudija& god, double prosjek)
    {
        m.lock();
        cout << "Cestitamo na uspjeshu na akademskoj godini : " << god << " s prosjekom: " << prosjek << endl;
        m.unlock();
    }
    void slanjeEmail(const GodinaStudija& god, double prosjek)
    {
        m.lock();
        cout << "FROM: info@fit.ba" << endl
            << "TO: " << _emailAdresa << endl
            << "Postovani " << _imePrezime << " evidentirali ste uspjeh  za " << god << " godinu studija." << endl
            << "Pozdrav," << endl
            << "FIT team" << endl;
        m.unlock();
        if (prosjek > 8.0)
        {
            thread sms(&Student::slanjeSMS, this, god, prosjek);
            sms.join();
        }
    }
    void evidentirajUspjeh(Uspjeh uspjeh)
    {
        m.lock();
        double suma = 0;
        for (int i = 0; i < uspjeh.GetPredmeti()->getTrenutno(); i++)
        {
            suma += uspjeh.GetPredmeti()->getElement1(i).GetOcjena();
        }
        suma /= uspjeh.GetPredmeti()->getTrenutno();
        m.unlock();

        thread email(&Student::slanjeEmail, this, *uspjeh.GetGodinaStudija(), suma);
        email.join();
    }
public:
    Student(const char* imePrezime, string emailAdresa, string brojTelefona) {
        _imePrezime = Alociraj(imePrezime);
        setEmail(emailAdresa);
        _brojTelefona = brojTelefona;
    }
    ~Student() {
        delete[] _imePrezime; _imePrezime = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, Student& obj) {
        COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
        return COUT;
    }

    vector<Uspjeh>* GetUspjeh() { return &_uspjeh; }
    string GetEmail() { return _emailAdresa; }
    string GetBrojTelefona() { return _brojTelefona; }
    char* GetImePrezime() { return _imePrezime; }
    bool AddPredmet(const Predmet& predmet,const GodinaStudija& godinaStudija,const DatumVrijeme& datumVrijme)
    {
        /*
    uspjeh se dodaje za svaki predmet na nivou godine studija.
    tom prilikom onemoguciti:
    - dodavanje istoimenih predmeta na nivou jedne godine,
    - dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jedne godine, 
    razmak izmedju dodavanja pojedinih predmeta mora biti najmanje 1 sat).
    godine (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom 
    (npr. prvo se moze dodati uspjeh za drugu godinu, pa onda za prvu godinu i sl.).
    Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    */
        vector<Uspjeh>::iterator it;
        it = _uspjeh.begin();
        for (it; it != _uspjeh.end(); it++)
        {
            if (*it->GetGodinaStudija() == godinaStudija)
            {
                for (size_t i = 0; i < it->GetPredmeti()->getTrenutno(); i++)
                {
                    if (it->GetPredmeti()->getElement1(i) == predmet)
                    {
                        return false;
                    }
                }
                if (!(ProsaoMinSat(datumVrijme,( it->GetPredmeti()->getElement2(it->GetPredmeti()->getTrenutno() - 1)))))
                {
                    return false;
                }
                it->GetPredmeti()->AddElement(predmet, datumVrijme);
                return true;
            }
        }
        _uspjeh.emplace_back(godinaStudija);
        _uspjeh.at(_uspjeh.size() - 1).GetPredmeti()->AddElement(predmet, datumVrijme);
        thread t(&Student::evidentirajUspjeh, this, _uspjeh.at(_uspjeh.size() - 1));
        t.join();
        return true;
    }
    int BrojPonavljanjaRijeci(string rijec)
    {
        regex rgx(rijec);
        int suma = 0;
        vector<Uspjeh>::iterator it;
        it = _uspjeh.begin();
        for (it; it != _uspjeh.end(); it++)
        {
            for (int i = 0; i < it->GetPredmeti()->getTrenutno(); i++)
            {
                string temp = it->GetPredmeti()->getElement1(i).GetNapomena();
                auto pocetak = sregex_iterator
                {
                    temp.begin(),
                    temp.end(),
                    rgx
                };
                auto kraj = sregex_iterator();
                suma += distance(pocetak, kraj);
            }
        }
        return suma;
    }
    vector<Predmet> operator()( DatumVrijeme* d1,  DatumVrijeme* d2)
    {
        vector<Predmet> temp;
        vector<Uspjeh>::iterator it = _uspjeh.begin();
        for (it; it < _uspjeh.end(); it++)
        {
            for (int i = 0; i < it->GetPredmeti()->getTrenutno(); i++)
            {
                if (*(d1) < it->GetPredmeti()->getElement2(i) && it->GetPredmeti()->getElement2(i) < *(d2))
                    temp.emplace_back(it->GetPredmeti()->getElement1(i));
            }
        }
        return temp;
    }
    Uspjeh* operator[](const char* godina)
    {
        GodinaStudija god;
        if (strcmp(godina, "PRVA") == 0)
            god = PRVA;
        else if (strcmp(godina, "DRUGA") == 0)
            god = DRUGA;
        else if (strcmp(godina, "TRECA") == 0)
            god = TRECA;
        else
            return NULL;


        Uspjeh* uspjeh = NULL;
        vector<Uspjeh>::iterator it = _uspjeh.begin();
        for (it; it != _uspjeh.end(); it++)
        {
            if (*it->GetGodinaStudija() == god)
            {
                uspjeh = new Uspjeh(god);
                *uspjeh = *it;
            }
        }
        return uspjeh;

    }
};

void main() {
    /****************************************************************************
    1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
    2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
    3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
    4. PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT
    5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUÈAJU
    DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
    6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
    ****************************************************************************/
    cout << "NA KRAJU ISPITA, RAD PREDAJTE U ODGOVARAJUCI FOLDER NA FTP SERVERU (INTEGRALNI)!" << endl;
    DatumVrijeme temp,
        datum19062019_1015(19, 6, 2019, 10, 15),
        datum20062019_1115(20, 6, 2019, 11, 15),
        datum30062019_1215(30, 6, 2019, 12, 15),
        datum05072019_1231(5, 7, 2019, 12, 31);
    //funkcija ToCharArray vraca datum i vrijeme kao char *. konverziju izvrsiti koristeci stringstream objekat.
    //voditi racuna o tome da se izmedju datuma i vremena nalazi samo jedan razmak, te da su vrijednosti dana i mjeseca 
    //iskazane kao dvije cifre
    cout << datum19062019_1015.ToCharArray() << endl;//treba ispisati: 19/06/2019 10:15
    temp = datum05072019_1231;
    cout << temp.ToCharArray() << endl;//treba ispisati: 05/07/2019 12:31

    const int DictionaryTestSize = 9;
    Dictionary<int, int> Dictionary1;
    for (size_t i = 0; i < DictionaryTestSize; i++)
        Dictionary1.AddElement(i + 1, i * i);
    cout << Dictionary1 << endl;

    try {
        //vraca elemente kolekcije koji se nalaze na lokacijama definisanim vrijednostima parametara (npr. 2 - 7). 
        //funkcija baca izuzetak u slucaju da se zahtijeva lokacija koja ne postoji ili je vrijednost posljednje lokacije manja od pocetne
        Dictionary<int, int> opseg = Dictionary1.getRange(2, 7);
        cout << opseg << endl;
        Dictionary1.getRange(7, 11);
    }
    catch (exception& err) {
        cout << err.what() << endl;
    }
    cout << Dictionary1 << endl;

    Dictionary<int, int> Dictionary2 = Dictionary1;
    cout << Dictionary2 << crt;

    Dictionary<int, int> Dictionary3;
    Dictionary3 = Dictionary1;
    cout << Dictionary3 << crt;

    //napomena se moze dodati i prilikom kreiranja objekta
    Predmet MAT("Matematika", 7, "Ucesce na takmicenju"),
        UIT("Uvod u informacijske tehnologije", 9),
        RM("Racunarske mreze", 8),
        EN("Engleski jezik", 6);
    UIT.DodajNapomenu("Pohvala za ostvareni uspjeh");
    cout << MAT << endl;

    /*
    email adresa mora biti u formatu: text-text@ nakon cega slijedi neka od sljedecih domena: hotmail.com ili
    //outlook.com ili fit.ba. Pod text se podrazumijeva bilo koje slovo, malo ili veliko.
    u slucaju da email adresa nije validna, postaviti je na defaultnu: notSet@fit.ba
    za provjeru koristiti regex
    */
    Student jasmin("Jasmin Azemovic", "jasmin.azemovic@hotmail.com", "033 281 172");
    Student adel("Adel Handzic", "adel.handzic@fit.ba", "033 281 170");
    Student emailNotValid("Ime Prezime", "korisnik@lazna.ba", "033 281 170");
    cout << jasmin << endl;
    cout << adel << endl;
    cout << emailNotValid << endl;

    /*
    uspjeh se dodaje za svaki predmet na nivou godine studija.
    tom prilikom onemoguciti:
    - dodavanje istoimenih predmeta na nivou jedne godine,
    - dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jedne godine, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje 1 sat).
    godine (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za drugu godinu, pa onda za prvu godinu i sl.).
    Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    */
    if (jasmin.AddPredmet(UIT, DRUGA, datum20062019_1115))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(RM, DRUGA, datum30062019_1215))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(EN, PRVA, datum19062019_1015))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(MAT, PRVA, datum20062019_1115))
        cout << "Predmet uspjesno dodan!" << crt;
    //ne treba dodati MAT jer je vec dodana u prvoj godini
    if (jasmin.AddPredmet(MAT, PRVA, datum05072019_1231))
        cout << "Predmet uspjesno dodan!" << crt;
    //ne treba dodati UIT jer nije prosao 1 sat od dodavanja posljednjeg predmeta
    if (jasmin.AddPredmet(UIT, PRVA, datum20062019_1115))
        cout << "Predmet uspjesno dodan!" << crt;
    /*nakon evidentiranja uspjeha na bilo kojem predmetu tj. prilikom uspjesno izvrsene funkcije AddPredmet, Studentu se salje email sa sadrzajem:
    FROM:info@fit.ba
    TO: emailStudenta
    Postovani ime i prezime, evidentirali ste uspjeh za X godinu studija.
    Pozdrav.
    FIT Team.

    ukoliko je, nakon dodavanja predmeta, prosjek na nivou te godine veci od 8.0 Studentu se, pored email-a, salje i SMS sa sadrzajem: "Svaka cast za uspjeh X.X       ostvaren u X godini studija".
    slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
    */
    cout << "USPJEH ISPISATI KORISTEÆI OSTREAM_ITERATOR" << endl;
    cout << jasmin << endl;
    //vraca broj ponavljanja odredjene rijeci u napomenama, koristiti sregex_iterator
    cout << "Rijec takmicenje se pojavljuje " << jasmin.BrojPonavljanjaRijeci("takmicenju") << " puta." << endl;

    //vraca niz predmeta koji su evidentiranih u periodu izmedju vrijednosti proslijedjenih parametara
    vector<Predmet> jasminUspjeh = jasmin(new DatumVrijeme(18, 06, 2019, 10, 15), new DatumVrijeme(21, 06, 2019, 10, 10));
    for (Predmet u : jasminUspjeh)
        cout << u << endl;

    Uspjeh* uspjeh_I_godina = jasmin["PRVA"];//vraca uspjeh Studenta ostvaren u prvoj godini studija
    if (uspjeh_I_godina != nullptr)
        cout << *uspjeh_I_godina << endl;

    cin.get();
    system("pause>0");
}