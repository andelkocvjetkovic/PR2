#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include<sstream>
#include<chrono>
#include<thread>
/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
****************************************************************************/

using namespace std;

enum Oblast { SoftverskiInzinjering, KomunikacijskiSistemi, SigurnostInformacijskihSistema, InteligentniSistemi };
const char* oblast_txt[] = { "Softverski inzinjering", "Komunikacijski sistemi", "Sigurnost informacijskih sistema", "Inteligentni sistemi" };
const char crt[] = "\n-------------------------------------------------------------------------\n";
template<class T1, class T2, int max>
class Kolekcija
{
    T1 _elementi1[max];
    T2 _elementi2[max];
    int _trenutno;
public:
    Kolekcija()
    {
        _trenutno = 0;
    }

    void AddElement(const T1& elem1, const T2& elem2)
    {
        if (_trenutno == max)
            throw exception("Dostigli ste maksimalan broj elemenata u kolekciji!");

        /*for (int i = 0; i < _trenutno; i++)
        {
            if (_elementi1[i] == elem1 || _elementi2[i] == elem2)
                throw exception("Nije moguce dodati duple elemente u kolekciju!");
        }*/
        _elementi1[_trenutno] = elem1;
        _elementi2[_trenutno++] = elem2;
    }

    T1* getElementi1Pok() { return _elementi1; }
    T2* getElementi2Pok() { return _elementi2; }
    T1& getElement1(int lokacija)  { return _elementi1[lokacija]; }
    T2& getElement2(int lokacija)  { return _elementi2[lokacija]; }
    int getTrenutno()const { return _trenutno; }

    friend ostream& operator<<(ostream& COUT, Kolekcija<T1, T2, max>& obj)
    {
        for (size_t i = 0; i < obj._trenutno; i++)
        {
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        }
        return COUT;
    }
};

//Deklaracija klase Nastavnik omogucava njeno koristenje u klasi ZavrsniRad, a definicija je data naknadno 
class Nastavnik;

class ZavrsniRad
{
    char* _nazivTeme;
    Oblast* _oblastTeme;
    string _datumPrijave; //Format: dd.MM.gggg
    //U vector pohraniti samo adresu objekta tipa Nastavnik, dakle bez alokacije nove memorije
    vector<Nastavnik*> _komisija;
    string _datumOdbrane; //Format: dd.MM.gggg (najranije 2 mjeseca od datuma prijave)
    int _ocjena;
public:
    ZavrsniRad() :_nazivTeme(nullptr), _oblastTeme(nullptr), _datumPrijave("NEMA VRIJEDNOST"), _datumOdbrane("NEMA VRIJEDNOST"), _ocjena(5)
    { }

    ZavrsniRad(string nazivTeme, Oblast oblastTeme, string datumPrijave) : _datumPrijave(datumPrijave), _oblastTeme(new Oblast(oblastTeme))
    {
        _nazivTeme = new char[nazivTeme.size() + 1];
        strcpy_s(_nazivTeme, nazivTeme.size() + 1, nazivTeme.c_str());
    }
    ZavrsniRad(const ZavrsniRad& org) :
        _nazivTeme(new char[strlen(org._nazivTeme) + 1]),
        _oblastTeme(new Oblast(*org._oblastTeme)),
        _datumPrijave(org._datumPrijave),
        _datumOdbrane(org._datumOdbrane),
        _ocjena(org._ocjena)
    {
        _komisija = org._komisija;
    }
    ~ZavrsniRad()
    {
        delete[] _nazivTeme; _nazivTeme = nullptr;
        delete _oblastTeme;	_oblastTeme = nullptr;
    }
    ZavrsniRad& operator=(const ZavrsniRad& obj)
    {
        if (this != &obj)
        {
            delete[]_nazivTeme;
            if (obj._nazivTeme != NULL)
            {
                _nazivTeme = new char[strlen(obj._nazivTeme) + 1];
                strcpy_s(_nazivTeme, strlen(obj._nazivTeme) + 1, obj._nazivTeme);
            }
            else
            {
                _nazivTeme = NULL;
            }
            delete _oblastTeme;
            if (obj._oblastTeme != NULL)
            {
                _oblastTeme = new Oblast(*obj._oblastTeme);
            }
            else
            {
                _oblastTeme = NULL;
            }
            _datumPrijave = obj._datumPrijave;
            _datumOdbrane = obj._datumOdbrane;
            _ocjena = obj._ocjena;
            _komisija = obj._komisija;
        }
        return *this;
    }
    char* GetNazivTeme()const { return _nazivTeme; }
    Oblast GetOblastTeme()const { return *_oblastTeme; }
    string GetDatumOdbrane() { return _datumOdbrane; }
    string GetDatumPrijave() { return _datumPrijave; }
    void setDatumObrane(string datumObrane) { _datumOdbrane = datumObrane; }
    void setOcjena(int ocijena) { _ocjena = ocijena; }
    int GetOcjena() { return _ocjena; }
    vector<Nastavnik*>& GetKomisija() { return _komisija; };

