#include <iostream>
#include<vector>
#include<iterator>
#include<regex>
using namespace std;

const char* crt = "\n---------------------------------------\n";

class DatumVrijeme
{
    int* _dan, * _mjesec, * _godina, * _sati, * _minuti;
public:
    DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0)
    {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
        _sati = new int(sati);
        _minuti = new int(minuti);
    }
    DatumVrijeme(const DatumVrijeme& org):
        _dan(new int(*org._dan)),_mjesec(new int(*org._mjesec)),_godina(new int(*org._godina)),
        _sati(new int(*org._sati)),_minuti(new int(*org._minuti))
    {   }
    ~DatumVrijeme()
    {
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
            _dan = new int(*obj._dan);
            _mjesec = new int(*obj._mjesec);
            _godina = new int(*obj._godina);
            _sati = new int(*obj._sati);
            _minuti = new int(*obj._minuti);
        }
        return *this;
    }
    friend ostream& operator<< (ostream& COUT, DatumVrijeme& obj)
    {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti;
        return COUT;
    }
};

template<class T1, class T2 = int>
class FITKolekcija
{
    T1* _elementi1;
    T2* _elementi2;
    int _trenutno;
public:
    FITKolekcija()
    {
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _trenutno = 0;
    }
    FITKolekcija(const FITKolekcija<T1,T2>& org):
        _elementi1(new T1(org._trenutno)), _elementi2(new T2(org._trenutno)),_trenutno(org._trenutno)
    {
        for (size_t i = 0; i < org.getTrenutno(); i++)
        {
            _elementi1[i] = org._elementi1[i];
            _elementi2[i] = org._elementi2[i];
        }
    }
    int getTrenutno()const { return _trenutno; }
    ~FITKolekcija()
    {
        delete[] _elementi1; _elementi1 = nullptr;
        delete[] _elementi2; _elementi2 = nullptr;
    }
    FITKolekcija& operator=(const FITKolekcija<T1, T2>& obj)
    {
        if (this != &obj)
        {
            delete[] _elementi1; _elementi1 = nullptr;
            delete[] _elementi2; _elementi2 = nullptr;
            _elementi1 = new T1[obj._trenutno];
            _elementi2 = new T2[obj._trenutno];
            for (int i = 0; i < obj.getTrenutno(); i++)
            {
                _elementi1[i] = obj._elementi1[i];
                _elementi2[i] = obj._elementi2[i];
            }
            _trenutno = obj._trenutno;
        }
        return *this;
    }
    T1* GetT1() { return _elementi1; }
    T2* GetT2() { return _elementi2; }
    int GetTrenutno() { return _trenutno; }
    void povecajTrenutno() { _trenutno++; }
    void Dodaj(T1 elem1, T2 elem2)
    {
        T1* temp1 = new T1[GetTrenutno() + 1];
        T2* temp2 = new T2[getTrenutno() + 1];
        for (size_t i = 0; i < getTrenutno(); i++)
        {
            temp1[i] = _elementi1[i];
            temp2[i] = _elementi2[i];
        }
        temp1[getTrenutno()] = elem1;
        temp2[getTrenutno()] = elem2;
        povecajTrenutno();
        delete[]_elementi1;
        delete[]_elementi2;
        _elementi1 = temp1;
        _elementi2 = temp2;
        temp1 = NULL;
        temp2 = NULL;
    }
    void Sortiraj()
    {
        int ok;
        for (int i = getTrenutno() - 1; i > 0; i--)
        {
            ok = 1;
            for (size_t j = 0; j < i; j++)
            {
                if (_elementi1[j + 1] < _elementi1[j])
                {
                    T1 temp1;
                    T2 temp2;
                    temp1 = _elementi1[j];
                    temp2 = _elementi2[j];

                    _elementi1[j] = _elementi1[j + 1];
                    _elementi2[j] = _elementi2[j + 1];

                    _elementi1[j + 1] = temp1;
                    _elementi2[j + 1] = temp2;
                    ok = 0;
                }
            }
            if (ok) break;
        }
    }
    void DodajNaLokaciju(T1 elem1, T2 elem2, int lokacija)
    {
        if (lokacija <= 0)
        {
            
            _elementi1[0] = elem1;
            _elementi2[0] = elem2;
        }
        else
        {
            
            _elementi1[lokacija - 1] = elem1;
            _elementi2[lokacija - 1] = elem2;
        }
        
    }
    FITKolekcija* GetJedinstveni()
    {
        FITKolekcija* novaKolekcija = new FITKolekcija<T1, T2>();
        int brojac = 0;
        novaKolekcija->Dodaj(_elementi1[brojac],_elementi2[brojac]);
        brojac++;
        int br = 0;
        for (int i = 1; i < getTrenutno(); i++)
        {
            br = 0;
            for (size_t j = 0; j < i; j++)
            {
                if (novaKolekcija->_elementi1[j] == _elementi1[i] &&
                     novaKolekcija->_elementi2[j] == _elementi2[i])
                    br++;
            }
            if (br==0)
            {
                novaKolekcija->Dodaj(_elementi1[i], _elementi2[i]);
                brojac++;
            }
            
        }
        novaKolekcija->_trenutno = brojac;
        return novaKolekcija;
    }

