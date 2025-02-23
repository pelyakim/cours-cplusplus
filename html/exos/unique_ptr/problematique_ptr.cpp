#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
using namespace std;

/***************
*
*  COMPILATION = c++ -std=c++11 problematique_ptr.cpp -o problematique_ptr
*  EXECUTION   = ulimit -v 200000
*                ./unique_ptr
*
*  RISQUE DE PLANTAGE DE LA MACHINE !
*
*****************************************/

/**********
* Cette classe contient DEUX ZONES MEMOIRE
*
* Le constructeur alloue la mémoire
* Le destructeur la libère
* Donc tout va bien
*
* Vraiment ? Pas si sur.... Ce code est vraiment problématique !
*
* 1/ Vérifiez ce qui se passe en décommentant tour à tout les lignes marquées 1, 2, 3 
* 2/ Remplacez les pointeurs dans Tab2 par des unique_ptr et vérifiez qu'il n'y a plus de problème
*
****************/

class Tab2 {
private:
    int* base1;
    int* base2;
    size_t s1;
    size_t s2;

public:
    /***
    * s1 et s2 = taille du premier et du second tableau
    *
    * En cas d'erreur d'allocation, new renvoie une exception de type bad_alloc
    *
    ****************/
    Tab2(size_t s1, size_t s2): s1(s1), s2(s2) 
    {
        base1 = new int[s1];
        base2 = new int[s2];
    };
    ~Tab2() {
        delete[](base1); 
        base1 = nullptr;
        delete[](base2);
        base2 = nullptr;
    };
};

int main() {

/* Fuite de mémoire:
       N'oubliez pas  ulimit -v 200000
       Observez la fuite à l'aide de top dans une autre fenêtre
       Vérifiez que la troisième ligne provoque une fuite, mais pas les deux autres
       Comme c'est bizarre !
*/

   for (;;) {
    try {
        Tab2 t = {1000,1000};             // 1 - ca marche
        // Tab2 t = {10000000000,1000};   // 2 - Pas de fuite: pourquoi ?
        // Tab2 t = {1000,10000000000};   // 3 - Fuite de mémoire: pourquoi ?
        } catch(const exception &e) {
            cerr << e.what() << '\n';
        };
   };

};

