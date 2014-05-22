

#include <tr1/unordered_map>
#include "othello_cut.h"
#include <iostream>
using namespace std;



class comparador{//Clase para la comparacion de costos de nodo
    bool maOMe;


public:

    comparador(bool mayorOMenor){
        maOMe = mayorOMenor;
    }
    bool operador(int izq, int der)
    {
        if(maOMe == true) return izq > der;
        else return izq < der;
    }   
};

class Scout{
public:
    Scout(){

    }

    bool test(state_t n, int depth, int v, comparador c,bool turn, bool pass){
        vector<state_t> sucesores = n.succ(turn);
        bool paso = false;
        if(sucesores[0] == n) paso = true;
        if((paso & pass) | depth==0 | n.is_full()) {
            /*if(turn)*/ return c.operador(n.value(),v);
            //else return c.operador(-n.value(),v);
        }
        for(int i=0; i < sucesores.size(); i++) {
            if(turn == true & test(sucesores[i],depth-1,v,c,!turn,paso)){
                return true;
            }
            if(turn == false & !test(sucesores[i],depth-1,v,c,!turn,paso)){
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

    int busqueda(state_t n, int depth,bool turn,bool pass){
        vector<state_t> sucesores = n.succ(turn);
        bool paso = false;
        int v;
        if(sucesores[0] == n) paso = true;
        if((paso & pass) | depth==0 | n.is_full()) {
            /*if(turn)*/ return n.value();
            //else return -n.value();
        }
        v = busqueda(sucesores[0],depth-1,!turn,paso);
        for(int i=1; i < sucesores.size(); i++) {
            if(turn == true & test(sucesores[i],depth-1,v,comparador(true),!turn,paso)){
                v = busqueda(sucesores[i],depth-1,!turn,paso);
            }
            if(turn == false & test(sucesores[i],depth-1,v,comparador(false),!turn,paso)){
                v = busqueda(sucesores[i],depth-1,!turn,paso);
            }
        }
        return v;

    }

};

int main(int argc, const char **argv) {
    state_t state;
    std::vector<state_t> v;
    /*cout << "Principal variation:" << endl;*/
    int lim = atoi(argv[1]);
    bool player = false, pass = false;
    int i;
    
    for(i = 0; i < lim ; ++i ) {
        player = i % 2 == 0; // black moves first!
        int pos = PV[i];
        //cout << state;
        //cout << (player ? "Black" : "White")
        //     << " moves at pos = " << pos << (pos == 36 ? " (pass)" : "")
        //     << endl;
        (pos == 36 ? pass = true : pass = false);
        state = state.move(player, pos);
        //cout << "Board after " << i+1 << (i == 0 ? " ply:" : " plies:") << endl;

    }
    //cout << state;
    
    Scout algo = Scout();
    cout << "Lo que dio: " << algo.busqueda(state, 34 - lim-1, !player, pass) << "\n";


    return 0;
}