    friend ostream& operator<< (ostream& COUT, FITKolekcija& obj)
    {
        for (size_t i = 0; i < obj._trenutno; i++)
            COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
        return COUT;
    }
};

enum vrstaDokumenta { PDF, DOC, TXT, HTML };

class Dokument
{
    vrstaDokumenta _vrsta;
    string _naziv;
    string _sadrzaj;
    int _brojStranica;
    int _brojacRijeci;
public:
    Dokument() { _brojStranica = 0; _brojacRijeci = 0; }
    Dokument(vrstaDokumenta vrsta, string naziv)
    {
        _naziv = naziv; _vrsta = vrsta; _brojStranica = 0;
    }
    string GetNaziv() { return _naziv; };
    string GetSadrzaj() { return _sadrzaj; };
    vrstaDokumenta GetVrsta() { return _vrsta; };
    void UvecajBrojStranica() { _brojStranica++; }
    int GetBrojStranica() { return _brojStranica; }
    void DodajSadrzaj(const char* sadrzaj)
    {
        for (int i = 0; i < strlen(sadrzaj); i++)
        {
            _brojacRijeci++;
            if (_brojacRijeci % 30 == 0)
            {
                UvecajBrojStranica();
            }
        }
        _sadrzaj += sadrzaj;
    }
    friend ostream& operator<<(ostream& COUT, Dokument& obj)
    {
        COUT << obj._naziv << "." << obj._vrsta << " (" << obj._brojStranica << ")\n" << obj._sadrzaj << endl;
        return COUT;
    }
};

class Uredjaj
{
protected:
    char* _proizvodjac;
    char* _model;
public:
    Uredjaj(const char* proizvodjac, const char* model)
    {
        int vel = strlen(proizvodjac) + 1;
        _proizvodjac = new char[vel];
        strcpy_s(_proizvodjac, vel, proizvodjac);
        vel = strlen(model) + 1;
        _model = new char[vel];
        strcpy_s(_model, vel, model);
    }
    ~Uredjaj()
    {
        delete[] _proizvodjac; _proizvodjac = nullptr;
        delete[] _model; _model = nullptr;
    }
    friend ostream& operator<<(ostream& COUT, Uredjaj& obj)
    {
        COUT << obj._proizvodjac << "." << obj._model << endl;
        return COUT;
    }
};

