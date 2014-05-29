#include <tr1/unordered_map>
#include "othello_cut.h"
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;

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

    pair<int,int> busqueda(state_t nod, int depth, int alpha, int beta, bool turn){
    	pair<int,int> valor_generados;
    	int v;
    	int numSuc = 0;
    	if(nod.terminal() || depth == 0){
    		if(turn) return make_pair(nod.value(),0);
    		else return make_pair(-nod.value(),0);
    	}
    	vector<state_t> sucesores = nod.succ(turn);
    	numSuc += sucesores.size();
    	for(int i = 0 ; i < sucesores.size(); i++){
    		if(i > 0){
    			valor_generados = busqueda(sucesores[i], depth -1, -alpha-1, -alpha, !turn);
	    		v = -valor_generados.first;
	    		numSuc += valor_generados.second;
	    		if(alpha < v && v < beta && depth > 1){
	    			valor_generados = busqueda(sucesores[i], depth -1, -beta, -v, !turn);
	    			v = max(-valor_generados.first,v);
	    			numSuc += valor_generados.second;
	    		}
    		}
    		else{
    			valor_generados = busqueda(sucesores[i], depth -1, -beta, -alpha, !turn);
    			v = -valor_generados.first;
    			numSuc += valor_generados.second;
    		}
    		alpha = max(alpha, v);
    		if(alpha >= beta) break;

    		
    	}
    	return make_pair(alpha,numSuc);



    }
};

int main() {
    state_t state;
    std::vector<state_t> v;

    

    ofstream output; //stream para archivo de salida
    streambuf *coutbuf; // stream para guardar salida standard

    coutbuf = cout.rdbuf();
    output.open("resultadosNegaScout3.txt");
    cout.rdbuf(output.rdbuf()); //Se cambia salida standard al archivo de salida

for(int j = 33; j >=0; j--){
    state_t state;
    int resultado;
    pair <int,int> valor_generados;
    bool player = false;
    int i;
    for(i = 0; i < j ; ++i ) {
        player = i % 2 == 0; // black moves first!
        int pos = PV[i];
        state = state.move(player, pos);
    }


        NegaScout nab = NegaScout();
        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        valor_generados = nab.busqueda(state, 34 - j-1,-MAXVALUE,MAXVALUE, !player);
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
        
        std::chrono::duration<double> tiempo_corrida = chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        if(tiempo_corrida.count() >= 3600){
            break;

        }
        cout <<  j << ", Resultado = "<< valor_generados.first;
        cout << ", "<<tiempo_corrida.count();
        cout << ", Nodos generados: " << valor_generados.second;
        cout << ", Nodos/seg: " << valor_generados.second/tiempo_corrida.count() << endl;
        cerr << "Termino " << j << endl;
    }

    output.close();


    return 0;
}