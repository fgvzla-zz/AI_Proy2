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
#include <tr1/unordered_map>
 #include <fstream>


using namespace std;

struct hash_function_t : public tr1::hash<state_t> {
    size_t operator()(const state_t &state) const {
        return state.hash();
    }
};

class hash_table_t : public tr1::unordered_map<state_t, pair<int, int>, hash_function_t> {

};

class NegamaxAlphaBeta{
public:
    hash_table_t tablaExact;
    hash_table_t tablaLower;
    hash_table_t tablaUpper;
    NegamaxAlphaBeta() {}

    int busqueda(state_t n, int depth, bool turn, int alph, int beta) {
        hash_table_t::iterator it;
        vector<state_t> sucesores;
        int alphaO = alph, bestVal = -MAXVALUE, valor;
        int alpha = alph;

        //Se busca en las tablas de hash correspondientes.
        it = tablaExact.find(n);
        if(it != tablaExact.end()) {
            if((*it).second.second >= depth) return (*it).second.first;
        } else {
            it = tablaLower.find(n);
            if(it != tablaLower.end()) {
                if((*it).second.second >= depth){
                    alpha = max(alpha, (*it).second.first);
                    if(alpha >= beta) return (*it).second.first;
                }
            } else {
                it = tablaUpper.find(n);
                if(it != tablaUpper.end()) {
                    if((*it).second.second >= depth) {
                        beta = min(beta, (*it).second.first);
                        if(alpha >= beta) return (*it).second.first;
                    }
                }
            }
        }

        sucesores = n.succ(turn);
        if(n.terminal() || depth == 0) {
            if(turn) return n.value();
            else return -n.value();
        }
        for(int i=0; i < sucesores.size(); i++) {
            valor = -busqueda(sucesores[i], depth-1, !turn, paso, -beta, -alpha);
            bestVal = max(bestVal, valor);
            alpha = max(alpha, valor);
            if(alpha>=beta) break;
        }
        if(bestVal <= alphaO) {
            //tablaUpper.erase(n);
            tablaUpper.insert(make_pair(n, make_pair(bestVal, depth)));
        } else if(bestVal >= beta) {
            //tablaLower.erase(n);
            tablaLower.insert(make_pair(n, make_pair(bestVal, depth)));
        } else {
            //tablaExact.erase(n);
            tablaExact.insert(make_pair(n, make_pair(bestVal, depth)));
        }
        return bestVal;

    }

};

int main() {

state_t state;
    std::vector<state_t> v;

    ofstream output; //stream para archivo de salida
    streambuf *coutbuf; // stream para guardar salida standard

    coutbuf = cout.rdbuf();
    output.open("resultadosAlphaBeta.txt");
    cout.rdbuf(output.rdbuf()); //Se cambia salida standard al archivo de salida

for(int j = 33; j >=0; j--){
    state_t state;
    int resultado;
    bool player = false;
    int i;
    for(i = 0; i < j ; ++i ) {
        player = i % 2 == 0; // black moves first!
        int pos = PV[i];
        (pos == 36 ? pass = true : pass = false);
        state = state.move(player, pos);
    }


        NegamaxAlphaBeta nab = NegamaxAlphaBeta();
        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        resultado = nab.busqueda(state, 34 - j-1, !player, -MAXVALUE, MAXVALUE);
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

