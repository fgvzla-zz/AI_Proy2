#include "othello_cut.h"
#include <iostream>

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
	NegamaxAlphaBeta(state_t estado) {
		//tabla.insert(make_pair(estado, estado.value()));
	}

	int busqueda(state_t n, int depth, bool turn, bool pass, int alph, int beta) {
		hash_table_t::iterator it;
		vector<state_t> sucesores;
		bool paso = false;
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
		if(sucesores[0] == n) paso = true;
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