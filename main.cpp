#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>

using namespace std;

struct klient{
	string identyfikator, nazwa, miasto;
};

struct towar{
	string identyfikator, nazwa, cena_jedn;
};

struct transakcja{
	string identyfikator_klienta, identyfikator_towaru, ilosc;
};

//struktura wykorzystywana dopiero na końcu przy zliczaniu dla każdego miasta
struct laczny_koszt{
    string miasto;
    float koszt;
};

void podziel_pliki(string *tablica, string nazwa_pliku, int &i);
void podziel_wiersz(string *str, string dane_do_analizy);
bool sprawdz_identyfokatory(string *identyfikatory, int ilosc_identyfikatorow);
void wypisz_dane(klient *klienci, towar *towary, transakcja *transakcje, int ilosc_klientow, int ilosc_towarow, int ilosc_transakcji);
void zlicz_ceny(laczny_koszt *lista_miast, int ilosc_miast);

int main()
{
	const int MAX_N = 100;
	string tablica_linii_klientow[MAX_N], tablica_linii_towarow[MAX_N], tablica_linii_transakcji[MAX_N];
	int ilosc_klientow=0, ilosc_towarow=0, ilosc_transakcji=0;

	podziel_pliki(tablica_linii_klientow, "klienci.txt", ilosc_klientow);
	podziel_pliki(tablica_linii_towarow, "towary.txt", ilosc_towarow);
	podziel_pliki(tablica_linii_transakcji, "transakcje.txt", ilosc_transakcji);

	klient tablica_klientow[ilosc_klientow];
	towar tablica_towarow[ilosc_towarow];
	transakcja tablica_transakcji[ilosc_transakcji];

    string id_klienci[ilosc_klientow];
    string id_towary[ilosc_towarow];


	for (int i = 0; i < ilosc_klientow; i++)
	{
		string str[3];
		podziel_wiersz(str, tablica_linii_klientow[i]);
		tablica_klientow[i].identyfikator = str[0];
		id_klienci[i] = str[0];
		tablica_klientow[i].nazwa = str[1];
		tablica_klientow[i].miasto = str[2];
	}

	if (sprawdz_identyfokatory(id_klienci, ilosc_klientow) == true)
    {
        cout<<"Id klientow powtarzaja sie! Kazde id musi byc unikatowe."<<endl;
        exit(0);
    }

	for (int i = 0; i < ilosc_towarow; i++)
	{
		string str[3];
		podziel_wiersz(str, tablica_linii_towarow[i]);
		tablica_towarow[i].identyfikator = str[0];
		id_towary[i] = str[0];
		tablica_towarow[i].nazwa = str[1];
		tablica_towarow[i].cena_jedn = str[2];
	}

    if (sprawdz_identyfokatory(id_towary, ilosc_towarow) == true)
    {
        cout<<"Id towarow powtarzaja sie! Kazde id musi byc unikatowe."<<endl;
        exit(0);
    }

	for (int i=0; i<ilosc_transakcji; i++)
	{
		string str[3];
		podziel_wiersz(str, tablica_linii_transakcji[i]);
		tablica_transakcji[i].identyfikator_klienta = str[0];
		tablica_transakcji[i].identyfikator_towaru = str[1];
		tablica_transakcji[i].ilosc = str[2];
	}

    wypisz_dane(tablica_klientow, tablica_towarow, tablica_transakcji, ilosc_klientow, ilosc_towarow, ilosc_transakcji);

	return 0;
}

//funkcja wczytuje każdy plik do osobnej tabeli (tutaj: zmienna *tablica), linia po linii i zaznacza, ile każda tabela ma elementów (tutaj: &i)
void podziel_pliki(string *tablica, string nazwa_pliku, int &i)
{
	fstream plik;
	string linia;

	plik.open(nazwa_pliku.c_str(), ios::in);

	if(plik.good() == false)
	{
		cout<<"Plik nie istnieje.";
		exit(0);
	}

	while (getline(plik, linia))
	{
		tablica[i] = linia;
	    i++;
 	}

	plik.close();
};

