

#include <tr1/unordered_map>
#include <iostream>
#include "othello_cut.h"
#include <fstream>
#include <chrono>
using namespace std;

struct hash_function_t : public tr1::hash<state_t>
 {
    size_t operator()(const state_t &state) const {
        return state.hash();
    }
};

class hash_table_t : public tr1::unordered_map<state_t, int, hash_function_t> {

};
class comparador{//Clase para la comparacion de costos de nodo
    bool maOMe;


public:

    comparador(bool mayorOMenor){
        maOMe = mayorOMenor;
    }
    bool operador(int izq, int der)
    {
        if(maOMe) return izq > der;
        else return izq < der;
    }   
};

class Scout{
public:
    hash_table_t tabla;
    Scout(){

    }
    bool test(state_t n, int depth, int v, comparador c,bool turn){
        
        if(n.terminal() || depth == 0) {
            return n.value() < v;
        }
        vector<state_t> sucesores = n.succ(turn);
        for(int i=0; i < sucesores.size(); i++) {

            if(turn == true && !test(sucesores[i],depth-1,v,c,!turn)){
                return false;
            }
            if(turn == false && test(sucesores[i],depth-1,v,c,!turn)){
                return true;
            }
        }
        if(turn == true){
            return true;
        }
        if(turn == false){
            return false;
        }
    }

    bool test2(state_t n, int depth, int v, comparador c,bool turn){
        
        if(n.terminal() || depth == 0) {
            return n.value() > v;
        }
        vector<state_t> sucesores = n.succ(turn);
        for(int i=0; i < sucesores.size(); i++) {

            if(turn == true && test2(sucesores[i],depth-1,v,c,!turn)){
                return true;
            }
            if(turn == false && !test2(sucesores[i],depth-1,v,c,!turn)){
                return false;
            }
        }
        if(turn == true){
            return false;
        }
        if(turn == false){
            return true;
        }
    }

    int busqueda(state_t n, int depth,bool turn){
        hash_table_t::iterator it;
        int v;
        it = tabla.find(n);
        if(it != tabla.end()) return (*it).second;
        if(n.terminal() || depth == 0) {
            return n.value();

        }

        vector<state_t> sucesores = n.succ(turn);
        v = busqueda(sucesores[0],depth-1,!turn);
        for(int i=1; i < sucesores.size(); i++) {
            if(turn == true && test2(sucesores[i],depth-1,v,comparador(true),!turn)){
                v = busqueda(sucesores[i],depth-1,!turn);
            }
            if(turn == false && test(sucesores[i],depth-1,v,comparador(false),!turn)){
                v = busqueda(sucesores[i],depth-1,!turn);
            }
        }

        tabla.insert(make_pair(n, v));

        return v;

    }

};

int main() {
    state_t state;
    std::vector<state_t> v;

    

    ofstream output; //stream para archivo de salida
    streambuf *coutbuf; // stream para guardar salida standard

    coutbuf = cout.rdbuf();
    output.open("resultadosScout.txt");
    cout.rdbuf(output.rdbuf()); //Se cambia salida standard al archivo de salida

for(int j = 33; j >=0; j--){
    state_t state;
    int resultado;
    bool player = false, pass = false;
    int i;
    for(i = 0; i < j ; ++i ) {
        player = i % 2 == 0; // black moves first!
        int pos = PV[i];
        state = state.move(player, pos);
    }


        Scout nab = Scout();
        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        resultado = nab.busqueda(state, 34 - j-1, !player);
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

