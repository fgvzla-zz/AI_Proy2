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


#include "Negamax.cpp"
#include <iostream>
#include <chrono>


using namespace std;

int main(int argc, const char **argv) {
    state_t state;
    std::vector<state_t> v;
    cout << "Principal variation:" << endl;
    int lim = atoi(argv[1]), resultado;
    bool player = false, pass = false;
    int i;
    for(i = 0; i < lim ; ++i ) {
        player = i % 2 == 0; // black moves first!
        int pos = PV[i];
        cout << state;
        cout << (player ? "Black" : "White")
             << " moves at pos = " << pos << (pos == 36 ? " (pass)" : "")
             << endl;
        (pos == 36 ? pass = true : pass = false);
        state = state.move(player, pos);
        cout << "Board after " << i+1 << (i == 0 ? " ply:" : " plies:") << endl;

    }
    cout << state;
    //cout << "Value of the game = " << state.value() << endl;
    //cout << "#bits per state = " << sizeof(state) * 8 << endl;
    Negamax nab = Negamax(state);
    cout << state.value() << endl;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    resultado = nab.busqueda(state, 34 - lim-1, !player, pass);
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    cout << "Negamax Resultado: " << resultado << "\n";
    std::chrono::duration<double> tiempo_corrida = chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    cout << "Tiempo de corrida: "<<tiempo_corrida.count() << "\n";

    return 0;
}