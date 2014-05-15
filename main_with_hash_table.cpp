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

class hash_table_t : public tr1::unordered_map<state_t, stored_info_t, hash_function_t> {
    
};

int main(int argc, const char **argv) {
    hash_table_t my_hash_table;
    std::vector<state_t> v, t;
    // stores one state in hash table
    state_t state;
    stored_info_t info;
    my_hash_table.insert(make_pair(state, info));

    // print some statistics about hash table
    cout << "#elements in hash table = " << my_hash_table.size() << endl;
    cout << "#buckets in hash table = " << my_hash_table.bucket_count() << endl;

    // search the hash table for state just inserted
    hash_table_t::iterator it = my_hash_table.find(state);
    if( it == my_hash_table.end() ) {
        cout << "Element NOT FOUND in hash table" << endl;
    } else {
        cout << "Element FOUND in hash table" << endl;
    }
    v = state.succ(true);
    int i=0;
    while(i < 2) {
        v = state.succ(true);
        i=0;
        if(v.size() != 0) {
            cout << "Sucesor " << 0 << ": "<< v[0] << "\n";
            t = v[i].succ(false);
            if(t.size() != 0) {
                cout << "Sucesor mas profundo " << 0 << ": "<< t[0] << "\n";
                state = t[0];
            }
        }
        if(v.size() == 0) {
            t = state.succ(false);
            if(t.size() == 0) break;
        }
    }

    return 0;
}

