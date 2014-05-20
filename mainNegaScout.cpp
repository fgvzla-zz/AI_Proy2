
#include <tr1/unordered_map>
#include "othello_cut.h"
#include <iostream>
using namespace std;

class NegaScout{
public:
    NegaScout(){}

    int busqueda(state_t nod, int depth, int alpha, int beta, bool turn, bool pass){
        vector<state_t> sucesores = nod.succ(turn);
        bool paso = false;
        int t;
        int m = alpha;

        if(sucesores[0] == nod) paso = true;
        if((paso & pass) | depth==0 | nod.is_full()) {
            if(turn) return nod.value();
            else return -nod.value();
        }
        

        for(int i = 0; i < sucesores.size();i++){
            if(i != 0){
                t = -busqueda(sucesores[i],depth-1,-m-1,-m,!turn,paso);
                if(m < t < beta){
                    t = -busqueda(sucesores[i],depth-1, -beta,-t, !turn, paso);
                }

            }else{
                t = -busqueda(sucesores[i],depth-1, -beta,-alpha, !turn,paso);    
            }
            m = max(m,t);
            if(m >= beta){
                break;
            }
        }

        return m;
        /*
        for(int i = 0; i < sucesores.size(); i++){
            t = - busqueda(sucesores[i], depth -1, -n,-max(n,alpha),!turn,paso);
            if(t>m){
                if(n==beta | depth < 3,t>=beta ){
                    m = t;
                }else{
                    m = -busqueda(sucesores[i],depth-1,-beta,-t,!turn,paso);
                }
            }
            if(m>=beta) return m;
            n=max(alpha,m)+1;
        }
        return m;
        */
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
    
    NegaScout algo = NegaScout();
    cout << "Lo que dio: " << algo.busqueda(state, 34 - lim-1,-MAXVALUE,MAXVALUE, !player, pass) << "\n";


    return 0;
}

