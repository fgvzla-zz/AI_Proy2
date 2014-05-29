#include "othello_cut.h"
#include <iostream>
#include <chrono>

#include <fstream>

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

struct hash_function_t : public tr1::hash<state_t>
 {
    size_t operator()(const state_t &state) const {
        return state.hash();
    }
};

class hash_table_t : public tr1::unordered_map<state_t, int, hash_function_t> {

};

class Negamax{
public:
	hash_table_t tablaNegros;
	hash_table_t tablaBlancos;

	Negamax() {
	}

	int busqueda(state_t n, int depth, bool turn) {
		hash_table_t::iterator it;
		vector<state_t> sucesores = n.succ(turn);
		int alpha = -MAXVALUE, valor, segundo;
		if(n.terminal() || depth == 0) {
			if(turn) return n.value();
			else return -n.value();
		}
		for(int i=0; i < sucesores.size(); i++) {
			if(!turn) {
				it = tablaBlancos.find(sucesores[i]);
				if(it == tablaBlancos.end()) {
					valor = -busqueda(sucesores[i], depth-1, !turn);
					alpha = max(alpha, valor);
					//Se mete el alpha en la tabla de hash para cuando se repita, no tener que volver a hacerlo.
					tablaBlancos.insert(make_pair(sucesores[i], valor));
				} else {
					segundo = (*it).second;
					alpha = max(alpha, segundo);
				}
			} else {
				it = tablaNegros.find(sucesores[i]);
				if(it == tablaNegros.end()) {
					valor = -busqueda(sucesores[i], depth-1, !turn);
					alpha = max(alpha, valor);
					//Se mete el alpha en la tabla de hash para cuando se repita, no tener que volver a hacerlo.
					tablaNegros.insert(make_pair(sucesores[i], valor));
				} else {
					segundo = (*it).second;
					alpha = max(alpha, segundo);
				}
			}
		}
		return alpha;
	}


};


int main(int argc, const char **argv) {
    state_t state;
    std::vector<state_t> v;
    //cout << "Principal variation:" << endl;
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
    //cout << state;
    //cout << "Value of the game = " << state.value() << endl;
    //cout << "#bits per state = " << sizeof(state) * 8 << endl;
    Negamax nab = Negamax();
    //cout << state.value() << endl;

    for(i = 0; i < 33; i++){
    	chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    	resultado = nab.busqueda(state, 34 - lim-1, !player);
    	chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    	cout << "Negamax Resultado: " << resultado << "\n";
    	std::chrono::duration<double> tiempo_corrida = chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    	cout << "Tiempo de corrida: "<<tiempo_corrida.count() << "\n";

    }
    

    return 0;
}