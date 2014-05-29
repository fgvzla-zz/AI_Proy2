

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
    pair<bool,long int> test(state_t n, int depth, int v, comparador c,bool turn){
        pair<bool,long int> test_generados;
        long int numSuc;
        if(n.terminal() || depth == 0) {
            return make_pair(n.value() < v,0);
        }
        vector<state_t> sucesores = n.succ(turn);
        numSuc += sucesores.size();
        for(int i=0; i < sucesores.size(); i++) {

            if(turn == true ){
                test_generados = test(sucesores[i],depth-1,v,c,!turn);
                numSuc += test_generados.second;
                if(!test_generados.first) return make_pair(false,0);
            }
            if(turn == false ){
                test_generados = test(sucesores[i],depth-1,v,c,!turn);
                numSuc += test_generados.second;
                if(test_generados.first) return make_pair(true,0);
            }
        }
        if(turn == true){
            return make_pair(true,0);
        }
        if(turn == false){
            return make_pair(false,0);
        }
    }

    pair<bool,long int> test2(state_t n, int depth, int v, comparador c,bool turn){
        pair<bool,long int> test_generados;
        long int numSuc;
        if(n.terminal() || depth == 0) {
            return make_pair(n.value() > v,0);
        }
        vector<state_t> sucesores = n.succ(turn);
        numSuc += sucesores.size();
        for(int i=0; i < sucesores.size(); i++) {

            if(turn == true){
                test_generados =  test2(sucesores[i],depth-1,v,c,!turn);
                numSuc += test_generados.second;
                if(test_generados.first) return make_pair(true, 0);
                
            }
            if(turn == false ){
                test_generados = test2(sucesores[i],depth-1,v,c,!turn);
                numSuc += test_generados.second;
                if(!test_generados.first) return make_pair(false, 0);
            }
        }
        if(turn == true){
            return make_pair(false,0);
        }
        if(turn == false){
            return make_pair(true,0);
        }
    }

    pair<int,long int> busqueda(state_t n, int depth,bool turn){
        hash_table_t::iterator it;
        pair<int,long int> valor_generados;
        pair<bool,long int> test_generados;
        long int numSuc = 0;
        it = tabla.find(n);
        if(it != tabla.end()) return make_pair((*it).second,0);
        if(n.terminal() || depth == 0) {
            return make_pair(n.value(),0);

        }

        vector<state_t> sucesores = n.succ(turn);
        numSuc += sucesores.size();
        valor_generados = busqueda(sucesores[0],depth-1,!turn);
        numSuc += valor_generados.second;
        for(int i=1; i < sucesores.size(); i++) {
            if(turn == true){
                test_generados = test2(sucesores[i],depth-1,valor_generados.first,comparador(true),!turn);
                numSuc += test_generados.second;
                if( test_generados.first){
                    valor_generados = busqueda(sucesores[i],depth-1,!turn);
                    numSuc += valor_generados.second;
                }
            }
            
            if(turn == false){
                test_generados = test(sucesores[i],depth-1,valor_generados.first,comparador(false),!turn);
                numSuc += test_generados.second;
                if(test_generados.first){
                    valor_generados = busqueda(sucesores[i],depth-1,!turn);    
                    numSuc+= valor_generados.second;
                }
                
            }
        }

        tabla.insert(make_pair(n, valor_generados.first));

        return make_pair(valor_generados.first,numSuc);

    }

};

int main() {
    state_t state;
    std::vector<state_t> v;

    pair<int,long int> valor_generados;

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
        valor_generados = nab.busqueda(state, 34 - j-1, !player);
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

