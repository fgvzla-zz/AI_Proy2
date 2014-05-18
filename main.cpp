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


using namespace std;

int main(int argc, const char **argv) {
    state_t state;
    std::vector<state_t> v;
    cout << "Principal variation:" << endl;
    int lim = atoi(argv[1]);
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

    Negamax nei = Negamax(state);
    cout << state.value() << endl;
    cout << "Lo que dio: " << nei.busqueda(state, 34 - lim-1, !player, pass) << "\n";

    /*if( argc > 1 ) {
        int n = atoi(argv[1]);
        cout << endl << "Apply " << n << " random movements at empty board:";
        state = state_t();
        for( int i = 0; i < n; ++i ) {
            bool player = i % 2 == 0; // black moves first
            int pos = state.get_random_move(player);
            state = state.move(player, pos);
            cout << " " << pos;
        }
        cout << endl << state;
    }*/

    return 0;
}

