
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

    pair<int,int> busqueda(state_t nod, int depth, int alpha, int beta, bool turn){
        int t;

        pair <int,int> valor_generados; //Par de enteros. El primer es el valor del juego, el segundo es el numero de nodos generados.
        int m = -MAXVALUE, n = beta;
        hash_table_t::iterator it;

        //Se busca en las tablas de hash correspondientes.
        
        it = tablaExact.find(nod);
        if(it != tablaExact.end()) {
            if((*it).second.second >= depth) return make_pair((*it).second.first,0);
        } else {
            it = tablaLower.find(nod);
            if(it != tablaLower.end()) {
                if((*it).second.second >= depth){
                    m = max(m, (*it).second.first);
                    if(m >= beta) return make_pair((*it).second.first,0);
                }
            } else {
                it = tablaUpper.find(nod);
                if(it != tablaUpper.end()) {
                    if((*it).second.second >= depth) {
                        beta = min(beta, (*it).second.first);
                        if(m >= beta) return make_pair((*it).second.first,0);
                    }
                }
            }
        }
        vector<state_t> sucesores = nod.succ(turn);
        
        int numSuc = sucesores.size();
        
        if(nod.terminal() || depth == 0) {
            if(turn) return make_pair(nod.value(),0);
            else return make_pair(-nod.value(),0);
        }

        for(int i = 0; i < sucesores.size(); i++){
            valor_generados = busqueda(sucesores[i], depth -1, -n,-max(m,alpha),!turn);
            t = -valor_generados.first;
            numSuc += valor_generados.second;
            if(t>m){
                if(n==beta || depth < 3 || t>=beta ){
                    m = t;
                }else{
                    valor_generados = busqueda(sucesores[i],depth-1,-beta,-t,!turn);
                    m = -valor_generados.first;
                    numSuc += valor_generados.second;
                }
            }
            if(m>=beta) return make_pair(m,numSuc);
            n=max(alpha,m)+1;
        }

        //Se revisa a que tabla de hash corresponde y se inserta.
        
        if(m <= alpha) {
            //tablaUpper.erase(nod);
            tablaUpper.insert(make_pair(nod, make_pair(m, depth)));
        } else if(m >= beta) {
            //tablaLower.erase(nod);
            tablaLower.insert(make_pair(nod, make_pair(m, depth)));
        } else {
            //tablaExact.erase(nod);
            tablaExact.insert(make_pair(nod, make_pair(m, depth)));
        }
        
        return make_pair(m,numSuc);
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
    pair <int,int> valor_generados;
    bool player = false;
    int i;
    for(i = 0; i < j ; ++i ) {
        player = i % 2 == 0; // black moves first!
        int pos = PV[i];
        state = state.move(player, pos);
    }


        NegaScout nab = NegaScout();
        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        valor_generados = nab.busqueda(state, 34 - j-1,-MAXVALUE,MAXVALUE, !player);
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
        
        std::chrono::duration<double> tiempo_corrida = chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        if(tiempo_corrida.count() >= 3600){
            break;

        }
        cout <<  j << ", Resultado = "<< valor_generados.first;
        cout << ", "<<tiempo_corrida.count();
        cout << ", Nodos generados: " << valor_generados.second;
        cout << ", Nodos/seg: " << valor_generados.second/tiempo_corrida.count() << endl;
        cerr << "Termino " << j << endl;
    }

    output.close();


    return 0;
}