class Printer:public Uredjaj
{
    FITKolekcija<DatumVrijeme, Dokument> _printaniDokumenti;
    vector<string> _zabranjeneRijeci;
public:
    Printer(const char* proizvodac, const char* model) :Uredjaj(proizvodac, model)
    {

    }
    FITKolekcija<DatumVrijeme, Dokument>& GetPrintaniDokumenti() { return _printaniDokumenti; };
    vector<string>& GetZabranjeneRijeci() { return _zabranjeneRijeci; };
    void DodajZabranjenuRijec(string rijec)
    {
        vector<string>::iterator itr;
        itr = _zabranjeneRijeci.begin();
        int br = 0;
        for (itr; itr != _zabranjeneRijeci.end(); itr++)
        {
            string temp = *itr;
            if (temp == rijec)
                br++;
        }
        if (br == 0)
            _zabranjeneRijeci.push_back(rijec);
    }
    void Printaj(DatumVrijeme dt,Dokument dk)
    {
        // DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDEÆA PRAVILA:
        // 1. NAZIV DOKUMENTA MORA BITI U SLJEDEÆEM FORMATU npr: ispitPRIII.doc
        // NAZIV DOKUMENTA MOŽE SADRŽAVATI SAMO SLOVA (NAJMANJE 5, NAJVIŠE 15), A EKSTENZIJA MOŽE 
        // BITI SAMO NEKA OD DOZVOLJENIH VRSTOMDOKUMENTA
        // 2. SADRŽAJ DOKUMENTA U SEBI NE SMIJE POSJEDOVATI NITI JEDNU ZABRANJENU RIJEÈ
        // UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA TREBA DA BACI EXCEPTION SA 
        // ODGOVARAJUÆOM PORUKOM, ALI ÆE DOKUMENT BITI POHRANJEN U KOLEKCIJU _printaniDokumenti. 
        // UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI 
        // SADRŽAJ DOKUMENTA KOJI SE PRINTA
        bool uvijet = false;
        _printaniDokumenti.Dodaj(dt, dk);
        string uvijer = "[a-zA-z]{5,15}(.doc|.txt)";
        regex rgx(uvijer);
        if (regex_match(dk.GetNaziv(), rgx))
            uvijet = true;
        else
            throw exception("Dokument ima neodgovarajuce ime");
            
        vector<string>::iterator itr = _zabranjeneRijeci.begin();

        for (itr; itr != _zabranjeneRijeci.end(); itr++)
        {
            string temp = *itr;
            size_t found = dk.GetSadrzaj().find(temp);
            if (found != std::string::npos)// kako ovo radi ja odoh spavat :D
            {
                string err = "U dokumentu se nalaze nedozvoljena rijeci, a to je \"" + temp+ "\"";
                throw exception(err.c_str());
            }
               
            else
                uvijet = true;
        }
        if (uvijet)
        {
            cout << "Pritanje uspjesno" << endl;
            cout << dk.GetSadrzaj() << endl;
        }
        

    }
    void UkloniDokumente()
    {

    }
    string GetTopZabranjenuRijec()
    {

    }
    void GetProsjecanBrojStranicaPoDatumu(DatumVrijeme dt)
    {

    }
};

