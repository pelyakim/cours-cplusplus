#include <cstdlib>
#include "tableau.hpp"

/****
 * Constructeur principal
 * 
 * param: n La taille du tableau en nombre de cellules
 * 
 ********************/
Tableau::Tableau(size_t n): sz(n) {
	cerr << "constructeur sz = " << n << " -> malloc + init\n";
	if (n==0) {
		A = nullptr;
	}
	else {	
		A = (float *) malloc(sz*sizeof(float));
		__copie(0.0,A,sz);
	}
}

/****
 * Constructeur à l'aide d'une liste d'initialisation
 * Tableau t = { 1,2,3 }
 * 
 * param: n La taille du tableau en nombre de cellules
 * 
 ********************/

Tableau::Tableau(const std::initializer_list<float>& l) {
     cerr << "constructeur liste init = -> malloc + init\n";
     sz = l.size();
     size_t i = 0;
     A = (float *) malloc(sz*sizeof(float));
     for (auto j=l.begin();j!=l.end();++j) {
		 A[i++] = *j;
	 }
}

/****
 * Constructeur de copie
 * on fait l'alloc de memoire puis on copie les deux zones mémoire
 * 
 * param: t Le tableau à copier
 * 
 *********************/
Tableau::Tableau (const Tableau & t): sz(t.sz) {
	cerr << "constructeur de copie -> malloc + copie" << '\n';
	A = (float *) malloc(sz*sizeof(float));
	__copie(t.A,A,sz);
}

/****
 * Opérateur d'affectation
 * Si les tailles sont identiques, on se contente de copier les zones de mémoire
 * Sinon:
 *    On rend la mémoire au système
 *    On refait l'alloc de mémoire 
 *    On copie les deux zones mémoire
 * 
 * param: t Le tableau à copier
 * return *this
 * 
 *********************/
Tableau & Tableau::operator=(const Tableau &t) {
	cerr << "operateur =" << " -> free + malloc + copie\n";
	if (this==&t)    // Pour gerer les cas A=A
		return *this;

	if (sz != t.sz) {
		// TODO - Utiliser realloc
		free(A);
		sz = t.sz;
		if (sz != 0) A = (float *) malloc(sz*sizeof(float));
	};
	__copie(t.A,A,sz);
	return *this;
}

/****
 * Constructeur de déplacement
 * on se contente de modifier les pointeurs !
 * 
 * param: t Le tableau à copier
 * 
 *********************/
Tableau::Tableau (Tableau && t) noexcept : sz(t.sz) {
	cerr << "constructeur de déplacement -> o" << '\n';
	A = t.A;
	sz= t.sz;
	t.A = nullptr;
	t.sz= 0;
}

/****
 * Opérateur d'affectation/déplacement
 * On libère la mémoire puis on bouge les pointeurs
 *
 * param: t Le tableau à copier
 * return *this
 * 
 *********************/
Tableau & Tableau::operator=(Tableau && t) noexcept {
	cerr << "operateur= déplacement -> free + o" << '\n';
	if (this!=&t)    // Pour gerer les cas A=A
	{
		free(A);
		A = t.A;
		sz= t.sz;
		t.A = nullptr;
		t.sz= 0;
	}
	return *this;
}

/****
 * Destructeur
 * Rendre la mémoire au système
 * 
 *********************/
Tableau::~Tableau() { 
	cerr << "destructeur (sz = " << sz << ") -> free\n";
	if ( A != nullptr) 
	{
		free(A);
		A = nullptr;
	}
}

// renvoie un element du tableau sans deborder
// pas la peine de tester i < 0, size_t est un type unsigned
// (decommentez ce qui suit vous verrez si cela compile)
float & Tableau::operator[](size_t i) {
	//if (i<0) {
	//  cerr << "ATTENTION Debordement de tableau - je renvoie tableau[0]\n";
	//  return *A;
	//} else
	if (i>=sz) {
		cerr << "ATTENTION Debordement de Tableau - je renvoie Tableau[sz-1]\n";
		return A[sz-1];
		// return *(A+sz-1);  // Une autre manière d'écrire la même chose
	} else {
		return A[i];
		//return *(A+i);
	};
}

// meme chose - version const
float Tableau::operator[](size_t i) const {
	if (i>=sz) {
		cerr << "ATTENTION Debordement de Tableau - je renvoie Tableau[sz-1]\n";
		return A[sz-1];
	} else {
		return A[i];
	};
}

// operateurs +=
// Le parametre est t, un autre Tableau
Tableau & Tableau::operator+=(const Tableau & t) {
	cerr << "operateur+=(const Tableau &) -> += sur une zone mémoire" << '\n';
	if (sz != t.sz) {
		cerr << "Ne peut pas ajouter deux Tableaux de tailles differentes" << '\n'; 
		exit(1);
	} else {
		for (size_t i=0; i < sz; i++) {
			A[i] += t[i];
		};
	};
	return *this;
}

// Le parametre est x, un flottant
Tableau & Tableau::operator+=(float x) {
	cerr << "operateur+=(float) -> += sur une zone mémoire" << '\n';
	for (size_t i=0; i < sz; i++) {
		A[i] += x;
	};
	return *this;
}

/***********************************
 * copie l'entier src dans la zone memoire pointee par dest
 * 
 * src = L'entier à copier partout
 * dest= La zone memoire de destination
 * s   = La taille de la zone mémoire
 * 
 ****************/
void Tableau::__copie (float src, float dest[], size_t s)
{
	for ( size_t i=0; i<s; i++) {
		dest[i] = src;
	}
}

/***********************************
 * copie une zone mémoire dans une autre
 * 
 * src = La zone mémoire source
 * dest= La zone mémoire destination
 * s   = La taille de la zone mémoire
 * 
 ****************/
void Tableau::__copie (float src[], float dest[], size_t s) {
	for (size_t i=0; i<s; i++) {
		dest[i] = src[i];
	};
}

// Deux autres manières d'écrire le même code: difficilement lisible, mais on voit ça souvent
// C'est du C de bas niveau ...
/*
void Tableau::copie (float src, float *dest, size_t s) {
	for (size_t i=0; i<s; i++) {
		*(dest++) = src;
	};
};
void Tableau::copie (float *src, float *dest, size_t s) {
	for (size_t i=0; i<s; i++) {
		*(dest++) = *(src++);
	};
}
*/

// Tableau operator+(Tableau t1, Tableau t2) {
/***************************
 * Opérateur +
 * 
 * params: t1, t2
 * return: Un tableau retourné par valeur
 * 
 *********************/
Tableau operator+(const Tableau& t1, const Tableau& t2) {
	cerr << "operateur+" << '\n';
	if (t1.size() != t2.size()) {
		cerr << "Ne peut pas ajouter deux Tableaux de tailles differentes" << '\n'; 
		exit(1);
	}
	Tableau s = t1;
	s += t2;
	return s;
}

/*************************
 * Opérateur d'impression
 * 
 * params: os = Le flux de sortie
 *         t  = Le tableau à imprimer
 * 
 * return: Le flux de sortie
 * 
 ******/
ostream & operator<<(ostream& os, const Tableau& t) {
	if (t.size() == 0 ) {
		os << "{}";
		return os;
	}
	
	size_t i_dernier = t.size() - 1;
	os << '{';
	for (size_t i=0; i<i_dernier; ++i) {
		os << t[i] << ',';
	}
	os << t[i_dernier] << '}';
	return os;
}

// Il n'y a plus qu'une seule fonction transform, on lui passe un Functor1p
... Tableau::transform(...) {
	for (unsigned int i=0; i< sz; i++) 
		A[i]=...;
}
