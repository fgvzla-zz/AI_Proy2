
#include <tr1/unordered_map>
#include "othello_cut.h"
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;

struct hash_function_t : public tr1::hash<state_t> {
    size_t operator()(const state_t &state) const {
        return state.hash();
    }
};

class hash_table_t : public tr1::unordered_map<state_t, pair<int, int>, hash_function_t> {

};

class NegaScout{
public:
    hash_table_t tablaExact;
    hash_table_t tablaLower;
    hash_table_t tablaUpper;
    NegaScout(){}

    int busqueda(state_t nod, int depth, int alpha, int beta, bool turn){
        vector<state_t> sucesores = nod.succ(turn);
        int t;
        int m = -MAXVALUE, n = beta;
        hash_table_t::iterator it;

        //Se busca en las tablas de hash correspondientes.
        it = tablaExact.find(nod);
        if(it != tablaExact.end()) {
            if((*it).second.second >= depth) return (*it).second.first;
        } else {
            it = tablaLower.find(nod);
            if(it != tablaLower.end()) {
                if((*it).second.second >= depth){
                    m = max(m, (*it).second.first);
                    if(m >= beta) return (*it).second.first;
                }
            } else {
                it = tablaUpper.find(nod);
                if(it != tablaUpper.end()) {
                    if((*it).second.second >= depth) {
                        beta = min(beta, (*it).second.first);
                        if(m >= beta) return (*it).second.first;
                    }
                }
            }
        }

        if(nod.terminal() || depth == 0) {
            if(turn) return nod.value();
            else return -nod.value();
        }

        for(int i = 0; i < sucesores.size(); i++){
            t = - busqueda(sucesores[i], depth -1, -n,-max(m,alpha),!turn);
            if(t>m){
                if(n==beta || depth < 3 || t>=beta ){
                    m = t;
                }else{
                    m = -busqueda(sucesores[i],depth-1,-beta,-t,!turn);
                }
            }
            if(m>=beta) return m;
            n=max(alpha,m)+1;
        }

        //Se revisa a que tabla de hash corresponde y se inserta.
        if(m <= alpha) {
            tablaUpper.erase(nod);
            tablaUpper.insert(make_pair(nod, make_pair(m, depth)));
        } else if(m >= beta) {
            tablaLower.erase(nod);
            tablaLower.insert(make_pair(nod, make_pair(m, depth)));
        } else {
            tablaExact.erase(nod);
            tablaExact.insert(make_pair(nod, make_pair(m, depth)));
        }

        return m;
    }
};

int main() {
    state_t state;
    std::vector<state_t> v;

    

    ofstream output; //stream para archivo de salida
    streambuf *coutbuf; // stream para guardar salida standard

    coutbuf = cout.rdbuf();
    output.open("resultadosNegaScout2.txt");
    cout.rdbuf(output.rdbuf()); //Se cambia salida standard al archivo de salida

for(int j = 33; j >=0; j--){
    state_t state;
    int resultado;
    bool player = false;
    int i;
    for(i = 0; i < j ; ++i ) {
        player = i % 2 == 0; // black moves first!
        int pos = PV[i];
        state = state.move(player, pos);
    }


        NegaScout nab = NegaScout();
        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        resultado = nab.busqueda(state, 34 - j-1,-MAXVALUE,MAXVALUE, !player);
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
        cout <<  j;
        std::chrono::duration<double> tiempo_corrida = chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        if(tiempo_corrida.count() >= 3600){
            break;

        }
        cout << ", "<<tiempo_corrida.count() << "\n";
        cerr << "Termino " << j << endl;
    }

    output.close();


    return 0;
}

