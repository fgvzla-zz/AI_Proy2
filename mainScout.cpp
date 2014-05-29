

#include <tr1/unordered_map>
#include <iostream>
#include <chrono>
#include "othello_cut.h"
//#include "NegamaxAlphaBeta.cpp"
using namespace std;



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
    Scout(){

    }
    bool test(state_t n, int depth, int v, comparador c,bool turn){
        
        if(n.terminal() || depth == 0) {
            //return c.operador(n.value(),v);
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
       
        int v;

        if(n.terminal() || depth == 0) {
            //cout << "Terminal: " << ((turn) ? n.value() : -n.value());
            //cout << ((turn) ? " MAX" : " MIN" ); 
            //cout << endl;
            return n.value();

        }

        vector<state_t> sucesores = n.succ(turn);
        v = busqueda(sucesores[0],depth-1,!turn);
        for(int i=1; i < sucesores.size(); i++) {
            if(turn == true && test2(sucesores[i],depth-1,v,comparador(true),!turn)){
                //NegamaxAlphaBeta buscador(sucesores[i]);
                //cout << "V = " << v << ", Valor MinMax: " << buscador.busqueda(sucesores[i], depth-1, !turn,true, -100, 100 ) << endl;
                v = busqueda(sucesores[i],depth-1,!turn);
            }
            if(turn == false && test(sucesores[i],depth-1,v,comparador(false),!turn)){
                v = busqueda(sucesores[i],depth-1,!turn);
            }
        }
        return v;

    }

};

int main(int argc, const char **argv) {
    state_t state;
    std::vector<state_t> v;
    /*cout << "Principal variation:" << endl;*/
    int lim = atoi(argv[1]), resultado;
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
    cout << state;
    
    Scout algo = Scout();
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    resultado = algo.busqueda(state, 34 - lim-1, !player);
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    cout << "Scout Resultado: " << resultado << "\n";
    std::chrono::duration<double> tiempo_corrida = chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    cout << "Tiempo de corrida: "<<tiempo_corrida.count() << "\n";
    return 0;
}

