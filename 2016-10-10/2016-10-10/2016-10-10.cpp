#include <iostream>
#include <vector>
#include <regex>
using namespace std;
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
    Datum(const Datum& org) :
        _dan(new int(*org._dan)), _mjesec(new int(*org._mjesec)), _godina(new int(*org._godina))
    {   }
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
            delete _dan; _dan = NULL;
            delete _mjesec; _mjesec = NULL;
            delete _godina; _godina = NULL;
            _dan = new int(*obj._dan);
            _mjesec = new int(*obj._mjesec);
            _godina = new int(*obj._godina);
        }
        return *this;
    }
    bool operator==(const Datum& obj)
    {
        if ((*_dan == *obj._dan) && (*_mjesec == *obj._mjesec) && (*_godina == *obj._godina))
            return true;
        return false;
    }
    friend ostream& operator<< (ostream& COUT, const Datum& obj)
    {
        COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
        return COUT;
    }
    void operator++(int i) {
        *_dan++;
        if (*_dan > 31) {
            *_dan = 1;
            *_mjesec++;
            if (*_mjesec > 12) {
                *_mjesec = 1;
                *_godina++;
            }
        }
    }
};
class Izuzetak :public exception
{
    friend ostream& operator<<(ostream& COUT, const Izuzetak& obj);
private:
    Datum _datum;//KORISTITE DANASNJI DATUM
    string _funkcija;//FUNKCIJA U KOJOJ JE NASTAO IZUZETAK
    int _linija;//LINIJA CODE-A U KOJOJ JE NASTAO IZUZETAK
public:
    Izuzetak(const char* poruka, string funkcija, int linijaC, int d, int m, int g)
        :exception(poruka), _datum(d, m, g), _linija(linijaC)
    {    }

};
ostream& operator<<(ostream& COUT, const Izuzetak& obj)
{
    COUT << obj.what() << endl;
    COUT << "Funkcija: " << obj._funkcija << "()" << endl;
    COUT << "Linija coda: " << obj._linija << endl;
    COUT << obj._datum << endl;
    return COUT;
}

template<class T1, class T2, int max>
class Kolekcija
{
    friend ostream& operator<< <>(ostream& COUT, const Kolekcija<T1, T2, max>& obj);
private:
    T1 _elementi1[max];
    T2 _elementi2[max];
    int* _trenutno;
public:
    Kolekcija():_trenutno(new int(0))
    {
        
    }
    Kolekcija(const Kolekcija<T1, T2, max>& org) :
        _trenutno(new int(*org._trenutno))
    {
        
        for (int i = 0; i < *(org._trenutno); i++)
        {
            _elementi1[i] = org._elementi1[i];
            _elementi2[i] = org._elementi2[i];
        }
    }
    ~Kolekcija()
    {
        delete _trenutno; _trenutno = NULL;
    }
    Kolekcija& operator=(const Kolekcija<T1, T2, max>& obj)
    {
        if (this != &obj)
        {
            
            delete _trenutno; _trenutno = NULL;
            _trenutno = new int(*(obj._trenutno));
            for (int i = 0; i < *(obj._trenutno); i++)
            {
                _elementi1[i] = obj._elementi1[i];
                _elementi2[i] = obj._elementi2[i];
            }
        }
        return *this;
    }
    
    int GetTrenutno()const { return *(_trenutno); }
    int GetMax() { return max; }
    T1& GetElement1(int index) 
    {
        return _elementi1[index];
    }
    T2& GetElement2(int index)
    {
        return _elementi2[index];
    }
    void setElement1(int index,const T1& elem) {
        _elementi1[index] = elem;
    }
    void setElement2(int index, const T2& elem2) {
        _elementi1[index] = elem2;
    }
    void AddElement(T1 elem1,T2 elem2)
    {
        if (*(_trenutno) < max)
        {
            _elementi1[*(_trenutno)] = elem1;
            _elementi2[*(_trenutno)] = elem2;
            (*_trenutno)++;
        }
        else
        {
            throw Izuzetak("Nema vise mjesta u kolekciji",__FUNCTION__,__LINE__,18,5,2020);
        }
    }
    int size()const  { return *(_trenutno); }
    
    
};
template<class T1,class T2,int max>
ostream& operator<< <>(ostream& COUT, const Kolekcija<T1, T2, max>& obj)
{
    for (int i = 0; i < *obj._trenutno; i++)
    {
        COUT << obj._elementi1[i] << '\t' << obj._elementi2[i] << endl;
    }
    return COUT;
}


enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };
class Kurs
{
    friend ostream& operator<<(ostream& COUT, const Kurs& obj);
private:
    enumKursevi _kurs;
    Datum _pocetak;
    Datum _kraj;
    char* _imePredavaca;
public:
    Kurs():_imePredavaca(NULL) {}
    Kurs(enumKursevi kurs, const char* imePrezime,Datum pocetak,Datum kraj)
        :_kurs(kurs),_imePredavaca(new char[strlen(imePrezime)+1]),_pocetak(pocetak),_kraj(kraj)
    {  
        strcpy_s(_imePredavaca, strlen(imePrezime) + 1, imePrezime);
    }
    Kurs(const Kurs& org) :
        _kurs(org._kurs), _pocetak(org._pocetak), _kraj(org._kraj), _imePredavaca(new char[strlen(org._imePredavaca) + 1])
    {
        strcpy_s(_imePredavaca, strlen(org._imePredavaca) + 1, org._imePredavaca);
    }
    ~Kurs()
    {
        delete[]_imePredavaca;
    }
    Kurs& operator=(const Kurs& obj)
    {
        if (this != &obj)
        {
            delete[]_imePredavaca; _imePredavaca = NULL;
            _imePredavaca = new char[strlen(obj._imePredavaca) + 1];
            strcpy_s(_imePredavaca, strlen(obj._imePredavaca) + 1, obj._imePredavaca);
            _kurs = obj._kurs;
            _pocetak = obj._pocetak;
            _kraj = obj._kraj;
        }
        return *this;
    }
    bool operator==(const Kurs& obj)
    {
        if ((this->_kurs == obj._kurs) && (this->_pocetak == obj._pocetak) && (this->_kraj == obj._kraj))
        {
            if(strcmp(this->_imePredavaca,obj._imePredavaca)==0)
                return true;
        }
        return false;
    }
};
ostream& operator<<(ostream& COUT, const Kurs& obj)
{
    COUT << "Kurs: " << obj._kurs << endl;
    COUT << "Ime predavaca: " << obj._imePredavaca << endl;
    COUT << "Pocetak kurs-a: " << obj._pocetak << "\tKraj kurs-a:" << obj._kraj << endl;
    return COUT;
}
class Polaznik
{
    static int ID;
    int _polaznikID;
    //SVAKOM NOVOM POLAZNIKU AUTOMATSKI DODIJELITI NOVI ID (AUTOINCREMENT) POCEVSI OD BROJA 1
    char* _imePrezime;
    string _kontaktTelefon; //BROJ TELEFONA TREBA BITI U FORMATU (06X)XXX-XXX 
    Kolekcija<Kurs*, int, 10> _uspjesnoOkoncaniKursevi;
    //INT PREDSTAVLJA OSTVARENI PROCENAT NA ISPITU, A JEDAN POLAZNIK MOZE POLOZITI NAJVISE 10 KURSEVA
public:
    Polaznik(const char* imePrezime, string telefon) : _polaznikID(ID++)
    {
        int size = strlen(imePrezime) + 1;
        _imePrezime = new char[size];
        strcpy_s(_imePrezime, size, imePrezime);
        _kontaktTelefon = telefon;
    }
    Polaznik( Polaznik& obj) : _kontaktTelefon(obj._kontaktTelefon), _polaznikID(obj.ID),
        _uspjesnoOkoncaniKursevi(obj._uspjesnoOkoncaniKursevi), _imePrezime(new char[strlen(obj._imePrezime) + 1]){
            strcpy_s(_imePrezime,strlen(obj._imePrezime)+1,obj._imePrezime);
            for (int i = 0; i < obj._uspjesnoOkoncaniKursevi.size(); i++) {
                _uspjesnoOkoncaniKursevi.setElement1(i, new Kurs(*obj._uspjesnoOkoncaniKursevi.GetElement1(i)));
            }
        }
    ~Polaznik()
    {
        delete[] _imePrezime; _imePrezime = nullptr;
        for (size_t i = 0; i < _uspjesnoOkoncaniKursevi.size(); i++)
        {
            delete _uspjesnoOkoncaniKursevi.GetElement1(i);
            _uspjesnoOkoncaniKursevi.GetElement1(i) = nullptr;
            
        }
    }
    Polaznik& operator=( Polaznik& obj) {
        if (this != &obj) {
            delete[] _imePrezime; _imePrezime = nullptr;
            for (size_t i = 0; i < _uspjesnoOkoncaniKursevi.size(); i++)
            {
                delete _uspjesnoOkoncaniKursevi.GetElement1(i);
                _uspjesnoOkoncaniKursevi.GetElement1(i) = nullptr;
            }
            _imePrezime = new char[strlen(obj._imePrezime) + 1];
            _uspjesnoOkoncaniKursevi = obj._uspjesnoOkoncaniKursevi;
            _kontaktTelefon = obj._kontaktTelefon;
            _polaznikID = obj._polaznikID;
            strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
            for (int i = 0; i < obj._uspjesnoOkoncaniKursevi.size(); i++) {
                _uspjesnoOkoncaniKursevi.setElement1(i, new Kurs(*obj._uspjesnoOkoncaniKursevi.GetElement1(i)));
            }

        }
        return *this;
    }
    friend ostream& operator<<(ostream& COUT, const Polaznik& obj) {

    }
};
int Polaznik::ID = 1;
class SkillsCentar
{
    string _nazivCentra;
    vector<Kurs> _kursevi;//KURSEVI KOJE NUDI ODREDJENI CENTAR
    Kolekcija<Kurs, Polaznik*, 150> _aplikanti;
    
public:
    SkillsCentar(string naziv) { _nazivCentra = naziv; }
    SkillsCentar(const SkillsCentar& obj) :_kursevi(obj._kursevi), _nazivCentra(obj._nazivCentra)
    {
        _aplikanti = obj._aplikanti;
    }
    SkillsCentar& operator=(const SkillsCentar& obj) {
        if (this != &obj) {
            _kursevi = obj._kursevi;
            _nazivCentra = obj._nazivCentra;
            _aplikanti = obj._aplikanti;
        }
    }
    bool ProvjeriKoliziju(Kurs kurs)
    {
        return true;
    }
    void AddKurs(Kurs kurs)
    {
        if (_kursevi.size() != 0) {

        }
        else
            _kursevi.push_back(kurs);
    }
    void AddAplikaciju(Kurs kurs, Polaznik polaznik)
    {

    }
};
const char* crt = "\n---------------------------------------\n";
bool ProvjeriFormatTelefona(string telefon)
{
    //(06X)XXX - XXX
    regex re("(06)[0-9]( )[0-9]{3}(-)[0-9]{3}");
    if (regex_match(telefon,re))
    {
        return true;
    }
    else
        return false;
}
void main()
{
    /****************************************************************************
    // 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
    // 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU
    //    RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
    // 3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
    // 4. PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT
    // 5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U
    //    TESTNOM CODE-U, OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE,
    //    POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
    // 6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
    ****************************************************************************/

#pragma region TestiranjeDatuma
    Datum danas(26, 11, 2015);
    Datum sutra(danas);
    Datum prekosutra;
    prekosutra = danas;
    cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
#pragma region TestiranjeKolekcije
    Kolekcija<int, int, 10> kolekcija1;
    try
    {
        for (size_t i = 0; i < 12; i++)
            kolekcija1.AddElement(i, i + 2);
        //FUNKCIJA TREBA BACITI IZUZETAK U SLUCAJ DA NEMA VISE PROSTORA ZA DODAVANJE NOVIH ELEMENATA
        cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
        //cout << kolekcija1.GetElement1(20) << " " << kolekcija1.GetElement2(20) << endl;
        //FUNKCIJA TREBA DA BACI IZUZETAK UKOLIKO ELEMENT NA ZAHTIJEVANOJ LOKACIJI NE POSTOJI
    }
    catch (Izuzetak& err)
    {
        cout << "Greska -> " << err << endl;
    }

    cout << kolekcija1 << endl;
    Kolekcija<int, int, 10> kolekcija2;
    kolekcija2 = kolekcija1;
    cout << kolekcija2 << crt;
    if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
        cout << "ISTI BROJ ELEMENATA" << endl;
    Kolekcija<int, int, 10> kolekcija3(kolekcija2);
    cout << kolekcija3 << crt;
#pragma endregion

#pragma region TestiranjeKursa
    Kurs sef(SoftwareEngeneeringFundamentals, "Jasmin Azemovic", Datum(28, 1, 2016), Datum(15, 2, 2016));
    cout << sef << endl;
    Kurs msql(MasteringSQL, "Adel Handzic", Datum(28, 2, 2016), Datum(15, 3, 2016));
    sef = msql;
    if (sef == msql)//KURSEVI SU ISTI AKO POSJEDUJU IDENTICNE VRIJEDNOSTI SVIH ATRIBUTA
        cout << "ISTI SU KURSEVI" << endl;
    Kurs msql2(msql);
    cout << msql2 << endl;
#pragma endregion

#pragma region TestiranjePolaznika
    string telefon1 = "061 111-111";
    string telefon2 = "(061)111222";
    string telefon3 = "061 111-333";

    if (!ProvjeriFormatTelefona(telefon1))
        cout << "Broj " << telefon1 << " nije validan" << endl;
    if (!ProvjeriFormatTelefona(telefon2))
        cout << "Broj " << telefon2 << " nije validan" << endl;
    if (!ProvjeriFormatTelefona(telefon3))
        cout << "Broj " << telefon3 << " nije validan" << endl;


    Polaznik denis("Denis Music", telefon1);
    Polaznik denis2("Denis2 Music2", telefon3);
    Polaznik denis3(denis);
    denis2 = denis;
#pragma endregion

#pragma region TestiranjeCentra
    try
    {
        SkillsCentar mostar("Skills Center Mostar");
        if (mostar.ProvjeriKoliziju(sef)) //JEDAN KURS NE SMIJE POCETI DOK DRUGI TRAJE TJ. VRIJEME ODRZAVANJA I
            mostar.AddKurs(sef);          //KURSA SE NE SMIJE POKLAPAT
        if (mostar.ProvjeriKoliziju(msql))
            mostar.AddKurs(msql);
        if (mostar.ProvjeriKoliziju(msql2))
            mostar.AddKurs(msql2);
        //ONEMOGUCITI DODAVANJE IDENTICNIH KURSEVA. FUNKCIJA TREBA DA BACI IZUZETAK UKOLIKO SE POKUSA DODATI IDENTICAN KURS
        mostar.AddAplikaciju(sef, denis);
        //ONEMOGUCITI APLICIRANJE ZA KURSEVE KOJI NISU REGISTROVANI U CENTRU
        mostar.AddAplikaciju(msql, denis);
        mostar.AddAplikaciju(msql, denis);
    //    //ONEMOGUCITI APLICIRANJE ZA ISTI KURS. FUNKCIJA BAZA IZUZETAK
    //    cout << denis2 << endl;
    //    denis2 = denis;
    //    if (denis == denis2) //POLAZNICI SU ISTI AKO POSJEDUJU ISTO IME I AKO SU POHADJALI ISTE KURSEVE
    //        cout << "ISTI SU" << endl;

    //    mostar.DodajUspjesnoOkoncanKurs(1, sef, 60);
    //    // BROJ 1 OZNACAVA ID POLAZNIKA. FUNKCIJA JE ZADUZENA DA POLAZNIKU 
    //    // DODA INFORMACIJU O USPJESNO POLOZENOM KURSU KOJI JE POSLAN KAO PARAMETAR. PREDUSLOV ZA DODAVANJE JE DA JE 
    //    // POLAZNIK PRETHODNO APLICIRAO ZA TAJ KURS, TE DA JE NA ISPITU OSTVARIO VISE OD 55%
    //    mostar.DodajUspjesnoOkoncanKurs(1, msql, 83);
    //    vector<Polaznik> listaPolaznika = mostar.GetPolazniciByPredavac("Jasmin Azemovic", SoftwareEngeneeringFundamentals);
    //    //VRACA INFORMACIJE O POLAZNICIMA KOJI SU KOD ODREDJENOG PREDAVACA USPJESNO OKONCALI ODREDJENI KURS

    //    if (mostar.RemoveKurs(sef))//ONEMOGUCITI UKLANJANJE KURSA KOJI SU VEC PRIJAVILI NEKI OD POLAZNIKA
    //        cout << "Kurs " << sef << " uspjesno uklonjen iz centra " << mostar << endl;

    //    SkillsCentar sarajevo = mostar;
    //    cout << sarajevo << endl;
    }
    catch (Izuzetak& err)
    {
        cout << err << endl;
    }
#pragma endregion
    
}
