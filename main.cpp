#include <iostream>
#include <fstream>
#include "string.h"
using namespace std;

class Film {
private:
    char *titlu;
    char *numeRegizor;
    int anAparitie;
    unsigned int durataMinute;

public:
    Film(const char *titlu, const char *numeRegizor, int anAparitie, unsigned int durataMinute)
    {
        size_t len;

        len = strlen(titlu);
        this->titlu = new char[len + 1];
        strcpy(this->titlu, titlu);

        len = strlen(numeRegizor);
        this->numeRegizor = new char[len + 1];
        strcpy(this->numeRegizor, numeRegizor);

        this->anAparitie = anAparitie;
        this->durataMinute = durataMinute;
    }

    Film(char *date)
    {
        char *p = strtok(date, "|");
        titlu = p;
        p = strtok(NULL, "|");
        numeRegizor = p;
        p = strtok(NULL, "|");
        anAparitie = atoi(p);
        p = strtok(NULL, "|");
        durataMinute = atoi(p);
    }

    Film()
    {
        titlu = NULL;
        numeRegizor = NULL;
        anAparitie = 0;
        durataMinute = 0;
    }

    ~Film()
    {
        delete[] titlu;
        delete[] numeRegizor;
    }

    Film& operator=(const Film &rhs) {
        if (this != &rhs)
            setTitlu(rhs.titlu);

        if (this != &rhs)
            setNumeRegizor(rhs.numeRegizor);

        this->anAparitie = rhs.anAparitie;
        this->durataMinute = rhs.durataMinute;

        return *this;
    }

    friend ostream& operator <<(ostream& os, const Film &film) {
        os << "Film: " << film.getTitlu()
             << " | Regizor: " << film.getNumeRegizor()
             << " | An: " << film.getAnAparitie()
             << " | Durata: " << film.getDurataMinute() << " minute.\n";

        return os;
    }

    void afiseaza() const
    {
        cout << *this;
    }

    friend istream& operator >>(istream& is, Film &film){
        cout << "Titlu: ";
        char buff[255];
        fflush(stdin);
        cin.getline(buff, 256);
        film.titlu = new char[strlen(buff) + 1];
        strcpy(film.titlu, buff);
        cout << "Regizor: ";
        fflush(stdin);
        cin.getline(buff, 256);
        film.numeRegizor = new char[strlen(buff) + 1];
        strcpy(film.numeRegizor, buff);
        cout << "An: ";
        cin >> film.anAparitie;
        cout << "Durata: ";
        cin >> film.durataMinute;
    }

    char *getTitlu() const
    {
        return titlu;
    }

    void setTitlu(const char *titlu)
    {
        size_t len = strlen(titlu);

        if (len == 0)
        {
            cout << "Eroare: Filmul trebuie sa aiba titlu!";
            return;
        }

        delete[] this->titlu;
        this->titlu = new char[len + 1];
        strcpy(this->titlu, titlu);
    }

    char *getNumeRegizor() const
    {
        return numeRegizor;
    }

    void setNumeRegizor(const char *numeRegizor)
    {
        size_t len = strlen(numeRegizor);

        if (len == 0)
        {
            cout << "Eroare: Filmul trebuie sa aiba regizor!";
            return;
        }

        delete[] numeRegizor;
        this->numeRegizor = new char[len + 1];
        strcpy(this->numeRegizor, numeRegizor);
    }

    int getAnAparitie() const
    {
        return anAparitie;
    }

    void setAnAparitie(int anAparitie)
    {
        if (anAparitie >= 2023)
        {
            cout << "Eroare: An aparitie invalid!";
            return;
        }

        this->anAparitie = anAparitie;
    }

    int getDurataMinute() const
    {
        return durataMinute;
    }

    void setDurataMinute(unsigned int durataMinute)
    {
        if (durataMinute <= 0)
        {
            cout << "Eroare: Durata invalida!";
            return;
        }

        this->durataMinute = durataMinute;
    }
};

class Cinema {

private:
    unsigned int nrFilme;
    Film *listaFilme;

public:
    Cinema(char fisier[])
    {
        ifstream fin(fisier);

        fin >> nrFilme;
        fin.ignore();

        listaFilme = (Film*) malloc(nrFilme * sizeof(Film));

        for (unsigned int i = 0; i < nrFilme; i ++)
        {
            char date[256];
            fin.getline(date, 256);

            listaFilme[i] = Film(date);
        }
    }