    friend ostream& operator<<(ostream& COUT, ZavrsniRad& r)
    {
        COUT << "Tema rada: " << r._nazivTeme << endl;
        COUT << "Oblast teme: " << oblast_txt[*r._oblastTeme] << endl;
        COUT << "Datum prijave rada: " << r._datumPrijave << endl;
        //Podatke o nastavnicima nije moguce ispisati ovdje jer klasa jos nije definisana
        return COUT;
    }
};

class Nastavnik
{
    friend ostream& operator<<(ostream& COUT, Nastavnik& obj);
private:
    string _imePrezime;
    Oblast _oblastIzboraUZvanje;
    //Parametar string predstavlja broj indeksa studenta koji prijavljuje zavrsni rad kod odredjenog nastavnika
    Kolekcija<string, ZavrsniRad, 10> _teme;
    bool provjeriDatum(string datumPrijave, string datumObrane)
    {
        stringstream obj;
        int temp, datumPrijeveDani = 0, datumObraneDani = 0;
        //Format: dd.MM.gggg
        //datumPrijave
        //dani
        obj.str(datumPrijave.substr(0, 2));
        obj >> temp;
        datumPrijeveDani += temp;
        //mjesec
        obj.clear();
        obj.str(datumPrijave.substr(3, 2));
        obj >> temp;
        datumPrijeveDani += temp*30;
        obj.clear();
        //godina
        obj.str(datumPrijave.substr(6, 4));
        obj >> temp;
        datumPrijeveDani += temp * 365;
        obj.clear();



        //datum obarne
        //dani
        obj.str(datumObrane.substr(0, 2));
        obj >> temp;
        datumObraneDani += temp;
        obj.clear();
        //mjesec
        obj.str(datumObrane.substr(3, 2));
        obj >> temp;
        datumObraneDani += temp * 30;
        obj.clear();
        //godina
        obj.str(datumObrane.substr(6, 4));
        obj >> temp;
        datumObraneDani += temp * 365;
        obj.clear();

        if ((datumObraneDani - datumPrijeveDani) > 60)
            return true;
        return false;

    }

public:
    Nastavnik(string imePrezime, Oblast oblastIzboraUZvanje) : _imePrezime(imePrezime), _oblastIzboraUZvanje(oblastIzboraUZvanje)
    {}

