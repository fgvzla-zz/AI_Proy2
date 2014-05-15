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

class hash_table_t : public tr1::unordered_map<state_t, int, hash_function_t> {

};

class Negamax{
public:
	hash_table_t tabla;

	Negamax(state_t estado) {
		tabla.insert(make_pair(estado, estado.value()));
	}

	int busqueda(state_t n, int depth, bool turn, bool pass) {
		vector<state_t> sucesores = n.succ(turn);
		int alpha = -MAXVALUE;
		bool paso = sucesores[0] == n;
		cout << "Entrando a " <<depth << endl;
		if((paso  & pass) | depth==0) {
			cout << "shit\n" << n.value() << endl;
			return n.value();
		}
		for(int i=0; i < sucesores.size(); i++) {

			if(!tabla.count(sucesores[i])) {
				alpha = max(alpha, -busqueda(sucesores[i], depth-1, !turn, paso));
				tabla.insert(make_pair(sucesores[i], sucesores[i].value()));
			} else if(paso) {
				alpha = max(alpha, -busqueda(sucesores[i], depth-1, !turn, paso));
			}
		}

		return alpha;
	}


};