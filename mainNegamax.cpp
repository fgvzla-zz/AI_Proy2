/*
 *  Copyright (C) 2012 Universidad Simon Bolivar
 * 
 *  Permission is hereby granted to distribute this software for
 *  non-commercial research purposes, provided that this copyright
 *  notice is included with any such distribution.
 *  
 *  THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 *  EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE
 *  SOFTWARE IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU
 *  ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
 *  
 *  Blai Bonet, bonet@ldc.usb.ve
 *
 *  Last revision: 11/08/2012
 *
 */


#include "othello_cut.h"
#include <iostream>
#include <chrono>

#include <fstream>

#include <tr1/unordered_map>
using namespace std;

struct stored_info_t {
    // [information to be stored in hash table]
    int infor_;
    stored_info_t() { } // need at least one ctor without arguments
    int infor() const { return infor_; }
    void setInfo(int profundo){
        infor_ = profundo;
    }
};

struct hash_function_t : public tr1::hash<state_t>
 {
    size_t operator()(const state_t &state) const {
        return state.hash();
    }
};

class hash_table_t : public tr1::unordered_map<state_t, int, hash_function_t> {

};

class Negamax{
public:
    hash_table_t tablaNegros;
    hash_table_t tablaBlancos;

    Negamax() {
    }

    int busqueda(state_t n, int depth, bool turn) {
        hash_table_t::iterator it;
        vector<state_t> sucesores = n.succ(turn);
        int alpha = -MAXVALUE, valor, segundo;
        if(n.terminal() || depth == 0) {
            if(turn) return n.value();
            else return -n.value();
        }
        for(int i=0; i < sucesores.size(); i++) {
            //Se tienen dos tablas, la tabla de blancos contiene los valores de los estados que se generaron por mover blanco.
            if(!turn) {
                it = tablaBlancos.find(sucesores[i]);
                //Si el sucesor[i] no esta en la tabla se busca el valor, si esta se toma el valor de la tabla.
                if(it == tablaBlancos.end()) {
                    valor = -busqueda(sucesores[i], depth-1, !turn);
                    alpha = max(alpha, valor);
                    //Se mete el alpha en la tabla de hash para cuando se repita, no tener que volver a hacerlo.
                    tablaBlancos.insert(make_pair(sucesores[i], valor));
                } else {
                    segundo = (*it).second;
                    alpha = max(alpha, segundo);
                }
            } else {
                it = tablaNegros.find(sucesores[i]);
                if(it == tablaNegros.end()) {
                    valor = -busqueda(sucesores[i], depth-1, !turn);
                    alpha = max(alpha, valor);
                    //Se mete el alpha en la tabla de hash para cuando se repita, no tener que volver a hacerlo.
                    tablaNegros.insert(make_pair(sucesores[i], valor));
                } else {
                    segundo = (*it).second;
                    alpha = max(alpha, segundo);
                }
            }
        }
        return alpha;
    }


};


int main() {
    state_t state;
    std::vector<state_t> v;

    

    ofstream output; //stream para archivo de salida
    streambuf *coutbuf; // stream para guardar salida standard

    coutbuf = cout.rdbuf();
    output.open("resultadosNegamax.txt");
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

        //Se corre y se toma el tiempo
        Negamax nab = Negamax();
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