
#include <tr1/unordered_map>
#include "othello_cut.h"
#include <iostream>
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

    int busqueda(state_t nod, int depth, int alpha, int beta, bool turn){
        vector<state_t> sucesores = nod.succ(turn);
        int t;
        int m = -MAXVALUE, n = beta;
        hash_table_t::iterator it;

        //Se busca en las tablas de hash correspondientes.
        it = tablaExact.find(nod);
        if(it != tablaExact.end()) {
            if((*it).second.second >= depth) return (*it).second.first;
        } else {
            it = tablaLower.find(nod);
            if(it != tablaLower.end()) {
                if((*it).second.second >= depth){
                    m = max(m, (*it).second.first);
                    if(m >= beta) return (*it).second.first;
                }
            } else {
                it = tablaUpper.find(nod);
                if(it != tablaUpper.end()) {
                    if((*it).second.second >= depth) {
                        beta = min(beta, (*it).second.first);
                        if(m >= beta) return (*it).second.first;
                    }
                }
            }
        }

        if(nod.terminal()) {
            if(turn) return nod.value();
            else return -nod.value();
        }
        

        /*for(int i = 0; i < sucesores.size();i++){
            if(i != 0){
                t = -busqueda(sucesores[i],depth-1,-m-1,-m,!turn,paso);
                if(m < t < beta){
                    t = -busqueda(sucesores[i],depth-1, -beta,-t, !turn, paso);
                }

            }else{
                t = -busqueda(sucesores[i],depth-1, -beta,-m, !turn,paso);    
            }
            m = max(m,t);
            if(m >= beta){
                break;
            }
        }*/

        for(int i = 0; i < sucesores.size(); i++){
            t = - busqueda(sucesores[i], depth -1, -n,-max(m,alpha),!turn);
            if(t>m){
                if(n==beta || depth < 3 || t>=beta ){
                    m = t;
                }else{
                    m = -busqueda(sucesores[i],depth-1,-beta,-t,!turn);
                }
            }
            if(m>=beta) return m;
            n=max(alpha,m)+1;
        }

        if(m <= alpha) {
            tablaUpper.erase(nod);
            tablaUpper.insert(make_pair(nod, make_pair(m, depth)));
        } else if(m >= beta) {
            tablaLower.erase(nod);
            tablaLower.insert(make_pair(nod, make_pair(m, depth)));
        } else {
            tablaExact.erase(nod);
            tablaExact.insert(make_pair(nod, make_pair(m, depth)));
        }

        return m;
        /*
        for(int i = 0; i < sucesores.size(); i++){
            t = - busqueda(sucesores[i], depth -1, -n,-max(n,alpha),!turn);
            if(t>m){
                if(n==beta | depth < 3,t>=beta ){
                    m = t;
                }else{
                    m = -busqueda(sucesores[i],depth-1,-beta,-t,!turn);
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
    cout << "Lo que dio: " << algo.busqueda(state, 34 - lim-1,-MAXVALUE,MAXVALUE, !player) << "\n";


    return 0;
}