    string GetImePrezime() { return _imePrezime; }
    Oblast GetOblast() { return _oblastIzboraUZvanje; }
    Kolekcija<string, ZavrsniRad, 10>& GetTeme() { return _teme; };
    bool DodajZavrsniRad(string brIndexa, string tema, Oblast oblastRada, string datum)
    {
        if (_oblastIzboraUZvanje != oblastRada)
            throw exception("Nastavnik ne posjeduje to zvanje");
        _teme.AddElement(brIndexa, ZavrsniRad(tema, oblastRada, datum));
    }
    void ZakaziOdbranuRada(string brojIndeksa, string datumObrane, vector<Nastavnik*> komisija)
    {
        // Funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu 
        // zavrsnog rada sto podrazumijeva definisanje datuma odbrane i liste clanova komisije pred kojima ce 
        // student braniti zavrsni rad. Odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio. 
        // Komisiju trebaju ciniti najmanje 2 nastavnika koji imaju izbor u zvanje u oblasti kojoj pripada tema rada. 
        // Datum odbrane ne smije biti manji od datuma prijave. U slucaju da bilo koji od navedenih uslova nije ispunjen
        //  funkcija treba	da baci izuzetak
        int brojClanovaKomisije = 0;
        vector<Nastavnik*>::iterator it = komisija.begin();
        for (it; it < komisija.end(); it++)
        {
            if ((*it)->GetOblast() == _oblastIzboraUZvanje)
            {
                brojClanovaKomisije++;
            }
        }
        for (int i = 0; i < _teme.getTrenutno(); i++)
        {
            if (brojIndeksa == _teme.getElement1(i))
            {
                if (_teme.getElement2(i).GetDatumPrijave() == "NEMA VRIJEDNOSTI")
                    throw exception("Student nije prijavio zavrsni rad");
                if (!(provjeriDatum(_teme.getElement2(i).GetDatumPrijave(), datumObrane)))
                    throw exception("Datum polaganja zavrsnog rada mora biti najmanje 2 mjeseca poslje prijave");
                _teme.getElement2(i).GetKomisija() = komisija;
                _teme.getElement2(i).setDatumObrane(datumObrane);
                return;
            }
        }
        throw exception("Student s tim brojem indeka nije prijavio zavrsni rad!");
    }
    bool operator()(string index, int ocjena)
    {
        // Studentu sa brojem indeksa IB130011 dodjeljuje ocjenu 8 na zavrsnom radu. Uslov za
        // dodjelu ocjene je da student posjeduje definisan datum odbrane i listu clanova komisije.
       // U zavisnosti od uspjesnosti izvrsenja, funkcija vraca true ili false
        for (int i = 0; i < _teme.getTrenutno(); i++)
        {
            if (_teme.getElement1(i) == index)
            {
                if (_teme.getElement2(i).GetDatumOdbrane() == "NEMA VRIJEDNOST")
                    return false;
                if (_teme.getElement2(i).GetKomisija().size() == 0)
                    return false;
                _teme.getElement2(i).setOcjena(ocjena);
                return true;
            }
        }
        return false;
    }
};
ostream& operator<<(ostream& COUT,  Nastavnik& obj)
{
    /*
        string _imePrezime;
        Oblast _oblastIzboraUZvanje;
         //Parametar string predstavlja broj indeksa studenta koji prijavljuje zavrsni rad kod odredjenog nastavnika
        Kolekcija<string, ZavrsniRad, 10> _teme;
    */
    // Ispisuje sve podatke o nastavniku i njegovim mentorstvima. Za clanove komisije je dovoljno 
    // ispisati samo ime i prezime.
    COUT << "Ime prezime nastavnika: " << obj._imePrezime << endl;
    COUT << "Oblast zvanja: " << oblast_txt[obj._oblastIzboraUZvanje] << endl;
    COUT << "Nastavnikovi mentorstva: " <<endl<< crt;
    for (int i = 0; i < obj._teme.getTrenutno(); i++)
    {
        COUT << "Broj indeksa: " << obj._teme.getElement1(i) << endl;
        COUT << "Informacije o temi zavrsnog rada: " << endl<< obj._teme.getElement2(i);
        COUT << "Clanovi komisije: " << endl;
        
        vector<Nastavnik*>::iterator itPocetak = obj._teme.getElement2(i).GetKomisija().begin();
        vector<Nastavnik*>::iterator itKraj = obj._teme.getElement2(i).GetKomisija().end();
        for (itPocetak; itPocetak != itKraj; itPocetak++)
        {
            COUT << ((*itPocetak)->GetImePrezime()) << endl;
        }
        COUT << crt;
       /* for (int i = 0; i < sizeV; i++)
        {
            COUT << "Ime prezime: " << obj._teme.getElement2(i).GetKomisija()[i]->GetImePrezime() << endl;
        }*/
        
    }
    return COUT;
}
string PronadjiNajStudenta(Nastavnik** nastavnici, int max)
{
    //Funkcija PronadjiNajStudenta ima zadatak da pronadje broj indeksa studenta koji je na zavrsnom 
       //radu ostvario najvecu ocjenu kod nastavnika koji posjeduje najvecu prosjecnu ocjenu na zavrsnim radovima.
       //Ukoliko se kod nastavnika sa najvecom prosjecnom ocjenom pojavi vise studenata sa istom ocjenom, 
       //onda funkcija vraca broj indeksa prvog pronadjenog studenta. Svim studentima koji su odbranili 
       //rad kod nastavnika sa najvecom prosjecno ocjenom, u zasebnom thread.u, poslati email poruku 
       //(mail adresa: brojIndeksa@edu.fit.ba) sa sadrzajem da su svoj zavrsni rad uspjesno odbranili sa 
       //vecom ili manjom ocjenom od prosjecne. Ukoliko niti jedan od nastavnika ne posjeduje evidentirano
       //mentorstvo na zavrsnom radu, funkcija vraca tekst: NIJE PRONADJEN
    float najProsjek = 0, prosjek = 0;
    string indx = "NIJE PRONADJEN";
    Nastavnik* nastavnikSaNajboljimProsekom = NULL;
    for (int i = 0; i < max; i++)
    {
        prosjek = 0;
        for (int j = 0; j < nastavnici[i]->GetTeme().getTrenutno(); j++)
        {
            prosjek += nastavnici[i]->GetTeme().getElement2(j).GetOcjena();
        }
        prosjek /= nastavnici[i]->GetTeme().getTrenutno();
        if (prosjek > najProsjek)
        {
            najProsjek = prosjek;
            nastavnikSaNajboljimProsekom = *(nastavnici + i);
        }
    }
    prosjek = 0;
    if (nastavnikSaNajboljimProsekom != NULL)
    {
        for (int i = 0; i < nastavnikSaNajboljimProsekom->GetTeme().getTrenutno(); i++)
        {
            if (nastavnikSaNajboljimProsekom->GetTeme().getElement2(i).GetOcjena() > prosjek)
            {
                prosjek = nastavnikSaNajboljimProsekom->GetTeme().getElement2(i).GetOcjena();
                indx = nastavnikSaNajboljimProsekom->GetTeme().getElement1(i);
            }
        }
        auto posaljiMail = [nastavnikSaNajboljimProsekom, najProsjek]() {
            for (int i = 0; i < nastavnikSaNajboljimProsekom->GetTeme().getTrenutno(); i++)
            {
                cout << crt;
                cout << "Saljem mail: " << nastavnikSaNajboljimProsekom->GetTeme().getElement1(i) << "@edu.fit.ba ..........\n";
                this_thread::sleep_for(chrono::seconds(5));
                if (nastavnikSaNajboljimProsekom->GetTeme().getElement2(i).GetOcjena() > najProsjek)
                {
                    cout << "Uspjesno ste obranili rad sa ocjenom vecom od prosjecne ocjene" << endl;
                }
                else
                {
                    cout << "Uspjesno ste odbranili rad, ali nazalost s ocijenom nizom od prosjecne. Ali to nema veze ipak ste odbranili rad"<<endl;
                }
                cout << crt;
            }
        };
        thread th1(posaljiMail);
        th1.join();
    }
    return indx;
}
void main()
{
    const int max = 4;
    Nastavnik* nastavnici[max];

    nastavnici[0] = new Nastavnik("Denis Music", SoftverskiInzinjering);
    nastavnici[1] = new Nastavnik("Zanin Vejzovic", KomunikacijskiSistemi);
    nastavnici[2] = new Nastavnik("Jasmin Azemovic", SigurnostInformacijskihSistema);
    nastavnici[3] = new Nastavnik("Emina Junuz", SoftverskiInzinjering);

    try
    {
        //Funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu. 
        //Sprijeciti dodavanje zavrsnih radova sa istom temom vise puta. Nastavnik moze imati (mentorisati) 
        //samo radove iz oblasti za koju posjeduje izbor u zvanje.U slucaju da se nastavniku pokusa dodati 
        //rad koji nije iz njegove oblasti funkcija treba da baci izuzetak sa odgovarajucom porukom

        //indeks, naslov, oblast, datum prijave 
        nastavnici[0]->DodajZavrsniRad("IB130011", "Multimedijalni informacijski sistem za visoko-obrazovnu ustanovu", SoftverskiInzinjering, "01.04.2017");
        nastavnici[0]->DodajZavrsniRad("IB120051", "Sistem za podršku rada kablovskog operatera", SoftverskiInzinjering, "03.03.2017");

        nastavnici[1]->DodajZavrsniRad("IB140102", "Praktièna analiza sigurnosti bežiènih raèunarskih mreža", KomunikacijskiSistemi, "22.08.2017");

        nastavnici[2]->DodajZavrsniRad("IB140002", "Primjena teorije informacija u procesu generisanja kriptografskih kljuèeva", SigurnostInformacijskihSistema, "10.09.2017");

        vector<Nastavnik*> komisija;//formira se komisija
        komisija.push_back(nastavnici[0]);
        komisija.push_back(nastavnici[2]);
        komisija.push_back(nastavnici[3]);

        // Funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu 
        // zavrsnog rada sto podrazumijeva definisanje datuma odbrane i liste clanova komisije pred kojima ce 
        // student braniti zavrsni rad. Odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio. 
        // Komisiju trebaju ciniti najmanje 2 nastavnika koji imaju izbor u zvanje u oblasti kojoj pripada tema rada. 
        // Datum odbrane ne smije biti manji od datuma prijave. U slucaju da bilo koji od navedenih uslova nije ispunjen
        //  funkcija treba	da baci izuzetak
        nastavnici[0]->ZakaziOdbranuRada("IB130011", "25.09.2017", komisija);
        nastavnici[0]->ZakaziOdbranuRada("IB120051", "25.09.2017", komisija);//student sa brojem indeksa IB130111 
                                                                             //jos uvijek nije prijavio rad

        // Studentu sa brojem indeksa IB130011 dodjeljuje ocjenu 8 na zavrsnom radu. Uslov za
        // dodjelu ocjene je da student posjeduje definisan datum odbrane i listu clanova komisije. U zavisnosti od 
        // uspjesnosti izvrsenja, funkcija vraca true ili false

        if ((*nastavnici[0])("IB130011", 8))
            cout << "Uspjesno ste ocijenili zavrsni rad!" << endl;
        if ((*nastavnici[0])("IB120051",10))
            cout << "Uspjesno ste ocijenili zavrsni rad!" << endl;

        // Ispisuje sve podatke o nastavniku i njegovim mentorstvima. Za clanove komisije je dovoljno 
        // ispisati samo ime i prezime.
        cout << *nastavnici[0] << endl;

        //Funkcija PronadjiNajStudenta ima zadatak da pronadje broj indeksa studenta koji je na zavrsnom 
        //radu ostvario najvecu ocjenu kod nastavnika koji posjeduje najvecu prosjecnu ocjenu na zavrsnim radovima.
        //Ukoliko se kod nastavnika sa najvecom prosjecnom ocjenom pojavi vise studenata sa istom ocjenom, 
        //onda funkcija vraca broj indeksa prvog pronadjenog studenta. Svim studentima koji su odbranili 
        //rad kod nastavnika sa najvecom prosjecno ocjenom, u zasebnom thread.u, poslati email poruku 
        //(mail adresa: brojIndeksa@edu.fit.ba) sa sadrzajem da su svoj zavrsni rad uspjesno odbranili sa 
        //vecom ili manjom ocjenom od prosjecne. Ukoliko niti jedan od nastavnika ne posjeduje evidentirano
        //mentorstvo na zavrsnom radu, funkcija vraca tekst: NIJE PRONADJEN

        cout << "Najsupjesniji student: " << PronadjiNajStudenta(nastavnici, max) << endl;

        //Baca izuzetak zbog neadekvatnog izbora u zvanje, odnosno oblasti
        //nastavnici[2]->DodajZavrsniRad("IB150008", "Razvoj sistema autentifikacije na osnovu biometrije glasa", InteligentniSistemi, "15.05.2017");
    }
    catch (exception& ex)
    {
        cout << "GRESKA -> " << ex.what() << endl;
    }

    for (int i = 0; i < max; i++)
    {
        delete nastavnici[i];
        nastavnici[i] = nullptr;
    }
}