    Cinema()
    {
        cout << "Numar de filme: ";
        cin >> nrFilme;

        listaFilme = (Film*) malloc(nrFilme * sizeof(Film));

        for (unsigned int i = 0; i < nrFilme; i ++)
        {
            cout << "Film #" << i + 1 << endl;

            Film filmNou;
            cin >> filmNou;

            listaFilme[i] = filmNou;
        }
    }

    Cinema(Film *listaFilme, int nrFilme)
    {
        this->listaFilme = listaFilme;
        this->nrFilme = nrFilme;
    }

    Cinema(const Cinema &c)
    {
        this->listaFilme = c.listaFilme;
        this->nrFilme = c.nrFilme;
    }

    ~Cinema()
    {
        listaFilme = NULL;
    }

    Cinema& operator+=(const Cinema &rhs)
    {
        unsigned int newNrFilme = nrFilme + rhs.nrFilme;

        listaFilme = (Film*) realloc(listaFilme, newNrFilme * sizeof(Film));

        int j = 0;
        for (int i = nrFilme; i < newNrFilme; i ++)
            listaFilme[i] = rhs.listaFilme[j ++];

        nrFilme = newNrFilme;

        return *this;
    }

    Cinema cautaDupaTitlu(char *titlu) const
    {
        Film *filmeRezultate;

        int nrRezultate = 0;
        for (int i = 0; i < nrFilme; i ++) {
            char *titluFilm_ = listaFilme[i].getTitlu();
            char *titluFilm = new char[strlen(titluFilm_) - 1];
            strcpy(titluFilm, titluFilm_);
            titluFilm = strupr(titluFilm);
            titlu = strupr(titlu);

            if (strstr(titluFilm, titlu)) {
                filmeRezultate = (Film*) realloc(filmeRezultate, (++ nrRezultate) * sizeof(Film));
                filmeRezultate[nrRezultate - 1] = listaFilme[i];
            }
        }

        Cinema rezultat(filmeRezultate, nrRezultate);

        return rezultat;
    }

    Cinema cautaDupaRegizor(char *numeRegizor) const
    {
        Film *filmeRezultate;

        int nrRezultate = 0;
        for (int i = 0; i < nrFilme; i ++) {
            char *numeRegizorFilm_ = listaFilme[i].getNumeRegizor();
            char *numeRegizorFilm = new char[strlen(numeRegizorFilm_) - 1];
            strcpy(numeRegizorFilm, numeRegizorFilm_);
            numeRegizorFilm = strupr(numeRegizorFilm);
            numeRegizor = strupr(numeRegizor);

            if (strstr(numeRegizorFilm, numeRegizor)) {
                filmeRezultate = (Film*) realloc(filmeRezultate, (++ nrRezultate) * sizeof(Film));
                filmeRezultate[nrRezultate - 1] = listaFilme[i];
            }
        }

        Cinema rezultat(filmeRezultate, nrRezultate);

        return rezultat;
    }

    void afiseaza() const
    {
        for (int i = 0; i < nrFilme; i ++)
            listaFilme[i].afiseaza();
    }
};

int main()
{
    Cinema cinema("date.in");
    cinema.afiseaza();

    bool isRunning = true;

    while (isRunning)
    {
        cout << "Optiuni: 1 - afiseaza lista filme, 2 - cauta dupa titlu, 3 - cauta dupa regizor,  4 - adauga filme, 5 - exit\n";

        int opt;
        cin >> opt;

        switch (opt)
        {
            case 1:
            {
                cinema.afiseaza();
                break;
            }

            case 2:
            {
                char query[256];
                cout << "Cauta dupa titlu:\n";
                fflush(stdin);
                cin.getline(query, 256);

                Cinema rezultat(cinema.cautaDupaTitlu(query));
                rezultat.afiseaza();

                break;
            }

            case 3:
            {
                char query[256];
                cout << "Cauta dupa regizor:\n";
                fflush(stdin);
                cin.getline(query, 256);

                Cinema rezultat(cinema.cautaDupaRegizor(query));
                rezultat.afiseaza();
                break;
            }

            case 4:
            {
                Cinema cinemaNou;
                cinema += cinemaNou;
                break;
            }

            case 5:
            {
                isRunning = false;
                break;
            }

            default:
            {
                cout << "Eroare: Optiune invalida!\n";
                break;
            }
        }
    }

    return 0;
}
