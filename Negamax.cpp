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
		//tabla.insert(make_pair(estado, estado.value()));
	}

	int busqueda(state_t n, int depth, bool turn, bool pass) {
		hash_table_t::iterator it;
		cout << "Principito" << "\n";
		cout << n;
		vector<state_t> sucesores = n.succ(turn);
		int alpha = -MAXVALUE, valor;
		bool paso = false;
		cout << "Entrando a " <<depth << endl;
		//Calculando si se tranco.
		if(sucesores[0] == n) paso = true;
		if((paso & pass) | depth==0 | n.is_full()) {
			cout << "Valor que devuelve \n" << n.value() << endl;
			return n.value();
		}
		for(int i=0; i < sucesores.size(); i++) {
			it = tabla.find(sucesores[i]);
			if(it == tabla.end()) {
				valor = -busqueda(sucesores[i], depth-1, !turn, paso);
				alpha = max(alpha, valor);
				//Se mete el alpha en la tabla de hash para cuando se repita, no tener que volver a hacerlo.
				tabla.insert(make_pair(sucesores[i], valor));
			} else {
				alpha = max(alpha, (*it).second);
			}
		}
		return alpha;
	}


};