void main()
{
    /****************************************************************************
    // 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
    // 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR
    //    ÆE BITI OZNACENO KAO "RE"
    // 3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
    // 4. PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT
    // 5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U,
    //    OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE
    //    IMENOVATI I DODAVATI PO ŽELJI.
    // 6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
    ****************************************************************************/
#pragma region TestiranjeDatumVrijeme
    DatumVrijeme danas(1, 2, 2017, 10, 15);
    DatumVrijeme sutra(danas);
    DatumVrijeme prekosutra;
    prekosutra = danas;
    cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
#pragma region FITKolekcija
    int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

    FITKolekcija<int, int> brojevi;
    brojevi.Dodaj(v196, v6);
    brojevi.Dodaj(v13, v32);
    brojevi.Dodaj(v98, v196);
    brojevi.Dodaj(v63, v13);
    brojevi.Dodaj(v98, v196);
    brojevi.Dodaj(v196, v6);

    cout << brojevi << crt;
    //SORTIRANJE ÈLANOVA KOLEKCIJE SE VRŠI U RASTUÆEM REDOSLIJEDU NA OSNOVU VRIJEDNOSTI ELEMENTA TIPA T1
    brojevi.Sortiraj();
    cout << brojevi << crt;
    //BROJ 2 SE ODNOSI NA LOKACIJU/INDEKS UNUTAR LISTE NA KOJI JE POTREBNO DODATI NOVE ELEMENTE
    brojevi.DodajNaLokaciju(v109, v6, 2);
    cout << brojevi << crt;
    brojevi.Sortiraj();
    cout << brojevi << crt;
    // METODA GetJedinstveni VRAÆA LISTU JEDINSTVENIH ELEMENATA TJ. ELEMENATA KOJI NE SADRŽE DUPLIKATE 
    // (POJAM DUPLIKAT SE ODNOSI NA ISTE VRIJEDNOSTI ELEMENATA T1 I T2). ELEMENTI KOJI SE DUPLIRAJU SE U 
    // OVOJ LISTI TREBAJU POJAVITI SAMO JEDNOM.
    FITKolekcija<int, int>* jedinstveni = brojevi.GetJedinstveni();
    cout << *jedinstveni << crt;
    *jedinstveni = brojevi;
    cout << *jedinstveni << crt;

    Dokument ispitPRIII(DOC, "ispitPRIII.doc");
    //BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREÐUJE PRILIKOM DODAVANJA SADRŽAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO
    //DA NA JEDNU STRANICU MOŽE STATI 30 ZNAKOVA UKLJUÈUJUÆI I RAZMAKE
    ispitPRIII.DodajSadrzaj("NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO RE");
    cout << "Broj stranica -> " << ispitPRIII.GetBrojStranica() << endl;
    Dokument ispitBaze(DOC, "ispitBaze.doc");
    ispitBaze.DodajSadrzaj("PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT");
    cout << ispitBaze << endl;//ISPISUJE SVE DOSTUPNE PODATKE O DOKUMENTU
//
    Printer hp3200("HP", "3200");
    //PRINTER NEÆE DOZVOLITI PRINTANJE DOKUMENATA U ÈIJEM SADRŽAJU SE NALAZI NEKA OD ZABRANJENIH RIJEÈI
    hp3200.DodajZabranjenuRijec("RE");// :)
    hp3200.DodajZabranjenuRijec("RAT");
    hp3200.DodajZabranjenuRijec("UBITI");
    hp3200.DodajZabranjenuRijec("RE");// ONEMOGUÆITI PONAVLJANJE ZABRANJENIH RIJEÈI, KORISTITI ITERATORE
    hp3200.DodajZabranjenuRijec("MRZITI");
    try
    {
        // DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDEÆA PRAVILA:
        // 1. NAZIV DOKUMENTA MORA BITI U SLJEDEÆEM FORMATU npr: ispitPRIII.doc
        // NAZIV DOKUMENTA MOŽE SADRŽAVATI SAMO SLOVA (NAJMANJE 5, NAJVIŠE 15), A EKSTENZIJA MOŽE 
        // BITI SAMO NEKA OD DOZVOLJENIH VRSTOMDOKUMENTA
        // 2. SADRŽAJ DOKUMENTA U SEBI NE SMIJE POSJEDOVATI NITI JEDNU ZABRANJENU RIJEÈ
        // UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA TREBA DA BACI EXCEPTION SA 
        // ODGOVARAJUÆOM PORUKOM, ALI ÆE DOKUMENT BITI POHRANJEN U KOLEKCIJU _printaniDokumenti. 
        // UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI 
        // SADRŽAJ DOKUMENTA KOJI SE PRINTA

        hp3200.Printaj(danas, ispitPRIII);
        hp3200.Printaj(danas, ispitBaze);


        // IMPLEMENTIRATI PRINTANJE U ZASEBNIM NITIMA UZ KORIŠTENJE sleep_for FUNKCIJE
        // VODITI RAÈUNA O DIJELJENJU ZAJEDNIÈKIH RESURSA
    }
    catch (exception& err)
    {
        cout << err.what() << endl;
    }

    /*KOD POREÐENJA DATUMA PRINTANJA NIJE POTREBNO POREDITI MINUTE, SAMO DATUM I SATE*/
    cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp3200.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
    cout << "Najcesce koristena zabranjena rijec je -> " << hp3200.GetTopZabranjenuRijec() << crt;

    Printer hp4000(hp3200);
    cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp4000.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
    cout << "Najcesce koristena zabranjena rijec je -> " << hp4000.GetTopZabranjenuRijec() << crt;
    hp4000.UkloniDokumente();//UKLANJA SVE DOKUMENTE KOJI NISU ZADOVOLJILI USLOVE ZA PRINTANJE
    cout << hp4000 << crt;

#pragma endregion
    delete jedinstveni;
    jedinstveni = NULL;
}