//funkcja dzieli każdy wiersz na 3 kolumny w tabeli stringów (tutaj: *str), które potem wczytuje do odpowiednich elementów danej struktury
void podziel_wiersz(string *str, string dane_do_analizy)
{
    int ilosc_slow = 0, ilosc_znakow = 0;
    string slowo = "";
    char tablica_znakow[dane_do_analizy.length()];

    while (ilosc_znakow < dane_do_analizy.length())
    {
        tablica_znakow[ilosc_znakow] = dane_do_analizy[ilosc_znakow];
        ilosc_znakow++;
    }

    int i = 0;
    while (i < ilosc_znakow + 1)
    {
        if ((tablica_znakow[i] == ' ') or (i == ilosc_znakow ))
        {
            str[ilosc_slow] = slowo;
            ilosc_slow += 1;
            slowo="";
        }
        else
            slowo += tablica_znakow[i];

        i++;
    }
}

//funkcja sprawdza, czy każdy klient i towar ma unikatowy identyfikator (po to tylko zostały stworzone tablice id_klienci i id_towary)
//funkcja zwraca true jeśli duplikat się pojawił, false jeśli nie
bool sprawdz_identyfokatory(string *identyfikatory, int ilosc_identyfikatorow)
{
    bool czy_sa_duplikaty = false;

    for (int i = 0; i < ilosc_identyfikatorow; i++)
    {
        int ilosc_powtorzen = 0;
        for (int j = 0; j < ilosc_identyfikatorow; j++)
        {
            if (identyfikatory[i] == identyfikatory[j])
            {
                ilosc_powtorzen += 1;
            }
        }
        if (ilosc_powtorzen >= 2)
            {
                czy_sa_duplikaty = true;
            }
    }

    return czy_sa_duplikaty;
}

//funkcja wykorzystana dopiero w następnej funkcji
// funkcja zlicza ceny każdego towaru w każdym mieście i wyświetla
// każde miasto tylko raz (stąd np. nadanie wartości lista_miast.miasto = "miasto wykorzystane")
void zlicz_ceny(laczny_koszt *lista_miast, int ilosc_miast)
{
    for (int i = 0; i < ilosc_miast; i++)
    {
        for (int j = i + 1; j < ilosc_miast; j++)
        {
            if ((lista_miast[i].miasto == lista_miast[j].miasto))
            {
                lista_miast[i].koszt += lista_miast[j].koszt;
                lista_miast[j].miasto = "miasto wykorzystane";
                lista_miast[j].koszt = 0;
            }
        }
        if (lista_miast[i].miasto != "miasto wykorzystane")
            cout<<lista_miast[i].miasto<<"   ";
        if (lista_miast[i].koszt != 0)
            cout<<lista_miast[i].koszt<<endl;
    }

}

//funkcja razem z poprzednią wypisuje odpowiednie dane
void wypisz_dane(klient *klienci, towar *towary, transakcja *transakcje, int ilosc_klientow, int ilosc_towarow, int ilosc_transakcji)
{
    for (int i = 0; i < ilosc_towarow; i++)
    {
        laczny_koszt lista_miast[ilosc_klientow];
        int ilosc_miast = 0;
        cout<<towary[i].nazwa<<" (ID: "<<towary[i].identyfikator<<")"<<endl;
        for (int j = 0; j < ilosc_transakcji; j++)
        {
            if (transakcje[j].identyfikator_towaru == towary[i].identyfikator)
            {
                for (int c = 0; c < ilosc_klientow; c++)
                {
                    if (klienci[c].identyfikator == transakcje[j].identyfikator_klienta)
                    {
                        lista_miast[ilosc_miast].miasto = klienci[c].miasto;
                        lista_miast[ilosc_miast].koszt = atof(transakcje[j].ilosc.c_str()) * atof(towary[i].cena_jedn.c_str());

                        ilosc_miast += 1;
                    }
                }
            }
        }
        zlicz_ceny(lista_miast, ilosc_miast);
        cout<<endl;
    }
}
